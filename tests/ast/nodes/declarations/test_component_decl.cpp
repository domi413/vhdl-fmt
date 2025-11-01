#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("ComponentDecl: Component for instantiation", "[declarations][component]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            component MyComponent is
                port (input : in std_logic; output : out std_logic);
            end component;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check component declaration when implemented
}

TEST_CASE("ComponentDecl: Component with generics", "[declarations][component]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            component MyComp is
                generic (WIDTH : integer := 8);
                port (input : in std_logic_vector(WIDTH-1 downto 0); output : out std_logic_vector(WIDTH-1 downto 0));
            end component;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check component with generics when implemented
}

TEST_CASE("ComponentDecl: Component with multiple ports", "[declarations][component]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            component Adder is
                port (a, b : in std_logic_vector(7 downto 0); sum : out std_logic_vector(7 downto 0); carry : out std_logic);
            end component;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check component with multiple ports when implemented
}
