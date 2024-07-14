transcript off
onbreak {quit -force}
onerror {quit -force}
transcript on

asim +access +r +m+axis_switch_0  -L axis_infrastructure_v1_1_0 -L axis_register_slice_v1_1_28 -L axis_switch_v1_1_28 -L xil_defaultlib -L unisims_ver -L unimacro_ver -L secureip -O5 xil_defaultlib.axis_switch_0 xil_defaultlib.glbl

do {axis_switch_0.udo}

run 1000ns

endsim

quit -force
