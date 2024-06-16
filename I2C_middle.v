/*
In questo file creo un' implementazione del blocco che prende i dati dallo switch e li imposta per
mandarli all' I2C
*/

module middle (
    /*Dichiarazione di tutti gli input e output del modulo.
    Di default sono di tipo Wire, quindi sono variabili che
    supportano solo l'assegnazione continua. Sono la rappresentazione
    in codice di fili elettrici*/
    
    input clk, //Input del clock dell'FPGA
    input rst, //Input del reset dell'FPGA

    //COLLEGAMENTI CON SWITCH
    input               sw_axis_tvalid,             //Wire per valid da SWITCH 
    output              sw_axis_tready,             //Ready per ricevere dati da SWITCH
    input [7:0]         sw_axis_tdata,              //Wire da 8bit in arrivo dallo SWITCH
    input               sw_axis_tlast,              //Wire per lastLAST da SWITCH 

    //COLLEGAMENTI CON FIFO2
    output              fifo_axis_tvalid,           //Dato valido x fifo
    input               fifo_axis_tready,           //fifo pronto a ricevere
    output [7:0]        fifo_axis_tdata,            //Wire da 7bit per la FIFO
    output              fifo_axis_tlast,            //Last x fifo

    //COLLEGAMENTI CON I2C x CMD
    output [6:0]        s_axis_cmd_address,         //Indirizzo per I2C da 7bit
    output              s_axis_cmd_start,           //Wire per il comando di start
    output              s_axis_cmd_read,            //Wire per il comando di R
    output              s_axis_cmd_write,           //Wire per il comando di W
    output              s_axis_cmd_write_multiple,  //Wire per il comando di WM
    output              s_axis_cmd_stop,            //Wire per il comando di stop
    output              s_axis_cmd_valid,           //Valid per rendere disponibile il comando all'I2C
    input               s_axis_cmd_ready,           //Ready dall'I2C per ricevere il comando
    
    //COLLEGAMENTI CON I2C x DATA
    output [7:0]        s_axis_data_tdata,          //Wire da 8bit per dati da inviare all'I2C
    output              s_axis_data_tvalid,         //Wire per valid da inviare all'I2C
    input               s_axis_data_tready,         //Ready dall'I2C per ricevere il dato
    output              s_axis_data_tlast,          //Last per i dati inviati all'I2C 
    
    //COLLEGAMENTI CON I2C x STATUS
    input               busy,                       //I2C sta usando i bus
    input               bus_control,                //I2C ha il controllo del bus
    input               bus_active,                 //Il bus è attivo
    input               missed_ack,                 //La periferica non ha inviato l'ack
    
    //COLLEGAMENTI I2C MASTER
    input [7:0]         m_axis_data_tdata,          //Input da I2C per mandarmi il dato letto
    input               m_axis_data_tvalid,         //Input da I2C per darmi il valid
    output              m_axis_data_tready,         //Output per dire all'I2C di mandarmi il dato
    input               m_axis_data_tlast,          //Input da I2C per darmi il last
    
    //COLLEGAMENTI CON I2C x CONFIGURAZIONE
    output [15:0]       prescale,                   //Wire di 16bit per impostare il prescale dell'I2C
    output wire         stop_on_idle                //wire per abilitare lo stop in caso di comando non valido

);

//-------------------------------------------------------------------------------------------

//Parametri di STATO con valori fissati per la FSM
localparam IDLE = 4'b0000;
localparam WAIT_BR = 4'b0001;
localparam VALID_CMD_READ = 4'b0010;
localparam READ_NUM_BYTE= 4'b0011;
localparam READ_SW_TO_WRITE = 4'b0100;
localparam SEND_I2C_TO_WRITE = 4'b0101;
localparam WRITE = 4'b0110;
localparam READ_ADRESS = 4'b0111;
localparam READ = 4'b1000;
localparam SEND_CMD_WRITE = 4'b1001;
localparam ERASE_MISSED_ACK = 4'b1010;

//Valori Prescale
localparam STANDARD_MODE = 250; //Prescale da assegnare se i bit di comando (4-5-6) corrispondono a 3'b001
localparam FAST_MODE = 64;      //Prescale da assegnare se i bit di comando (4-5-6) corrispondono a 3'b010
localparam FAST_MODE_PLUS = 25; //Prescale da assegnare se i bit di comando (4-5-6) corrispondono a 3'b100


/*Di seguito tutte le variabili necessarie al funzionamento della macchina a stati
Le variabili _reg contengono i valori aggiornati, mentyre le _next i valori da aggiornare
al colpo di clock successivo.
Le _reg sono poi necessarie, tramite gli assign per assegnare il valore agli output wire.
Ogni wire output ha associata una variabiule _reg ed una _next*/

//MIDDLE -> SWITCH
reg [7:0]   stato_reg, stato_next; //Variabile a 8bit
reg         sw_axis_tready_reg, sw_axis_tready_next; //Variabili a singolo bit

//MIDDLE -> I2C
reg [6:0]   s_axis_cmd_address_next, s_axis_cmd_address_reg;
reg         s_axis_cmd_read_next, s_axis_cmd_read_reg;
reg         s_axis_cmd_write_next, s_axis_cmd_write_reg;
reg         s_axis_cmd_write_multiple_next, s_axis_cmd_write_multiple_reg;
reg         s_axis_cmd_stop_next, s_axis_cmd_stop_reg;
reg         s_axis_cmd_valid_next, s_axis_cmd_valid_reg;
reg         s_axis_cmd_start_reg, s_axis_cmd_start_next;
reg [7:0]   s_axis_data_tdata_next, s_axis_data_tdata_reg;
reg         s_axis_data_tvalid_next, s_axis_data_tvalid_reg;
reg         s_axis_data_tlast_next, s_axis_data_tlast_reg;
reg         m_axis_data_tready_next, m_axis_data_tready_reg;
reg [15:0]  prescale_next, prescale_reg = 64;

//MIDDLE
reg [7:0]   num_byte_reg, num_byte_next;
reg         stop_on_idle_next, stop_on_idle_reg;


//MIDDLE -> FIFO2
reg fifo_axis_tvalid_next, fifo_axis_tvalid_reg;
reg [7:0] fifo_axis_tdata_next, fifo_axis_tdata_reg;
reg fifo_axis_tlast_next, fifo_axis_tlast_reg;

integer cnt_read_from_I2C_next, cnt_read_from_I2C_reg = 0; //Variabile intera a 32 bit

//-------------------------------------------------------------------------------------------
//CODICE MACCHINA A STATI MIDDLE

/*Blocco sincrono che fa progredire la macchina a stati e tutte le variabili in
corrispondenza di ogni rising edge del clock. Ad ogni rising edge del clock vengono
aggiornate le _reg con i valori contenuti nelle _next*/
always @(posedge clk) begin
    if (rst == 0) begin
        /*Settaggio dei valori di default se ho reset basso.
        L'assegnazione è di tipo "non-blocking" ovvero che
        non blocca l'esecuzione delle parti successive di codice*/
        stato_reg <= IDLE;
        sw_axis_tready_reg <= 0;
        s_axis_cmd_address_reg <= 0;
        s_axis_cmd_start_reg <= 0;
        s_axis_cmd_read_reg <= 0;
        s_axis_cmd_write_reg <= 0;
        s_axis_cmd_write_multiple_reg <= 0;
        s_axis_cmd_stop_reg <= 0;
        s_axis_cmd_valid_reg <= 0;
        s_axis_data_tvalid_reg <= 0;
        s_axis_data_tlast_reg <= 0;
        s_axis_data_tdata_reg <= 0;
        num_byte_reg <= 0;
        m_axis_data_tready_reg <= 0;
        fifo_axis_tvalid_reg <= 0;
        fifo_axis_tdata_reg <= 0;
        fifo_axis_tlast_reg <= 0;
        stop_on_idle_reg <= 0;
        cnt_read_from_I2C_reg <= 0;
        prescale_reg <= 64;
    end else begin
        /*Codice in assegnazione non-blocking che aggiorna i valori delle _reg con i _next*/
        stato_reg <= stato_next;
        sw_axis_tready_reg <= sw_axis_tready_next;
        s_axis_cmd_address_reg <= s_axis_cmd_address_next;
        s_axis_cmd_start_reg <= s_axis_cmd_start_next;
        s_axis_cmd_read_reg <= s_axis_cmd_read_next;
        s_axis_cmd_write_reg <= s_axis_cmd_write_next;
        s_axis_cmd_write_multiple_reg <= s_axis_cmd_write_multiple_next;
        s_axis_cmd_stop_reg <= s_axis_cmd_stop_next;
        s_axis_cmd_valid_reg <= s_axis_cmd_valid_next;
        s_axis_data_tvalid_reg <= s_axis_data_tvalid_next;
        s_axis_data_tlast_reg <= s_axis_data_tlast_next;
        s_axis_data_tdata_reg <= s_axis_data_tdata_next;
        num_byte_reg <= num_byte_next;
        m_axis_data_tready_reg <= m_axis_data_tready_next;
        fifo_axis_tvalid_reg <= fifo_axis_tvalid_next;
        fifo_axis_tdata_reg <= fifo_axis_tdata_next;
        fifo_axis_tlast_reg <= fifo_axis_tlast_next;
        stop_on_idle_reg <= stop_on_idle_next;
        cnt_read_from_I2C_reg <= cnt_read_from_I2C_next;
        prescale_reg <= prescale_next;
    end 
end

/*ASSIGN per rendere effettiva la FSM
Si tratta di un tipo di assegnazione specifico per le variabili wire.
Dato che si tratta di fili elettrici e la corrente passa o non passa continuamente,
l'assegnazione è continua ed aggiornata istantaneamente ai valori dei _reg.
Le variabili a cui vengono assegnati i valori dei _reg sono gli output del modulo MIDDLE*/

assign sw_axis_tready = sw_axis_tready_reg; 

assign s_axis_cmd_address = s_axis_cmd_address_reg;
assign s_axis_cmd_start = s_axis_cmd_start_reg;
assign s_axis_cmd_read = s_axis_cmd_read_reg;
assign s_axis_cmd_write = s_axis_cmd_write_reg;
assign s_axis_cmd_write_multiple = s_axis_cmd_write_multiple_reg;
assign s_axis_cmd_stop = s_axis_cmd_stop_reg;
assign s_axis_cmd_valid = s_axis_cmd_valid_reg;

assign s_axis_data_tdata = s_axis_data_tdata_reg;
assign s_axis_data_tvalid = s_axis_data_tvalid_reg;
assign s_axis_data_tlast = s_axis_data_tlast_reg;
assign m_axis_data_tready = m_axis_data_tready_reg;

assign fifo_axis_tdata = fifo_axis_tdata_reg;
assign fifo_axis_tvalid = fifo_axis_tvalid_reg;
assign fifo_axis_tdata = fifo_axis_tdata_reg;
assign fifo_axis_tlast = fifo_axis_tlast_reg;

assign prescale = prescale_reg;
assign stop_on_idle = stop_on_idle_reg;


/*Blocco Combinatoriale
Qui è implementata la macchina a stati dove vengono eseguite operazioni sulle
variabili _next. L'assegnazione è bloccante, ovvero che esegue le istruzioni
una dopo l'altra.
Questo blocco viene eseguito ogni qualvolta una delle variabili contenute si aggiorna.*/
always @(*) begin
    //Qui sono presenti i valori di default dei next
    sw_axis_tready_next = 0;
    stato_next = stato_reg;
    s_axis_cmd_address_next = s_axis_cmd_address_reg;
    s_axis_cmd_start_next = s_axis_cmd_start_reg;
    s_axis_cmd_read_next = s_axis_cmd_read_reg;
    s_axis_cmd_write_next = s_axis_cmd_write_reg;
    s_axis_cmd_write_multiple_next = s_axis_cmd_write_multiple_reg;
    s_axis_cmd_stop_next = s_axis_cmd_stop_reg;
    s_axis_cmd_valid_next = s_axis_cmd_valid_reg;
    s_axis_data_tdata_next = s_axis_data_tdata_reg;
    s_axis_data_tvalid_next = 0;
    s_axis_data_tlast_next = s_axis_data_tlast_reg;
    num_byte_next = num_byte_reg;
    m_axis_data_tready_next = 0;
    fifo_axis_tvalid_next = 0;
    fifo_axis_tdata_next = fifo_axis_tdata_reg;
    fifo_axis_tlast_next = fifo_axis_tlast_reg;
    stop_on_idle_next = 0;
    cnt_read_from_I2C_next = cnt_read_from_I2C_reg;
    prescale_next = prescale_reg;
    
    //CASE FSM
    case (stato_reg)
        IDLE:begin
            //Verifico il valid dallo switch
            if (~sw_axis_tvalid) begin
                stato_next = IDLE;
                sw_axis_tready_next = 1; //Comunico allo switch che il middle è pronto a ricevere.
                
                //Per spegnere lettura e scrittura
                s_axis_cmd_valid_next = 0;
                s_axis_data_tlast_next = 0;
            end else begin
                /*Il primo vettore di 8 bit che viene passato al middle è configurato per comunicare
                ciò che dovrà fare l'I2C. I bit in determinate posizioni del vettore dicono se deve
                partire la lettura o la scrittura ed il codiuce del prescaler.
                Il blocco IDLE indirizza ai blocchi successivi in base a ciò che viene
                letto da questo primo vettore*/

                //Controllo dei bit 4-5-6 del byte di comando per impostare il prescaler
                if (sw_axis_tdata[6:4] == 3'b001) begin
                    prescale_next = STANDARD_MODE; 
                end else begin
                        if (sw_axis_tdata[6:4] == 3'b010) begin
                            prescale_next = FAST_MODE;
                        end
                    end else begin
                            if (sw_axis_tdata[6:4] == 3'b100) begin
                                    prescale_next = FAST_MODE_PLUS;
                            end
                        end

                if (sw_axis_tdata[1]) begin//LETTURA
                    if (sw_axis_tdata[7]) begin
                        /*Se il comando è di leggere lo status dell'I2C, vengono settati alcuni bit e
                        vengono immediatamente mandati in uscita. Si ritorna in attesa di nuovi dati.*/
                        stato_next = IDLE;
                        fifo_axis_tvalid_next = 1;
                        fifo_axis_tdata_next[0] = missed_ack;
                        fifo_axis_tdata_next[1] = busy;
                        fifo_axis_tdata_next[2] = bus_control;
                        fifo_axis_tdata_next[3] = bus_active;
                        fifo_axis_tdata_next[7:4] = 0;
                        fifo_axis_tlast_next = 1;
                    end else begin
                        /*Qui viene impostato il comando di lettura da girare all'I2C.
                        Il blocco successivo a cui si passa è la lettura dell'indirizzo della periferica
                        collegata all'I2C dalla quale si leggeranno i dati.*/

                        s_axis_cmd_start_next = sw_axis_tdata[0];
                        s_axis_cmd_read_next = 1;
                        s_axis_cmd_write_next = 0;
                        s_axis_cmd_write_multiple_next = 0;
                        s_axis_cmd_stop_next = sw_axis_tdata[3];
                        stato_next = READ_ADRESS;
                        sw_axis_tready_next = 1;//VERIFICARE SE FUNZIONA!!
                    end
                end else begin
                    if(~sw_axis_tdata[1]) begin
                        if(sw_axis_tdata[2])begin//WRITE MULTIPLE
                            /*Qui viene impostato il comando di scrittura multipla da girare all'I2C.
                            Il blocco successivo a cui si passa è la lettura dell'indirizzo della periferica
                            collegata all'I2C alla quale si invieranno i dati.*/
                            s_axis_cmd_start_next = sw_axis_tdata[0];
                            s_axis_cmd_read_next = 0;
                            s_axis_cmd_write_next = 0;
                            s_axis_cmd_write_multiple_next = 1;
                            s_axis_cmd_stop_next = sw_axis_tdata[3];
                            stato_next = READ_ADRESS;
                            sw_axis_tready_next = 1;//VERIFICARE SE FUNZIONA!!
                        end else begin//WRITE SINGLE
                            /*Qui viene impostato il comando di scrittura singola da girare all'I2C.
                            Il blocco successivo a cui si passa è la lettura dell'indirizzo della periferica
                            collegata all'I2C alla quale si invieranno i dati.*/
                            s_axis_cmd_start_next = sw_axis_tdata[0];
                            s_axis_cmd_read_next = 0;
                            s_axis_cmd_write_next = 1;
                            s_axis_cmd_write_multiple_next = 0;
                            s_axis_cmd_stop_next = sw_axis_tdata[3];
                            stato_next = READ_ADRESS;
                            sw_axis_tready_next = 1;//VERIFICARE SE FUNZIONA!!
                        end
                    end
                end
            end
        end

        READ_ADRESS: begin
            if (~sw_axis_tvalid) begin
                /*Se SW non comunica il valid, rimango in questo blocco*/
                stato_next = READ_ADRESS;
                sw_axis_tready_next = 1;
            end else begin
                if (sw_axis_tvalid) begin
                    /*Qui viene letto l'indirizzo e viene scelto il blocco successivo in base ai comandi 
                    ricevuti nel blocco precedente*/
                    s_axis_cmd_address_next = sw_axis_tdata[6:0];
                    if (s_axis_cmd_write_reg || s_axis_cmd_write_multiple_reg) begin
                        stato_next = SEND_CMD_WRITE;
                    end else begin
                        if (s_axis_cmd_read_reg) begin
                        stato_next = READ_NUM_BYTE;
                        sw_axis_tready_next = 1;
                        end
                    end 
                end
            end
        end
        
        /*Questo blocco attende che l'I2C sia pronto a ricevere il comando di scrittura*/
        SEND_CMD_WRITE: begin
                if (~s_axis_cmd_ready) begin
                    stato_next = SEND_CMD_WRITE;
                end else begin
                    stato_next = READ_SW_TO_WRITE;
                    s_axis_cmd_valid_next = 1;
                    sw_axis_tready_next = 1;
                end
        end

        /*Questo blocco attende lo switch per prelevare i dato da scrivere tramite I2C*/
        READ_SW_TO_WRITE: begin
            if(~missed_ack) begin
                /*Il missed_ack è un segnale che arriva dall'I2C. Viene impostato ad 1 se l'indirizzo
                fornito all'I2C non corrisponde a nessuna deklle periferiche collegate. Questo
                controllo fa si che si interrompa tutta l'esecuzione e che si passi al blocco
                ERASE_MISSED_ACK*/
                if (~sw_axis_tvalid) begin
                    stato_next = READ_SW_TO_WRITE;
                    sw_axis_tready_next = 1;
                end else begin
                    s_axis_data_tdata_next = sw_axis_tdata;
                    stato_next = SEND_I2C_TO_WRITE;
                    s_axis_data_tvalid_next = 1;
                    if (sw_axis_tlast) begin
                        s_axis_data_tlast_next = 1;
                    end
                end
            end else begin
                stato_next = ERASE_MISSED_ACK;
                sw_axis_tready_next = 1;
                s_axis_cmd_valid_next = 0;
            end
        end

        /*Questo blocco invia il dato da scrivere tramite I2C all'I2C.
        Se non viene settato ad 1 nel laster.v il last (qui s_axis_data_tlast_reg), si ritorna 
        al READ_SW_TO_WRITE e si procede ciclicamente.
        Con il last a 1 si ritorna in attesa in IDLE.*/
        SEND_I2C_TO_WRITE: begin
            if (~s_axis_data_tready) begin
                stato_next = SEND_I2C_TO_WRITE;
                s_axis_data_tvalid_next = 1;
            end else begin
                if (s_axis_data_tlast_reg) begin
                    stato_next = IDLE;
                end else begin
                    stato_next = READ_SW_TO_WRITE;
                    sw_axis_tready_next = 1;
                end
            end 
        end

        /*Questo blocco esaurisce i dati in arrivo dallo switch se si verifica un missed_ack.
        Vengono letti senza essere usati e all'arrivo del last, l'esecuzione torna in attesa in IDLE.*/
        ERASE_MISSED_ACK: begin
            if (~sw_axis_tlast && ~s_axis_data_tlast_reg) begin
                stato_next = ERASE_MISSED_ACK;
                sw_axis_tready_next = 1;
            end else begin
                if (sw_axis_tlast && ~s_axis_data_tlast_reg) begin
                    s_axis_data_tlast_next = 1;
                    stato_next = ERASE_MISSED_ACK;
                    //sw_axis_tready_next = 1; da definire
                end else begin
                    if (s_axis_data_tlast_reg) begin
                        stato_next = IDLE;
                    end
                end
            end
        end 

        /*Questo blocco è solo per la lettura, che necessita di sapere quanti byte dovrAnno essere letti
        dalla periferica collegata all'I2C.*/
        READ_NUM_BYTE: begin
                if (~sw_axis_tvalid) begin
                stato_next = READ_NUM_BYTE;
                sw_axis_tready_next = 1;
                end else begin
                    if (sw_axis_tvalid) begin 
                        num_byte_next = sw_axis_tdata;
                        stato_next = VALID_CMD_READ;
                    end
                end
        end

        /*Dopo la lettura dell'indirizzo, è possibile avviare il comando di lettura 
        tramite questo blocco.*/
        VALID_CMD_READ: begin
            if (~s_axis_cmd_ready) begin
                stato_next = VALID_CMD_READ;
            end else begin
                /*Il cmd_valid è ciò che da' il via all'I2C nel ricevere il comando*/
                s_axis_cmd_valid_next = 1;
                /*A questo punto deve essere comunicato all'I2C che middle.v è pronto a ricevere
                i dati letti dalla periferica collegata all'I2C.*/
                m_axis_data_tready_next = 1;
                stato_next = READ;
            end
                
        end

        /*Questo blocco riceve i dati letti dall'I2C che vengono salvati per poi essere trasmessi
        al fifo."*/
        READ: begin
            if(~missed_ack)begin
                if (~m_axis_data_tvalid) begin 
                    stato_next = READ;
                    m_axis_data_tready_next = 1;
                end else begin
                    fifo_axis_tdata_next = m_axis_data_tdata;
                    fifo_axis_tvalid_next = 1;
                    cnt_read_from_I2C_next = cnt_read_from_I2C_reg + 1;
                    m_axis_data_tready_next = 0;
                    if (cnt_read_from_I2C_reg == num_byte_reg - 1) begin
                        /*Questo controllo fa si che una volta raggiunto il penultimo dato da leggere,
                        viene impostato il last per il br e settato lo stop per la lettura.*/
                        fifo_axis_tlast_next = 1;
                        cnt_read_from_I2C_next = 0;
                        //s_axis_cmd_valid_next = 0;                                             
                    end
                    stato_next = WAIT_BR;
                end 
            end else begin
                stato_next = IDLE;
                s_axis_cmd_stop_next = 1;
                s_axis_cmd_valid_next = 0;
                s_axis_cmd_read_next = 0;
            end
        end

        /*Questo blocco aspetta che la fifo sia pronta a riceverei dati. Le invia dati fino a 
        quando non viene settato il tlast nel blocco  READ.*/
        WAIT_BR: begin
           if (fifo_axis_tready) begin
                if (fifo_axis_tlast_reg) begin
                    stato_next = IDLE;
                    s_axis_cmd_stop_next = 1;
                    s_axis_cmd_read_next = 0;
                end else begin 
                    stato_next = READ;
                    m_axis_data_tready_next = 1;
                end
            end else begin
                stato_next = WAIT_BR;
                fifo_axis_tvalid_next = 1;
            end 
        end            

        /*READ: begin
            if(~missed_ack)begin
                if (~m_axis_data_tvalid ) begin 
                stato_next = READ;
                m_axis_data_tready_next = 1;
                end else begin
                    if (cnt_read_from_I2C_reg < num_byte_reg) begin
                        if (fifo_axis_tready) begin
                            stato_next = READ;
                            cnt_read_from_I2C_next = cnt_read_from_I2C_reg + 1;
                            fifo_axis_tdata_next = m_axis_data_tdata;
                            fifo_axis_tvalid_next = 1;
                        end else begin
                            stato_next = READ;
                        end
                        if(cnt_read_from_I2C_reg == num_byte_reg - 1) begin
                            fifo_axis_tlast_next = 1;
                            stato_next = READ;
                            s_axis_cmd_stop_next = 1;                                               
                        end
                    end else if (cnt_read_from_I2C_reg == num_byte_reg) begin
                        stato_next = IDLE;
                        cnt_read_from_I2C_next = 0;
                        //stop_on_idle_next = 1;//è come se gli mandassi uno stop GIGANTE                   
                    end
                end
            end else begin
                stato_next = IDLE;
                s_axis_cmd_stop_next = 1;
                //sw_axis_tready_next = 1;
                //stop_on_idle_next = 1;
            end
        end */
    endcase
end

//-------------------------------------------------------------------------------------------

endmodule