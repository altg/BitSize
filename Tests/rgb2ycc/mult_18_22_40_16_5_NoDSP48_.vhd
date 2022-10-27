library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.STD_LOGIC_ARITH.all;
use IEEE.STD_LOGIC_UNSIGNED.all;
entity mult_18_22_40_16_5_NoDSP48_Sig_6 is
  generic (
    a_width : integer := 18;
    b_width : integer := 22;
    result_width : integer := 40;
    result_start : integer := 16;
    pipestages : integer := 5
  );
  port (
    clk : in std_logic;
    ce : in std_logic;
    in_a : in std_logic_vector(0 to a_width-1);
    in_b : in std_logic_vector(0 to b_width-1);
    result : out std_logic_vector(0 to result_width-result_start-1)
  );
attribute mult_style: string;
attribute use_dsp48: string;
attribute use_dsp48 of mult_18_22_40_16_5_NoDSP48_Sig_6 : entity is "no";
end mult_18_22_40_16_5_NoDSP48_Sig_6;
architecture rtl of mult_18_22_40_16_5_NoDSP48_Sig_6 is
type pipe_reg_t is array (0 to pipestages-1) of std_logic_vector(0 to (a_width + b_width)-1);
signal pipe_reg : pipe_reg_t;
attribute mult_style of pipe_reg : signal is "pipe_lut";
begin
  p0: process(clk)
  begin
    if clk'event and clk = '1' and ce = '1' then
      pipe_reg(0) <= signed(in_a) * signed(in_b);
      for i in 0 to pipestages-2 loop
        pipe_reg(i+1) <= pipe_reg(i);
      end loop; -- i
    end if;
  end process p0;
  result <= pipe_reg(pipestages-1)(0 to result_width-result_start-1);
end rtl;
