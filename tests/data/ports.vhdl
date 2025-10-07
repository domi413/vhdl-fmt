entity MyEntity is
    port (
        clk     : in  std_logic;
        rst_n   : in  std_logic;
        load_val: in  std_logic_vector(DATA_WIDTH-1 downto 0);
        count   : out std_logic_vector(DATA_WIDTH-1 downto 0)
    );
end MyEntity;