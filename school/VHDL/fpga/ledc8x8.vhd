----------------------------------------------------------------------------------
-- Company: FIT.VUTBR
-- Engineer: Richard Chomjak
-- 
-- Create Date:    10:10:18 10/13/2011 
-- Design Name: 
-- Module Name:    matica - Behavioral 
-- Project Name: 
-- Target Devices: FITkit
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: Vypisovanie inicialov na maticovy display (8X8) \
-- v mojom pripade RC
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_arith.all;
use IEEE.STD_LOGIC_unsigned.all;


entity ledc8x8 is
Port (
		SMCLK	:in std_logic;              -- V entite su prvky(porty), komunikuju aj s externymi zariadeniami
		RESET :in std_logic;				
		ROW:out std_logic_vector(0 to 7);
		LED:out std_logic_vector(0 to 7)
		);
end ledc8x8;



architecture Behavioral of ledc8x8 is		-- signaly, ktore prepajaju dane funkcne bloky

signal	 	   ce:std_logic;
signal 	  switch:std_logic;
signal 	dec:std_logic_vector(0 to 7);
signal 	ctrl_cnt:std_logic_vector(21 downto 0) :=(others => '0'); 
signal 		  row_cnt:STD_LOGIC_VECTOR(7 downto 0);

begin
--- ctrl_cnt
ctrl_cnt_proc: process (SMCLK, RESET,CE,ctrl_cnt)

begin

if RESET = '1' then
	ctrl_cnt <="0000000000000000000000";  -- Ak pride reset vynuluje sa signal ctrl_cnt

-- Spomalenie hodin, ak pride signal a je TRUE, 
-- tak inkrementuje ctrl_cnt

 elsif SMCLK'event AND SMCLK='1' then    
	ctrl_cnt<=ctrl_cnt +'1';
	
-- Pri dosiahnuti hodnoty 0xFF hodinovy signal na riadenie 
-- ostatnych blokov inkrementuje
	
	if ctrl_cnt(7 downto 0) ="11111111" then
		ce<='1';
	else 
		ce<='0';
	end if;

 end if;
 
switch<=ctrl_cnt(21);

end process;



--- row_cnt
row_cnt_proc: process (SMCLK, RESET,CE,row_cnt)

begin
if RESET='1' then

row_cnt<="01111111";

elsif SMCLK'event AND SMCLK='1' then

	if ce = '1' then
	
	row_cnt <= row_cnt(0) & row_cnt(7 downto 1);	 
	
	end if;
 end if;
 
 ROW<=row_cnt;
 
end process;

--- dec

dec_proc: process (ce,row_cnt)

begin

case (row_cnt) is

	when "01111111" => dec <= "11100000";
	when "10111111" => dec <= "10010000";
	when "11011111" => dec <= "11100000";
	when "11101111" => dec <= "10100111";
	when "11110111" => dec <= "10010100";
	when "11111011" => dec <= "00000100";
	when "11111101" => dec <= "00000100";
	when "11111110" => dec <= "00000111";
	when others => dec <="00000000";  -- kvoli prehladnosti/simulacii aj ked MUX je kompletny

end case;

end process;


---blink

blink_proc: process(ce,dec,switch)

begin

case switch is

	when '1' => led <="00000000";
	when '0' => led <=dec;
	when others => led <="00000000";

end case;


end process;

end Behavioral;

