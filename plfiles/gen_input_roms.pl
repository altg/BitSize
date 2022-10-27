#!/usr/bin/perl

$PLHOME=$ENV{BITSIZE_HOME}."/plfiles";

$GEN_CORE_PL=$PLHOME."/gen_xcore.pl";

require $GEN_CORE_PL;


$num_of_inputs = $ENV{NUM_INPUTS};

$os_name = qx{ uname -o };


main();

sub main{
#system( "$ENV{SIGNALGEN}/gen_rom_data.exe $ENV{BWFILE} asctest.dat $ENV{INPUT_ROM_DEPTH} $ENV{BLANK_ROMS}" );

gen_xcore_prj();

for $i (1..$num_of_inputs){

  gen_rom_core2( $i );

}


gen_rom_addr_ctr( "rom_addr_count" );




}


sub gen_rom_core2{

 my $no = shift(@_);

  my $core_name = "input_rom" .$no;

  my $data_file = "rom".$no.".coe";

  my $xcore_setting_file = $core_name.".xco";

  open( XCOFILE , ">$xcore_setting_file" );

print XCOFILE (<<INFO);

# BEGIN Project Options
SET flowvendor = Synplicity
SET vhdlsim = True
SET verilogsim = False
SET workingdirectory = .
SET speedgrade = $ENV{FPGA_SPEED_GRADE}
SET simulationfiles = Behavioral
SET asysymbol = False
SET addpads = False
SET device = $ENV{FPGA_PART}
SET implementationfiletype = Edif
SET busformat = BusFormatParenNotRipped
SET foundationsym = False
SET package = $ENV{FPGA_PACKAGE}
SET createndf = False
SET designentry = VHDL
SET devicefamily = $ENV{FPGA_ARCH}
SET formalverification = False
SET removerpms = False
# END Project Options
# BEGIN Select
SELECT Single_Port_Block_Memory family Xilinx,_Inc. 6.2
# END Select
# BEGIN Parameters
CSET handshaking_pins=false
CSET init_value=0
CSET coefficient_file=$data_file
CSET select_primitive=16kx1
CSET initialization_pin_polarity=Active_High
CSET global_init_value=0
CSET depth=$ENV{INPUT_ROM_DEPTH}
CSET write_enable_polarity=Active_High
CSET port_configuration=Read_Only
CSET enable_pin_polarity=Active_High
CSET component_name=$core_name
CSET active_clock_edge=Rising_Edge_Triggered
CSET additional_output_pipe_stages=0
CSET disable_warning_messages=true
CSET limit_data_pitch=18
CSET primitive_selection=Optimize_For_Area
CSET enable_pin=false
CSET init_pin=false
CSET write_mode=Read_After_Write
CSET has_limit_data_pitch=false
CSET load_init_file=true
CSET width=$ENV{INPUT_ROM_WIDTH}
CSET register_inputs=false
# END Parameters
GENERATE

INFO

close( XCOFILE );

print "Generating ROM Core $core_name\n";

gen_xcore( $xcore_setting_file );

}

sub gen_rom_core{

  my $no = shift(@_);

  my $core_name = "input_rom" .$no;

  my $data_file = "rom".$no.".coe";

  my $xcore_setting_file = $core_name.".xco";

  open( XCOFILE , ">$xcore_setting_file" );

print XCOFILE (<<INFO);

SET BusFormat = BusFormatParenNotRipped
SET SimulationOutputProducts = VHDL
SET XilinxFamily = Virtex2
SET OutputOption = DesignFlow
SET DesignFlow = VHDL
SET FlowVendor = Synplicity
SET FormalVerification = None
SELECT Single_Port_Block_Memory Virtex2 Xilinx,_Inc. 6.1
CSET primitive_selection = Optimize_For_Area
CSET init_value = 0
CSET register_inputs = false
CSET write_enable_polarity = Active_High
CSET init_pin = false
CSET initialization_pin_polarity = Active_High
CSET global_init_value = 0
CSET select_primitive = 16kx1
CSET enable_pin = false
CSET write_mode = Read_After_Write
CSET port_configuration = Read_And_Write
CSET component_name = $core_name
CSET active_clock_edge = Rising_Edge_Triggered
CSET disable_warning_messages = true
CSET handshaking_pins = false
CSET width = $ENV{INPUT_ROM_WIDTH}
CSET load_init_file = true
CSET enable_pin_polarity = Active_High
CSET additional_output_pipe_stages = 0
CSET coefficient_file = $data_file
CSET has_limit_data_pitch = false
CSET limit_data_pitch = 18
CSET depth = $ENV{INPUT_ROM_DEPTH}
GENERATE

INFO

close( XCOFILE );

print "Generating ROM Core $core_name\n";

gen_xcore( $xcore_setting_file );



}


sub gen_rom_addr_ctr{


 my $core_name = shift(@_);

 my $xcore_setting_file = $core_name.".xco"; 

 my $output_width = log( $ENV{INPUT_ROM_DEPTH} ) / log( 2 );

 open( XCOFILE , ">$xcore_setting_file" );

 print XCOFILE (<<INFO);

SET BusFormat = BusFormatParenNotRipped
SET SimulationOutputProducts = VHDL
SET XilinxFamily = Virtex2
SET OutputOption = DesignFlow
SET DesignFlow = VHDL
SET FlowVendor = Synplicity
SET FormalVerification = None
SELECT Binary_Counter Virtex2 Xilinx,_Inc. 6.0
CSET ce_override_for_load = false
CSET async_init_value = 0
CSET create_rpm = $ENV{BS_XCORE_CTR_RPM}
CSET clock_enable = false
CSET load = false
CSET ce_overrides = sync_controls_override_ce
CSET load_sense = active_high
CSET sync_init_value = 0
CSET operation = up
CSET threshold_1 = false
CSET threshold_0 = false
CSET count_style = count_by_constant
CSET restrict_count = false
CSET count_by_value = 1
CSET component_name = rom_addr_count
CSET threshold_early = true
CSET asynchronous_settings = none
CSET threshold_1_value = MAX
CSET count_to_value = MAX
CSET threshold_0_value = MAX
CSET threshold_options = non_registered
CSET set_clear_priority = clear_overrides_set
CSET output_width = $output_width
CSET synchronous_settings = none
GENERATE

INFO


print "Generating ROM Core $core_name\n";

gen_xcore( $xcore_setting_file );

}


sub gen_xcore{

my $xcore_setting_file = shift(@_);

#system( "coregen.bat -b $xcore_setting_file" ) if ( $os_name =~ /Cygwin/ );

$core_gen_info = qx{coregen.exe -b $xcore_setting_file} if ( $os_name =~ /Cygwin/ );

$core_gen_info = qx{coregen -b $xcore_setting_file} if ( $os_name =~ /GNU\/Linux/ );



if ( $ENV{DEBUG_OUT} eq "true" ){

 print $core_gen_info."\n";

}

#system( "coregen -b $xcore_setting_file" ) if ( $os_name =~ /GNU\/Linux/ );


}
