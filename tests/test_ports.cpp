#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "builder/adapter/antlr_void_adapter.hpp"
#include "builder/assembly/assembler.hpp"
#include "builder/translator.hpp"
#include "builder/visitor.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {
auto loadVhdl(const std::string &filename) -> std::string
{
    const std::ifstream file(filename);
    REQUIRE(file.is_open());
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
} // namespace

TEST_CASE("Parse entity ports into AST", "[integration][ports]")
{
    // --- Input VHDL ---
    const std::string vhdl_code{ loadVhdl(std::string(TEST_DATA_DIR) + "/ports.vhdl") };

    // --- Setup ANTLR ---
    antlr4::ANTLRInputStream input(vhdl_code);
    vhdlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    vhdlParser parser(&tokens);

    // Parse a design file
    auto *tree{ parser.design_file() };

    // --- Build AST ---
    ast::DesignFile design;
    builder::Assembler builder(design.units);
    builder::Translator translator(builder, tokens);
    builder::Visitor visitor(translator);
    builder::adapter::AntlrVoidAdapter adapter(visitor);

    tree->accept(&adapter);

    // --- Verify AST ---
    REQUIRE(design.units.size() == 1);
    auto *entity{ dynamic_cast<ast::Entity *>(design.units[0].get()) };
    REQUIRE(entity != nullptr);
    REQUIRE(entity->name == "MyEntity");

    auto &port_clause = entity->port_clause;

    // Ports
    REQUIRE(port_clause->ports.size() == 4);

    // clk
    REQUIRE(port_clause->ports[0]->names == std::vector<std::string>{ "clk" });
    REQUIRE(port_clause->ports[0]->mode == "in");
    REQUIRE(port_clause->ports[0]->type_name == "std_logic");

    // rst_n
    REQUIRE(port_clause->ports[1]->names == std::vector<std::string>{ "rst_n" });
    REQUIRE(port_clause->ports[1]->mode == "in");
    REQUIRE(port_clause->ports[1]->type_name == "std_logic");

    // load_val
    REQUIRE(port_clause->ports[2]->names == std::vector<std::string>{ "load_val" });
    REQUIRE(port_clause->ports[2]->mode == "in");
    REQUIRE(port_clause->ports[2]->type_name == "std_logic_vector");
    REQUIRE(port_clause->ports[2]->constraints.size() == 1);
    REQUIRE(port_clause->ports[2]->constraints[0]->direction == "downto");

    // count
    REQUIRE(port_clause->ports[3]->names == std::vector<std::string>{ "count" });
    REQUIRE(port_clause->ports[3]->mode == "out");
    REQUIRE(port_clause->ports[3]->type_name == "std_logic_vector");
}
