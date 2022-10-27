#!/usr/bin/perl

use lib "$ENV{BITSIZE_HOME}/plfiles/site_perl";
use XML::Simple;

use POSIX;

use Data::Dumper;

$PLHOME=$ENV{BITSIZE_HOME}."/plfiles";

$PARSE_CORE_PL=$PLHOME."/parse_edif.pl";

require $PARSE_CORE_PL;

main();

sub main{

  $file_name = $ARGV[0];

  #$edif_top_level = "bitsize_test.edf";

  $edif_top_level = $file_name;
  $edif_top_level =~ s/.pwr/.edf/;

  $test_name = $ARGV[2];

  read_pwr_file( $file_name );

  cal_logic_pwr();

  cal_signal_pwr( $edif_top_level );

  print_test();

  $matlab_file_name = $ARGV[3];

  read_pnr_file( $ARGV[1] );

  get_timing_data( $file_name )."\n";

  gen_matlab_data_script( $test_name , $matlab_file_name );

}


sub read_pwr_file{

  my $filename = shift( @_ );

  my $logic_sec = 0;

  my $summary_sec =0;

  my $output_sum = 0;

  my $system_name = qx/uname -o/;

  my $line;

  if ( $system_name =~ /Cygwin/ ){
    print "Doing DOS to Unix conversion.\n";
    system( "dos2unix $filename" );
  }

  open( PWRFILE , "<$filename" ) or die "File $filename not found $!\n";

  $clk_pwr = 0;

while( $line = <PWRFILE> ){

     chomp( $line );

     $summary_sec = 1 if (  $line =~ s/^Power summary:.*// );

    $summary_sec = 0 if (  $line =~ s/^Thermal summary:.*// );

    $logic_sec = 1 if ( $line =~ s/^Logic.*// );

    $logic_sec = 0 if (( ( $line =~ /^Signals/ ) || ( $line =~ /^Inputs/ ) || ( $line =~ /^Outputs/ ) )  && ( $logic_sec == 1 ));

    $signal_sec = 1 if ( $line =~ s/^Signals.*// );

    $signal_sec = 0 if (( ( $line =~ /^Logic/ ) || ( $line =~ /^Inputs/ ) )  && ( $signal_sec == 1 ));



    $line =~ s/^--*//;


     if ( $summary_sec == 1 ){

       if ( $line =~ /Clocks:/ ){

       @data = split( " " , $line );

       $clk_pwr = $data[2];

     }


         if ( $line =~ /Inputs:/ ){

       @data = split( " " , $line );

       $input_pwr = $data[2];

     }

        if ( $line =~ /Outputs:/ ){


	  $output_sum = 1;
	}

      if (( $line =~ /Vcco33/ ) && ( $output_sum == 1)){

       @data = split( " " , $line );

       $output_pwr = $data[2];
       $output_sum = 0;
     }


        if ( $line =~ /Logic:/ ){

       @data = split( " " , $line );

       $logic_pwr = $data[2];

     }


        if ( $line =~ /Signals:/ ){

       @data = split( " " , $line );

       $signal_pwr = $data[2];

     }

     }


    if ( ( $logic_sec == 1 ) && (  $line !~ s/^[- ]*$// ))  {

      $line =~ s/\s+/:/g;

      #print "$line \n";

      @line_segs = split( /:/ , $line );

      @logic_nodes = split( /\// ,  $line_segs[0] );


      $logic_nodes{$logic_nodes[0]}{$logic_nodes[1]}{CAPACITANCE} = $line_segs[1];
      $logic_nodes{$logic_nodes[0]}{$logic_nodes[1]}{FREQ} = $line_segs[2];
      $logic_nodes{$logic_nodes[0]}{$logic_nodes[1]}{CURRENT} = $line_segs[3];

      if ( $#line_segs == 4 ){

      $logic_nodes{$logic_nodes[0]}{$logic_nodes[1]}{POWER} = $line_segs[4];
    }
      else{
	$next_line = <PWRFILE>;
	chomp($next_line);
	$logic_nodes{$logic_nodes[0]}{$logic_nodes[1]}{POWER} = $next_line;
      }

      #print " $logic_nodes[0] ,  $logic_nodes[1] , $line_segs[4] \n";

    }
    elsif ( ( $signal_sec == 1 ) && (  $line !~ s/^[- ]*$// ))  {

      if( $line =~ /\// ){
	
	@sig_data = split( ' ' , $line );
	
	@sig_nodes = split( /\// ,  $sig_data[0] );

	 $sig_data[5] =~ s/~//g;

	
	$node_signals{$sig_nodes[0]}{$sig_nodes[1]}= {
						      'LOADS' => $sig_data[1],
						      'CAPACITANCE' => $sig_data[2],
						      'FREQ' => $sig_data[3],
						      'CURRENT' => $sig_data[4],
						      'POWER' => $sig_data[5],
						     };

      }
      else{

		@sig_data = split( ' ' , $line );
	
	
	 $sig_data[5] =~ s/~//g;

	$non_node_signals{$sig_data[0]} = {
						      'LOADS' => $sig_data[1],
						      'CAPACITANCE' => $sig_data[2],
						      'FREQ' => $sig_data[3],
						      'CURRENT' => $sig_data[4],
						      'POWER' => $sig_data[5],
						     };



      }


    }

   }

 
}


sub print_test{



my $t_pwr = 0;

 for my $name ( keys %logic_pwr ){


    print "$name : $logic_pwr{$name} \n";

    $t_pwr += $logic_pwr{$name};

  }

 print "Logic PWR Total : $t_pwr \n";

my $s_pwr = 0;
my $s_out_pwr = 0;
while ( my ($k, $v) = each %signal_pwr ){

  print "$k : $v->{intra} : $v->{out_sig} \n";
  $s_pwr += $v->{intra};
  $s_out_pwr += $v->{out_sig};
}

print "Intra Routing Signal PWR Total : $s_pwr \n";
print "Inter Comp. Output Signal PWR Total : $s_out_pwr \n";

print "Total Signal PWR : ".($s_pwr + $s_out_pwr)."\n";

   print (<<INFO);

Power Summary

CLK : $clk_pwr uW
INPUT : $input_pwr uW
OUTPUT : $output_pwr uW
SIGNAL : $signal_pwr uW
LOGIC : $logic_pwr uW

INFO


}

sub cal_logic_pwr{

  my $l_pwr = 0;

    for $name ( keys %logic_nodes ){

      $l_pwr = 0;
      for $sig_name ( keys %{$logic_nodes{$name}} ) {

	$logic_rec = $logic_nodes{$name}{$sig_name};


	#print " $logic_rec->{POWER} \n";

	#$l_pwr += $logic_nodes{$name}{$sig_name}{"POWER"};
	$l_pwr += $logic_rec->{POWER};
	
    }

      $logic_pwr{$name} = $l_pwr;
  }


}



sub cal_signal_pwr{

  my $l_pwr = 0;

  my $edif_top_level = shift(@_);

  $output_nets = get_outport_nets( $edif_top_level );

  #print Dumper($output_nets);


    for my $name ( keys %node_signals ){

      $s_pwr = 0;
      $s_out_pwr = 0;

      my @node_out_sigs = keys %{$output_nets->{$name}};

      my @out_sigs;


      for my $sig_name ( keys %{$node_signals{$name}} ) {

	$logic_rec = $node_signals{$name}{$sig_name};
	
	@sig_names2 = grep{/$sig_name/} @node_out_sigs;

	if ( $#sig_names2 == 0 ){
	   $s_out_pwr += $logic_rec->{POWER};
	   push( @out_sigs , $sig_name );
        }
	else{
	 
	  $s_pwr += $logic_rec->{POWER};
	
      }
    }

      $signal_pwr{$name} = {
	                  'intra' => $s_pwr,
			  'out_sig' => $s_out_pwr,
			   };

      $signal_freq{$name} = \@out_sigs;
  }



  for my $inst_name ( keys %{$output_nets} ){
     my @out_sigs;

    my $out_sig_pwr = 0;

     for my $out_sig ( keys %{$output_nets->{$inst_name}} )
       {
	 #print "$out_sig : $non_node_signals{$out_sig}->{POWER}\n";
	 $out_sig_pwr += $non_node_signals{$out_sig}->{POWER};
	 push( @out_sigs , $out_sig );
       }

    $signal_pwr{$inst_name} = {
	                  'intra' => $signal_pwr{$inst_name}->{intra} + 0,
			  'out_sig' => $signal_pwr{$inst_name}->{out_sig} + $out_sig_pwr,
			   };
     $signal_freq{$inst_name} = \@out_sigs;

  }



}


sub get_timing_data{

  my $data_file =  shift( @_ );

  #$data_file =~ s/.pwr/_timing.xml/;

  $data_file =~ s/.pwr/.twx/;

  if ( -e $data_file ){
  print "$data_file \n";
  my $xs1 = XML::Simple->new();

  my $doc = $xs1->XMLin($data_file);

  $sys_min_period = $doc->{twSum}->{twStats}->{twMinPer};
  $sys_max_freq = $doc->{twSum}->{twStats}->{twMaxFreq};

  print "Min Period. $sys_min_period\n";
  print "Max Freq. $sys_max_freq\n";


  }
 else{
   print "Timing File $data_file not found, not doing timing data collection.\n";

  $sys_min_period = 0;
  $sys_max_freq = 0;


 }

  open( DATAEXCH , "$ENV{BS_DATA_EXCH}" );
  @data_lines = <DATAEXCH>;
  close(DATAEXCH);

  @tb_data_l = grep( /TB_PERIOD/ , @data_lines );

  @tb_data = split( ' ', $tb_data_l[$#tb_data_l]);
  $tb_period =  $tb_data[1];

  print "TB Period $tb_period\n";

}

sub gen_matlab_data_script{

   my $system_name = shift( @_ );

   my $m_file_name = shift( @_ );

  # my $new_file = 0;

#   if ( !( -e $m_file_name ) ){

#     $new_file = 1;
#   }

  $system_name =~ s/.dat//;

   @system_data = split( '_' , $system_name );


#   print $m_file_name;


   open( MFILE , ">>$m_file_name" )  or die "Failed to Open $m_file_name in $!\n";

  #  print MFILE "pwr_data2 = [];\n" if ( $new_file == 1 );
$logic_pwr += 0;

   print MFILE (<<INFO);

pwrstruct = struct(  'logic_pwr' , $logic_pwr , 'signal_pwr' , $signal_pwr , 'input_pwr' , $input_pwr , 'output_pwr' , $output_pwr , 'clock_pwr' , $clk_pwr , 'slices' , $used_slices , 'total_slices' , $total_slices , 'min_period' , $sys_min_period , 'max_freq' , $sys_max_freq , 'tb_period' , $tb_period , 'sim_cycles' , $ENV{BS_SIM_CYCLES} , 'pwr_units' , '$ENV{XPWR_POWER_UNIT}' , 'freq_units' , '$ENV{XPWR_FREQ_UNIT}'  );



INFO

 print MFILE "pwrstruct.logic_pwr_bd = struct( ";

$k_ctr = 0;

@logic_keys = sort(  keys %logic_pwr );
 for $name ( @logic_keys ){


   # print MFILE " struct( 'comp_name' , '$name' ,  'pwr' , $logic_pwr{$name} ) ";
   $name =~ s/\(/_/g;

   $name =~ s/\)/_/g;

   print MFILE " '$name' ,  $logic_pwr{$name} ";
   $k_ctr++;
   if ( $k_ctr <= $#logic_keys ){
     print MFILE " , ";
   }

  }

print MFILE ");\n";

print MFILE "pwrstruct.signal_intra_pwr_bd = struct( ";



$k_ctr = 0;

@signal_keys = sort(  keys %signal_pwr );
 for $name ( @signal_keys ){


   # print MFILE " struct( 'comp_name' , '$name' ,  'pwr' , $logic_pwr{$name} ) ";
   print MFILE " '$name' ,  struct( 'pwr' , [ $signal_pwr{$name}->{intra} , $signal_pwr{$name}->{out_sig} ] ,";
   print MFILE " 'freqs' ,  struct(  ";

   @freqs = $signal_freq{$name};

   foreach ( @freqs ){
     my @fa = @$_;
     my $ctr = 0;
     foreach ( @fa ){
       if ( $name =~ /MUL/ ){
       if  ( ! $_ eq "" ){
	my $freq = 0. + $node_signals{$name}{$_}->{FREQ};
        print MFILE "'$_' ,  $freq ";
      }
      else{
	print MFILE " 'N0' , 0 ";

      }
     }
       else{
	 my $n = $_;
	 $n =~ s/(\w+)\((\d)\)/\1(0\2)/;
	 #print "$_  $n \n";

	 $n =~ s/\(/_/;
	 $n =~ s/\)//;
	 my $freq = 0 + $non_node_signals{$_}->{FREQ};
	 print MFILE "'$n' , $freq  ";
       }
       $ctr++;
       if ( $ctr <= $#fa ){
	   print MFILE " , ";
       }
    }
   }


   print MFILE " )) ";


   $k_ctr++;
   if ( $k_ctr <= $#signal_keys ){
     print MFILE " , ";
   }

  }

print MFILE ");\n";

print MFILE "\n$system_data[1].$system_data[2].$system_data[3].bw$system_data[4] = pwrstruct;\n";

print MFILE "clear pwrstruct;\n";
#print MFILE "pwr_data.$system_name = pwrstruct;\n";

# pwr_data = [ pwr_data pwrstruct ];

}


sub read_pnr_file{

   my $filename = shift( @_ );

   my $line;

   open( PNRFILE , "<$filename" ) or die "File $filename not found $!\n";

   $used_slices = 0;
   $total_slices = 0;

   while( $line = <PNRFILE> ){

     chomp( $line );

     if( $line =~ /Number of occupied Slices:(.*)/ ){
       @line_segs = split( " " , $1 );

       print " Slices Used : $line_segs[0]  / $line_segs[3] \n";
       $used_slices = $line_segs[0];
       $total_slices = $line_segs[3];

       $total_slices =~ s/,//;
     }


   }

}




