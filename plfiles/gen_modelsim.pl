#!/usr/bin/perl


$sim_test_name = $ARGV[0];


print "TEST NAME $sim_test_name\n";
#$sim_test_name = $ENV{BS_DESIGN_NAME};



#$run_time = $ARGV[0];

$min_cycles=$ENV{BS_SIM_CYCLES};

open( DATAEXCH , "$ENV{BS_DATA_EXCH}" );
@data_lines = <DATAEXCH>;
close(DATAEXCH);



@tb_data_l = grep( /TB_PERIOD/ , @data_lines );


@tb_data = split( ' ', $tb_data_l[$#tb_data_l]);

$tb_period =  $tb_data[1];

$run_time = $tb_period * $min_cycles;

print "Generating ModelSim simulation script file\n";

print "TB Period $tb_period : Min Cycles $min_cycles : TB Run Time $run_time\n";

gen_prj_file();

gen_sim_tb_top();

sub gen_prj_file
{

open( PRJFILE , ">modsim.do" );

$sim_file_name = "$sim_test_name\_timesim";

#$testbench_name = "bitsize_tb"; #"$sim_test_name\_testbench";

$testbench_name = $ENV{BS_TB_NAME};


$vcd_file = "$sim_test_name.vcd";

$t_entity_name = "tb_comp";#"$sim_test_name\_glitch_1";

$r_t = $run_time."ns";

#$half_period= "10ns";
#$full_period= "20ns";

#force sys_clk 0 $half_period, 1 $half_period -repeat $full_period

print PRJFILE (<<INFO);

vlib work

vsim -quiet -t ps -sdftyp /$t_entity_name=$sim_file_name.sdf $testbench_name


vcd file $vcd_file
vcd on
vcd add $testbench_name/$t_entity_name/*


run $r_t
vcd flush

quit


INFO
#vsim -quiet -t ps -sdftyp /$t_entity_name=$sim_file_name.sdf $testbench_name

close(PRJFILE);


open( PRJFILE2 , ">modsim_compile.do" );


print PRJFILE2 (<<INFO);

vlib work

vcom -quiet  -nowarn 1 $sim_file_name.vhd
vcom -quiet -93 -nowarn 1 $testbench_name.vhd

quit

INFO

close(PRJFILE2);


}


sub gen_sim_tb_top{

open( TB_TOP , ">bitsize_tb_hw.vhd" );

print TB_TOP (<<INFO);

-- Bitsize Simulation Top Level Test Bench
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE IEEE.std_logic_unsigned.all;
USE ieee.std_logic_arith.ALL;
USE std.textio.ALL;

entity bitsize_tb_hw is

end bitsize_tb_hw; 


architecture tb_arch of bitsize_tb_hw is

  component bitsize_test_top
    port (
      clk    : in  std_logic;
      sysout : out std_logic);
  end component;      

  constant PERIOD : time := 176 ns;
  signal sys_clk : std_logic := '0';


  signal out1 : std_logic := '0';


begin  -- tb_arch       
      
sys_clk <= not sys_clk after PERIOD/2;      
      
writein_proc: process(sys_clk)      
      
FILE out_f1	  : TEXT OPEN write_mode IS "hw_out1.dat";      
VARIABLE out_buf1	 : LINE;      
VARIABLE out_var1	 : natural;      
      
begin       
if rising_edge(sys_clk) then       
      
	out_var1 := conv_integer( out1 );      
	write( out_buf1, out_var1);      
	writeline( out_f1, out_buf1);      
      
end if;      
end process writein_proc;      
      
tb_comp : bitsize_test_top      
  port map (      
    clk    => sys_clk,      
    sysout => out1);      
      
      
end tb_arch;      


INFO

close( TB_TOP );

}
