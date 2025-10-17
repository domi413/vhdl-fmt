-- Top-level entity for testing the AST builder
entity ExampleEntity is
  -- Configuration parameters
  generic (
    WIDTH      : integer := 8;      -- Bit width
    ENABLE_LOG : boolean := true    -- Enable debug output
  );

  -- I/O ports
  port (
    -- simple constant
        enable : in  std_logic := '1';
    -- constant expression
    width  : in  integer := 8 + 4;
    clk    : in  std_logic;         -- System clock
    rst_n  : in  std_logic;         -- Active-low reset
    data_i : in  std_logic_vector(WIDTH-1 downto 0); -- Input data bus
    data_o : out std_logic_vector(WIDTH-1 downto 0)  -- Output data bus
  );
end ExampleEntity;

architecture rtl of ExampleEntity is
  -- Internal signal declarations
  signal temp : std_logic_vector(7 downto 0);
  signal sum  : integer := 4 + 2;   -- Simple expression test
begin
  -- Architecture body (left intentionally empty)
end rtl;