#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("InterfacePackage: As generic parameter (VHDL-2008)", "[interface_package][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MyEntity is
            generic (package IntPkg is new GenericPkg generic map (<>));
            port (clk : in std_logic);
        end MyEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // REQUIRE(design.units.size() == 1);
    // auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    // REQUIRE(entity != nullptr);
}

TEST_CASE("InterfacePackage: With box notation for unspecified generics", "[interface_package][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Processor is
            generic (package MathPkg is new GenericMathPkg generic map (<>));
            port (data : in std_logic_vector(7 downto 0));
        end Processor;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    // REQUIRE(entity != nullptr);
}

TEST_CASE("InterfacePackage: Multiple interface packages", "[interface_package][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MultiPkgEntity is
            generic (
                package MathPkg is new GenericMathPkg generic map (<>);
                package IOPkg is new GenericIOPkg generic map (<>)
            );
        end MultiPkgEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    // REQUIRE(entity != nullptr);
}

TEST_CASE("InterfacePackage: Mixed with regular generics", "[interface_package][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity MixedGenericEntity is
            generic (
                WIDTH : integer := 8;
                package UtilPkg is new GenericUtilPkg generic map (<>);
                ENABLE_DEBUG : boolean := false
            );
        end MixedGenericEntity;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    // REQUIRE(entity != nullptr);
}
