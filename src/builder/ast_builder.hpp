#ifndef BUILDER_AST_BUILDER_HPP
#define BUILDER_AST_BUILDER_HPP

#include "ast/nodes/design_file.hpp"

#include <filesystem>
#include <istream>
#include <string>

namespace builder {

/// @brief Build AST from a file path
///
/// Encapsulates the entire ANTLR parsing pipeline (lexer, parser, token stream)
/// and AST construction (trivia binding, translation) into a simple interface.
///
/// @param path Path to VHDL source file
/// @return Populated DesignFile AST
/// @throws std::runtime_error if file cannot be opened or parsed
[[nodiscard]]
auto buildFromFile(const std::filesystem::path &path) -> ast::DesignFile;

/// @brief Build AST from an input stream
/// @param input Input stream containing VHDL source code
/// @return Populated DesignFile AST
/// @throws std::runtime_error if parsing fails
[[nodiscard]]
auto buildFromStream(std::istream &input) -> ast::DesignFile;

/// @brief Build AST from a string
/// @param vhdl_code VHDL source code as string
/// @return Populated DesignFile AST
/// @throws std::runtime_error if parsing fails
[[nodiscard]]
auto buildFromString(const std::string &vhdl_code) -> ast::DesignFile;

} // namespace builder

#endif /* BUILDER_AST_BUILDER_HPP */
