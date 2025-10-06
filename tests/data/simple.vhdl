-- Entity declaration for a simple counter module
entity MyEntity is
    generic (
        -- Configuration constants
        CONST_V      : integer := 42;           -- Example constant
        DATA_WIDTH   : natural := 8;            -- Bit width of the counter
        USE_RESET    : boolean := true;         -- Whether reset is used
        INIT_VALUE   : integer := 0;            -- Initial counter value
        CLOCK_FREQ   : real := 100.0e6;         -- 100 MHz clock frequency
        DEVICE_FAMILY: string := "Generic"      -- Target device family
    );
    port (
        -- Clock and Reset
        clk      : in  std_logic;               -- System clock
        rst_n    : in  std_logic;               -- Active-low reset

        -- Enable and data interface
        enable   : in  std_logic;               -- Enable counting
        load     : in  std_logic;               -- Load input value
        load_val : in  std_logic_vector(DATA_WIDTH-1 downto 0); -- Load data

        -- Counter output
        count    : out std_logic_vector(DATA_WIDTH-1 downto 0); -- Counter output
        overflow : out std_logic                -- Overflow flag
    );
end MyEntity;
