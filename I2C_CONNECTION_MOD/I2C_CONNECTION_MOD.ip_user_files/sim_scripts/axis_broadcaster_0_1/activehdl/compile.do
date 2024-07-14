transcript off
onbreak {quit -force}
onerror {quit -force}
transcript on

vlib work
vmap -link {C:/Users/Francesco/Documents/TESI/Progetti/I2C_CONNECTION_MOD/I2C_CONNECTION_MOD.cache/compile_simlib/activehdl}
vlib activehdl/xpm
vlib activehdl/axis_infrastructure_v1_1_0
vlib activehdl/xil_defaultlib
vlib activehdl/axis_broadcaster_v1_1_27

vlog -work xpm  -sv2k12 "+incdir+../../../ipstatic/hdl" -l xpm -l axis_infrastructure_v1_1_0 -l xil_defaultlib -l axis_broadcaster_v1_1_27 \
"C:/Xilinx/Vivado/2023.1/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \

vcom -work xpm -93  \
"C:/Xilinx/Vivado/2023.1/data/ip/xpm/xpm_VCOMP.vhd" \

vlog -work axis_infrastructure_v1_1_0  -v2k5 "+incdir+../../../ipstatic/hdl" -l xpm -l axis_infrastructure_v1_1_0 -l xil_defaultlib -l axis_broadcaster_v1_1_27 \
"../../../ipstatic/hdl/axis_infrastructure_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../ipstatic/hdl" -l xpm -l axis_infrastructure_v1_1_0 -l xil_defaultlib -l axis_broadcaster_v1_1_27 \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_1/hdl/tdata_axis_broadcaster_0.v" \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_1/hdl/tuser_axis_broadcaster_0.v" \

vlog -work axis_broadcaster_v1_1_27  -v2k5 "+incdir+../../../ipstatic/hdl" -l xpm -l axis_infrastructure_v1_1_0 -l xil_defaultlib -l axis_broadcaster_v1_1_27 \
"../../../ipstatic/hdl/axis_broadcaster_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../ipstatic/hdl" -l xpm -l axis_infrastructure_v1_1_0 -l xil_defaultlib -l axis_broadcaster_v1_1_27 \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_1/hdl/top_axis_broadcaster_0.v" \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_1/sim/axis_broadcaster_0.v" \

vlog -work xil_defaultlib \
"glbl.v"

