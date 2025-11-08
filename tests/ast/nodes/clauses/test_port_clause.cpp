#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("makePortClause: Single input port", "[clauses][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names.size() == 1);
    REQUIRE(port.names[0] == "clk");
    REQUIRE(port.mode == "in");
    REQUIRE(port.type_name == "std_logic");
}

TEST_CASE("makePortClause: Single output port", "[clauses][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (valid : out std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "valid");
    REQUIRE(port.mode == "out");
    REQUIRE(port.type_name == "std_logic");
}

TEST_CASE("makePortClause: Multiple ports same declaration", "[clauses][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk, rst, enable : in std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names.size() == 3);
    REQUIRE(port.names[0] == "clk");
    REQUIRE(port.names[1] == "rst");
    REQUIRE(port.names[2] == "enable");
    REQUIRE(port.mode == "in");
}

TEST_CASE("makePortClause: Multiple separate port declarations", "[clauses][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (
                clk : in std_logic;
                rst : in std_logic;
                data_out : out std_logic_vector(7 downto 0)
            );
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 3);

    REQUIRE(entity->port_clause.ports[0].names[0] == "clk");
    REQUIRE(entity->port_clause.ports[0].mode == "in");

    REQUIRE(entity->port_clause.ports[1].names[0] == "rst");
    REQUIRE(entity->port_clause.ports[1].mode == "in");

    REQUIRE(entity->port_clause.ports[2].names[0] == "data_out");
    REQUIRE(entity->port_clause.ports[2].mode == "out");
}

TEST_CASE("makePortClause: Inout port", "[clauses][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : inout std_logic_vector(7 downto 0));
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "data");
    REQUIRE(port.mode == "inout");
}

TEST_CASE("makePortClause: Buffer port", "[clauses][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data_buf : buffer std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "data_buf");
    REQUIRE(port.mode == "buffer");
}
