#include "ast/design_file.hpp"
#include "builder/assembly/node_builder.hpp"
#include "builder/translator.hpp"
#include "builder/visitor.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parse entity ports into AST", "[integration][ports]")
{
    // --- Input VHDL ---
    const std::string vhdl_code = R"vhdl(
        entity MyEntity is
            port (
                clk     : in  std_logic;
                rst_n   : in  std_logic;
                load_val: in  std_logic_vector(DATA_WIDTH-1 downto 0);
                count   : out std_logic_vector(DATA_WIDTH-1 downto 0)
            );
        end MyEntity;
    )vhdl";

    // --- Setup ANTLR ---
    antlr4::ANTLRInputStream input(vhdl_code);
    vhdlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    vhdlParser parser(&tokens);

    // Parse a design file
    auto *tree = parser.design_file();

    // --- Build AST ---
    ast::DesignFile design;
    builder::ASTBuilder builder(design.units);
    builder::Translator translator(builder, tokens);
    builder::Visitor visitor(translator);

    visitor.walk(tree);

    // --- Verify AST ---
    REQUIRE(design.units.size() == 1);
    auto *entity = dynamic_cast<ast::Entity *>(design.units[0].get());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->name == "MyEntity");

    // Ports
    REQUIRE(entity->ports.size() == 4);

    // clk
    REQUIRE(entity->ports[0]->names == std::vector<std::string>{ "clk" });
    REQUIRE(entity->ports[0]->mode == "in");
    REQUIRE(entity->ports[0]->type == "std_logic");

    // rst_n
    REQUIRE(entity->ports[1]->names == std::vector<std::string>{ "rst_n" });
    REQUIRE(entity->ports[1]->mode == "in");
    REQUIRE(entity->ports[1]->type == "std_logic");

    // load_val
    REQUIRE(entity->ports[2]->names == std::vector<std::string>{ "load_val" });
    REQUIRE(entity->ports[2]->mode == "in");
    REQUIRE(entity->ports[2]->type == "std_logic_vector");
    REQUIRE(entity->ports[2]->constraints.size() == 1);
    REQUIRE(entity->ports[2]->constraints[0]->direction == "downto");

    // count
    REQUIRE(entity->ports[3]->names == std::vector<std::string>{ "count" });
    REQUIRE(entity->ports[3]->mode == "out");
    REQUIRE(entity->ports[3]->type == "std_logic_vector");
}
