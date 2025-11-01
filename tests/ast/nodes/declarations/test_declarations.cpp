#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// =============================================================================
// Signal Declarations
// =============================================================================

TEST_CASE("SignalDecl: Single signal with type", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal temp : std_logic;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names.size() == 1);
    REQUIRE(signal->names[0] == "temp");
    REQUIRE(signal->type_name == "std_logic");
    REQUIRE_FALSE(signal->init_expr.has_value());
}

TEST_CASE("SignalDecl: Signal with initialization", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal count : integer := 42;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names.size() == 1);
    REQUIRE(signal->names[0] == "count");
    REQUIRE(signal->type_name == "integer");
    REQUIRE(signal->init_expr.has_value());
}

TEST_CASE("SignalDecl: Multiple signals same declaration", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal clk, rst, enable : std_logic;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names.size() == 3);
    REQUIRE(signal->names[0] == "clk");
    REQUIRE(signal->names[1] == "rst");
    REQUIRE(signal->names[2] == "enable");
    REQUIRE(signal->type_name == "std_logic");
}

TEST_CASE("SignalDecl: Vector signal with constraints", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names[0] == "data");
    REQUIRE(signal->type_name == "std_logic_vector");
    REQUIRE(signal->constraints.size() == 1);
}

TEST_CASE("SignalDecl: Vector with aggregate initialization", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0) := (others => '0');
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names[0] == "data");
    REQUIRE(signal->type_name == "std_logic_vector");
    REQUIRE(signal->init_expr.has_value());
}

// =============================================================================
// Constant Declarations
// =============================================================================

TEST_CASE("ConstantDecl: Simple constant with initialization", "[declarations][constant]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            constant WIDTH : integer := 8;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *constant = std::get_if<ast::ConstantDecl>(arch->decls.data());
    REQUIRE(constant != nullptr);
    REQUIRE(constant->names.size() == 1);
    REQUIRE(constant->names[0] == "WIDTH");
    REQUIRE(constant->type_name == "integer");
    REQUIRE(constant->init_expr.has_value());
}

TEST_CASE("ConstantDecl: Multiple constants same declaration", "[declarations][constant]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            constant MIN, MAX, DEFAULT : integer := 42;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *constant = std::get_if<ast::ConstantDecl>(arch->decls.data());
    REQUIRE(constant != nullptr);
    REQUIRE(constant->names.size() == 3);
    REQUIRE(constant->names[0] == "MIN");
    REQUIRE(constant->names[1] == "MAX");
    REQUIRE(constant->names[2] == "DEFAULT");
    REQUIRE(constant->type_name == "integer");
    REQUIRE(constant->init_expr.has_value());
}

TEST_CASE("ConstantDecl: Boolean constant", "[declarations][constant]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            constant ENABLE : boolean := true;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *constant = std::get_if<ast::ConstantDecl>(arch->decls.data());
    REQUIRE(constant != nullptr);
    REQUIRE(constant->names[0] == "ENABLE");
    REQUIRE(constant->type_name == "boolean");
}

// =============================================================================
// Generic Parameters
// =============================================================================

TEST_CASE("GenericParam: Single generic with default", "[declarations][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (WIDTH : integer := 8);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 1);

    auto &generic = entity->generic_clause.generics[0];
    REQUIRE(generic.names.size() == 1);
    REQUIRE(generic.names[0] == "WIDTH");
    REQUIRE(generic.type_name == "integer");
    REQUIRE(generic.default_expr.has_value());
}

TEST_CASE("GenericParam: Multiple generics", "[declarations][generic]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            generic (
                WIDTH : integer := 8;
                RESET_VAL : integer := 0;
                ENABLE_ASYNC : boolean := false
            );
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->generic_clause.generics.size() == 3);

    REQUIRE(entity->generic_clause.generics[0].names[0] == "WIDTH");
    REQUIRE(entity->generic_clause.generics[0].type_name == "integer");

    REQUIRE(entity->generic_clause.generics[1].names[0] == "RESET_VAL");
    REQUIRE(entity->generic_clause.generics[1].type_name == "integer");

    REQUIRE(entity->generic_clause.generics[2].names[0] == "ENABLE_ASYNC");
    REQUIRE(entity->generic_clause.generics[2].type_name == "boolean");
}

// =============================================================================
// Port Declarations
// =============================================================================

TEST_CASE("Port: Input port", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names.size() == 1);
    REQUIRE(port.names[0] == "clk");
    REQUIRE(port.mode == "in");
    REQUIRE(port.type_name == "std_logic");
}

TEST_CASE("Port: Output port", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : out std_logic_vector(7 downto 0));
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "data");
    REQUIRE(port.mode == "out");
    REQUIRE(port.type_name == "std_logic_vector");
    REQUIRE(port.constraints.size() == 1);
}

TEST_CASE("Port: Multiple ports same declaration", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk, rst, enable : in std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names.size() == 3);
    REQUIRE(port.names[0] == "clk");
    REQUIRE(port.names[1] == "rst");
    REQUIRE(port.names[2] == "enable");
    REQUIRE(port.mode == "in");
}

TEST_CASE("Port: Mixed port modes", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (
                clk : in std_logic;
                data_in : in std_logic_vector(7 downto 0);
                data_out : out std_logic_vector(7 downto 0);
                valid : out std_logic
            );
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 4);

    REQUIRE(entity->port_clause.ports[0].mode == "in");
    REQUIRE(entity->port_clause.ports[1].mode == "in");
    REQUIRE(entity->port_clause.ports[2].mode == "out");
    REQUIRE(entity->port_clause.ports[3].mode == "out");
}

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

// TEST_CASE("VariableDecl: Variable in process", "[declarations][variable]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//         begin
//             process
//                 variable count : integer := 0;
//             begin
//                 count := count + 1;
//             end process;
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check variable declaration when implemented
// }

// TEST_CASE("TypeDecl: Enumeration type", "[declarations][type]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             type StateType is (IDLE, RUNNING, DONE);
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check type declaration when implemented
// }

// TEST_CASE("TypeDecl: Record type", "[declarations][type]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             type RecordType is record
//                 field1 : integer;
//                 field2 : std_logic;
//             end record;
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check type declaration when implemented
// }

// TEST_CASE("TypeDecl: Array type", "[declarations][type]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             type ByteArray is array (0 to 15) of std_logic_vector(7 downto 0);
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check type declaration when implemented
// }

// TEST_CASE("SubtypeDecl: Constrained subtype", "[declarations][subtype]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             subtype SmallInt is integer range 0 to 255;
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check subtype declaration when implemented
// }

// TEST_CASE("FunctionDecl: Function prototype", "[declarations][function]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             function Add(a, b : integer) return integer;
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check function declaration when implemented
// }

// TEST_CASE("ProcedureDecl: Procedure prototype", "[declarations][procedure]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             procedure Reset(signal clk : in std_logic);
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check procedure declaration when implemented
// }

// TEST_CASE("ComponentDecl: Component for instantiation", "[declarations][component]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             component MyComponent is
//                 port (input : in std_logic; output : out std_logic);
//             end component;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check component declaration when implemented
// }

// TEST_CASE("AliasDecl: Simple alias", "[declarations][alias]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal LongSignalName : std_logic;
//             alias ShortName is LongSignalName;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check alias declaration when implemented
// }

// TEST_CASE("AliasDecl: Object alias with slice", "[declarations][alias]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal data : std_logic_vector(15 downto 0);
//             alias byte : std_logic_vector(7 downto 0) is data(15 downto 8);
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check object alias when implemented
// }

// TEST_CASE("AliasDecl: Non-object alias", "[declarations][alias]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             alias sl is std_logic;
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check non-object alias when implemented
// }

// TEST_CASE("AttributeDecl: Attribute declaration", "[declarations][attribute]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package P is
//             attribute KEEP : boolean;
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check attribute declaration when implemented
// }

// TEST_CASE("AttributeSpec: Attribute specification", "[declarations][attribute]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal my_signal : std_logic;
//             attribute KEEP of my_signal : signal is true;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check attribute specification when implemented
// }

// TEST_CASE("FileDecl: File declaration", "[declarations][file]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             file data_file : text open read_mode is "data.txt";
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check file declaration when implemented
// }

// TEST_CASE("UseClause: Use clause import", "[declarations][use]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         use work.MyPackage.all;
//         entity E is end E;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check use clause when implemented
// }

// TEST_CASE("LibraryClause: Library declaration", "[declarations][library]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         library IEEE;
//         use IEEE.std_logic_1164.all;
//         entity E is end E;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check library clause when implemented
// }

// TEST_CASE("SharedVariable: Shared variable declaration", "[declarations][shared_variable]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             shared variable count : integer := 0;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check shared variable when implemented
// }

// TEST_CASE("FunctionBody: Function implementation", "[declarations][function]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package body P is
//             function Add(a, b : integer) return integer is
//             begin
//                 return a + b;
//             end function Add;
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check function body when implemented
// }

// TEST_CASE("ProcedureBody: Procedure implementation", "[declarations][procedure]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         package body P is
//             procedure Reset(signal clk : in std_logic) is
//             begin
//                 wait until rising_edge(clk);
//             end procedure Reset;
//         end P;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check procedure body when implemented
// }
