#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Port: Single input port", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk : in std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names.size() == 1);
    REQUIRE(port.names[0] == "clk");
    REQUIRE(port.mode == "in");
    REQUIRE(port.type_name == "std_logic");
}

TEST_CASE("Port: Single output port", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (valid : out std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "valid");
    REQUIRE(port.mode == "out");
    REQUIRE(port.type_name == "std_logic");
}

TEST_CASE("Port: Output vector port with constraints", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : out std_logic_vector(7 downto 0));
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "data");
    REQUIRE(port.mode == "out");
    REQUIRE(port.type_name == "std_logic_vector");
    REQUIRE(port.constraints.size() == 1);
}

TEST_CASE("Port: Multiple ports same declaration", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (clk, rst, enable : in std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names.size() == 3);
    REQUIRE(port.names[0] == "clk");
    REQUIRE(port.names[1] == "rst");
    REQUIRE(port.names[2] == "enable");
    REQUIRE(port.mode == "in");
}

TEST_CASE("Port: Mixed port modes", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (
                clk : in std_logic;
                data_in : in std_logic_vector(7 downto 0);
                data_out : out std_logic_vector(7 downto 0);
                valid : out std_logic
            );
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 4);

    REQUIRE(entity->port_clause.ports[0].mode == "in");
    REQUIRE(entity->port_clause.ports[0].names[0] == "clk");

    REQUIRE(entity->port_clause.ports[1].mode == "in");
    REQUIRE(entity->port_clause.ports[1].names[0] == "data_in");

    REQUIRE(entity->port_clause.ports[2].mode == "out");
    REQUIRE(entity->port_clause.ports[2].names[0] == "data_out");

    REQUIRE(entity->port_clause.ports[3].mode == "out");
    REQUIRE(entity->port_clause.ports[3].names[0] == "valid");
}

TEST_CASE("Port: Inout port", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (bidirectional : inout std_logic_vector(7 downto 0));
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "bidirectional");
    REQUIRE(port.mode == "inout");
    REQUIRE(port.type_name == "std_logic_vector");
}

TEST_CASE("Port: Buffer port", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (feedback : buffer std_logic);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "feedback");
    REQUIRE(port.mode == "buffer");
    REQUIRE(port.type_name == "std_logic");
}

TEST_CASE("Port: Integer type port", "[declarations][port]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (count : in integer range 0 to 255);
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "count");
    REQUIRE(port.mode == "in");
    REQUIRE(port.type_name == "integer");
}
