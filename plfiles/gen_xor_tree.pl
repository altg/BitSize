#!/usr/bin/perl

$nbw = $ARGV[0];

$K = log( $nbw ) / log( 2 );

for $i (0..$K - 1){


  gen_stage( $i , $K );




}

gen_vhdl( $ARGV[1] );




sub gen_stage{


  my $s = shift(@_);

  my $K = shift(@_);

  my $num_of_sigs = 2**$s;

  my $num_of_sigs_in = 2**( $K - $s );

  my $num_of_sigs_out = 2**( $K - $s - 1);

  my $num_of_ops = 2**( $K - $s - 1);

print (<<INFO);

$s : $num_of_sigs_in :  $num_of_sigs_out : $num_of_ops

INFO


  for $x (1..$num_of_ops){

    $sig1 = "sig_".$s."_".($x*2 - 1);

    $sig2 = "sig_".$s."_".($x*2);

    $sigout = "sig_".($s+1)."_".$x;


    if ( $s == 0 ){

       $body_dec = "$sig1 <= in_bus(".($x*2 - 2).");\n";
       push( @body_decs , $body_dec );
       $body_dec = "$sig2 <= in_bus(".($x*2 - 1).");\n";
       push( @body_decs , $body_dec );
    }


    gen_op( $sig1 , $sig2  , $sigout );

    push(@sig_names, $sig1 );
    push(@sig_names, $sig2 );

    if ( $s == $K - 1 ){

      #$body_dec = "out_sig <= $sigout;\n";
      $body_dec = (<<INFO);
out_reg: process (sys_clk)
begin  -- process out_reg

  if sys_clk'event and sys_clk = '1' then  -- rising clock edge

    out_sig <= $sigout;

  end if;
end process out_reg;

INFO

      push( @body_decs , $body_dec );
      push( @sig_names , $sigout );
    }


  }


}

sub gen_op{

  my $sig_in1 = shift(@_);
  my $sig_in2 = shift(@_);

  my $sig_out = shift(@_);


  $body_dec =  "$sig_out <= $sig_in1 xor $sig_in2;\n\n";

  push( @body_decs , $body_dec );

}


sub gen_vhdl{

my $vhdl_file = shift( @_ );


open( VFILE , ">$vhdl_file" );

print VFILE (<<INFO);


library ieee;
use ieee.std_logic_1164.all;



entity xor_tree is

  port (
    in_bus : in  std_logic_vector( $nbw - 1 downto 0);
    sys_clk : in std_logic;
    out_sig    : out std_logic);

end xor_tree;



architecture xor_tree_arch of xor_tree is

INFO

foreach (@sig_names)
{

print VFILE "signal $_ : std_logic := '0';\n"

}


print VFILE (<<INFO);

begin  -- xor_tree_arch

INFO

foreach (@body_decs)
{

print VFILE "$_ \n";

}


print VFILE (<<INFO);

end xor_tree_arch;

INFO


close( VFILE );





}
