#!/usr/bin/perl

use Data::Dumper;

use POSIX;

main();

sub main{

my $sig_info;

$sig_info = parse_infile( $ARGV[0] );

#$rec = $sig_info->{IN};

#$rec2 = $$rec[0];
#print Dumper( $rec2 );

gen_vhdl_file( $sig_info , $ARGV[1] );

#update_synproj( "synproj.prj" , "synproj_hw.prj" );


}


sub parse_infile{


my $filename = shift( @_ );


open( INFILE , "$filename" ) or die "File not Found $!\n";

my $line;

my %sig_data = ();

my @outsig,@insig,@tempsig;

while( $line = <INFILE> ){


  chomp($line);


  @linedata = split( ' ' , $line );





   $rec = {
			       "NAME" => $linedata[0],
			       "ATYPE" => $linedata[1],
			       "BW" => $linedata[3] + $linedata[4],
			      };

  if ( $linedata[2] eq 'IN' ){


    push( @insig , $rec );


  }
  elsif ( $linedata[2] eq 'OUT' ){

      push( @outsig , $rec );


  }else{


     push( @tempsig , $rec );
  }




}


$sig_data{ IN } = \@insig;
$sig_data{ OUT } = \@outsig;
$sig_data{ TMP } = \@tempsig;



return \%sig_data;

}



sub gen_vhdl_file{

my $sig_info2 = shift( @_ );

my $out_filename = shift( @_ );

my $clk_pin = 'AG18';

my $out_pin = 'R27';

#my $out_pin = 'U31';

open( OUTFILE , ">$out_filename" );


print OUTFILE (<<INFO);

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_arith.ALL;


entity bitsize_test_top is

  port (
    clk : in  std_logic;                       -- global clock
    sysout : out std_logic);  -- output

  attribute xc_loc : string;

  attribute xc_loc of clk : signal is "$clk_pin";

  attribute xc_loc of sysout : signal is "$out_pin";

end bitsize_test_top;

architecture bitsize_test_top_arch of bitsize_test_top is

attribute syn_black_box : boolean;
INFO

#print OUTFILE Dumper( $sig_info2 );

my @inputsig2 = $sig_info2->{IN};
my $inputsig = $sig_info2->{IN};


my @rom_inst;

for $i ( 1..$#$inputsig+1 ){

my $rec = $$inputsig[$i - 1];

#print OUTFILE Dumper( $rec );

$input_addr_width = log( $ENV{INPUT_ROM_DEPTH} ) / log( 2 );

print OUTFILE (<<INFO);

component input_rom$i
        port (
        addr: IN std_logic_VECTOR($input_addr_width - 1 downto 0);
        clk: IN std_logic;
        dout: OUT std_logic_VECTOR(31 downto 0));
end component;


attribute syn_black_box of input_rom$i: component is true;

signal $rec->{NAME}_in : std_logic_vector( 31 downto 0 ) :=  ( others => '0' );

INFO

my $str = (<<INFO);

 input_rom$i\_1: input_rom$i
    port map (
      addr => addr_int,
      clk  => clk,
      dout => $rec->{NAME}_in);

INFO

push( @rom_inst , $str );

}

my $outputsig = $sig_info2->{OUT};

my @comp_inst;

push( @comp_inst , "bitsize_test_1: bitsize_test\n" );
push( @comp_inst , "\tport map (\n" );


print OUTFILE (<<INFO);

component bitsize_test
    port (
INFO
my $sig;

for $sig  ( @$inputsig ){

  #print Dumper( $sig );

  print OUTFILE "\t$sig->{NAME}_in\t: in std_logic_vector( $sig->{BW} - 1 downto 0 );\n";

  push( @comp_inst , "\t$sig->{NAME}_in\t=> $sig->{NAME}_in( $sig->{BW} - 1 downto 0 ),\n" );

}


print OUTFILE "\tclk\t: in  std_logic;\n";

push( @comp_inst , "\tclk\t=> clk," );

my @xortree_in;

my @sigdecs;


my $total_out_bw = 0;

for $sig ( @$outputsig ){

  #print Dumper( $sig );

  print OUTFILE "\t$sig->{NAME}_out\t: out std_logic_vector( $sig->{BW} - 1 downto 0 )";
  push( @comp_inst , "\t$sig->{NAME}_out\t=> $sig->{NAME}( $sig->{BW} - 1 downto 0 )" );

  if ( $sig ne $$outputsig[ $#$outputsig ]){

    print OUTFILE ";\n";

    push( @comp_inst , ",\n" );
  }

  push( @sigdecs , " signal $sig->{NAME}\t: std_logic_vector( $sig->{BW} - 1 downto 0 ) := ( others => '0' );\n" );

  push( @xortree_in , "&$sig->{NAME}");

  $total_out_bw += $sig->{BW};

}

#print $total_out_bw . " " .log( $total_out_bw )."\n";
my $xor_tree_width = 2 ** ceil( log( $total_out_bw ) / log( 2 ) );

push( @comp_inst , ");\n" );

print OUTFILE (<<INFO);
);
end component;

component rom_addr_count
	port (
	Q: OUT std_logic_VECTOR($input_addr_width - 1 downto 0);
	CLK: IN std_logic);
end component;

component xor_tree
    port (
      in_bus  : in  std_logic_vector($xor_tree_width - 1 downto 0);
      sys_clk : in std_logic;
      out_sig : out std_logic);
end component;


  attribute syn_black_box of rom_addr_count: component is true;

 

  signal addr_int : std_logic_vector( $input_addr_width - 1 downto 0 ) :=  ( others => '0' );

  signal xor_dummy : std_logic_vector( $xor_tree_width - $total_out_bw - 1 downto 0 ) := (others => '0' );

INFO

# signal we_int : std_logic := '0';
#  signal din_dummy : std_logic_vector( 31 downto 0 ) :=  ( others => '0' );
foreach (@sigdecs) {print OUTFILE;}



print OUTFILE (<<INFO);

begin  -- bitsize_test_top_arch

  rom_addr_count_1: rom_addr_count
    port map (
      Q   => addr_int,
      CLK => clk);

INFO

foreach (@rom_inst){print OUTFILE;}

foreach (@comp_inst){print OUTFILE;}

print OUTFILE (<<INFO);

xor_tree_1: xor_tree
    port map (
INFO

print OUTFILE "\tin_bus  => xor_dummy";

foreach (@xortree_in){print OUTFILE;}

print OUTFILE ",\n";

print OUTFILE (<<INFO);
      \tsys_clk => clk,
      \tout_sig => sysout);


end bitsize_test_top_arch;

INFO

close( OUTFILE );



$xor_gen = qx{make gen_xor_tree XOR_TREE_WIDTH=$xor_tree_width};

print "$xor_gen \n";



}


sub update_synproj{


  my $filein = shift(@_);

  my $fileout = shift(@_);

  open( FILEIN , $filein ) or die "File not found $!\n";

  open( FILEOUT , ">$fileout" );


  my $line;

  my $in_addfile = 0;

  while( $line = <FILEIN> ){

    chomp( $line );

    if ( $line eq "#add_file options" ) {
      $in_addfile = 1;

      print FILEOUT (<<INFO);

#add_file options
add_file -vhdl -lib work "xor_tree.vhd"
add_file -vhdl -lib work "bitsize_test.vhd"
add_file -vhdl -lib work "bitsize_test_top.vhd"

INFO
    } ;

    if ( $line =~ /#implementation:/ ) { $in_addfile = 0; };

    if ( $line =~ /set_option -top_module/ ){

      $line = "set_option -top_module \"bitsize_test_top\"";
    }


    if ( $line =~ /project -result_file/ ){

      $line = "project -result_file \"./bitsize_test_top.edf\"";

    }

    if ( $in_addfile == 0 ){


     print FILEOUT "$line\n";
    }

  }


  close( FILEIN );

  close( FILEOUT );


}
