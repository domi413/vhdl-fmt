#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Package Instantiation
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Generic package instantiation (VHDL-2008)",
          "[package_instantiation][design_unit][vhdl2008][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package IntPkg is new GenericPkg generic map (dtype => integer);
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "IntPkg");
    // REQUIRE(pkg_inst->generic_package_name == "GenericPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Single Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Captures single generic map parameter",
          "[package_instantiation][generic_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package RealPkg is new MathPkg generic map (T => real);
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "RealPkg");
    // REQUIRE(pkg_inst->generic_package_name == "MathPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Multiple Generic Maps
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Handles multiple generic map parameters",
          "[package_instantiation][generic_map][.future]")
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

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "CustomPkg");
    // REQUIRE(pkg_inst->generic_map.size() >= 3);
}

// -----------------------------------------------------------------------------
// Package Instantiation with Type Generic
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Supports type as generic parameter",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package LogicVectorPkg is new VectorPkg
            generic map (element_type => std_logic);
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "LogicVectorPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Constant Generic
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Supports constant as generic parameter",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package Width8Pkg is new ParametricPkg
            generic map (BUS_WIDTH => 8);
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "Width8Pkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Function Generic
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Supports function as generic parameter",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package CustomOpPkg is new OperatorPkg
            generic map (op_func => my_custom_operation);
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "CustomOpPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Qualified Name
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Supports qualified generic package name",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package MyIntPkg is new work.GenericPkg generic map (dtype => integer);
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "MyIntPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Library Prefix
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Handles library.package qualified names",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package LocalPkg is new mylib.GenericPkg generic map (T => natural);
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "LocalPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Expression in Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Supports expressions in generic map",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package ComputedPkg is new GenericPkg
            generic map (SIZE => 2**8 - 1);
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "ComputedPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Named Association
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Uses named association in generic map",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package NamedPkg is new GenericPkg
            generic map (
                data_type => std_logic,
                width => 16,
                enable_debug => false
            );
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "NamedPkg");
}

// -----------------------------------------------------------------------------
// Multiple Package Instantiations
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Handles multiple instantiations in file",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package IntPkg is new GenericPkg generic map (dtype => integer);
        package RealPkg is new GenericPkg generic map (dtype => real);
        package BoolPkg is new GenericPkg generic map (dtype => boolean);
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 3);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *inst1 = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(inst1 != nullptr);
    // REQUIRE(inst1->name == "IntPkg");

    // auto *inst2 = std::get_if<ast::PackageInstantiation>(&design.units[1]);
    // REQUIRE(inst2 != nullptr);
    // REQUIRE(inst2->name == "RealPkg");

    // auto *inst3 = std::get_if<ast::PackageInstantiation>(&design.units[2]);
    // REQUIRE(inst3 != nullptr);
    // REQUIRE(inst3->name == "BoolPkg");
}

// -----------------------------------------------------------------------------
// Package Instantiation with Nested Types
// -----------------------------------------------------------------------------
TEST_CASE("PackageInstantiation: Supports complex type in generic map",
          "[package_instantiation][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package ArrayPkg is new GenericArrayPkg
            generic map (elem_type => std_logic_vector(7 downto 0));
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when PackageInstantiation design unit is implemented
    // auto *pkg_inst = std::get_if<ast::PackageInstantiation>(&design.units[0]);
    // REQUIRE(pkg_inst != nullptr);
    // REQUIRE(pkg_inst->name == "ArrayPkg");
}
