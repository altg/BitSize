#!/usr/bin/perl

$source_file = $ARGV[0];
$target_file = $ARGV[1];

$syn_prj_file = "synproj.prj";

print "Generating SynplifyPro project file\n";

gen_syn_prj();

sub gen_syn_prj{

$ctime_str = gmtime;

$file_path = qx{pwd};

$c_dir = $file_path;

$c_dir =~ s/\/.*\///;

chomp($c_dir);

$dos_dir = qx{cygpath -d $file_path};

chomp($dos_dir);

$top_module = $source_file;

$top_module =~ s/.vhd// if ( $#ARGV < 2 );

$top_module = $ARGV[2] if (  $#ARGV > 1 );


$device_tech = $ENV{FPGA_ARCH};
$device_part = $ENV{FPGA_PART};
$device_package = $ENV{FPGA_PACKAGE};
$device_speed_grade = $ENV{FPGA_SPEED_GRADE};

#$device_tech = "VIRTEX2";
#$device_part = "XC2V1000";
#$device_package = "BG575";
#$device_speed_grade = "-6";

print (<<INFO);
For FPGA $device_tech $device_part $device_package $device_speed_grade

INFO

open( PRJFILE , ">$syn_prj_file" );


print PRJFILE (<<INFO);

#-- Synplicity, Inc.
#-- Version 7.7
#-- Project file $dos_dir\\$syn_prj_file
#-- Written on $ctime_str


#add_file options
add_file -vhdl -lib work "$source_file"


#implementation: "$c_dir"
#impl -add $c_dir

#device options
set_option -technology $device_tech
set_option -part $device_part;
set_option -package $device_package;
set_option -speed_grade $device_speed_grade;

#compilation/mapping options
set_option -default_enum_encoding default
set_option -symbolic_fsm_compiler 0
set_option -resource_sharing 0
set_option -use_fsm_explorer 0
set_option -top_module "$top_module"

#map options
set_option -frequency $ENV{SYNPLICIY_REQ_FREQ}
set_option -fanout_limit 10000
set_option -disable_io_insertion 0
set_option -pipe 0
set_option -update_models_cp 0
set_option -verification_mode 0
set_option -fixgatedclocks 0
set_option -modular 0
set_option -retiming 0

#simulation options
set_option -write_verilog 0
set_option -write_vhdl 0

#automatic place and route (vendor) options
set_option -write_apr_constraint 1

#set result format/file last
project -result_file "./$target_file"

#implementation attributes
set_option -vlog_std v2001
set_option -synthesis_onoff_pragma 0
set_option -compiler_compatible 1
set_option -auto_constrain_io 0
impl -active "$c_dir"



INFO


close(PRJFILE);


}
