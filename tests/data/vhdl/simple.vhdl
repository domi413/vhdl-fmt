-- Top-level entity for testing the AST builder
entity ExampleEntity is
  generic (
    WIDTH      : integer := 8; 
    ENABLE_LOG : boolean := true 
  );

  port (
    enable : in  std_logic := '1';
    width  : in  integer := 8 + 4;
    clk    : in  std_logic; 
    rst_n  : in  std_logic; 
    data_i : in  std_logic_vector(WIDTH-1 downto 0);
    data_o : out std_logic_vector(WIDTH-1 downto 0)
  );
end ExampleEntity;

architecture rtl of ExampleEntity is
  signal temp : std_logic_vector(7 downto 0);
  signal sum  : integer := 4 + 2;
  signal v : std_logic_vector(7 downto 0) := (others => '0');
  constant WORD_SIZE : integer := 32;
begin
  -- Simple concurrent signal assignment
  data_o <= data_i;
  
  -- Process with sensitivity list
  process(clk, rst_n)
  begin
    if rst_n = '0' then
      temp <= (others => '0');
    elsif rising_edge(clk) then
      temp <= data_i;
    end if;
  end process;
  
  -- Another process with case statement
  process(temp)
  begin
    case temp(1 downto 0) is
      when "00" =>
        temp <= "00000001";
      when "01" =>
        temp <= "00000010";
      when others =>
        temp <= "00000011";
    end case;
  end process;
  
end rtl;