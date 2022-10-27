# $Author: amag97 $
# $Id: asc2bs.pl,v 1.14 2005/12/13 14:30:25 amag97 Exp $

# To preprocess stream.cxx before compiling with bitsize


# Open the file
open( PFILE, $ARGV[0] ) or die "File not found $!\n";

$stream_loop_dec = 0;
$inside_stream = 0;
$inside_loop = 0;
$var_dec_start = 0;
$inside_main = 0;

$Def_Sign="TWOSCOMPLEMENT";

$var_dec = "";


$line_no = 1;
$new_stmt = "";

$\ = "\n";
LINE:
while (<PFILE>) {
    chomp;

    if(/#include "asc.h"/)
      {
	$_="";
	$new_stmt .= "#include \"bitsize.h\"";
      }
    elsif(/main\(.*\)/){
      $inside_main = 1;
     # $new_stmt .= "#include \"bitsize.h\"";
    }
    elsif(/STREAM_START/)
      {
      $inside_stream = 1;
      }
    elsif(/DefaultSign =(.*);/)
      {
	$Def_Sign = $1;
	$_= "//".$_;
      }
    elsif(/ROUNDING_CHOICE =.*/){
        $_= "//".$_;
    }
    elsif(/STREAM_OPTIMIZE =.*/){
      $_= "//".$_;
    }
    elsif( /USE_HARDMULT =.*/){
      $_= "//".$_;
    }
    elsif(/STREAM_LOOP.*/)
      {
	$stream_loop_dec = 1;
	$inside_loop = 1;

      }


    elsif(/.*=.*/)
      {
	if(( $inside_loop == 0 ) && ( $inside_stream == 1 )){
	  if(( $stream_loop_dec eq 0 ) && ( $var_dec_start eq 1 ) && ( $inside_main eq 1 ))
	    {
	      $inside_loop = 1;
	      $new_stmt .=  "\tSTREAM_LOOP( 10 )";
	    }

	}
	

      }
    $stmt_rec = {};
    $stmt_rec->{STMT} = $new_stmt;
    $stmt_rec->{LINENO} = $line_no;
    push @stmtbuff, $stmt_rec;
    if(/(.*)\/\/(.*)/){
	$var_dec = $1."//".$2;
      }
    elsif(/\/\/(.*)/){
        $var_dec = "//".$1;
    } 
    elsif(/HWfix \&(.*) = .\(new HWfix\((.*),(.*),(.*),(.*)\)\);/){
      $var_dec = "\tstatic HWfix &".$1." = *(new HWfix(" . $2 . "," . $3 . "," . $4 . "," . $5 . ",\"" . $1. "\"));";
      #$var_dec = "\tstatic HWfix ".$1."(" . $2 . "," . $3 . "," . $4 . "," . $5 . ",\"" . $1. "\");";
      }
    elsif(/HWfix \&(.*) = .\(new HWfix\((.*),(.*),(.*)\)\);/){
      $var_dec = "\tstatic HWfix &".$1." = *(new HWfix(" . $2 . "," . $3 . "," . $4 . "," . $Def_Sign . ",\"" . $1. "\"));";
      #$var_dec = "\tstatic HWfix ". $1 . "(". $2 . "," . $3 . "," . $4 . "," . $Def_Sign . ",\"" . $1. "\");";
     }
    elsif(/HWfix (.*)\((.*),.*range\((.*)\).*,(.*),(.*)\);/){
      # push @vardecbuff , $2;
      #$var_dec = "\tHWfix ". $1 . "(". $2 . ",range(" . $3 . "," . $4 . "), ".$5." ," . $Def_Sign . ",\"" . $1. "\");";
      $var_dec = "\tHWfix $1 ( $2 , range( $3 ) , $4 , $5 , \"".$1."\");";
      $var_dec_start = 1;
    }
    elsif(/HWfix (.*)\((.*),(.*),(.*),(.*)\);/){
      # push @vardecbuff , $2;
      $var_dec = "\tHWfix ". $1 . "(". $2 . "," . $3 . "," . $4 . "," . $5 . ",\"" . $1. "\");";
      $var_dec_start = 1;
    }
    elsif(/HWfix (.*)\((.*),(.*),(.*)\);/){
      # push @vardecbuff , $2;
      $var_dec = "\tHWfix ". $1 . "(". $2 . "," . $3 . "," . $4 . "," . $Def_Sign . ",\"" . $1. "\");";
      $var_dec_start = 1;
    }
    elsif(/HWvector\<HWfix\> \&(.*) = \*new HWvector\<HWfix\>\((.*), new HWfix\((.*),(.*),(.*)\),(.*)\);/){
      $var_dec = "\tstatic HWvector<HWfix> &" . $1 . "= *new  HWvector<HWfix>";
      $var_dec = $var_dec ."(" .$2 . ", new HWfix(" .$3.",".$4.",".$5.",".$Def_Sign . ",\"" . $1. "\"),".$6.");";
      #$var_dec = "found";
      }
    elsif(/HWfloat (.*)\((.*),(.*),(.*),(.*)\);/){
      $var_dec = "\tHWfloat $1 ( $2 , (fsize)( $3 ) , (fsize)( $4 ) , $Def_Sign , \"$1\" );";
      $var_dec_start = 1;
    }
     elsif(/HWfloat (.*)\((.*),(.*),(.*)\);/){
      # push @vardecbuff , $2;
      $var_dec = "\tHWfloat ". $1 . "(". $2 . "," . $3 . "," . $4 . "," . $Def_Sign . ", \"" . $1. "\" );";
      $var_dec_start = 1;
    }
    elsif(/HWlns (.*)\((.*),(.*),(.*),(.*)\);/){
      $var_dec = "\tHWlns $1 ( $2 , (fsize)( $3 ) , (fsize)( $4 ) , $Def_Sign , \"$1\" );";
      $var_dec_start = 1;
    }
    elsif(/HWlns (.*)\((.*),(.*),(.*)\);/){
      # push @vardecbuff , $2;
      $var_dec = "\tHWlns ". $1 . "(". $2 . "," . $3 . "," . $4 . "," . $Def_Sign . ", \"" . $1. "\" );";
      $var_dec_start = 1;
    }
    else
    {
	$var_dec = $_;
    }

    $stmt_rec = {};
    $stmt_rec->{STMT} = $var_dec;
    $stmt_rec->{LINENO} = $line_no;
    push @stmtbuff, $stmt_rec;


  $line_no++;
  $new_stmt = "";

  }


print "//Code PreProcessed by ASC2BitSize";
print "//(c) Altaf Abdul Gaffar";


@stmbuff2 = reverse @stmtbuff;
while( @stmbuff2 ){

  $stmt_rec = pop @stmbuff2;
  #print "#line ".$stmt_rec->{LINENO}. " \"".$ARGV[0]."\"";
  printf("%s\n", $stmt_rec->{STMT});
}

# while( @vardecbuff ){
#   print pop @vardecbuff;
# }
