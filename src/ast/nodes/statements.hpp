#ifndef AST_NODES_STATEMENTS_HPP
#define AST_NODES_STATEMENTS_HPP

#include "ast/node.hpp"
#include "ast/nodes/expressions.hpp"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

// Forward declarations
struct ConcurrentSignalAssignmentStatement;
struct SignalAssignmentStatement;
struct IfStatement;
struct CaseStatement;
struct ProcessStatement;
struct ForLoopStatement;
struct WhileLoopStatement;
struct WaitStatement;
struct AssertStatement;
struct ReportStatement;
struct NextStatement;
struct ExitStatement;
struct ReturnStatement;
struct ProcedureCallStatement;
struct NullStatement;

/// Variant type for concurrent statements (outside processes)
using ConcurrentStatement = std::variant<ConcurrentSignalAssignmentStatement, ProcessStatement>;

/// Variant type for sequential statements (inside processes)
using SequentialStatement = std::variant<SignalAssignmentStatement,
                                         IfStatement,
                                         CaseStatement,
                                         ForLoopStatement,
                                         WhileLoopStatement,
                                         WaitStatement,
                                         AssertStatement,
                                         ReportStatement,
                                         NextStatement,
                                         ExitStatement,
                                         ReturnStatement,
                                         ProcedureCallStatement,
                                         NullStatement>;

/// @brief Concurrent signal assignment: target <= value;
struct ConcurrentSignalAssignmentStatement : NodeBase
{
    Expression target;
    Expression value;
};

/// @brief Sequential signal/variable assignment: target := value;
struct SignalAssignmentStatement : NodeBase
{
    Expression target;
    Expression value;
};

/// @brief If statement with optional elsif and else branches
struct IfStatement : NodeBase
{
    struct Branch
    {
        std::optional<NodeTrivia> trivia;
        Expression condition; // Empty for else branch
        std::vector<SequentialStatement> body;
    };

    Branch if_branch;                   // The initial if
    std::vector<Branch> elsif_branches; // elsif clauses
    std::optional<Branch> else_branch;  // Optional else
};

/// @brief Case statement with when clauses
struct CaseStatement : NodeBase
{
    struct WhenClause
    {
        std::optional<NodeTrivia> trivia;
        std::vector<Expression> choices; // Can be multiple: when 1 | 2 | 3 =>
        std::vector<SequentialStatement> body;
    };

    Expression selector; // The expression being switched on
    std::vector<WhenClause> when_clauses;
};

/// @brief Process statement (sensitivity list + sequential statements)
struct ProcessStatement : NodeBase
{
    std::optional<std::string> label;
    std::vector<std::string> sensitivity_list;
    std::vector<SequentialStatement> body;
};

/// @brief For loop: for i in range loop ... end loop;
struct ForLoopStatement : NodeBase
{
    std::string iterator; // Loop variable name
    Expression range;     // Range expression (e.g., 0 to 10)
    std::vector<SequentialStatement> body;
};

/// @brief While loop: while condition loop ... end loop;
struct WhileLoopStatement : NodeBase
{
    Expression condition;
    std::vector<SequentialStatement> body;
};

/// @brief Wait statement: wait for/on/until condition
struct WaitStatement : NodeBase
{
    std::optional<std::string> label;
    std::vector<std::string> sensitivity_list; // ON sensitivity list
    std::optional<Expression> condition;       // UNTIL condition
    std::optional<Expression> timeout;         // FOR timeout
};

/// @brief Assert statement: assert condition report message severity level
struct AssertStatement : NodeBase
{
    std::optional<std::string> label;
    Expression condition;
    std::optional<Expression> report_expr;
    std::optional<Expression> severity_expr;
};

/// @brief Report statement: report message severity level
struct ReportStatement : NodeBase
{
    std::optional<std::string> label;
    Expression message;
    std::optional<Expression> severity_expr;
};

/// @brief Next statement: next [loop_label] [when condition]
struct NextStatement : NodeBase
{
    std::optional<std::string> label;
    std::optional<std::string> loop_label; // Which loop to continue
    std::optional<Expression> condition;   // WHEN condition
};

/// @brief Exit statement: exit [loop_label] [when condition]
struct ExitStatement : NodeBase
{
    std::optional<std::string> label;
    std::optional<std::string> loop_label; // Which loop to exit
    std::optional<Expression> condition;   // WHEN condition
};

/// @brief Return statement: return [expression]
struct ReturnStatement : NodeBase
{
    std::optional<std::string> label;
    std::optional<Expression> value;
};

/// @brief Procedure call statement: procedure_name(args)
struct ProcedureCallStatement : NodeBase
{
    std::optional<std::string> label;
    std::string procedure_name;
    std::optional<Expression> args; // Arguments (single expr or GroupExpr)
};

/// @brief Null statement: null;
struct NullStatement : NodeBase
{
    std::optional<std::string> label;
};

} // namespace ast

#endif /* AST_NODES_STATEMENTS_HPP */
