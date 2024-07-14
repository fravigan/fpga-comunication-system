transcript off
onbreak {quit -force}
onerror {quit -force}
transcript on

vlib work
vmap -link {C:/Users/Francesco/Documents/TESI/Progetti/I2C_CONNECTION_MOD/I2C_CONNECTION_MOD.cache/compile_simlib/activehdl}
vlib activehdl/axis_infrastructure_v1_1_0
vlib activehdl/axis_register_slice_v1_1_28
vlib activehdl/axis_switch_v1_1_28
vlib activehdl/xil_defaultlib

vlog -work axis_infrastructure_v1_1_0  -v2k5 "+incdir+../../../ipstatic/hdl" -l axis_infrastructure_v1_1_0 -l axis_register_slice_v1_1_28 -l axis_switch_v1_1_28 -l xil_defaultlib \
"../../../ipstatic/hdl/axis_infrastructure_v1_1_vl_rfs.v" \

vlog -work axis_register_slice_v1_1_28  -v2k5 "+incdir+../../../ipstatic/hdl" -l axis_infrastructure_v1_1_0 -l axis_register_slice_v1_1_28 -l axis_switch_v1_1_28 -l xil_defaultlib \
"../../../ipstatic/hdl/axis_register_slice_v1_1_vl_rfs.v" \

vlog -work axis_switch_v1_1_28  -v2k5 "+incdir+../../../ipstatic/hdl" -l axis_infrastructure_v1_1_0 -l axis_register_slice_v1_1_28 -l axis_switch_v1_1_28 -l xil_defaultlib \
"../../../ipstatic/hdl/axis_switch_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../ipstatic/hdl" -l axis_infrastructure_v1_1_0 -l axis_register_slice_v1_1_28 -l axis_switch_v1_1_28 -l xil_defaultlib \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_switch_0/sim/axis_switch_0.v" \


vlog -work xil_defaultlib \
"glbl.v"

