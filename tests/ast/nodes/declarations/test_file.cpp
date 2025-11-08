#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("makeFileDecl: File declaration", "[declarations][file]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            file data_file : text open read_mode is "data.txt";
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check file declaration when implemented
}

TEST_CASE("makeFileDecl: File with different modes", "[declarations][file]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            file input_file : text open read_mode is "input.txt";
            file output_file : text open write_mode is "output.txt";
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check file declarations with modes when implemented
}

TEST_CASE("makeFileDecl: File without open mode", "[declarations][file]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            file log_file : text is "log.txt";
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check file without mode when implemented
}

TEST_CASE("makeFileDecl: File in subprogram", "[declarations][file]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            procedure ReadFile is
                file f : text open read_mode is "test.txt";
            begin
                -- read
            end procedure;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check file in subprogram when implemented
}
