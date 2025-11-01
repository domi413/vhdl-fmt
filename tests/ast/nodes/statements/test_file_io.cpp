#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("FileIOStatement: Read from file", "[statements][file_io]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
                file data_file : text;
                variable data_value : integer;
            begin
                read(data_file, data_value);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check file IO statement when implemented
}

TEST_CASE("FileIOStatement: Write to file", "[statements][file_io]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
                file data_file : text;
                variable data_value : integer := 42;
            begin
                write(data_file, data_value);
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO: Check file IO statement when implemented
}
