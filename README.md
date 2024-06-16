Programmi in Verilog per un sistema di comunicazione PC <-> I2C sviluppato per un progetto di tesi triennale in Fisica.
I programmi qui presenti devono essere completati con IP fornite da Xilinx Vivado per il corretto funzionamento.

top.v -> modulo per collegare i moduli tra di loro
laster.v -> modulo per impostare il last ai moduli successivi
middle.v -> modulo per elaborare il comando da pc ed istruire l'i2c per comunicare con periferiche
i2c_master.v -> modulo per l'i2c
uart_tx.v -> modulo per la trasmissione di dati tramite protocollo seriale
uart_rx.v -> modulo per la ricezione di dati tramite protocollo seriale
