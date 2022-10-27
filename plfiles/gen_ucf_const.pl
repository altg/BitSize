#!/usr/bin/perl


$ucf_file_name = $ENV{BS_DESIGN_NAME}.'.ucf';

$ucf_file_name = $ARGV[0];

gen_ucf_file( $ucf_file_name );

$xflow_opt_file_name = $ENV{BS_XFLOW_OPT_FILE};

if ( $ENV{MPPR_MODE} == 1 ){
  gen_xflow_mppr_opt_file( $xflow_opt_file_name );
}
else{
  gen_xflow_opt_file( $xflow_opt_file_name );

}

sub gen_ucf_file(){

  my $file_name = shift( @_ );

  print "*** Generating UCF file $file_name :\n";

  open( UCFFILE , ">$file_name" );

  print "clk   : $ENV{BS_REQ_PERIOD}\n";

  print UCFFILE "NET clk PERIOD = $ENV{BS_REQ_PERIOD};\n";

  close( UCFFILE );

  print "************************\n";

}




sub gen_xflow_opt_file(){

  my $file_name = shift( @_ );


  print "*** Generating XFLOW Opt file $file_name :\n"; 

  open( OPTFILE , ">$file_name" );



print OPTFILE (<<INFO);

FLOWTYPE = FPGA;
###############################################################
## Filename: balanced.opt
##
## Option File For Xilinx FPGA Implementation Flow
## 
## Version: 6.1.1
## $Header: /devl/xcs/repo/env/Jobs/Xflow/data/optionfiles/virtex_fpga_balanced.opt,v 1.13 2003/03/17 22:45:59 jpan Exp $
###############################################################
#
# Options for Translator
#
# Type "ngdbuild -h" for a detailed list of ngdbuild command line options
#
Program ngdbuild 
-p <partname>;        # Partname to use - picked from xflow commandline
-nt timestamp;        # NGO File generation. Regenerate only when
                      # source netlist is newer than existing 
                      # NGO file (default)
-intstyle xflow;      # Message Reporting Style: ise, xflow, or silent
#-bm <design>.bmm     # Block RAM memory map file
<userdesign>;         # User design - pick from xflow command line
<design>.ngd;         # Name of NGD file. Filebase same as design filebase
End Program ngdbuild

#
# Options for Mapper
#
# Type "map -h <arch>" for a detailed list of map command line options
#
Program map
-o <design>_map.ncd;     # Output Mapped ncd file
-intstyle xflow;         # Message Reporting Style: ise, xflow, or silent
# -fp <design>.mfp;      # Floorplan file
# -gm incremental;	 # Guide mode
-timing;                 # Perform Timing-Driven Packing and Placement
-ol $ENV{BS_MAP_EFFORT};               # Map Effort Levels. -ol [std|med|high]
                         # Note: If -timing is not specified, -ol switch is ignored.
                         # Device families supported: Virtex2, Virtex2P
<inputdir><design>.ngd;  # Input NGD file
<inputdir><design>.pcf;  # Physical constraints file
END Program map

#
# Options for Post Map Trace
#
# Type "trce -h" for a detailed list of trce command line options
#
Program post_map_trce
-e 3;                        # Produce error report limited to 3 items per constraint
-xml <design>_map.twx;       # Output XML version of the timing report
-intstyle xflow;             # Message Reporting Style: ise, xflow, or silent
#-o <design>_map.twr;        # Output trace report file
#-tsi <design>_map.tsi;      # Produce Timing Specification Interaction report
#-fastpaths;                 # Report minimum paths in verbose mode
#-nodatasheet;               # Do not create the datasheet section of the report
<inputdir><design>_map.ncd;  # Input mapped ncd
<inputdir><design>.pcf;      # Physical constraints file
END Program post_map_trce

#
# Options for Place and Route
#
# Type "par -h" for a detailed list of par command line options
#
Program par
-w;                       # Overwrite existing placed and routed ncd
-ol $ENV{BS_PAR_EFFORT};                  # Overall effort level
-intstyle xflow;          # Message Reporting Style: ise, xflow, or silent
# -gm incremental;	  # Guide mode
<design>_map.ncd;         # Input mapped NCD file
<inputdir><design>.ncd;   # Output placed and routed NCD
<inputdir><design>.pcf;   # Input physical constraints file
END Program par

#
# Options for Post Par Trace
#
# Type "trce -h" for a detailed list of trce command line options
#
Program post_par_trce
-e 3;                        # Produce error report limited to 3 items per constraint
-intstyle xflow;             # Message Reporting Style: ise, xflow, or silent
-xml <design>.twx;           # Output XML version of the timing report
#-o <design>.twr;            # Output trace report file
#-tsi <design>.tsi;          # Produce Timing Specification Interaction report
#-fastpaths;                 # Report minimum paths in verbose mode
#-nodatasheet;               # Do not create the datasheet section of the report
<inputdir><design>.ncd;      # Input placed and routed ncd
<inputdir><design>.pcf;      # Physical constraints file
END Program post_par_trce


INFO

}



sub gen_xflow_mppr_opt_file{

 my $file_name = shift( @_ );


  print "*** Generating XFLOW MPPR Opt file $file_name :\n";

  open( OPTFILE , ">$file_name" );

my $cost_table_no = $ENV{MTEST_NO} * 10;

print OPTFILE (<<INFO);

FLOWTYPE = FPGA;
###############################################################
## Filename: overnight.opt
##
## Option File For Xilinx FPGA Implementation Flow
##
## Version: 6.1.1
## $Header: /devl/xcs/repo/env/Jobs/Xflow/data/optionfiles/virtex_overnight.opt,v 1.12 2003/03/17 22:45:59 jpan Exp $
###############################################################
#
# Options for Translator
#
# Type "ngdbuild -h" for a detailed list of ngdbuild command line options
#
Program ngdbuild 
-p <partname>;        # Partname to use - picked from xflow commandline
-nt timestamp;        # NGO File generation. Regenerate only when
                      # source netlist is newer than existing 
                      # NGO file (default)
-intstyle xflow;      # Message Reporting Style: ise, xflow, or silent
#-bm <design>.bmm     # Block RAM memory map file
<userdesign>;         # User design - pick from xflow command line
<design>.ngd;         # Name of NGD file. Filebase same as design filebase
End Program ngdbuild

#
# Options for Mapper
#
# Type "map -h <arch>" for a detailed list of map command line options
#
Program map
-o <design>_map.ncd;     # Output Mapped ncd file
-intstyle xflow;         # Message Reporting Style: ise, xflow, or silent
# -fp <design>.mfp;      # Floorplan file
# -gm incremental;	 # Guide mode
-timing;               # Perform Timing-Driven Packing and Placement
# -ol med;               # Map Effort Levels. -ol [std|med|high]
                         # Note: If -timing is not specified, -ol switch is ignored.
                         # Device families supported: Virtex2, Virtex2P
<inputdir><design>.ngd;  # Input NGD file
<inputdir><design>.pcf;  # Physical constraints file
END Program map

#
# Options for Post Map Trace
#
# Type "trce -h" for a detailed list of trce command line options
#
Program post_map_trce
-e 3;                        # Produce error report limited to 3 items per constraint
-xml <design>_map.twx;       # Output XML version of the timing report
-intstyle xflow;             # Message Reporting Style: ise, xflow, or silent
#-o <design>_map.twr;        # Output trace report file
#-tsi <design>_map.tsi;      # Produce Timing Specification Interaction report
#-fastpaths;                 # Report minimum paths in verbose mode
#-nodatasheet;               # Do not create the datasheet section of the report
<inputdir><design>_map.ncd;  # Input mapped ncd
<inputdir><design>.pcf;      # Physical constraints file
END Program post_map_trce

#
# Options for Place and Route with Multi-Pass Place and Route support
#
# Type "par -h" for a detailed list of par command line options
#
Program par
-w;                       	# Overwrite existing placed and routed ncd
-ol high;                    	# Overall effort level
#-t 1;			  	# Starting placer cost table entry
-t $cost_table_no;
-n $ENV{MPPR_LIMIT}; 	        # Number of iterations
-s 1; 			  	# Number of results to save 
<inputdir><design>_map.ncd;     # Input mapped NCD file
mppr.dir;			# Subdirectory where PAR's best result is saved
<inputdir><design>.pcf;   	# Input physical constraints file
END Program par

#
# Options for Post Par Trace
#
# Type "trce -h" for a detailed list of trce command line options
#
Program post_par_trce
-e 3;                        # Produce error report limited to 3 items per constraint
-intstyle xflow;             # Message Reporting Style: ise, xflow, or silent
-xml <design>.twx;           # Output XML version of the timing report
#-o <design>.twr;            # Output trace report file
#-tsi <design>.tsi;          # Produce Timing Specification Interaction report
#-fastpaths;                 # Report minimum paths in verbose mode
#-nodatasheet;               # Do not create the datasheet section of the report
<inputdir><design>.ncd;      # Input placed and routed ncd
<inputdir><design>.pcf;      # Physical constraints file
END Program post_par_trce



INFO


}
