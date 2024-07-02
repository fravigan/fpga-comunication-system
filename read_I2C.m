function [to_read_vec] = read_I2C(adr, numb, mode_i2c)
    to_read_vec(1) = 0x00;              %Primo cmd laster

    %Impostazione della Modalità di funzionamento dell'I2C
    if(mode_i2c == "S")
        to_read_vec(2) = 0b00010010;    %Standard Mode
    elseif(mode_i2c == "F")
        to_read_vec(2) = 0b00100010;    %Fast Mode
    elseif(mode_i2c == "FP")
        to_read_vec(2) = 0b01000010;    %Fast Mode Plus
    end

    to_read_vec(3) = 0x00;              %Secondo cmd laster
    to_read_vec(4) = uint8(adr);        %Indirizzo della periferica I2C
    to_read_vec(5) = 0b1;               %Terzo ed ultimo cmd lasteR
    to_read_vec(6) = uint8(numb);       %Numero bytes da leggere
end