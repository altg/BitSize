#!/usr/bin/perl

use lib "$ENV{BITSIZE_HOME}/plfiles/site_perl";
use XML::Simple;

use POSIX;

main();

sub main(){

  my $timing_file_name = $ARGV[0];

  my $tb_file_name = $ARGV[1];

  my $sim_period = 0;

  if ( $#ARGV == 2 ){

    $sim_period = $ARGV[2];

  }
  else{

    $sim_period = get_timing_data( $timing_file_name );

  }

  set_tb_period( $tb_file_name , $sim_period);

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

  $sys_timing_found = 1;

  print "Min Period. $sys_min_period\n";
  print "Max Freq. $sys_max_freq\n";
  }
 else{
   print "Timing File $data_file not found, not doing timing data collection.\n";

  $sys_min_period = 0;
  $sys_max_freq = 0;

   $sys_timing_found = 0;

 }

  $sys_min_period;

}


sub set_tb_period{


  my $tb_file_name = shift( @_ );

  my $my_sim_period = shift( @_ );

  my $tmp_file_name = qx[mktemp];

  chomp( $tmp_file_name );

  #print $tmp_file_name."\n";

  $tb_period = ceil( $my_sim_period );

  if ( $tb_period % 2 ){

    $tb_period++;

  }else{

    $tb_period += 2;
  }


  $tb_period *= $ENV{BS_PERIOD_MUL};

  print "TB clk Period : $tb_period \n";

  open( TBFILE , "$tb_file_name" ) or die "$tb_file_name not found $! \n";

  open( TMPFILE , ">$tmp_file_name" );

  while( $line = <TBFILE> ){
    chomp( $line );

    if ( $line =~ /constant PERIOD/ ){

      $line = "constant PERIOD : time := $tb_period ns;";

    }
    print TMPFILE "$line \n";

  }

  close( TBFILE );

  close( TMPFILE );


  system( "cp $tmp_file_name $tb_file_name" );


  open( DATAEXCH , ">>$ENV{BS_DATA_EXCH}" );

  print DATAEXCH "TB_PERIOD $tb_period\n";

  close( DATAEXCH );

}
