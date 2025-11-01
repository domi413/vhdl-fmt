#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Package Declaration
// -----------------------------------------------------------------------------
TEST_CASE("Package: Basic package declaration with shared declarations",
          "[package][design_unit][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package MyPackage is
            constant MAX_VAL : integer := 100;
            function Add(a, b : integer) return integer;
        end MyPackage;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "MyPackage");
    // REQUIRE(pkg->end_label.has_value());
    // REQUIRE(pkg->end_label.value() == "MyPackage");
}

// -----------------------------------------------------------------------------
// Package with Constants
// -----------------------------------------------------------------------------
TEST_CASE("Package: Captures constant declarations", "[package][declarations][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package Constants is
            constant MAX_VAL : integer := 100;
            constant MIN_VAL : integer := 0;
            constant DEFAULT_WIDTH : integer := 8;
        end Constants;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "Constants");
    // REQUIRE(pkg->decls.size() >= 3);
}

// -----------------------------------------------------------------------------
// Package with Function Declarations
// -----------------------------------------------------------------------------
TEST_CASE("Package: Captures function declarations", "[package][subprograms][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package MathPkg is
            function Add(a, b : integer) return integer;
            function Subtract(a, b : integer) return integer;
            function Multiply(a, b : integer) return integer;
        end MathPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "MathPkg");
}

// -----------------------------------------------------------------------------
// Package with Type Declarations
// -----------------------------------------------------------------------------
TEST_CASE("Package: Captures type declarations", "[package][types][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package TypesPkg is
            type state_type is (IDLE, RUNNING, STOPPED);
            type byte is array (7 downto 0) of bit;
            subtype small_int is integer range 0 to 255;
        end TypesPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "TypesPkg");
}

// -----------------------------------------------------------------------------
// Package with Procedure Declarations
// -----------------------------------------------------------------------------
TEST_CASE("Package: Captures procedure declarations", "[package][subprograms][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package ProcPkg is
            procedure Reset(signal clk : in std_logic);
            procedure Write(signal data : out std_logic_vector);
        end ProcPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "ProcPkg");
}

// -----------------------------------------------------------------------------
// Package without End Label
// -----------------------------------------------------------------------------
TEST_CASE("Package: Can omit end label", "[package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package SimplePkg is
            constant VAL : integer := 42;
        end;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "SimplePkg");
}

// -----------------------------------------------------------------------------
// Minimal Package
// -----------------------------------------------------------------------------
TEST_CASE("Package: Minimal empty package", "[package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package EmptyPkg is
        end EmptyPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "EmptyPkg");
    // REQUIRE(pkg->decls.empty());
}

// -----------------------------------------------------------------------------
// Package with Mixed Declarations
// -----------------------------------------------------------------------------
TEST_CASE("Package: Handles mixed declaration types", "[package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package MixedPkg is
            type state_t is (IDLE, ACTIVE);
            constant TIMEOUT : integer := 1000;
            function IsIdle(state : state_t) return boolean;
            signal shared_sig : std_logic;
        end MixedPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Package design unit is implemented
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "MixedPkg");
}
