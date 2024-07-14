Programmi sviluppati per un sistema di comunicazione PC <-> FPGA <-> I2C sviluppato per un progetto di Tesi Triennale in Fisica con titolo "Sistema di comunicazione ottico basato su FPGA per l’esperimento CUPID".
I programmi qui presenti devono essere completati con IP fornite da Xilinx Vivado per il corretto funzionamento.
Si riporta anche uno schema del design con tutti gli IP utilizzati.

VERILOG CODES

top.v -> modulo per collegare i moduli tra di loro

laster.v -> modulo per impostare il last ai moduli successivi

middle.v -> modulo per elaborare il comando da pc ed istruire l'i2c per comunicare con periferiche

i2c_master.v -> modulo per l'i2c by alexforencich (https://github.com/alexforencich/verilog-i2c/tree/master)

uart_tx.v -> modulo per la trasmissione di dati tramite protocollo seriale by ben-marshall (https://github.com/ben-marshall/uart)

uart_rx.v -> modulo per la ricezione di dati tramite protocollo seriale by ben-marshall (https://github.com/ben-marshall/uart)



TESTBENCHE CODES

I2C_middle_tb_R_W.v -> testbench minimale per testare la ricezione di comandi e dati del middle.v


MATLAB CODES

write_I2C.m -> funzione Matlab per generare la sequenza di bytes necessaria ad avviare la scrittura su una periferica I2C

read_I2C.m -> funzione Matlab per generare la sequenza di bytes necessaria ad avviare la lettura su una periferica I2C

term_display_test.mlx -> script Matlab che invia le sequenze di bytes all'FPGA tramite protocollo seriale UART ed attende eventuali bytes letti da una periferica 

CS.mlx -> script Matlab per verificare lo status dell' I2C tramite il comando Controllo Status (CS)

SCHEMA

design.png -> Schema del design
