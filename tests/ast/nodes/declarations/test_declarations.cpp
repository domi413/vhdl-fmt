#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// =============================================================================
// Mixed Declarations
// =============================================================================

TEST_CASE("Multiple declaration types in architecture", "[declarations][mixed]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal clk : std_logic;
            constant MAX : integer := 100;
            signal data : std_logic_vector(7 downto 0);
            constant MIN : integer := 0;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 4);

    REQUIRE(std::holds_alternative<ast::SignalDecl>(arch->decls[0]));
    REQUIRE(std::holds_alternative<ast::ConstantDecl>(arch->decls[1]));
    REQUIRE(std::holds_alternative<ast::SignalDecl>(arch->decls[2]));
    REQUIRE(std::holds_alternative<ast::ConstantDecl>(arch->decls[3]));

    auto *sig1 = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(sig1->names[0] == "clk");

    auto *const1 = std::get_if<ast::ConstantDecl>(&arch->decls[1]);
    REQUIRE(const1->names[0] == "MAX");

    auto *sig2 = std::get_if<ast::SignalDecl>(&arch->decls[2]);
    REQUIRE(sig2->names[0] == "data");

    auto *const2 = std::get_if<ast::ConstantDecl>(&arch->decls[3]);
    REQUIRE(const2->names[0] == "MIN");
}

TEST_CASE("Architecture with no declarations", "[declarations][empty]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.empty());
}

// =============================================================================
// Not Yet Implemented Declarations (commented out)
// =============================================================================

TEST_CASE("VariableDecl: Variable in process", "[declarations][variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
        begin
            process
                variable count : integer := 0;
            begin
                count := count + 1;
            end process;
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check variable declaration when implemented
}

TEST_CASE("TypeDecl: Enumeration type", "[declarations][type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type StateType is (IDLE, RUNNING, DONE);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check type declaration when implemented
}

TEST_CASE("TypeDecl: Record type", "[declarations][type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type RecordType is record
                field1 : integer;
                field2 : std_logic;
            end record;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check type declaration when implemented
}

TEST_CASE("TypeDecl: Array type", "[declarations][type]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            type ByteArray is array (0 to 15) of std_logic_vector(7 downto 0);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check type declaration when implemented
}

TEST_CASE("SubtypeDecl: Constrained subtype", "[declarations][subtype]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            subtype SmallInt is integer range 0 to 255;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check subtype declaration when implemented
}

TEST_CASE("FunctionDecl: Function prototype", "[declarations][function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            function Add(a, b : integer) return integer;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function declaration when implemented
}

TEST_CASE("ProcedureDecl: Procedure prototype", "[declarations][procedure]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            procedure Reset(signal clk : in std_logic);
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure declaration when implemented
}

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

TEST_CASE("AliasDecl: Simple alias", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal LongSignalName : std_logic;
            alias ShortName is LongSignalName;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check alias declaration when implemented
}

TEST_CASE("AliasDecl: Object alias with slice", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(15 downto 0);
            alias byte : std_logic_vector(7 downto 0) is data(15 downto 8);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check object alias when implemented
}

TEST_CASE("AliasDecl: Non-object alias", "[declarations][alias]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            alias sl is std_logic;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check non-object alias when implemented
}

TEST_CASE("AttributeDecl: Attribute declaration", "[declarations][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            attribute KEEP : boolean;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check attribute declaration when implemented
}

TEST_CASE("AttributeSpec: Attribute specification", "[declarations][attribute]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal my_signal : std_logic;
            attribute KEEP of my_signal : signal is true;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check attribute specification when implemented
}

TEST_CASE("FileDecl: File declaration", "[declarations][file]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            file data_file : text open read_mode is "data.txt";
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check file declaration when implemented
}

TEST_CASE("UseClause: Use clause import", "[declarations][use]")
{
    constexpr std::string_view VHDL_FILE = R"(
        use work.MyPackage.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check use clause when implemented
}

TEST_CASE("LibraryClause: Library declaration", "[declarations][library]")
{
    constexpr std::string_view VHDL_FILE = R"(
        library IEEE;
        use IEEE.std_logic_1164.all;
        entity E is end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check library clause when implemented
}

TEST_CASE("SharedVariable: Shared variable declaration", "[declarations][shared_variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            shared variable count : integer := 0;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check shared variable when implemented
}

TEST_CASE("FunctionBody: Function implementation", "[declarations][function]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            function Add(a, b : integer) return integer is
            begin
                return a + b;
            end function Add;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check function body when implemented
}

TEST_CASE("ProcedureBody: Procedure implementation", "[declarations][procedure]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body P is
            procedure Reset(signal clk : in std_logic) is
            begin
                wait until rising_edge(clk);
            end procedure Reset;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check procedure body when implemented
}
