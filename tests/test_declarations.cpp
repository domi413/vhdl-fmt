#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/adapter/antlr_void_adapter.hpp"
#include "builder/translator.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "builder/visitor.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <variant>

namespace {

auto buildAstFromSource(const std::string &vhdl_code) -> std::unique_ptr<ast::DesignFile>
{
    antlr4::ANTLRInputStream input(vhdl_code);
    vhdlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    vhdlParser parser(&tokens);

    auto *tree = parser.design_file();

    auto design = std::make_unique<ast::DesignFile>();
    builder::TriviaBinder trivia(tokens);
    builder::Translator translator(trivia, tokens);
    translator.setUnitsDestination(design->units);
    builder::Visitor visitor(translator);
    builder::adapter::AntlrVoidAdapter adapter(visitor);

    tree->accept(&adapter);
    return design;
}

} // namespace

// -----------------------------------------------------------------------------
// Architecture declarations: signals and constants
// -----------------------------------------------------------------------------
TEST_CASE("Architecture captures signal declarations", "[declarations][architecture]")
{
    const std::string vhdl = R"(
        entity E is end E;
        architecture A of E is
            signal temp : std_logic;
            signal count : integer := 42;
        begin
        end A;
    )";

    auto design = buildAstFromSource(vhdl);
    REQUIRE(design->units.size() == 2);
    
    auto *arch = std::get_if<ast::Architecture>(design->units.data() + 1);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 2);

    // First signal: temp
    auto *signal1 = std::get_if<ast::SignalDecl>(&arch->decls[0]);
    REQUIRE(signal1 != nullptr);
    REQUIRE(signal1->names.size() == 1);
    REQUIRE(signal1->names[0] == "temp");
    REQUIRE(signal1->type_name == "std_logic");
    REQUIRE_FALSE(signal1->init_expr.has_value());

    // Second signal: count with initialization
    auto *signal2 = std::get_if<ast::SignalDecl>(&arch->decls[1]);
    REQUIRE(signal2 != nullptr);
    REQUIRE(signal2->names.size() == 1);
    REQUIRE(signal2->names[0] == "count");
    REQUIRE(signal2->type_name == "integer");
    REQUIRE(signal2->init_expr.has_value());
}

TEST_CASE("Architecture captures constant declarations", "[declarations][architecture]")
{
    const std::string vhdl = R"(
        entity E is end E;
        architecture A of E is
            constant WIDTH : integer := 8;
            constant ENABLE : boolean := true;
        begin
        end A;
    )";

    auto design = buildAstFromSource(vhdl);
    REQUIRE(design->units.size() == 2);
    
    auto *arch = std::get_if<ast::Architecture>(design->units.data() + 1);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 2);

    // First constant: WIDTH
    auto *const1 = std::get_if<ast::ConstantDecl>(&arch->decls[0]);
    REQUIRE(const1 != nullptr);
    REQUIRE(const1->names.size() == 1);
    REQUIRE(const1->names[0] == "WIDTH");
    REQUIRE(const1->type_name == "integer");
    REQUIRE(const1->init_expr.has_value());

    // Second constant: ENABLE
    auto *const2 = std::get_if<ast::ConstantDecl>(&arch->decls[1]);
    REQUIRE(const2 != nullptr);
    REQUIRE(const2->names.size() == 1);
    REQUIRE(const2->names[0] == "ENABLE");
    REQUIRE(const2->type_name == "boolean");
    REQUIRE(const2->init_expr.has_value());
}

TEST_CASE("Architecture captures mixed declarations", "[declarations][architecture]")
{
    const std::string vhdl = R"(
        entity E is end E;
        architecture A of E is
            signal clk : std_logic;
            constant MAX : integer := 100;
            signal data : std_logic_vector(7 downto 0);
            constant MIN : integer := 0;
        begin
        end A;
    )";

    auto design = buildAstFromSource(vhdl);
    REQUIRE(design->units.size() == 2);
    
    auto *arch = std::get_if<ast::Architecture>(design->units.data() + 1);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 4);

    // Verify types in order: signal, constant, signal, constant
    REQUIRE(std::holds_alternative<ast::SignalDecl>(arch->decls[0]));
    REQUIRE(std::holds_alternative<ast::ConstantDecl>(arch->decls[1]));
    REQUIRE(std::holds_alternative<ast::SignalDecl>(arch->decls[2]));
    REQUIRE(std::holds_alternative<ast::ConstantDecl>(arch->decls[3]));

    // Verify names
    auto *sig1 = std::get_if<ast::SignalDecl>(&arch->decls[0]);
    REQUIRE(sig1->names[0] == "clk");
    
    auto *const1 = std::get_if<ast::ConstantDecl>(&arch->decls[1]);
    REQUIRE(const1->names[0] == "MAX");
    
    auto *sig2 = std::get_if<ast::SignalDecl>(&arch->decls[2]);
    REQUIRE(sig2->names[0] == "data");
    
    auto *const2 = std::get_if<ast::ConstantDecl>(&arch->decls[3]);
    REQUIRE(const2->names[0] == "MIN");
}

TEST_CASE("Architecture with no declarations", "[declarations][architecture]")
{
    const std::string vhdl = R"(
        entity E is end E;
        architecture A of E is
        begin
        end A;
    )";

    auto design = buildAstFromSource(vhdl);
    REQUIRE(design->units.size() == 2);
    
    auto *arch = std::get_if<ast::Architecture>(design->units.data() + 1);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.empty());
}

TEST_CASE("Signal with multiple names", "[declarations][signal]")
{
    const std::string vhdl = R"(
        entity E is end E;
        architecture A of E is
            signal clk, rst, enable : std_logic;
        begin
        end A;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *arch = std::get_if<ast::Architecture>(design->units.data() + 1);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(&arch->decls[0]);
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names.size() == 3);
    REQUIRE(signal->names[0] == "clk");
    REQUIRE(signal->names[1] == "rst");
    REQUIRE(signal->names[2] == "enable");
    REQUIRE(signal->type_name == "std_logic");
}

TEST_CASE("Constant with multiple names", "[declarations][constant]")
{
    const std::string vhdl = R"(
        entity E is end E;
        architecture A of E is
            constant MIN, MAX, DEFAULT : integer := 42;
        begin
        end A;
    )";

    auto design = buildAstFromSource(vhdl);
    auto *arch = std::get_if<ast::Architecture>(design->units.data() + 1);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *constant = std::get_if<ast::ConstantDecl>(&arch->decls[0]);
    REQUIRE(constant != nullptr);
    REQUIRE(constant->names.size() == 3);
    REQUIRE(constant->names[0] == "MIN");
    REQUIRE(constant->names[1] == "MAX");
    REQUIRE(constant->names[2] == "DEFAULT");
    REQUIRE(constant->type_name == "integer");
    REQUIRE(constant->init_expr.has_value());
}
