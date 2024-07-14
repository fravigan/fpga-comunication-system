vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/axis_infrastructure_v1_1_0
vlib modelsim_lib/msim/xil_defaultlib
vlib modelsim_lib/msim/axis_broadcaster_v1_1_27

vmap axis_infrastructure_v1_1_0 modelsim_lib/msim/axis_infrastructure_v1_1_0
vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib
vmap axis_broadcaster_v1_1_27 modelsim_lib/msim/axis_broadcaster_v1_1_27

vlog -work axis_infrastructure_v1_1_0  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../ipstatic/hdl/axis_infrastructure_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_2/hdl/tdata_axis_broadcaster_0.v" \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_2/hdl/tuser_axis_broadcaster_0.v" \

vlog -work axis_broadcaster_v1_1_27  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../ipstatic/hdl/axis_broadcaster_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu  "+incdir+../../../ipstatic/hdl" \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_2/hdl/top_axis_broadcaster_0.v" \
"../../../../I2C_CONNECTION_MOD.gen/sources_1/ip/axis_broadcaster_0_2/sim/axis_broadcaster_0.v" \

vlog -work xil_defaultlib \
"glbl.v"

