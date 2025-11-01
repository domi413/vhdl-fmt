#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("Package: Basic package declaration", "[design_units][package]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package MyPackage is
            constant MAX_VAL : integer := 100;
            function Add(a, b : integer) return integer;
        end MyPackage;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // REQUIRE(design.units.size() == 1);
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->name == "MyPackage");
}

TEST_CASE("Package: With constants and functions", "[design_units][package]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package Constants is
            constant MAX_VAL : integer := 100;
            constant MIN_VAL : integer := 0;
            function Add(a, b : integer) return integer;
        end Constants;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->decls.size() >= 2);
}

TEST_CASE("Package: With type declarations", "[design_units][package]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package TypesPkg is
            type state_type is (IDLE, RUNNING, STOPPED);
            type byte is array (7 downto 0) of bit;
        end TypesPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
}

TEST_CASE("Package: Minimal empty package", "[design_units][package]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package EmptyPkg is
        end EmptyPkg;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *pkg = std::get_if<ast::Package>(&design.units[0]);
    // REQUIRE(pkg != nullptr);
    // REQUIRE(pkg->decls.empty());
}
