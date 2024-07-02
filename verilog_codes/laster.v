/*
Il modulo laster prende in ingrtesso i dati dall'UART ed imposta il TLAST
quando necessario ed a seconda della sequenza di dati passati dall'UART.
*/

module laster (
    /*Dichiarazione di tutti gli input e output del modulo.
    Di default sono di tipo Wire, quindi sono variabili che
    supportano solo l'assegnazione continua. Sono la rappresentazione
    in codice di fili elettrici. Ogni filo può essere acceso
    o spento, quindi 1 o 0.*/
    
    input clk, //Input del clock dell'FPGA
    input rst, //Input del reset dell'FPGA

    //COLLEGAMENTI CON SWITCH
    output          fifo_axis_tvalid,       //Wire valid x SWITCH 
    input           fifo_axis_tready,       //Wire ready da SWITCH
    output [7:0]    fifo_axis_tdata,        //Wire a 8 bit data per SWITCH
    output          fifo_axis_tlast,        //Wire last per SWITCH

    //COLLEGAMENTI CON UART
    input           uart_rx_break,          //Input non utilizzato
    input           uart_rx_valid,          //Wire Valid per laster da uart
    input [7:0]     uart_rx_data,           //Wire a 8 bit data x laster
    output wire     uart_rx_en              //Wire ENABLE UART per UART
);


//-------------------------------------------------------------------------------------------

//Parametri di STATO
localparam RICEZ_CMD = 1;
localparam RICEZ_DATA = 0;

/*Di seguito tutte le variabili necessarie al funzionamento della macchina a stati
Le variabili _reg contengono i valori aggiornati, mentyre le _next i valori da aggiornare
al colpo di clock successivo.
Le _reg sono poi necessarie, tramite gli assign per assegnare il valore agli output wire.
Ogni wire output ha associata una variabiule _reg ed una _next*/

//LASTER x SWITCH
reg         stato_next, stato_reg;                              //Variabili a singolo bit
reg         fifo_axis_tvalid_next, fifo_axis_tvalid_reg;        //Variabili a singolo bit
reg [7:0]   fifo_axis_tdata_next, fifo_axis_tdata_reg;          //Variabile a 8bit
reg         fifo_axis_tlast_next, fifo_axis_tlast_reg;          //Variabili a singolo bit

//-------------------------------------------------------------------------------------------
//CODICE BLOCCO TRA I2C e SWITCH

/*Blocco sincrono che fa progredire la macchina a stati e tutte le variabili in
corrispondenza di ogni rising edge del clock. Ad ogni rising edge del clock vengono
aggiornate le _reg con i valori contenuti nelle _next*/

always @(posedge clk) begin
    if (rst == 0) begin
        /*Settaggio dei valori di default se ho reset basso.
        L'assegnazione è di tipo "non-blocking" ovvero che
        non blocca l'esecuzione delle parti successive di codice*/

        stato_reg <= RICEZ_CMD;
        fifo_axis_tvalid_reg <= 0;
        fifo_axis_tdata_reg <= 0;
        fifo_axis_tlast_reg <= 0;
    end else begin

        /*Codice in assegnazione non-blocking che aggiorna i valori delle _reg con i _next*/
        stato_reg <= stato_next;
        fifo_axis_tvalid_reg <= fifo_axis_tvalid_next;
        fifo_axis_tdata_reg <= fifo_axis_tdata_next;
        fifo_axis_tlast_reg <= fifo_axis_tlast_next;
    end 
end

/*ASSIGN per rendere effettiva la FSM
Si tratta di un tipo di assegnazione specifico per le variabili wire.
Dato che si tratta di fili elettrici e la corrente passa o non passa continuamente,
l'assegnazione è continua ed aggiornata istantaneamente ai valori dei _reg.
Le variabili a cui vengono assegnati i valori dei _reg sono gli output del modulo LASTER*/

assign fifo_axis_tvalid = fifo_axis_tvalid_reg;
assign fifo_axis_tdata = fifo_axis_tdata_reg;
assign fifo_axis_tlast = fifo_axis_tlast_reg;
assign uart_rx_en = 1; //Output mantenuto ad 1 per rendere il modulo sempre pronto a ricevere dati




//Macchina a stati LASTER

/*Blocco Combinatoriale
Qui è implementata la macchina a stati dove vengono eseguite operazioni sulle
variabili _next. L'assegnazione è bloccante, ovvero che esegue le istruzioni
una dopo l'altra.
Questo blocco viene eseguito ogni qualvolta una delle variabili contenute si aggiorna.*/
always @(*) begin 

    //Qui sono presenti i valori di default dei _next
    stato_next = stato_reg;
    fifo_axis_tvalid_next = 0;
    fifo_axis_tdata_next = fifo_axis_tdata_reg;
    fifo_axis_tlast_next = fifo_axis_tlast_reg;
    
    /*CASE STATEMENT che verifica il valore di stato_reg
    per indirizzare l'esecuzione al blocco corrispondente.
    I blocchi di questo case sono RICEZ_CMD e RICEZ_DATA*/
    case (stato_reg)
        RICEZ_CMD: begin
            /*Questo blocco controlla i CMD_LAST in arrivo dall'UART.
            Appena l'UART alza il VALID, vieneeffettuato il controllo:
            se CMD_LAST == 1, viene messo il TLAST per la FIFO ad 1,
            altrimenti a 0.
            Lo stato successivo è sempre il RICEZ_DATA.*/
            if (~uart_rx_valid) begin
                stato_next = RICEZ_CMD;
            end else begin
                if (uart_rx_data == 1) begin
                    fifo_axis_tlast_next = 1;                   //TLAST per la FIFO ad 1
                    stato_next = RICEZ_DATA;                    //Prossimo stato
                end else if (uart_rx_data == 0) begin
                    fifo_axis_tlast_next = 0;                   //TLAST per la FIFO a 0
                    stato_next = RICEZ_DATA;                    //Prossimo stato
                end
            end
        end

        RICEZ_DATA: begin
            /*In questo blocco la macchina a stati attende 
            un byte di dati dall'UART e lo passa alla FIFO
            se è pronta a ricevere.
            Per come è impostata la macchina a stati, se il 
            dato in ricezione è l'ultimo, la macchina ritorna
            in RICEZ_DATA ad attendere bytes dall'UART.*/
            if (~uart_rx_valid) begin
                stato_next = RICEZ_DATA;
            end else begin
                if (fifo_axis_tready) begin
                    fifo_axis_tdata_next = uart_rx_data;        //Passaggio del dato alla FIFO
                    stato_next = RICEZ_CMD;                     //Prossimo stato
                    fifo_axis_tvalid_next = 1;                  //Valid x FIFO
                end else begin
                    stato_next = RICEZ_DATA;
                end
            end
        end
    endcase
end

//-------------------------------------------------------------------------------------------

endmodule
