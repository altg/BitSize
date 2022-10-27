#!/usr/bin/perl;

use Data::Dumper;

main();

sub main(){

  my $data_name;
  my $filename;

  #my %p_data = ();

  open( DIRFILES , "ls *.pwr |" );


  while( $filename = <DIRFILES> ){

    chomp($filename);

   $data_name = $filename;

   $data_name =~ s/\.pwr//;

   $data_name =~ s/bs_//;

   @d_names = split( '_' , $data_name );


   #print "Parsing $filename ";

   #$p_data{$d_names[1]}{$d_names[2]} = read_pwr_file( $filename );

    #$p_data{$d_names[1]}{$d_names[2]}{$d_names[3]}{$d_names[4]}{$d_names[5]}{$d_names[6]} = read_pwr_file( $filename );

    $p_data{$d_names[0]}{$d_names[1]}{$d_names[2]}{$d_names[3]}{$d_names[4]}{$d_names[5]} = read_pwr_file( $filename );



  }

  output_mfile( $ARGV[0] );

  #print Dumper( %p_data );


}


sub output_mfile{

  #my $pwr_data = shift( @_ );

 my $mfile_name = shift( @_ );

 open( MFILE , ">$mfile_name" );

# while( my ($k, $v) = each %p_data ){

#   @periods = sort {$a <= $b} keys %{$v};

#   #while( my ($k2 , $v2) = each %{$v} ){

#   for $k2 ( @periods ){

#     print "$k2 \n";
#     $v2 = $v->{$k2};

#     print MFILE "@d_names[0]".".".$k.".ns".$k2." = struct( ";
#     print MFILE "'total' , $v2->{TOTAL} , 'clk' , $v2->{CLK} , 'input' , $v2->{INPUT} , 'output' , $v2->{OUTPUT},";
#     print MFILE "'logic' , $v2->{LOGIC} , 'signal' , $v2->{SIGNAL} ";

#     print MFILE ");\n";
#   }

# }


 #print Dumper( %p_data );

  while ( my ($k1, $v1) = each %p_data ) {
    # print "$k1 \n";
    while ( my ($k2 , $v2 ) = each %{$v1} ) {
      while ( my ($k3 , $v3 ) = each %{$v2} ) {
	while ( my ($k4 , $v4 ) = each %{$v3}) {
	  while ( my ($k5 , $v5 ) = each %{$v4}) {
	    #while ( my ($k6 , $v6 ) = each %{$v5}) {
	    for $k6 ( sort { $a <=> $b } (  keys  %{$v5} ) ){
	      print $k6."\n";
	      $v6 = $v5->{$k6};

	      #print MFILE "$k1.$k2.$k3.bw$k4.ns$k6.$k5 = struct(";

	      print MFILE "$k1.$k2.$k3.bw$k4.$k5.ns$k6 = struct(";
	      print MFILE "'total' , $v6->{TOTAL} , 'clk' , $v6->{CLK} , 'input' , $v6->{INPUT} , 'output' , $v6->{OUTPUT},";
	      print MFILE "'logic' , $v6->{LOGIC} , 'signal' , $v6->{SIGNAL} ";

	      print MFILE ");\n";
	    }

	  }
	}
      }
    }
  }

}


sub read_pwr_file{

  my $filename = shift( @_ );


  my $logic_sec = 0;

  my $summary_sec =0;

  my $output_sum = 0;

  my $system_name = qx/uname -o/;

  my $line;

  my %pwr_data = ();

 
  if ( $system_name =~ /Cygwin/ ) {
    print "Doing DOS to Unix conversion.\n";
    system( "dos2unix $filename" );
  }

  open( PWRFILE , "<$filename" ) or die "File $filename not found $!\n";

  $clk_pwr = 0;

  while ( $line = <PWRFILE> ) {

    chomp( $line );

    $summary_sec = 1 if (  $line =~ s/^Power summary:.*// );
    $summary_sec = 0 if (  $line =~ s/^Thermal summary:.*// );

    $logic_sec = 1 if ( $line =~ s/^Logic.*// );

    $logic_sec = 0 if (( ( $line =~ /^Signals/ ) || ( $line =~ /^Inputs/ ) || ( $line =~ /^Outputs/ ) )  && ( $logic_sec == 1 ));

    $signal_sec = 1 if ( $line =~ s/^Signals.*// );

    $signal_sec = 0 if (( ( $line =~ /^Logic/ ) || ( $line =~ /^Inputs/ ) )  && ( $signal_sec == 1 ));



    $line =~ s/^--*//;


    if ( $summary_sec == 1 ) {


      if ( $line =~ /Total estimated/ )
	{

	  $line =~ s/Total.*://;

	  $line =~ s/ //g;

	  $pwr_data{TOTAL} = $line;


	}


      if ( $line =~ /Clocks:/ ) {

        @data = split( " " , $line );

	$clk_pwr = $data[2];

	$pwr_data{CLK} = $clk_pwr;


      }


      if ( $line =~ /Inputs:/ ) {

        @data = split( " " , $line );

	$input_pwr = $data[2];

        $pwr_data{INPUT} = $input_pwr;

      }

      if ( $line =~ /Outputs:/ ) {


	$output_sum = 1;
      }

      if (( $line =~ /Vcco33/ ) && ( $output_sum == 1)) {

        @data = split( " " , $line );

	$output_pwr = $data[2];
	$output_sum = 0;

	$pwr_data{OUTPUT} = $output_pwr;
      }


      if ( $line =~ /Logic:/ ) {

	@data = split( " " , $line );

	$logic_pwr = $data[2];

       $pwr_data{LOGIC} = $logic_pwr;

      }


      if ( $line =~ /Signals:/ ) {

        @data = split( " " , $line );

	$signal_pwr = $data[2];

        $pwr_data{SIGNAL} = $signal_pwr;

      }

    }

  }


 return \%pwr_data;

}



