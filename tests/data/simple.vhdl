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
  -- Architecture body (left intentionally empty)
end rtl;