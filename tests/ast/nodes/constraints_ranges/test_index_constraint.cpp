#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("IndexConstraint: Single range constraint", "[constraints_ranges][index_constraint]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : in std_logic_vector(7 downto 0));
        end E;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    const auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    const auto &port = entity->port_clause.ports[0];
    REQUIRE(port.constraint.has_value());

    const auto *index_constraint = std::get_if<ast::IndexConstraint>(&port.constraint.value());
    REQUIRE(index_constraint != nullptr);
    REQUIRE_FALSE(index_constraint->ranges.children.empty());
}

TEST_CASE("IndexConstraint: Ascending range constraint", "[constraints_ranges][index_constraint]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : out std_logic_vector(0 to 15));
        end E;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    const auto *entity = std::get_if<ast::Entity>(&design.units[0]);
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    const auto &port = entity->port_clause.ports[0];
    REQUIRE(port.constraint.has_value());

    const auto *index_constraint = std::get_if<ast::IndexConstraint>(&port.constraint.value());
    REQUIRE(index_constraint != nullptr);
    REQUIRE_FALSE(index_constraint->ranges.children.empty());
}

TEST_CASE("IndexConstraint: Multi-dimensional array constraint",
          "[constraints_ranges][index_constraint]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            type matrix_t is array(0 to 7, 0 to 7) of std_logic;
            signal matrix : matrix_t;
        begin
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 2);
}
