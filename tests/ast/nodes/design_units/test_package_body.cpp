#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Package Body Declaration
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Implementation of package subprograms",
          "[package_body][design_unit][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body MyPackage is
            function Add(a, b : integer) return integer is
            begin
                return a + b;
            end Add;
        end MyPackage;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "MyPackage");
    // REQUIRE(pkg_body->end_label.has_value());
    // REQUIRE(pkg_body->end_label.value() == "MyPackage");
}

// -----------------------------------------------------------------------------
// Package Body with Function Implementations
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Captures function implementations", "[package_body][subprograms][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body MathPkg is
            function Add(a, b : integer) return integer is
            begin
                return a + b;
            end function Add;

            function Subtract(a, b : integer) return integer is
            begin
                return a - b;
            end function Subtract;
        end MathPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "MathPkg");
}

// -----------------------------------------------------------------------------
// Package Body with Procedure Implementations
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Captures procedure implementations", "[package_body][subprograms][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body ProcPkg is
            procedure Reset(signal clk : in std_logic) is
            begin
                wait until rising_edge(clk);
            end procedure Reset;

            procedure Write(signal data : out std_logic_vector) is
            begin
                data <= (others => '0');
            end procedure Write;
        end ProcPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "ProcPkg");
}

// -----------------------------------------------------------------------------
// Package Body with Local Declarations
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Handles local declarations", "[package_body][declarations][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body UtilPkg is
            constant LOCAL_CONST : integer := 42;
            type local_type is (A, B, C);

            function GetValue return integer is
            begin
                return LOCAL_CONST;
            end function GetValue;
        end UtilPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "UtilPkg");
    // REQUIRE(pkg_body->decls.size() >= 2);
}

// -----------------------------------------------------------------------------
// Package Body without End Label
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Can omit end label", "[package_body][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body SimplePkg is
            function Identity(x : integer) return integer is
            begin
                return x;
            end function;
        end;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "SimplePkg");
}

// -----------------------------------------------------------------------------
// Minimal Package Body
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Minimal empty package body", "[package_body][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body EmptyPkg is
        end EmptyPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "EmptyPkg");
}

// -----------------------------------------------------------------------------
// Package Body with Complex Function
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Complex function with local variables", "[package_body][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body ComplexPkg is
            function Factorial(n : integer) return integer is
                variable result : integer := 1;
                variable i : integer;
            begin
                for i in 2 to n loop
                    result := result * i;
                end loop;
                return result;
            end function Factorial;
        end ComplexPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "ComplexPkg");
}

// -----------------------------------------------------------------------------
// Package Body with Mixed Subprograms
// -----------------------------------------------------------------------------
TEST_CASE("PackageBody: Handles both functions and procedures", "[package_body][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body MixedPkg is
            function Add(a, b : integer) return integer is
            begin
                return a + b;
            end function;

            procedure Log(message : string) is
            begin
                report message;
            end procedure;
        end MixedPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageBody design unit is implemented
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "MixedPkg");
}
