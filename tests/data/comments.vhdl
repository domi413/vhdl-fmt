-- License text

-- Entity declaration for a simple counter module
entity MyEntity is
    generic (
        -- Configuration constants
        CONST_V      : integer := 42;           -- Example constant
        DATA_WIDTH   : natural := 8             -- Bit width of the counter
    );
    port (
        -- Clock and Reset
        clk      : in  std_logic;               -- System clock
        rst_n    : in  std_logic                -- Active-low reset
    );
end MyEntity;
