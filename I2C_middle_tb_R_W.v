`timescale 1ns / 1ps

/*
 * Il seguente Testbench è estremamente essenziale e testa solo la ricezione dei dati del modulo 
 * middle dallo SWITCH. Si dovrebbe completare con tutti gli effettivi input ed output e possibilmente con un
 modulo I2C collegato a periferiche fittizie.
 */

module test_i2c_middle_read;

    reg clk = 0;
    reg rst = 0;

    //COLLEGAMENTI SWITCH <-> MIDDLE
    wire        sw_axis_tready;             //Wire in ingresso dal middle
    reg         sw_axis_tvalid = 0;         //Variabile per generare stimolo al middle
    reg [7:0]   sw_axis_tdata = 0;          //Variabile per generare stimolo al middle
    reg         sw_axis_tlast = 0;          //Variabile per generare stimolo al middle


    integer cnt_x_sda = 0;                  //Contatore intero

//Testbench
initial begin
    /*Qui viene generato un clock fittizio.
    L'istruzione forever esegue "#10 clk = ~clk;" per 
    tutta la durata del testbench" */

    forever #10 clk = ~clk;
end

initial begin
    repeat (2) @(negedge clk);
    rst = 1; //Si inizia con un reset
    
    repeat (4) @(negedge clk);

    //Invio del comando (modificare in base a quello da inviare, R o W)
    wait(sw_axis_tready);
    sw_axis_tdata[0] = 0;
    sw_axis_tdata[1] = 1;
    sw_axis_tdata[2] = 0;
    sw_axis_tdata[3] = 0;
    sw_axis_tdata[4] = 0;
    sw_axis_tdata[5] = 0;
    sw_axis_tdata[6] = 0;
    sw_axis_tdata[7] = 0;
    sw_axis_tvalid = 1;         //Comunico la validità del comando
    
    repeat (2) @(negedge clk);
    sw_axis_tvalid = 0;
    
    repeat (2) @(negedge clk);
    wait(sw_axis_tready);       //Si attende che il middle sia pronto per ricevere
    sw_axis_tdata = 8'h22;      //Invio dell'indirizzo
    sw_axis_tvalid = 1;
    
    repeat (2) @(negedge clk);
    sw_axis_tvalid = 0;
    
    //Commentare per testare la Scrittura
    repeat (4) @(negedge clk);
    wait(sw_axis_tready);
    sw_axis_tdata = 8'h03;      //Invio il numero di Byte
    sw_axis_tlast = 1;
    sw_axis_tvalid = 1;
    
    /*
    //Scommentare per testare la Scrittura
    wait(sw_axis_tready);
    sw_axis_tdata = 8'ha5;      //Invio primo dato
    sw_axis_tvalid = 1;

    repeat (2) @(negedge clk);
    sw_axis_tvalid = 0;

    repeat (4) @(negedge clk);
    wait(sw_axis_tready);
    sw_axis_tdata = 8'h33;      //Invio secondo dato
    sw_axis_tlast = 1;          //Imposto il TLAST
    sw_axis_tvalid = 1;         
    */

    repeat (2) @(negedge clk);
    sw_axis_tvalid = 0;
    
    repeat (2) @(negedge clk);
    
    //Terminazione del Testbench
    $display("Finish simulation at time %d", $time);
    $finish();


end

//Implementazione parziale del middle
middle test_middle_impl(
    .clk(clk),
    .rst(rst),
    /
    .sw_axis_tvalid(sw_axis_tvalid),        // wire [0 : 0] sw_axis_tvalid, quando lo switch ha un dato valido 
    .sw_axis_tdata(sw_axis_tdata),          // wire [7 : 0] sw_axis_tdata, dati in arrivo dallo switch
    .sw_axis_tlast(sw_axis_tlast),          // wire [0 : 0] m_axis_tlast, dice quando è arrivato l'ultimo dato
    .sw_axis_tready(sw_axis_tready)         // Ingresso [0 : 0]
);

endmodule