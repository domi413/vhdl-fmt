#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/table.hpp"
#include "emit/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

using emit::Doc;
using emit::test::defaultConfig;

TEST_CASE("Table with simple alignment", "[table]")
{
    using emit::makeTable;

    const std::vector<std::vector<Doc>> rows = {
        { Doc::text("enable"),   Doc::text(":"), Doc::text("in"),  Doc::text("std_logic")        },
        { Doc::text("clk"),      Doc::text(":"), Doc::text("in"),  Doc::text("std_logic")        },
        { Doc::text("data_out"), Doc::text(":"), Doc::text("out"), Doc::text("std_logic_vector") }
    };

    auto table = makeTable(rows, defaultConfig());

    const std::string expected = "enable   : in  std_logic\n"
                                 "clk      : in  std_logic\n"
                                 "data_out : out std_logic_vector";
    REQUIRE(table.render(defaultConfig()) == expected);
}

TEST_CASE("Table composable with other Doc operations", "[table]")
{
    using emit::makeTable;

    const std::vector<std::vector<Doc>> rows = {
        { Doc::text("port1"), Doc::text(":"), Doc::text("in"),  Doc::text("std_logic") },
        { Doc::text("port2"), Doc::text(":"), Doc::text("out"), Doc::text("std_logic") }
    };

    auto table = makeTable(rows, defaultConfig());
    auto doc
      = Doc::text("entity MyEntity is") / Doc::bracket(Doc::text("port ("), table, Doc::text(");"));

    const std::string expected = "entity MyEntity is\n"
                                 "port (\n"
                                 "  port1 : in  std_logic\n"
                                 "  port2 : out std_logic\n"
                                 ");";

    REQUIRE(doc.render(defaultConfig()) == expected);
}

TEST_CASE("Empty table", "[table]")
{
    using emit::makeTable;

    const std::vector<std::vector<Doc>> rows;
    auto table = makeTable(rows, defaultConfig());

    REQUIRE(table.render(defaultConfig()).empty());
}
