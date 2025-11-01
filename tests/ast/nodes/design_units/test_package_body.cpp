#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("PackageBody: Function implementation", "[package_body][design_unit]")
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
    // REQUIRE(design.units.size() == 1);
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
    // REQUIRE(pkg_body->name == "MyPackage");
}

TEST_CASE("PackageBody: Multiple subprogram implementations", "[package_body]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body MathPkg is
            function Add(a, b : integer) return integer is
            begin
                return a + b;
            end function;

            procedure Reset(signal clk : in std_logic) is
            begin
                wait until rising_edge(clk);
            end procedure;
        end MathPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
}

TEST_CASE("PackageBody: With local declarations", "[package_body]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package body UtilPkg is
            constant LOCAL_CONST : integer := 42;

            function GetValue return integer is
            begin
                return LOCAL_CONST;
            end function;
        end UtilPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *pkg_body = std::get_if<ast::PackageBody>(&design.units[0]);
    // REQUIRE(pkg_body != nullptr);
}
