/*
In questo file creo un' implementazione del blocco che prende i dati dallo switch e li imposta per
mandarli all' I2C
*/

module laster (
    input clk,
    input rst,

    //COLLEGAMENTI CON SWITCH
    output          fifo_axis_tvalid,//Dato valido x lo SWITCH 
    input           fifo_axis_tready,//Switch pronto a ricevere
    output [7:0]    fifo_axis_tdata,//Dati x Switch
    output          fifo_axis_tlast,//Tlast x switch

    //COLLEGAMENTI CON UART
    input           uart_rx_break, //NON LO USO
    input           uart_rx_valid, //Dato disponibile dall'UART
    input [7:0]     uart_rx_data,   //Dato dall'UART
    output wire     uart_rx_en
);


//-------------------------------------------------------------------------------------------

//Parametri di STATO
localparam RICEZ_CMD = 1;
localparam RICEZ_DATA = 0;


//Variabili interne al modulo
//LASTER x SWITCH
reg         stato_next, stato_reg;
reg         fifo_axis_tvalid_next, fifo_axis_tvalid_reg;//Dato valido x lo SWITCH 
reg [7:0]   fifo_axis_tdata_next, fifo_axis_tdata_reg;//Dati x Switch
reg         fifo_axis_tlast_next, fifo_axis_tlast_reg;//Tlast x switch

//-------------------------------------------------------------------------------------------
//CODICE BLOCCO TRA I2C e SWITCH

//Blocco Sincrono (Fa progredire lo stato della FSM)
always @(posedge clk) begin
    if (rst == 0) begin
        //Settaggio dei valori di default in rst
        stato_reg <= RICEZ_CMD;
        fifo_axis_tvalid_reg <= 0;
        fifo_axis_tdata_reg <= 0;
        fifo_axis_tlast_reg <= 0;
    end else begin
        //Progredisco i reg con i next
        stato_reg <= stato_next;
        fifo_axis_tvalid_reg <= fifo_axis_tvalid_next;
        fifo_axis_tdata_reg <= fifo_axis_tdata_next;
        fifo_axis_tlast_reg <= fifo_axis_tlast_next;
    end 
end

//ASSIGN per rendere effettiva la FSM
assign fifo_axis_tvalid = fifo_axis_tvalid_reg;
assign fifo_axis_tdata = fifo_axis_tdata_reg;
assign fifo_axis_tlast = fifo_axis_tlast_reg;
assign uart_rx_en = 1; //Lo tengo sempre ad 1 in quanto devo sempre essere pronto a ricevere dati.




//Macchina a stati LASTER
always @(*) begin
    /*Valori di default per le variabili*/ 
    stato_next = stato_reg;
    fifo_axis_tvalid_next = 0;
    fifo_axis_tdata_next = fifo_axis_tdata_reg;
    fifo_axis_tlast_next = fifo_axis_tlast_reg;
    
    //CASE FSM
    case (stato_reg)
        RICEZ_CMD: begin
            /*In questo blocco la macchina a stati attende 
            un byte di comando dall'UART ed imposta il TLAST 
            per la FIFO se è pari ad 1*/
            if (~uart_rx_valid) begin
                stato_next = RICEZ_CMD;
            end else begin
                if (uart_rx_data == 1) begin
                    fifo_axis_tlast_next = 1;
                    stato_next = RICEZ_DATA;
                end else begin
                    stato_next = RICEZ_DATA;
                end
            end
        end

        RICEZ_DATA: begin
            /*In questo blocco la macchina a stati attende 
            un byte di dati dall'UART e lo passa alla FIFO
            se è pronta a ricevere.*/
            if (~uart_rx_valid) begin
                stato_next = RICEZ_DATA;
            end else begin
                if (fifo_axis_tready) begin
                    fifo_axis_tdata_next = uart_rx_data;
                    stato_next = RICEZ_CMD;
                    fifo_axis_tvalid_next = 1;
                end else begin
                    stato_next = RICEZ_DATA;
                end
            end
        end
    endcase
end

//Domanda: il t last va bene che rimanga alzato per un colpo di clock solamente?

//-------------------------------------------------------------------------------------------

endmodule