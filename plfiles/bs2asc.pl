#! /usr/bin/perl


# $Author: amag97 $
# $Id: bs2asc.pl,v 1.10 2004/12/23 18:05:10 amag97 Exp $

# To update stream.cxx with new bitwidths calculated by bitsize


# Open the file

open( DATFILE, $ARGV[0] ) or die "Data File not found $!\n";

open( ASCFILE, $ARGV[1] ) or die "Stream File not found $!\n";



$\ = "\n";
LINE:
while (<DATFILE>) {
  chomp;
  if( ! /#.*/ ){
    ($var_name , $var_type , $int_bw , $frac_bw , $int_state , $frac_state ) = split / : /;
       $bit_rec = {};
       $bit_rec->{VARTYPE} = $var_type;
       $bit_rec->{INTBW} = $int_bw;
       $bit_rec->{FRACBW} = $frac_bw;
       $bit_rec->{INTSTATE} = $int_state;
       $bit_rec->{FRACSTATE} = $frac_state;
       $bit_data{$var_name} = $bit_rec;
       $global_var_type = $var_type;
  }

}


#@var_names = keys %bit_data;

#while( @var_names ){

#  $v_name = pop( @var_names );
#  $bit_rec= $bit_data{$v_name};

#  printf  "%s : %d : %d \n" , $v_name , $bit_rec->{INTBW} , $bit_rec->{FRACBW};


#}

$write_mode = 1;


if ( $global_var_type =~ /FIX/ )
  {
   read_fx();
  }
else{

   read_flt();
}


sub read_fx{

#if ( $global_var_type =~ /FIX/ ){
$\ = "\n";
LINE:
while (<ASCFILE>) {
chomp;
   if( /\/\/ (.*)/ ){
    # $var_dec = $1."//".$2;
     $var_dec = $_;
      push @stmtbuff , $var_dec;
   }
   elsif(/HWfix (.*)\((.*),(.*),(.*),(.*)\);/){
      $bit_rec = $bit_data{$1};
      $var_name = $1;

      if ( $write_mode == 0 ){
      $var_dec = "\tHWfix ".$1."(". $2 . "," . ($bit_rec->{INTBW} + $bit_rec->{FRACBW}) . "," . $bit_rec->{FRACBW}. "" . ");";
      }
      else{
	
        if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $intbw_value = "OPT_INTBW_".$var_name;
	}
	else
	{
	  $intbw_value = $bit_rec->{INTBW};

	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $fracbw_value = "OPT_FRACBW_".$var_name;
	}
	else
	{
	  $fracbw_value = $bit_rec->{FRACBW};
	}
	

	$var_dec = "\tHWfix ".$1."(". $2 . ", " . ($intbw_value ." + ". $fracbw_value) . " , " . $fracbw_value. ");";

	#$var_dec = "\tHWfix ".$1."(". $2 . " , ( OPT_FRACBW_".$var_name." + OPT_INTBW_". $var_name . ") , OPT_FRACBW_".$var_name." );";
	
      }

      if( ( $bit_rec->{INTSTATE} == 1 ) || ( $bit_rec->{FRACSTATE} == 1 ) ){

	$var_dec = $var_dec."// <- ";

	if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWINT_OPT ";
	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWFRAC_OPT ";
	}

      }

      push @stmtbuff , $var_dec;
     }
   elsif(/HWfix (.*)\((.*),(.*),(.*)\);/){
      $bit_rec = $bit_data{$1};
      $var_name = $1;

      if ( $write_mode == 0 ){
      $var_dec = "\tHWfix ".$1."(". $2 . "," . ($bit_rec->{INTBW} + $bit_rec->{FRACBW}) . "," . $bit_rec->{FRACBW}. ");";
      }
      else{
	
        if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $intbw_value = "OPT_INTBW_".$var_name;
	}
	else
	{
	  $intbw_value = $bit_rec->{INTBW};

	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $fracbw_value = "OPT_FRACBW_".$var_name;
	}
	else
	{
	  $fracbw_value = $bit_rec->{FRACBW};
	}
	

	$var_dec = "\tHWfix ".$1."(". $2 . ", " . ($intbw_value ." + ". $fracbw_value) . " , " . $fracbw_value. " );";

	#$var_dec = "\tHWfix ".$1."(". $2 . " , ( OPT_FRACBW_".$var_name." + OPT_INTBW_". $var_name . ") , OPT_FRACBW_".$var_name." );";
	
      }

      if( ( $bit_rec->{INTSTATE} == 1 ) || ( $bit_rec->{FRACSTATE} == 1 ) ){

	$var_dec = $var_dec."// <- ";

	if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWINT_OPT ";
	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWFRAC_OPT ";
	}

      }

      push @stmtbuff , $var_dec;
     }
   elsif(/HWvector\<HWfix\> (.*)\((.*),*new HWfix\((.*),(.*),(.*)\),(.*)\);/){
     $bit_rec = $bit_data{$1};
     $var_dec = "\tHWvector<HWfix> " . $1;
     $var_dec = $var_dec ."(" .$2 . "new HWfix(".$3.",";
     $var_dec = $var_dec .($bit_rec->{INTBW} + $bit_rec->{FRACBW}) . "," . $bit_rec->{FRACBW};
     $var_dec = $var_dec ." ),".$6.");";
      push @stmtbuff , $var_dec;
   }
  elsif( /ROUNDING_CHOICE =.*/ )
     {

       $var_dec = "ROUNDING_CHOICE = NOROUND;\n";
       push @stmtbuff , $var_dec;
      #  $var_dec = "STREAM_OPTIMIZE = LATENCY;\n";
#        push @stmtbuff , $var_dec;
     #  $var_dec = "MULTIPLY_TYPE = XC4000_MULT;\n";
#       push @stmtbuff , $var_dec;
     }
    else
    {
	push @stmtbuff , $_;
    }


}

#}
}

sub read_flt{

$\ = "\n";
LINE:
while (<ASCFILE>) {
chomp;
   if( /\/\/ (.*)/ ){
    # $var_dec = $1."//".$2;
     $var_dec = $_;
     push @stmtbuff , $var_dec;
   }
   elsif(/HWfloat (.*)\((.*),(.*),(.*),(.*)\);/){
      $bit_rec = $bit_data{$1};
      $var_name = $1;
      if ( $write_mode == 0 ){
      $var_dec = "\tHWfloat ".$1."(". $2 . "," . ($bit_rec->{INTBW} + $bit_rec->{FRACBW}) . "," . $bit_rec->{FRACBW}. "" . ");";
      }
      else{
	
        if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $intbw_value = "OPT_EXPBW_".$var_name;
	}
	else
	{
	  $intbw_value = $bit_rec->{INTBW};

	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $fracbw_value = "OPT_MANBW_".$var_name;
	}
	else
	{
	  $fracbw_value = $bit_rec->{FRACBW};
	}
	

	$var_dec = "\tHWfloat ".$1."(". $2 . ", " . ($intbw_value ." + ". $fracbw_value) . " , " . $fracbw_value. ");";

	#$var_dec = "\tHWfix ".$1."(". $2 . " , ( OPT_MANBW_".$var_name." + OPT_EXPBW_". $var_name . ") , OPT_MANBW_".$var_name." );";
	
      }

      if( ( $bit_rec->{INTSTATE} == 1 ) || ( $bit_rec->{FRACSTATE} == 1 ) ){

	$var_dec = $var_dec."// <- ";

	if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWEXP_OPT ";
	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWMAN_OPT ";
	}

      }

      push @stmtbuff , $var_dec;
     }
   elsif(/HWfloat (.*)\((.*),(.*),(.*)\);/){
      $bit_rec = $bit_data{$1};
      $var_name = $1;

      if ( $write_mode == 0 ){
      $var_dec = "\tHWfloat ".$1."(". $2 . "," . ($bit_rec->{INTBW} + $bit_rec->{FRACBW}) . "," . $bit_rec->{FRACBW}. ");";
      }
      else{
	
        if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $intbw_value = "OPT_EXPBW_".$var_name;
	}
	else
	{
	  $intbw_value = $bit_rec->{INTBW};

	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $fracbw_value = "OPT_MANBW_".$var_name;
	}
	else
	{
	  $fracbw_value = $bit_rec->{FRACBW};
	}
	

	$var_dec = "\tHWfloat ".$1."(". $2 . ", " . ($intbw_value ." + ". $fracbw_value) . " , " . $fracbw_value. " );";

	#$var_dec = "\tHWfix ".$1."(". $2 . " , ( OPT_MANBW_".$var_name." + OPT_EXPBW_". $var_name . ") , OPT_MANBW_".$var_name." );";
	
      }

      if( ( $bit_rec->{INTSTATE} == 1 ) || ( $bit_rec->{FRACSTATE} == 1 ) ){

	$var_dec = $var_dec."// <- ";

	if( ( $bit_rec->{INTSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWEXP_OPT ";
	}
	
	if( ( $bit_rec->{FRACSTATE} == 1 ) ){
	  $var_dec = $var_dec."BWMAN_OPT ";
	}

      }

      push @stmtbuff , $var_dec;
     }
   elsif(/HWvector\<HWfloat\> (.*)\((.*),*new HWfloat\((.*),(.*),(.*)\),(.*)\);/){
     $bit_rec = $bit_data{$1};
     $var_dec = "\tHWvector<HWfix> " . $1;
     $var_dec = $var_dec ."(" .$2 . "new HWfloat(".$3.",";
     $var_dec = $var_dec .($bit_rec->{INTBW} + $bit_rec->{FRACBW}) . "," . $bit_rec->{FRACBW};
     $var_dec = $var_dec ." ),".$6.");";
      push @stmtbuff , $var_dec;
   }
  elsif( /ROUNDING_CHOICE =.*/ )
     {

       $var_dec = "ROUNDING_CHOICE = NOROUND;\n";
       push @stmtbuff , $var_dec;
       $var_dec = "STREAM_OPTIMIZE = LATENCY;\n";
       push @stmtbuff , $var_dec;
       $var_dec = "MULTIPLY_TYPE = XC4000_MULT;\n";
       push @stmtbuff , $var_dec;
       $var_dec = "gpipe= PIPE_NONE;\ngnorm= YES;\n";
       $var_dec = "glod = LOD;\ngalg = STANDARD;\ngsh = BARREL;\n";
       push @stmtbuff , $var_dec;
     }
    else
    {
	push @stmtbuff , $_;
    }


}

}



print "//Code Bitwidth Optimised by BitSize";
print "//(c) Altaf Abdul Gaffar";


@var_names = keys %bit_data;

@var_names = reverse sort @var_names;
$ctr = 1;
while( @var_names ){

 $v_name = pop( @var_names );
 $bit_rec = $bit_data{$v_name};


 $stmt1 = "";
 $stmt2 = "";
 $stmt3 = "";

 if ( $bit_rec->{VARTYPE} =~ /FIX/ ) {

   if ( ( $bit_rec->{INTSTATE} == 1 ) ) {
     $stmt1 = $stmt1 ."#define OPT_INTBW_".$v_name." ".$bit_rec->{INTBW}."\n";
     $stmt2 = $stmt2 ."#define OPT_INTBW_".$v_name." atoi( argv[".$ctr."] )\n";
     $stmt3 = $stmt3 .$ctr++." ".$v_name." OPT INT ".$bit_rec->{INTBW}."\n";
   }
	
   if ( ( $bit_rec->{FRACSTATE} == 1 ) ) {
     $stmt1 = $stmt1 ."#define OPT_FRACBW_".$v_name." ".$bit_rec->{FRACBW}."\n";
     $stmt2 = $stmt2 ."#define OPT_FRACBW_".$v_name." atoi( argv[".$ctr."] )\n";
     $stmt3 = $stmt3 .$ctr++." ".$v_name." OPT FRAC ".$bit_rec->{FRACBW}."\n";
   }

 } else {

   if ( ( $bit_rec->{INTSTATE} == 1 ) ) {
     if ( $bit_rec->{INTBW} < 3 ) {
       $int_bw = 3;
     } else {
       $int_bw = $bit_rec->{INTBW};
     }

     $stmt1 = $stmt1."#define OPT_EXPBW_".$v_name." ".$int_bw."\n";
     $stmt2 = $stmt2 ."#define OPT_EXPBW_".$v_name." atoi( argv[".$ctr."] )\n";
     $stmt3 = $stmt3 .$ctr++." ".$v_name." OPT EXP ".$int_bw."\n";
   }
	
   if ( ( $bit_rec->{FRACSTATE} == 1 ) ) {
     $stmt1 = $stmt1 ."#define OPT_MANBW_".$v_name." ".$bit_rec->{FRACBW}."\n";
     $stmt2 = $stmt2 ."#define OPT_MANBW_".$v_name." atoi( argv[".$ctr."] )\n";
     $stmt3 = $stmt3 .$ctr++." ".$v_name." OPT MAN ".$bit_rec->{FRACBW}."\n";
   }


 }





 push @pre_proc1 , $stmt1;
 push @pre_proc2 , $stmt2;

 push @ascsim_tests , $stmt3;

 #$ctr++;

}

$total_args = $ctr;

@pre_proc1 = reverse @pre_proc1;
@pre_proc2 = reverse @pre_proc2;

print "#ifdef ASCSIM_TESTS\n";
while( @pre_proc2 ) { print pop @pre_proc2; }
print "#else\n";
while( @pre_proc1 ) { print pop @pre_proc1; }
print "#endif\n";



@stmbuff2 = reverse @stmtbuff;
while( @stmbuff2 ){
  print pop @stmbuff2;
}


open( SIMDATA , ">ascsim_tests.dat" ) or die "Unable to open file $!\n";

#@ascsim_tests = reverse @ascsim_tests;

foreach $stmt ( @ascsim_tests ){

  print SIMDATA  $stmt;

}
