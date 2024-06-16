/*Il modulo top ha come funzione di collegare tutti i moduli necessari alla realizzazione
del sistema di comunicazione.
Nella pratica, questo significa dichiarare dei wire ed implementare i vari moduli utilizzandoli
correttamente.*/

module top (
    //Clock e reset
    input               clk,
    input               rstn,

    //Inout dell'I2C
    inout               scl,
    inout               sda,

    //Segnali per mantenere a 1 logico SDA e SCL
    output              scl_pup,
    output              sda_pup,

    //Ingresso ed uscita dell'UART
    input   wire        uart_rxd,
    output  wire        uart_txd
);

//-------------------------------------------------------------------------------------------

//Creazione dei Wire per connettere i moduli
//PIN I2C
wire            scl_i;
wire            scl_o;
wire            scl_t;
    
wire            sda_i;
wire            sda_o;
wire            sda_t;

//SWITCH NON USATI
wire [1:0]      s_req_suppress;
wire            s_decode_err;


//UART <-> LASTERS
wire            uart_rx_break_to_laster;
wire            uart_rx_valid_to_laster;
wire [7:0]      uart_rx_data_to_laster;
wire            laster_rx_en_to_uart;

//LASTERS <-> FIFO
wire            laster_axis_tvalid_to_fifo;
wire            fifo_axis_tready_to_laster;
wire [7:0]      laster_axis_tdata_to_fifo;
wire            laster_axis_tlast_to_fifo;

//FIFO <-> SWITCH
wire            fifo_axis_tvalid_to_switch;
wire            sw_axis_tready_to_fifo;
wire [7:0]      fifo_axis_tdata_to_switch;
wire            fifo_axis_tlast_to_switch;

//SWITCH <-> MIDDLE
wire            sw_axis_tvalid_to_middle;
wire            middle_axis_tready_to_switch;
wire [7:0]      sw_axis_tdata_to_middle;
wire            sw_axis_tlast_to_middle;

//MIDDLE <-> FIFO2
wire            middle_axis_tvalid_to_fifo2;
wire            fifo2_axis_tready_to_middle;
wire [7:0]      middle_axis_tdata_to_fifo2;
wire            middle_axis_tlast_to_fifo2;

//FIFO2 <-> UART
wire            master_fifo2_axis_tvalid_to_uart;
wire            uart_axis_tready_to_fifo2; 
wire [7:0]      master_fifo2_axis_tdata_to_uart;
wire            master_fifo2_axis_tlast_to_uart;

//MIDDLE <-> I2C
//CMD
wire [6:0]      middle_axis_cmd_address_to_I2C;
wire            middle_axis_cmd_start_to_I2C;
wire            middle_axis_cmd_read_to_I2C;
wire            middle_axis_cmd_write_to_I2C;
wire            middle_axis_cmd_write_multiple_to_I2C;
wire            middle_axis_cmd_stop_to_I2C;
wire            middle_axis_cmd_valid_to_I2C;
wire            I2C_axis_cmd_ready_to_middle; 
//DATA
wire [7:0]      middle_axis_data_tdata_to_I2C;
wire            middle_axis_data_tvalid_to_I2C;
wire            I2C_axis_data_tready_to_middle; 
wire            middle_axis_data_tlast_to_I2C;
//STATUS
wire            I2C_busy_to_middle;
wire            I2C_bus_control_to_middle;
wire            I2C_bus_active_to_middle;
wire            I2C_missed_ack_to_middle;
//MASTER I2C
wire [7:0]      master_I2C_axis_data_tdata_to_middle; 
wire            master_I2C_axis_data_tvalid_to_middle; 
wire            middle_axis_data_tready_to_master_I2C; 
wire            master_I2C_axis_data_tlast_to_middle; 
//CONFIG
wire [15:0]     middle_prescale_to_I2C;
wire            stop_on_idle;

//VARI FILI
wire uart_axis_BUSY_to_master_fifo2; //Busy dell'UART

//ASSIGN VARI
assign scl_pup = 1;
assign sda_pup = 1;
assign s_req_suppress = 0;
assign s_decode_err = 0;
assign uart_axis_tready_to_fifo2 = ~uart_axis_BUSY_to_master_fifo2; //Il tready alla fifo2 viene dato se uart è non busy

//-------------------------------------------------------------------------------------------
//IMPLEMENTAZIONE MODULI

//UART RICEZIONE
uart_rx impl_uart_rx(
    .clk(clk),
    .resetn(rstn),
    .uart_rxd(uart_rxd),
    .uart_rx_en(laster_rx_en_to_uart),
    .uart_rx_break(uart_rx_break_to_laster),
    .uart_rx_valid(uart_rx_valid_to_laster),
    .uart_rx_data(uart_rx_data_to_laster)
);

//UART TRASMISSIONE
uart_tx impl_uart_tx(
    .clk(clk),
    .resetn(rstn),
    .uart_txd(uart_txd),
    .uart_tx_busy(uart_axis_BUSY_to_master_fifo2),//Per dire quando non è occupata l'uart al broadcaster
    .uart_tx_en(master_fifo2_axis_tvalid_to_uart),
    .uart_tx_data(master_fifo2_axis_tdata_to_uart)
);

//LASTER
laster impl_laster(
    .clk(clk),
    .rst(rstn),

    //COLLEGAMENTI CON SWITCH
    .fifo_axis_tvalid(laster_axis_tvalid_to_fifo),
    .fifo_axis_tready(fifo_axis_tready_to_laster),
    .fifo_axis_tdata(laster_axis_tdata_to_fifo),
    .fifo_axis_tlast(laster_axis_tlast_to_fifo),

    //COLLEGAMENTI CON UART
    .uart_rx_break(uart_rx_break_to_laster),
    .uart_rx_valid(uart_rx_valid_to_laster),
    .uart_rx_data(uart_rx_data_to_laster),
    .uart_rx_en(laster_rx_en_to_uart)
);

//FIFO
axis_data_fifo_0 impl_fifo (
    .s_axis_aresetn(rstn),                          // input wire s_axis_aresetn
    .s_axis_aclk(clk),                              // input wire s_axis_aclk
    .s_axis_tvalid(laster_axis_tvalid_to_fifo),     // input wire s_axis_tvalid
    .s_axis_tready(fifo_axis_tready_to_laster),     // output wire s_axis_tready
    .s_axis_tdata(laster_axis_tdata_to_fifo),       // input wire [7 : 0] s_axis_tdata
    .s_axis_tlast(laster_axis_tlast_to_fifo),       // input wire s_axis_tlast
    .m_axis_tvalid(fifo_axis_tvalid_to_switch),     // output wire m_axis_tvalid
    .m_axis_tready(sw_axis_tready_to_fifo),         // input wire m_axis_tready
    .m_axis_tdata(fifo_axis_tdata_to_switch),       // output wire [7 : 0] m_axis_tdata
    .m_axis_tlast(fifo_axis_tlast_to_switch)        // output wire m_axis_tlast
);

//FIFO2
axis_data_fifo_1 impl_fifo2 (
  .s_axis_aresetn(rstn),                            // input wire s_axis_aresetn
  .s_axis_aclk(clk),                                // input wire s_axis_aclk
  .s_axis_tvalid(middle_axis_tvalid_to_fifo2),      // input wire s_axis_tvalid
  .s_axis_tready(fifo2_axis_tready_to_middle),      // output wire s_axis_tready
  .s_axis_tdata(middle_axis_tdata_to_fifo2),        // input wire [7 : 0] s_axis_tdata
  .s_axis_tlast(middle_axis_tlast_to_fifo2),        // input wire s_axis_tlast
  .m_axis_tvalid(master_fifo2_axis_tvalid_to_uart), // output wire m_axis_tvalid
  .m_axis_tready(uart_axis_tready_to_fifo2),        // input wire m_axis_tready
  .m_axis_tdata(master_fifo2_axis_tdata_to_uart),   // output wire [7 : 0] m_axis_tdata
  .m_axis_tlast(master_fifo2_axis_tlast_to_uart)    // output wire m_axis_tlast
);

//SWITCH
axis_switch_0 impl_switch (
    .aclk(clk),                                     // input wire aclk
    .aresetn(rstn),                                 // input wire aresetn
    .s_axis_tvalid(fifo_axis_tvalid_to_switch),     // input wire [1 : 0] s_axis_tvalid
    .s_axis_tready(sw_axis_tready_to_fifo),         // output wire [1 : 0] s_axis_tready
    .s_axis_tdata(fifo_axis_tdata_to_switch),       // input wire [15 : 0] s_axis_tdata
    .s_axis_tlast(fifo_axis_tlast_to_switch),       // input wire [1 : 0] s_axis_tlast
    .m_axis_tvalid(sw_axis_tvalid_to_middle),       // output wire [0 : 0] m_axis_tvalid
    .m_axis_tready(middle_axis_tready_to_switch),   // input wire [0 : 0] m_axis_tready
    .m_axis_tdata(sw_axis_tdata_to_middle),         // output wire [7 : 0] m_axis_tdata
    .m_axis_tlast(sw_axis_tlast_to_middle),         // output wire [0 : 0] m_axis_tlast
    .s_req_suppress(s_req_suppress),                // input wire [1 : 0] s_req_suppress
    .s_decode_err(s_decode_err)                     // output wire [1 : 0] s_decode_err
);

//MIDDLE
middle impl_middle(
    .clk(clk),
    .rst(rstn),

    //COLLEGAMENTI CON SWITCH
    .sw_axis_tvalid(sw_axis_tvalid_to_middle),      
    .sw_axis_tready(middle_axis_tready_to_switch),  
    .sw_axis_tdata(sw_axis_tdata_to_middle),        
    .sw_axis_tlast(sw_axis_tlast_to_middle),        

    //COLLEGAMENTI CON FIFO
    .fifo_axis_tvalid(middle_axis_tvalid_to_fifo2),     
    .fifo_axis_tready(fifo2_axis_tready_to_middle),
    .fifo_axis_tdata(middle_axis_tdata_to_fifo2),
    .fifo_axis_tlast(middle_axis_tlast_to_fifo2),

    //COLLEGAMENTI CON I2C x CMD
    .s_axis_cmd_address(middle_axis_cmd_address_to_I2C),
    .s_axis_cmd_start(middle_axis_cmd_start_to_I2C),
    .s_axis_cmd_read(middle_axis_cmd_read_to_I2C),
    .s_axis_cmd_write(middle_axis_cmd_write_to_I2C),
    .s_axis_cmd_write_multiple(middle_axis_cmd_write_multiple_to_I2C),
    .s_axis_cmd_stop(middle_axis_cmd_stop_to_I2C),
    .s_axis_cmd_valid(middle_axis_cmd_valid_to_I2C),
    .s_axis_cmd_ready(I2C_axis_cmd_ready_to_middle),

    //COLLEGAMENTI CON I2C x DATA(Implementato sotto)
    .s_axis_data_tdata(middle_axis_data_tdata_to_I2C),
    .s_axis_data_tvalid(middle_axis_data_tvalid_to_I2C),
    .s_axis_data_tready(I2C_axis_data_tready_to_middle), 
    .s_axis_data_tlast(middle_axis_data_tlast_to_I2C),

    //COLLEGAMENTI CON I2C x STATUS (Tutti INPUT)
    .busy(I2C_busy_to_middle),
    .bus_control(I2C_bus_control_to_middle),
    .bus_active(I2C_bus_active_to_middle),
    .missed_ack(I2C_missed_ack_to_middle),
    
    //COLLEGAMENTI I2C MASTER (Ricezione dati lettura)
    .m_axis_data_tdata(master_I2C_axis_data_tdata_to_middle), 
    .m_axis_data_tvalid(master_I2C_axis_data_tvalid_to_middle), 
    .m_axis_data_tready(middle_axis_data_tready_to_master_I2C), 
    .m_axis_data_tlast(master_I2C_axis_data_tlast_to_middle), 
    
    //COLLEGAMENTI CON I2C x CONFIGURAZIONE
    .prescale(middle_prescale_to_I2C),
    .stop_on_idle(stop_on_idle)
);

/*ILA (Integrated Logic Analyzer)
Modulo che registra dei campioni delle connessioni elencate e permette di analizzarli
tramite interfaccia grafica tramite Vivado*/

ila_0 impl_ila (
	.clk(clk),                                      // input wire clk

	.probe0(sw_axis_tdata_to_middle),               // input wire [7:0]  probe0  
	.probe1(middle_axis_cmd_address_to_I2C),        // input wire [6:0]  probe1 
	.probe2(middle_axis_data_tdata_to_I2C),         // input wire [7:0]  probe2 
	.probe3(sw_axis_tvalid_to_middle),              // input wire [0:0]  probe3 
	.probe4(middle_axis_tready_to_switch),          // input wire [0:0]  probe4 
	.probe5(sw_axis_tlast_to_middle),               // input wire [0:0]  probe5 
	.probe6(middle_axis_cmd_start_to_I2C),          // input wire [0:0]  probe6 
	.probe7(middle_axis_cmd_read_to_I2C),           // input wire [0:0]  probe7 
	.probe8(middle_axis_cmd_write_to_I2C),          // input wire [0:0]  probe8 
	.probe9(middle_axis_cmd_write_multiple_to_I2C), // input wire [0:0]  probe9 
	.probe10(middle_axis_cmd_stop_to_I2C),          // input wire [0:0]  probe10 
	.probe11(middle_axis_cmd_valid_to_I2C),         // input wire [0:0]  probe11 
	.probe12(I2C_axis_cmd_ready_to_middle),         // input wire [0:0]  probe12 
	.probe13(middle_axis_data_tvalid_to_I2C),       // input wire [0:0]  probe13 
	.probe14(I2C_axis_data_tready_to_middle),       // input wire [0:0]  probe14 
	.probe15(middle_axis_data_tlast_to_I2C),        // input wire [0:0]  probe15 
	.probe16(I2C_busy_to_middle),                   // input wire [0:0]  probe16 
	.probe17(I2C_bus_control_to_middle),            // input wire [0:0]  probe17 
	.probe18(I2C_bus_active_to_middle),             // input wire [0:0]  probe18 
	.probe19(I2C_missed_ack_to_middle),             // input wire [0:0]  probe19
    .probe20(master_I2C_axis_data_tdata_to_middle), // input wire [7:0]  probe20 
	.probe21(master_I2C_axis_data_tvalid_to_middle),// input wire [0:0]  probe21 
	.probe22(middle_axis_data_tready_to_master_I2C),// input wire [0:0]  probe22 
	.probe23(master_I2C_axis_data_tlast_to_middle), // input wire [0:0]  probe23
    .probe24(middle_axis_tdata_to_fifo2),           // input wire [7:0]  probe24 
	.probe25(master_fifo2_axis_tdata_to_uart),      // input wire [7:0]  probe25 
	.probe26(fifo2_axis_tready_to_middle),          // input wire [0:0]  probe26 
	.probe27(middle_axis_tlast_to_fifo2),           // input wire [0:0]  probe27 
	.probe28(master_fifo2_axis_tvalid_to_uart),     // input wire [0:0]  probe28 
	.probe29(uart_axis_tready_to_fifo2),            // input wire [0:0]  probe29 
	.probe30(master_fifo2_axis_tlast_to_uart),      // input wire [0:0]  probe30 
    .probe31(middle_axis_tvalid_to_fifo2),          // input wire [0:0]  probe30 
    .probe32(master_fifo2_axis_tlast_to_uart)       // input wire [0:0]  probe30 
);

//I2C MASTER
i2c_master impl_i2c_master(
    .clk(clk),
    .rst(rstn),
    .s_axis_cmd_address(middle_axis_cmd_address_to_I2C),
    .s_axis_cmd_start(middle_axis_cmd_start_to_I2C),
    .s_axis_cmd_read(middle_axis_cmd_read_to_I2C),
    .s_axis_cmd_write(middle_axis_cmd_write_to_I2C),
    .s_axis_cmd_write_multiple(middle_axis_cmd_write_multiple_to_I2C),
    .s_axis_cmd_stop(middle_axis_cmd_stop_to_I2C),
    .s_axis_cmd_valid(middle_axis_cmd_valid_to_I2C),
    .s_axis_cmd_ready(I2C_axis_cmd_ready_to_middle),
    .s_axis_data_tdata(middle_axis_data_tdata_to_I2C),
    .s_axis_data_tvalid(middle_axis_data_tvalid_to_I2C),
    .s_axis_data_tready(I2C_axis_data_tready_to_middle),
    .s_axis_data_tlast(middle_axis_data_tlast_to_I2C),
    .m_axis_data_tdata(master_I2C_axis_data_tdata_to_middle),
    .m_axis_data_tvalid(master_I2C_axis_data_tvalid_to_middle),
    .m_axis_data_tready(middle_axis_data_tready_to_master_I2C),
    .m_axis_data_tlast(master_I2C_axis_data_tlast_to_middle),
    .scl_i(scl_i),
    .sda_i(sda_i),
    .scl_o(scl_o),
    .scl_t(scl_t),
    .sda_o(sda_o),
    .sda_t(sda_t),
    .busy(I2C_busy_to_middle),
    .bus_control(I2C_bus_control_to_middle),
    .bus_active(I2C_bus_active_to_middle),
    .missed_ack(I2C_missed_ack_to_middle),
    .prescale(middle_prescale_to_I2C),
    .stop_on_idle(stop_on_idle)
);

/*I due moduli seguenti gestiscono autonomamente i tristate pin che produce il modulo I2C MASTER*/
//IOBUF SCL
IOBUF #(
   .DRIVE(12),                  // Specify the output drive strength
   .IBUF_LOW_PWR("TRUE"),       // Low Power - "TRUE", High Performance = "FALSE"
   .IOSTANDARD("DEFAULT"),      // Specify the I/O standard
   .SLEW("SLOW")                // Specify the output slew rate
) IOBUF_inst_SCL (
   .O(scl_i),                   // Buffer output
   .IO(scl),                    // Buffer inout port (connect directly to top-level port)
   .I(scl_o),                   // Buffer input
   .T(scl_tT)                   // 3-state enable input, high=input, low=output
);

//IOBUF SDA
IOBUF #(
   .DRIVE(12),                  // Specify the output drive strength
   .IBUF_LOW_PWR("TRUE"),       // Low Power - "TRUE", High Performance = "FALSE"
   .IOSTANDARD("DEFAULT"),      // Specify the I/O standard
   .SLEW("SLOW")                // Specify the output slew rate
) IOBUF_inst_SDA (
   .O(sda_i),                   // Buffer output
   .IO(sda),                    // Buffer inout port (connect directly to top-level port)
   .I(sda_o),                   // Buffer input
   .T(sda_t)                    // 3-state enable input, high=input, low=output
);

endmodule