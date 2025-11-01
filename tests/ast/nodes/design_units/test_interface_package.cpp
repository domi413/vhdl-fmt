#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Interface Package
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Package used as generic parameter (VHDL-2008)",
          "[interface_package][generic][vhdl2008][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MyEntity is
            generic (package IntPkg is new GenericPkg generic map (<>));
            port (clk : in std_logic);
        end MyEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // REQUIRE(entity->generic_clause.generics.size() == 1);
    // const auto &gen = entity->generic_clause.generics[0];
    // Check if generic is an interface package
}

// -----------------------------------------------------------------------------
// Interface Package with Box (<>) Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Uses box notation for unspecified generics",
          "[interface_package][generic_map][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Processor is
            generic (
                package MathPkg is new GenericMathPkg generic map (<>)
            );
            port (data : in std_logic_vector(7 downto 0));
        end Processor;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify that the generic map uses box notation
}

// -----------------------------------------------------------------------------
// Interface Package with Default Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Supports default generic map values", "[interface_package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity ConfigurableEntity is
            generic (
                package ConfigPkg is new GenericConfigPkg generic map (default)
            );
        end ConfigurableEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify default keyword in generic map
}

// -----------------------------------------------------------------------------
// Interface Package with Partial Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Allows partial generic map specification",
          "[interface_package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity PartialEntity is
            generic (
                WIDTH : integer := 8;
                package DataPkg is new GenericDataPkg
                    generic map (data_width => WIDTH, others => <>)
            );
        end PartialEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify mixed specified and box generic map
}

// -----------------------------------------------------------------------------
// Multiple Interface Packages
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Entity with multiple interface packages",
          "[interface_package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MultiPkgEntity is
            generic (
                package MathPkg is new GenericMathPkg generic map (<>);
                package IOPkg is new GenericIOPkg generic map (<>);
                package DebugPkg is new GenericDebugPkg generic map (<>)
            );
        end MultiPkgEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // REQUIRE(entity->generic_clause.generics.size() == 3);
}

// -----------------------------------------------------------------------------
// Interface Package with Regular Generics
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Mixed with regular generic parameters",
          "[interface_package][generic][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MixedGenericEntity is
            generic (
                WIDTH : integer := 8;
                DEPTH : integer := 256;
                package UtilPkg is new GenericUtilPkg generic map (<>);
                ENABLE_DEBUG : boolean := false
            );
            port (clk : in std_logic);
        end MixedGenericEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // REQUIRE(entity->generic_clause.generics.size() == 4);
    // Verify mix of regular generics and interface package
}

// -----------------------------------------------------------------------------
// Interface Package with Qualified Name
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Supports qualified package names", "[interface_package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity QualifiedEntity is
            generic (
                package LocalPkg is new work.GenericPkg generic map (<>)
            );
        end QualifiedEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify qualified name parsing (work.GenericPkg)
}

// -----------------------------------------------------------------------------
// Interface Package with Library Prefix
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Handles library.package qualified names",
          "[interface_package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity LibEntity is
            generic (
                package ExtPkg is new mylib.GenericExtPkg generic map (<>)
            );
        end LibEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify library prefix handling
}

// -----------------------------------------------------------------------------
// Interface Package in Architecture
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Can be used in architecture generics",
          "[interface_package][architecture][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity TestEntity is
            generic (package TestPkg is new GenericTestPkg generic map (<>));
        end TestEntity;

        architecture RTL of TestEntity is
        begin
            -- Use TestPkg here
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify interface package is available in architecture
}

// -----------------------------------------------------------------------------
// Interface Package with Specific Generic Map
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Allows specific generic map values", "[interface_package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity SpecificEntity is
            generic (
                package TypePkg is new GenericTypePkg
                    generic map (element_type => std_logic_vector(7 downto 0))
            );
        end SpecificEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify specific type mapping in generic map
}

// -----------------------------------------------------------------------------
// Nested Interface Package Usage
// -----------------------------------------------------------------------------
TEST_CASE("InterfacePackage: Supports nested generic instantiation", "[interface_package][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity NestedEntity is
            generic (
                package OuterPkg is new GenericOuterPkg
                    generic map (
                        package InnerPkg is new GenericInnerPkg generic map (<>)
                    )
            );
        end NestedEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);

    // TODO: Uncomment when InterfacePackage is implemented
    // Verify nested package instantiation
}
