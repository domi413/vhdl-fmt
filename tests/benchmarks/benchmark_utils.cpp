#include "benchmark_utils.hpp"

#include <antlr4-runtime/ANTLRInputStream.h>
#include <antlr4-runtime/BailErrorStrategy.h>
#include <antlr4-runtime/CommonTokenStream.h>
#include <antlr4-runtime/DefaultErrorStrategy.h>
#include <antlr4-runtime/Exceptions.h>
#include <antlr4-runtime/atn/ParserATNSimulator.h>
#include <antlr4-runtime/atn/PredictionMode.h>
#include <memory>
#include <string_view>
#include <vhdlLexer.h>
#include <vhdlParser.h>

namespace utils {

constexpr std::string_view STRESS_TEST_VHDL = R"(
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity benchmark_entity is
    Generic ( WIDTH : integer := 32 );
    Port (
        clk      : in  STD_LOGIC;
        rst      : in  STD_LOGIC;
        data_in  : in  STD_LOGIC_VECTOR (WIDTH-1 downto 0);
        data_out : out STD_LOGIC_VECTOR (WIDTH-1 downto 0)
    );
end benchmark_entity;

architecture Behavioral of benchmark_entity is
    type memory_t is array (0 to 255) of std_logic_vector(WIDTH-1 downto 0);
    signal ram : memory_t := (others => (others => '0'));
    signal addr : integer range 0 to 255 := 0;
begin
    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                addr <= 0;
                data_out <= (others => '0');
            else
                if addr < 255 then
                    addr <= addr + 1;
                else
                    addr <= 0;
                end if;
                if (data_in(0) = '1' and data_in(1) = '0') or (data_in(WIDTH-1) = '1') then
                    ram(addr) <= data_in;
                end if;
                data_out <= ram(addr);
            end if;
        end if;
    end process;
end Behavioral;
)";

ParsingContext::ParsingContext(std::string_view source)
{
    input = std::make_unique<antlr4::ANTLRInputStream>(source);
    lexer = std::make_unique<vhdlLexer>(input.get());
    tokens = std::make_unique<antlr4::CommonTokenStream>(lexer.get());
    tokens->fill();
    parser = std::make_unique<vhdlParser>(tokens.get());
}

void ParsingContext::parse(bool use_sll)
{
    auto *interpreter = parser->getInterpreter<antlr4::atn::ParserATNSimulator>();

    if (use_sll) {
        interpreter->setPredictionMode(antlr4::atn::PredictionMode::SLL);
        parser->setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());
        parser->removeErrorListeners();
    } else {
        interpreter->setPredictionMode(antlr4::atn::PredictionMode::LL_EXACT_AMBIG_DETECTION);
        parser->setErrorHandler(std::make_shared<antlr4::DefaultErrorStrategy>());
    }

    tree = parser->design_file();
}

auto benchmarkRawParse(std::string_view source, bool use_sll) -> vhdlParser::Design_fileContext *
{
    antlr4::ANTLRInputStream input(source);
    vhdlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    vhdlParser parser(&tokens);
    auto *interpreter = parser.getInterpreter<antlr4::atn::ParserATNSimulator>();

    if (use_sll) {
        interpreter->setPredictionMode(antlr4::atn::PredictionMode::SLL);
        parser.setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());
        parser.removeErrorListeners();
        try {
            return parser.design_file();
        } catch (const antlr4::ParseCancellationException &) {
            // SLL Failed, retry LL (just like production code)
            tokens.reset();
            parser.reset();
            parser.setErrorHandler(std::make_shared<antlr4::DefaultErrorStrategy>());
            interpreter->setPredictionMode(antlr4::atn::PredictionMode::LL);
            return parser.design_file();
        }
    } else {
        interpreter->setPredictionMode(antlr4::atn::PredictionMode::LL_EXACT_AMBIG_DETECTION);
        return parser.design_file();
    }
}

} // namespace utils
