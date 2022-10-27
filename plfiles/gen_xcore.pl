#!/usr/bin/perl

#PL script to generate core customisation and invoke xilinx coregen

#$core_type = $ARGV[0];

#$core_name = $ARGV[1];

#$core_inputa_bw = $ARGV[2];

#$core_inputb_bw = $ARGV[3];

#$core_reg_inputs = $ARGV[4];

#gen_xcore_main(  $ARGV[0] , $ARGV[1] , $ARGV[2], $ARGV[3], $ARGV[4]);

sub gen_xcore_main{

$core_type = shift( @_ );

$core_name = shift( @_ );

$core_inputa_bw = shift( @_ );

$core_inputb_bw = shift( @_ );

$core_reg_inputs = shift( @_ );

$core_mult_cvalue = shift( @_ );

$FPGA_family = $ENV{FPGA_ARCH};

$xcore_global_settings = (<<INFO);

# CoreGen Global Settings
SET BusFormat = BusFormatParenNotRipped
SET SimulationOutputProducts = VHDL
SET XilinxFamily = Virtex2
SET OutputOption = DesignFlow
SET DesignFlow = VHDL
SET FlowVendor = Synplicity
SET FormalVerification = None
SET OverwriteFiles = True

INFO

$ctime_string = gmtime;

#cd print "$ctime_string\n";

gen_xcore_prj();

$xcore_setting_file = "xcore_set.xco";

$xcore_name = $core_name;

gen_xcore_name() if ( $core_name =~ /auto/ );

gen_mult_core()  if ( $core_type =~ /^[mM]ul/ );

gen_add_core() if ( $core_type =~ /[aA]dd/ );

gen_regcore() if ( $core_type =~ /[rR]eg/ );

gen_cmult_core() if ( $core_type =~ /cmul/ );

print "Generating $xcore_name\n";

run_xcoregen();

}

sub gen_xcore_name{

$xcore_name = "$core_type\_$core_inputa_bw"."x$core_inputb_bw";

$xcore_name = $xcore_name."_clk" if ( $core_reg_inputs =~ /true/ );

$xcore_name = "$core_type\_$core_inputa_bw"  if ( $core_type =~ /reg/ );

}


sub gen_mult_core{

$xcore_type = "Multiplier Virtex2 Xilinx,_Inc. 7.0";

$xcore_output_bw = $core_inputa_bw + $core_inputb_bw;

$xcore_mult_set = (<<INFO);

# Mult Specific Settings
SELECT $xcore_type
CSET component_name = $xcore_name
CSET port_a_width = $core_inputa_bw
CSET port_b_width = $core_inputb_bw
CSET register_input = $core_reg_inputs

CSET output_width = $xcore_output_bw

CSET create_rpm = $ENV{BS_XCORE_MUL_RPM}
CSET clock_enable = false
CSET output_options = Non_Registered
CSET port_b_constant = false
CSET port_a_input = Parallel
CSET reloadable = false

CSET nd = false
CSET multiplier_type = Parallel

CSET reload_options = Stop_During_Reload
CSET ce_overrides = CE_Overrides_SCLR
CSET memory_type = Distributed_Memory
CSET multiplier_construction = Use_LUTs
CSET port_a_data = Unsigned
CSET virtex2_multiplier_optimization = Area
CSET rdy = false

CSET port_b_data = Signed
CSET synchronous_clear = false
CSET asynchronous_clear = false
CSET load_done_output = false
CSET rfd = false
CSET clk_cycles_per_input = 1
CSET style = Rectangular_Shape
CSET port_b_constant_value = 1
CSET pipelined = Minimum
CSET output_hold_register = false

GENERATE


INFO

open( XCFILE , ">$xcore_setting_file" );

print XCFILE  "$xcore_global_settings\n $xcore_mult_set\n";

close(XCFILE);

}

sub gen_cmult_core{

$xcore_type = "Multiplier Virtex2 Xilinx,_Inc. 7.0";

$xcore_output_bw = $core_inputa_bw + $core_inputb_bw;

$xcore_mult_set = (<<INFO);

# Mult Specific Settings
SELECT $xcore_type
CSET component_name = $xcore_name
CSET port_a_width = $core_inputa_bw
CSET port_b_width = $core_inputb_bw
CSET register_input = $core_reg_inputs

CSET output_width = $xcore_output_bw

CSET create_rpm = $ENV{BS_XCORE_CMUL_RPM}
CSET clock_enable = false
CSET output_options = Non_Registered
CSET port_b_constant = true
CSET port_a_input = Parallel
CSET reloadable = false

CSET nd = false
CSET multiplier_type = Parallel

CSET reload_options = Stop_During_Reload
CSET ce_overrides = CE_Overrides_SCLR
CSET memory_type = Distributed_Memory
CSET multiplier_construction = Use_LUTs
CSET port_a_data = Unsigned
CSET virtex2_multiplier_optimization = Area
CSET rdy = false

CSET port_b_data = Signed
CSET synchronous_clear = false
CSET asynchronous_clear = false
CSET load_done_output = false
CSET rfd = false
CSET clk_cycles_per_input = 1
CSET style = Rectangular_Shape
CSET port_b_constant_value = $core_mult_cvalue
CSET pipelined = Minimum
CSET output_hold_register = false

GENERATE


INFO

open( XCFILE , ">$xcore_setting_file" );

print XCFILE  "$xcore_global_settings\n $xcore_mult_set\n";

close(XCFILE);

}

sub gen_add_core{

$xcore_type = "Adder_Subtracter Virtex2 Xilinx,_Inc. 7.0" ;

$xcore_output_bw = max( $core_inputa_bw , $core_inputb_bw ) + 1;


$xcore_add_set = (<<INFO);

# Adder Specific Settings
SELECT $xcore_type
CSET component_name = $xcore_name

CSET port_a_width = $core_inputa_bw
CSET port_b_width = $core_inputb_bw

CSET output_width = $xcore_output_bw

CSET async_init_value = 0
CSET create_rpm = $ENV{BS_XCORE_ADD_RPM}
CSET clock_enable = false
CSET output_options = non_registered
CSET overflow_output = false
CSET port_b_constant = false
CSET ce_overrides = sync_controls_override_ce
CSET bypass_sense = active_high
CSET port_a_sign = unsigned
CSET ce_override_for_bypass = true
CSET sync_init_value = 0
CSET operation = add
CSET latency = 0
CSET port_b_sign = unsigned

CSET asynchronous_settings = none
CSET bypass = false
CSET carry_borrow_input = false
CSET carry_borrow_output = false
CSET port_b_constant_value = 0
CSET set_clear_priority = clear_overrides_set
CSET synchronous_settings = none

GENERATE

INFO

open( XCFILE , ">$xcore_setting_file" );

print XCFILE "$xcore_global_settings\n $xcore_add_set\n";

close(XCFILE);

}



sub gen_regcore{

$xcore_reg_set = (<<INFO);

SELECT FD-based_Parallel_Register Virtex2 Xilinx,_Inc. 7.0
CSET component_name = $xcore_name
CSET data_width = $core_inputa_bw

CSET async_init_value = 0
CSET set_clear_priority = clear_overrides_set
CSET ce_overrides = sync_controls_override_ce
CSET sync_init_value = 0
CSET clock_enable = false
CSET create_rpm = $ENV{BS_XCORE_REG_RPM}
CSET asynchronous_settings = none
GENERATE

INFO

open( XCFILE , ">$xcore_setting_file" );

print XCFILE "$xcore_global_settings\n $xcore_reg_set\n";

close(XCFILE);

}

sub run_xcoregen{

$os_name = qx{ uname -o };

system( "coregen.exe -b $xcore_setting_file" ) if ( $os_name =~ /Cygwin/ );

system( "coregen -b $xcore_setting_file" ) if ( $os_name =~ /GNU\/Linux/ );


}

sub max {
    if ($_[0]<$_[1]) {return $_[1]} else {return $_[0]};
}


sub gen_xcore_prj{

my $ctime_string = gmtime;

open( XCOREPRJ , ">coregen.prj" );

print XCOREPRJ (<<INFO);

#COREGen Project File
#$ctime_string
overwritefiles=True
blkmemsp_v5_0|Xilinx,\ Inc.|5.0=false
sid_v3_1|Xilinx,\ Inc.|3.1=false
C_DIST_MEM_V7_0|Xilinx,\ Inc.|7.0=false
decode_8b10b_v5_0|Xilinx,\ Inc.|5.0=false
viterbi_v4_0|Xilinx,\ Inc.|4.0=true
C_REG_FD_V6_0|Xilinx,\ Inc.|6.0=false
C_REG_FD_V3_0|Xilinx,\ Inc.|3.0=false
C_MUX_BIT_V5_0|Xilinx,\ Inc.|5.0=false
convolution_v2_0|Xilinx,\ Inc.|2.0=false
pci32_v3_0|Xilinx,\ Inc.|3.0=true
C_TWOS_COMP_V4_0|Xilinx,\ Inc.|4.0=false
C_SHIFT_FD_V6_0|Xilinx,\ Inc.|6.0=false
xlnx_PCI32sII|Xilinx,\ Inc.|3.0=false
gig_eth_pcs_pma_v4_0|Xilinx,\ Inc.|4.0=true
C_DDS_V4_2|Xilinx,\ Inc.|4.2=false
C_MAC_V4_0|Xilinx,\ Inc.|4.0=false
ten_gig_eth_mac_v4_0|Xilinx,\ Inc.|4.0=true
C_ACCUM_V5_0|Xilinx,\ Inc.|5.0=false
C_GATE_BIT_BUS_V6_0|Xilinx,\ Inc.|6.0=false
C_GATE_BIT_BUS_V3_0|Xilinx,\ Inc.|3.0=false
xlnx_pci_express_x1|Xilinx,\ Inc.|1.0=true
xilinxfamily=Virtex2
C_DIST_MEM_V7_1|Xilinx,\ Inc.|7.1=true
C_COUNTER_BINARY_V5_0|Xilinx,\ Inc.|5.0=false
C_DIST_MEM_V4_1|Xilinx,\ Inc.|4.1=false
C_ADDSUB_V4_0|Xilinx,\ Inc.|4.0=false
C_COUNTER_BINARY_V2_0|Xilinx,\ Inc.|2.0=false
outputoption=DesignFlow
C_MUX_SLICE_BUFE_V2_0|Xilinx,\ Inc.|2.0=false
dividervht|Xilinx,\ Inc.|2.0=true
xfft_v3_0|Xilinx,\ Inc.|3.0=true
C_GATE_BIT_V4_0|Xilinx,\ Inc.|4.0=false
simvendor=ModelSim
mult_gen_v6_0|Xilinx,\ Inc.|6.0=false
pcix64_v5_0|Xilinx,\ Inc.|5.0=true
C_MUX_BUS_V2_0|Xilinx,\ Inc.|2.0=false
C_TWOS_COMP_V7_0|Xilinx,\ Inc.|7.0=false
vfft64v2|Xilinx,\ Inc.|2.0=true
C_DA_FIR_V7_0|Xilinx,\ Inc.|7.0=false
C_REG_LD_V6_0|Xilinx,\ Inc.|6.0=false
mac_v2_0|Xilinx,\ Inc.|2.0=true
C_REG_LD_V3_0|Xilinx,\ Inc.|3.0=false
C_ADDSUB_V7_0|Xilinx,\ Inc.|7.0=true
sid_v2_0|Xilinx,\ Inc.|2.0=false
C_SHIFT_RAM_V6_0|Xilinx,\ Inc.|6.0=false
C_MUX_SLICE_BUFT_V2_0|Xilinx,\ Inc.|2.0=false
C_MUX_SLICE_BUFE_V5_0|Xilinx,\ Inc.|5.0=false
C_SHIFT_RAM_V3_0|Xilinx,\ Inc.|3.0=false
cmpy_v1_0|Xilinx,\ Inc.|1.0=true
C_GATE_BIT_V7_0|Xilinx,\ Inc.|7.0=true
C_MUX_BUS_V5_0|Xilinx,\ Inc.|5.0=false
rio_v1_3|Xilinx,\ Inc.|1.3=true
C_SIN_COS_V5_0|Xilinx,\ Inc.|5.0=true
outputproducts=ImpNetlist;VHDLSim
cam_v4_0|Xilinx,\ Inc.|4.0=false
sdivider_v3_0|Xilinx,\ Inc.|3.0=true
C_MAC_FIR_V2_0|Xilinx,\ Inc.|2.0=false
C_COMPARE_V4_0|Xilinx,\ Inc.|4.0=false
C_DA_2D_DCT_V2_0|Xilinx,\ Inc.|2.0=true
C_BIT_CORRELATOR_V3_0|Xilinx,\ Inc.|3.0=true
cordic_v2_0|Xilinx,\ Inc.|2.0=false
xlnx_PCI64v|Xilinx,\ Inc.|3.0=false
fileversion=4
C_DIST_MEM_V3_0|Xilinx,\ Inc.|3.0=false
viterbi_v3_0|Xilinx,\ Inc.|3.0=false
decode_8b10b_v4_0|Xilinx,\ Inc.|4.0=false
C_MUX_SLICE_BUFT_V5_0|Xilinx,\ Inc.|5.0=false
encode_8b10b_v4_0|Xilinx,\ Inc.|4.0=false
C_REG_FD_V2_0|Xilinx,\ Inc.|2.0=false
async_fifo_v6_0|Xilinx,\ Inc.|6.0=true
xapp265|Xilinx,\ Inc.|1.1=true
C_DECODE_BINARY_V5_0|Xilinx,\ Inc.|5.0=false
C_DECODE_BINARY_V2_0|Xilinx,\ Inc.|2.0=false
rs_encoder_v5_0|Xilinx,\ Inc.|5.0=true
rs_decoder_v4_0|Xilinx,\ Inc.|4.0=false
C_SHIFT_FD_V2_0|Xilinx,\ Inc.|2.0=false
blkmemdp_v6_0|Xilinx,\ Inc.|6.0=false
C_GATE_BUS_V4_0|Xilinx,\ Inc.|4.0=false
blkmemdp_v3_0|Xilinx,\ Inc.|3.0=false
posphyl3_link_v1_0|Xilinx,\ Inc.|3.2=true
C_MAC_V3_0|Xilinx,\ Inc.|3.0=true
sync_fifo_v2_0|Xilinx,\ Inc.|2.0=false
C_MAC_FIR_V5_0|Xilinx,\ Inc.|5.0=true
C_COMPARE_V7_0|Xilinx,\ Inc.|7.0=true
pci64_v3_0|Xilinx,\ Inc.|3.0=true
blkmemsp_v4_0|Xilinx,\ Inc.|4.0=false
C_DIST_MEM_V6_0|Xilinx,\ Inc.|6.0=false
C_REG_FD_V5_0|Xilinx,\ Inc.|5.0=false
C_MUX_BIT_V7_0|Xilinx,\ Inc.|7.0=true
C_MUX_BIT_V4_0|Xilinx,\ Inc.|4.0=false
convolution_v4_0|Xilinx,\ Inc.|4.0=true
aurora_v1_0|Xilinx,\ Inc.|2.0=true
C_TWOS_COMP_V3_0|Xilinx,\ Inc.|3.0=false
posphyl4_v6_0_1|Xilinx,\ Inc.|6.0.1=false
C_SHIFT_FD_V5_0|Xilinx,\ Inc.|5.0=false
C_GATE_BUS_V7_0|Xilinx,\ Inc.|7.0=true
C_ACCUM_V7_0|Xilinx,\ Inc.|7.0=true
sync_fifo_v5_0|Xilinx,\ Inc.|5.0=true
formalverification=None
C_ACCUM_V4_0|Xilinx,\ Inc.|4.0=false
C_GATE_BIT_BUS_V5_0|Xilinx,\ Inc.|5.0=false
C_GATE_BIT_BUS_V2_0|Xilinx,\ Inc.|2.0=false
vfft1024|Xilinx,\ Inc.|1.0=true
C_COUNTER_BINARY_V4_0|Xilinx,\ Inc.|4.0=false
C_ADDSUB_V3_0|Xilinx,\ Inc.|3.0=false
vfft1024v2|Xilinx,\ Inc.|2.0=true
C_MUX_SLICE_BUFE_V4_0|Xilinx,\ Inc.|4.0=false
C_SHIFT_RAM_V2_0|Xilinx,\ Inc.|2.0=false
xfft_v2_0|Xilinx,\ Inc.|2.0=false
designflow=VHDL
C_GATE_BIT_V3_0|Xilinx,\ Inc.|3.0=false
rs_decoder_v4_1|Xilinx,\ Inc.|4.1=false
mult_gen_v5_0|Xilinx,\ Inc.|5.0=false
blkmemdp_v6_1|Xilinx,\ Inc.|6.1=true
C_TWOS_COMP_V6_0|Xilinx,\ Inc.|6.0=true
xlnx_pci64_dk|Xilinx,\ Inc.|1.0=true
vfft64|Xilinx,\ Inc.|1.0=true
C_DDC_V1_0|Xilinx,\ Inc.|1.0=true
C_DA_FIR_V9_0|Xilinx,\ Inc.|9.0=true
C_DA_FIR_V6_0|Xilinx,\ Inc.|6.0=false
C_DDS_V5_0|Xilinx,\ Inc.|5.0=true
C_REG_LD_V5_0|Xilinx,\ Inc.|5.0=false
C_REG_LD_V2_0|Xilinx,\ Inc.|2.0=false
C_COUNTER_BINARY_V7_0|Xilinx,\ Inc.|7.0=false
lfsr_v3_0|Xilinx,\ Inc.|3.0=true
lockedprops=
C_ADDSUB_V6_0|Xilinx,\ Inc.|6.0=false
xfft_v2_1|Xilinx,\ Inc.|2.1=false
C_CIC_V3_0|Xilinx,\ Inc.|3.0=true
C_MUX_SLICE_BUFE_V7_0|Xilinx,\ Inc.|7.0=true
C_SHIFT_RAM_V5_0|Xilinx,\ Inc.|5.0=false
async_fifo_v5_0|Xilinx,\ Inc.|5.0=false
blkmemdp_v3_2|Xilinx,\ Inc.|3.2=false
C_GATE_BIT_V6_0|Xilinx,\ Inc.|6.0=false
simulationoutputproducts=VHDL
C_MUX_BUS_V7_0|Xilinx,\ Inc.|7.0=false
C_MUX_BUS_V4_0|Xilinx,\ Inc.|4.0=false
C_SIN_COS_V4_0|Xilinx,\ Inc.|4.0=false
HDLC32_catalog|Xilinx,\ Inc.|1.0=true
busformat=BusFormatParenNotRipped
cam_v3_0|Xilinx,\ Inc.|3.0=false
vfft256v2|Xilinx,\ Inc.|2.0=true
C_MAC_FIR_V1_0|Xilinx,\ Inc.|1.0=false
C_COMPARE_V3_0|Xilinx,\ Inc.|3.0=false
vfft16|Xilinx,\ Inc.|1.0=true
xlnx_PCI64s2|Xilinx,\ Inc.|3.0=false
cordic_v1_0|Xilinx,\ Inc.|1.0=false
flowvendor=Synplicity
posphyl4_v6_1|Xilinx,\ Inc.|6.1=true
posphyl4_lite_v2_0_1|Xilinx,\ Inc.|2.0.1=true
magicnumber=-1172307782
viterbi_v2_0|Xilinx,\ Inc.|2.0=false
C_MUX_SLICE_BUFT_V7_0|Xilinx,\ Inc.|7.0=true
sid_v4_0|Xilinx,\ Inc.|4.0=true
C_DIST_MEM_V2_0|Xilinx,\ Inc.|2.0=false
C_MUX_SLICE_BUFT_V4_0|Xilinx,\ Inc.|4.0=false
encode_8b10b_v3_0|Xilinx,\ Inc.|3.0=false
fifo_generator_v1_0|Xilinx,\ Inc.|1.0=false
HDLC1_catalog|Xilinx,\ Inc.|1.0=true
C_SIN_COS_V4_1|Xilinx,\ Inc.|4.1=false
C_DECODE_BINARY_V4_0|Xilinx,\ Inc.|4.0=false
rs_decoder_v3_0|Xilinx,\ Inc.|3.0=false
rs_encoder_v4_0|Xilinx,\ Inc.|4.0=false
blkmemdp_v5_0|Xilinx,\ Inc.|5.0=false
C_GATE_BUS_V3_0|Xilinx,\ Inc.|3.0=false
sync_fifo_v4_0|Xilinx,\ Inc.|4.0=false
C_MAC_FIR_V4_0|Xilinx,\ Inc.|4.0=false
tcc_decoder_v1_0|Xilinx,\ Inc.|1.0=true
C_ACCUM_V3_0|Xilinx,\ Inc.|3.0=false
sync_fifo_v1_0|Xilinx,\ Inc.|1.0=false
C_COMPARE_V6_0|Xilinx,\ Inc.|6.0=false
cordic_v1_1|Xilinx,\ Inc.|1.1=false
blkmemsp_v3_0|Xilinx,\ Inc.|3.0=false
fifo_generator_v1_1|Xilinx,\ Inc.|1.1=true
async_fifo_v5_1|Xilinx,\ Inc.|5.1=false
decode_8b10b_v6_0|Xilinx,\ Inc.|6.0=true
C_DIST_MEM_V5_0|Xilinx,\ Inc.|5.0=false
C_REG_FD_V7_0|Xilinx,\ Inc.|7.0=true
C_MUX_BIT_V6_0|Xilinx,\ Inc.|6.0=false
C_REG_FD_V4_0|Xilinx,\ Inc.|4.0=false
C_MUX_BIT_V3_0|Xilinx,\ Inc.|3.0=false
convolution_v3_0|Xilinx,\ Inc.|3.0=false
rs_encoder_v4_1|Xilinx,\ Inc.|4.1=false
mult_gen_v4_0|Xilinx,\ Inc.|4.0=false
xfft1024_v1_1|Xilinx,\ Inc.|1.1=true
C_DECODE_BINARY_V7_0|Xilinx,\ Inc.|7.0=true
C_SHIFT_FD_V7_0|Xilinx,\ Inc.|7.0=true
C_TWOS_COMP_V2_0|Xilinx,\ Inc.|2.0=false
flexbus4_v1_0|Xilinx,\ Inc.|1.0=false
C_SHIFT_FD_V4_0|Xilinx,\ Inc.|4.0=false
C_GATE_BUS_V6_0|Xilinx,\ Inc.|6.0=false
C_ACCUM_V6_0|Xilinx,\ Inc.|6.0=false
C_GATE_BIT_BUS_V7_0|Xilinx,\ Inc.|7.0=true
C_GATE_BIT_BUS_V4_0|Xilinx,\ Inc.|4.0=false
C_DIST_MEM_V5_1|Xilinx,\ Inc.|5.1=false
C_COUNTER_BINARY_V3_0|Xilinx,\ Inc.|3.0=false
lfsr_v2_0|Xilinx,\ Inc.|2.0=false
blkmemsp_v6_0|Xilinx,\ Inc.|6.0=false
C_ADDSUB_V5_0|Xilinx,\ Inc.|5.0=false
C_ADDSUB_V2_0|Xilinx,\ Inc.|2.0=false
C_SIN_COS_V4_2|Xilinx,\ Inc.|4.2=false
C_MUX_SLICE_BUFE_V3_0|Xilinx,\ Inc.|3.0=false
xaui_v4_0|Xilinx,\ Inc.|4.0=true
vfft16v2|Xilinx,\ Inc.|2.0=true
xlnx_PCIX64_virtex_e_ii|Xilinx,\ Inc.|5.0=false
C_GATE_BIT_V5_0|Xilinx,\ Inc.|5.0=false
C_GATE_BIT_V2_0|Xilinx,\ Inc.|2.0=false
mult_gen_v7_0|Xilinx,\ Inc.|7.0=true
C_MUX_BUS_V3_0|Xilinx,\ Inc.|3.0=false
C_TWOS_COMP_V5_0|Xilinx,\ Inc.|5.0=false
C_DA_FIR_V8_0|Xilinx,\ Inc.|8.0=false
blkmemsp_v6_1|Xilinx,\ Inc.|6.1=true
C_DA_FIR_V5_0|Xilinx,\ Inc.|5.0=false
xlnx_PCI64|Xilinx,\ Inc.|3.0=false
C_REG_LD_V7_0|Xilinx,\ Inc.|7.0=true
C_DDS_V4_0|Xilinx,\ Inc.|4.0=false
C_REG_LD_V4_0|Xilinx,\ Inc.|4.0=false
C_DA_1D_DCT_V2_1|Xilinx,\ Inc.|2.1=true
C_COUNTER_BINARY_V6_0|Xilinx,\ Inc.|6.0=true
C_SHIFT_RAM_V7_0|Xilinx,\ Inc.|7.0=true
C_MUX_SLICE_BUFE_V6_0|Xilinx,\ Inc.|6.0=false
C_SHIFT_RAM_V4_0|Xilinx,\ Inc.|4.0=false
async_fifo_v4_0|Xilinx,\ Inc.|4.0=false
xlnx_PCI32v|Xilinx,\ Inc.|3.0=false
vfft32_v3_0|Xilinx,\ Inc.|3.0=true
C_MUX_BUS_V6_0|Xilinx,\ Inc.|6.0=true
cam_v5_0|Xilinx,\ Inc.|5.0=true
blkmemsp_v3_2|Xilinx,\ Inc.|3.2=false
C_COMPARE_V2_0|Xilinx,\ Inc.|2.0=false
C_DDS_V4_1|Xilinx,\ Inc.|4.1=false
vfft256|Xilinx,\ Inc.|1.0=true
cordic_v3_0|Xilinx,\ Inc.|3.0=true
simelaboptions=
C_DIST_MEM_V4_0|Xilinx,\ Inc.|4.0=false
C_MUX_SLICE_BUFT_V6_0|Xilinx,\ Inc.|6.0=false
sid_v3_0|Xilinx,\ Inc.|3.0=false
xilinxsubfamily=Virtex2
C_MUX_SLICE_BUFT_V3_0|Xilinx,\ Inc.|3.0=false
encode_8b10b_v5_0|Xilinx,\ Inc.|5.0=true
C_MUX_BIT_V2_0|Xilinx,\ Inc.|2.0=false
gig_eth_mac_v4_0|Xilinx,\ Inc.|4.0=true
C_DECODE_BINARY_V6_0|Xilinx,\ Inc.|6.0=false
C_DECODE_BINARY_V3_0|Xilinx,\ Inc.|3.0=false
rs_decoder_v5_0|Xilinx,\ Inc.|5.0=true
C_SHIFT_FD_V3_0|Xilinx,\ Inc.|3.0=false
corelibraryid=0
rs_encoder_v3_0|Xilinx,\ Inc.|3.0=false
C_GATE_BUS_V5_0|Xilinx,\ Inc.|5.0=false
blkmemdp_v4_0|Xilinx,\ Inc.|4.0=false
C_GATE_BUS_V2_0|Xilinx,\ Inc.|2.0=false
sync_fifo_v3_0|Xilinx,\ Inc.|3.0=false
C_MAC_FIR_V3_0|Xilinx,\ Inc.|3.0=false
C_ACCUM_V2_0|Xilinx,\ Inc.|2.0=false
tcc_encoder_v1_0|Xilinx,\ Inc.|1.0=true
C_COMPARE_V5_0|Xilinx,\ Inc.|5.0=false

INFO

close( XCOREPRJ);

}

1;
