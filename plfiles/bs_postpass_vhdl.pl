#!/usr/bin/perl


$PLHOME=$ENV{BITSIZE_HOME}."/plfiles";

$GEN_CORE_PL=$PLHOME."/gen_xcore.pl";

require $GEN_CORE_PL;

main();

sub main(){

  parse_file();

  gen_cores();


}


sub parse_file()
  {

    $input_filename = $ARGV[0];

    open( INFILE , "<$input_filename" ) or die ( "File not found $input_filename\n$!" );

    while ( $line = <INFILE> ) {

      chomp( $line );

      if ( $line =~ /BS_GEN_CORE/ ) {


	@line_data = split( ' ' , $line );

	#( $cc1 , $cc2 , $core_name , $reg_mode ) = split( /[ ]/ , $line );

	$cc1 = $line_data[0];
	$cc2 = $line_data[1];
	$core_name = $line_data[2];
	$reg_mode = $line_data[3];

	#print "$#line_data $core_name \n";

	if ( $core_name =~ /cmul/ ){

	  $mul_cvalue = $line_data[4];

	}else{

	  $mul_cvalue = 0;

	}



	( $core_type , $bw_data ) = split( /_/ , $core_name );

	( $bw_a , $bw_b ) = split( /x/ , $bw_data );

	if ( $reg_mode =~ /NOREG/ ){

	  $reg_mode2 = "false";
	}
	else
	  {

	    $reg_mode2 = "true";
	  }

	$core_data = { "CORENAME" => $core_name,
		       "CORETYPE" => $core_type,
		       "INBW_A" => $bw_a,
		       "INBW_B" => $bw_b,
		       "REGMODE" => $reg_mode2,
		       "CVALUE" =>  $mul_cvalue,
		     };

	push( @core_array , $core_data );

      }


    }



  }


sub gen_cores()
  {

    for $core (@core_array) {


      print  $core->{CORENAME}." ".$core->{CORETYPE};

      $core_fname = $core->{CORENAME}.".edn";

      if ( !( -e $core_fname ) ){
	print " Generating ...\n ";
	gen_xcore_main(  $core->{CORETYPE} , $core->{CORENAME}  , $core->{INBW_A} , $core->{INBW_B} , $core->{REGMODE} , $core->{CVALUE});
      }
      else{

	print " Already present not Generating. \n";
      }	

    }



    }
