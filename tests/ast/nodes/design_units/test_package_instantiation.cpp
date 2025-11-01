#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("PackageInstantiation: Generic package instantiation (VHDL-2008)", "[package_instantiation][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package IntPkg is new GenericPkg generic map (dtype => integer);
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // REQUIRE(design.units.size() == 1);
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "IntPkg");
}

TEST_CASE("PackageInstantiation: With multiple generic parameters", "[package_instantiation][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package CustomPkg is new GenericPkg
            generic map (
                dtype => std_logic_vector,
                WIDTH => 32,
                SIGNED => true
            );
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
}

TEST_CASE("PackageInstantiation: With qualified package name", "[package_instantiation][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package MyIntPkg is new work.GenericPkg generic map (dtype => integer);
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
}
