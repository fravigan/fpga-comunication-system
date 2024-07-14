vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/axis_infrastructure_v1_1_0
vlib modelsim_lib/msim/axis_register_slice_v1_1_28
vlib modelsim_lib/msim/axis_switch_v1_1_28
vlib modelsim_lib/msim/xil_defaultlib

vmap axis_infrastructure_v1_1_0 modelsim_lib/msim/axis_infrastructure_v1_1_0
vmap axis_register_slice_v1_1_28 modelsim_lib/msim/axis_register_slice_v1_1_28
vmap axis_switch_v1_1_28 modelsim_lib/msim/axis_switch_v1_1_28
vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib

vlog -work axis_infrastructure_v1_1_0  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../ipstatic/hdl/axis_infrastructure_v1_1_vl_rfs.v" \

vlog -work axis_register_slice_v1_1_28  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../ipstatic/hdl/axis_register_slice_v1_1_vl_rfs.v" \

vlog -work axis_switch_v1_1_28  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../ipstatic/hdl/axis_switch_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_switch_0/sim/axis_switch_0.v" \


vlog -work xil_defaultlib \
"glbl.v"

