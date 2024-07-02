function [to_write_vec] = write_I2C(adr, write_vec, mode_i2c)
    to_write_vec(1) = 0x00;             %Primo cmd laster
    
    %Impostazione della Modalità di funzionamento dell'I2C
    if(mode_i2c == "S")
        to_write_vec(2) = 0b00011100;   %Standard Mode
    elseif(mode_i2c == "F")
        to_write_vec(2) = 0b00101100;   %Fast Mode
    elseif(mode_i2c == "FP")
        to_write_vec(2) = 0b01001100;   %Fast Mode Plus
    end

    to_write_vec(3) = 0x00;             %Secondo cmd laster
    to_write_vec(4) = uint8(adr);       %Indirizzo della periferica I2C

    %Vengono messi in coda i by7tes da scrivere, alternati dagli
    %zeri per il cmd laster.
    to_write_vec(6:2:2*length(write_vec)+4) = write_vec;
    
    %Nella penultima posizione del vettore viene inserito 
    %il cmd laster = 1 
    to_write_vec(end-1) = 0b1;
end

