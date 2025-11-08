#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "vhdlParser.h"

#include <CommonTokenStream.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace builder {

class Translator final
{
    TriviaBinder &trivia_;
    antlr4::CommonTokenStream &tokens_;

  public:
    Translator(TriviaBinder &tv, antlr4::CommonTokenStream &tokens) : trivia_(tv), tokens_(tokens)
    {
    }

    /// @brief Build the entire design file by walking the CST
    void buildDesignFile(ast::DesignFile &dest, vhdlParser::Design_fileContext *ctx);

    ~Translator() = default;

    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

  private:
    // clang-format off

    // Declarations
    [[nodiscard]] auto makeAliasDecl(vhdlParser::Alias_declarationContext *ctx) -> ast::AliasDecl;
    [[nodiscard]] auto makeAttributeDecl(vhdlParser::Attribute_declarationContext *ctx) -> ast::AttributeDecl;
    [[nodiscard]] auto makeBaseUnitDecl(vhdlParser::Base_unit_declarationContext *ctx) -> ast::BaseUnitDecl;
    [[nodiscard]] auto makeBranchQuantityDecl(vhdlParser::Branch_quantity_declarationContext *ctx) -> ast::BranchQuantityDecl;
    [[nodiscard]] auto makeComponentDecl(vhdlParser::Component_declarationContext *ctx) -> ast::ComponentDecl;
    [[nodiscard]] auto makeConfigurationDecl(vhdlParser::Configuration_declarationContext *ctx) -> ast::ConfigurationDecl;
    [[nodiscard]] auto makeConstantDecl(vhdlParser::Constant_declarationContext *ctx) -> ast::ConstantDecl;
    [[nodiscard]] auto makeDisconnectionSpecification(vhdlParser::Disconnection_specificationContext *ctx) -> ast::DisconnectionSpecification;
    [[nodiscard]] auto makeElementDecl(vhdlParser::Element_declarationContext *ctx) -> ast::ElementDecl;
    [[nodiscard]] auto makeEntityDecl(vhdlParser::Entity_declarationContext *ctx) -> ast::EntityDecl;
    [[nodiscard]] auto makeFileDecl(vhdlParser::File_declarationContext *ctx) -> ast::FileDecl;
    [[nodiscard]] auto makeFreeQuantityDecl(vhdlParser::Free_quantity_declarationContext *ctx) -> ast::FreeQuantityDecl;
    [[nodiscard]] auto makeGroupDecl(vhdlParser::Group_declarationContext *ctx) -> ast::GroupDecl;
    [[nodiscard]] auto makeGroupTemplateDecl(vhdlParser::Group_template_declarationContext *ctx) -> ast::GroupTemplateDecl;
    [[nodiscard]] auto makeInterfaceConstantDecl(vhdlParser::Interface_constant_declarationContext *ctx) -> ast::InterfaceConstantDecl;
    [[nodiscard]] auto makeInterfaceDecl(vhdlParser::Interface_declarationContext *ctx) -> ast::InterfaceDecl;
    [[nodiscard]] auto makeInterfaceFileDecl(vhdlParser::Interface_file_declarationContext *ctx) -> ast::InterfaceFileDecl;
    [[nodiscard]] auto makeInterfacePortDecl(vhdlParser::Interface_port_declarationContext *ctx) -> ast::InterfacePortDecl;
    [[nodiscard]] auto makeInterfaceQuantityDecl(vhdlParser::Interface_quantity_declarationContext *ctx) -> ast::InterfaceQuantityDecl;
    [[nodiscard]] auto makeInterfaceSignalDecl(vhdlParser::Interface_signal_declarationContext *ctx) -> ast::InterfaceSignalDecl;
    [[nodiscard]] auto makeInterfaceTerminalDecl(vhdlParser::Interface_terminal_declarationContext *ctx) -> ast::InterfaceTerminalDecl;
    [[nodiscard]] auto makeInterfaceVariableDecl(vhdlParser::Interface_variable_declarationContext *ctx) -> ast::InterfaceVariableDecl;
    [[nodiscard]] auto makeNatureDecl(vhdlParser::Nature_declarationContext *ctx) -> ast::NatureDecl;
    [[nodiscard]] auto makeNatureElementDecl(vhdlParser::Nature_element_declarationContext *ctx) -> ast::NatureElementDecl;
    [[nodiscard]] auto makeObjectDecl(vhdlParser::Object_declarationContext *ctx) -> ast::ObjectDecl;
    [[nodiscard]] auto makePackageDecl(vhdlParser::Package_declarationContext *ctx) -> ast::PackageDecl;
    [[nodiscard]] auto makeQuantityDecl(vhdlParser::Quantity_declarationContext *ctx) -> ast::QuantityDecl;
    [[nodiscard]] auto makeSecondaryUnitDecl(vhdlParser::Secondary_unit_declarationContext *ctx) -> ast::SecondaryUnitDecl;
    [[nodiscard]] auto makeSignalDecl(vhdlParser::Signal_declarationContext *ctx) -> ast::SignalDecl;
    [[nodiscard]] auto makeSourceQuantityDecl(vhdlParser::Source_quantity_declarationContext *ctx) -> ast::SourceQuantityDecl;
    [[nodiscard]] auto makeSubnatureDecl(vhdlParser::Subnature_declarationContext *ctx) -> ast::SubnatureDecl;
    [[nodiscard]] auto makeSubprogramDecl(vhdlParser::Subprogram_declarationContext *ctx) -> ast::SubprogramDecl;
    [[nodiscard]] auto makeSubtypeDecl(vhdlParser::Subtype_declarationContext *ctx) -> ast::SubtypeDecl;
    [[nodiscard]] auto makeTerminalDecl(vhdlParser::Terminal_declarationContext *ctx) -> ast::TerminalDecl;
    [[nodiscard]] auto makeTypeDecl(vhdlParser::Type_declarationContext *ctx) -> ast::TypeDecl;
    [[nodiscard]] auto makeVariableDecl(vhdlParser::Variable_declarationContext *ctx) -> ast::VariableDecl;

    // Statements
    [[nodiscard]] auto makeArchitectureStatement(vhdlParser::Architecture_statementContext *ctx) -> ast::ArchitectureStatement;
    [[nodiscard]] auto makeAssertionStatement(vhdlParser::Assertion_statementContext *ctx) -> ast::AssertionStatement;
    [[nodiscard]] auto makeBlockStatement(vhdlParser::Block_statementContext *ctx) -> ast::BlockStatement;
    [[nodiscard]] auto makeBreakStatement(vhdlParser::Break_statementContext *ctx) -> ast::BreakStatement;
    [[nodiscard]] auto makeCaseStatement(vhdlParser::Case_statementContext *ctx) -> ast::CaseStatement;
    [[nodiscard]] auto makeComponentInstantiationStatement(vhdlParser::Component_instantiation_statementContext *ctx) -> ast::ComponentInstantiationStatement;
    [[nodiscard]] auto makeConcurrentAssertionStatement(vhdlParser::Concurrent_assertion_statementContext *ctx) -> ast::ConcurrentAssertionStatement;
    [[nodiscard]] auto makeConcurrentBreakStatement(vhdlParser::Concurrent_break_statementContext *ctx) -> ast::ConcurrentBreakStatement;
    [[nodiscard]] auto makeConcurrentProcedureCallStatement(vhdlParser::Concurrent_procedure_call_statementContext *ctx) -> ast::ConcurrentProcedureCallStatement;
    [[nodiscard]] auto makeConcurrentSignalAssignmentStatement(vhdlParser::Concurrent_signal_assignment_statementContext *ctx) -> ast::ConcurrentSignalAssignmentStatement;
    [[nodiscard]] auto makeConditionalSignalAssignmentStatement(vhdlParser::Conditional_signal_assignmentContext *ctx) -> ast::ConcurrentSignalAssignmentStatement;
    [[nodiscard]] auto makeEntityStatement(vhdlParser::Entity_statementContext *ctx) -> ast::EntityStatement;
    [[nodiscard]] auto makeExitStatement(vhdlParser::Exit_statementContext *ctx) -> ast::ExitStatement;
    [[nodiscard]] auto makeGenerateStatement(vhdlParser::Generate_statementContext *ctx) -> ast::GenerateStatement;
    [[nodiscard]] auto makeIfStatement(vhdlParser::If_statementContext *ctx) -> ast::IfStatement;
    [[nodiscard]] auto makeLoopStatement(vhdlParser::Loop_statementContext *ctx) -> ast::LoopStatement;
    [[nodiscard]] auto makeNextStatement(vhdlParser::Next_statementContext *ctx) -> ast::NextStatement;
    [[nodiscard]] auto makeProcedureCallStatement(vhdlParser::Procedure_call_statementContext *ctx) -> ast::ProcedureCallStatement;
    [[nodiscard]] auto makeProcessStatement(vhdlParser::Process_statementContext *ctx) -> ast::ProcessStatement;
    [[nodiscard]] auto makeReportStatement(vhdlParser::Report_statementContext *ctx) -> ast::ReportStatement;
    [[nodiscard]] auto makeReturnStatement(vhdlParser::Return_statementContext *ctx) -> ast::ReturnStatement;
    [[nodiscard]] auto makeSelectedSignalAssignmentStatement(vhdlParser::Selected_signal_assignment_statementContext *ctx) -> ast::ConcurrentSignalAssignmentStatement;
    [[nodiscard]] auto makeSequenceOfStatements(vhdlParser::Sequence_of_statementsContext *ctx) -> std::vector<ast::SequentialStatement>;
    [[nodiscard]] auto makeSequentialStatement(vhdlParser::Sequential_statementContext *ctx) -> ast::SequentialStatement;
    [[nodiscard]] auto makeSignalAssignmentStatement(vhdlParser::Signal_assignment_statementContext *ctx) -> ast::SignalAssignmentStatement;
    [[nodiscard]] auto makeSimpleSimultaneousStatement(vhdlParser::Simple_simultaneous_statementContext *ctx) -> ast::SimpleSimultaneousStatement;
    [[nodiscard]] auto makeSimultaneousCaseStatement(vhdlParser::Simultaneous_case_statementContext *ctx) -> ast::SimultaneousCaseStatement;
    [[nodiscard]] auto makeSimultaneousIfStatement(vhdlParser::Simultaneous_if_statementContext *ctx) -> ast::SimultaneousIfStatement;
    [[nodiscard]] auto makeSimultaneousProceduralStatement(vhdlParser::Simultaneous_procedural_statementContext *ctx) -> ast::SimultaneousProceduralStatement;
    [[nodiscard]] auto makeSimultaneousStatement(vhdlParser::Simultaneous_statementContext *ctx) -> ast::SimultaneousStatement;
    [[nodiscard]] auto makeVariableAssignStatement(vhdlParser::Variable_assignment_statementContext *ctx) -> ast::SignalAssignmentStatement;
    [[nodiscard]] auto makeWaitStatement(vhdlParser::Wait_statementContext *ctx) -> ast::WaitStatement;

    // Expressions & Literals
    [[nodiscard]] auto makeAbstractLiteral(vhdlParser::Abstract_literalContext *ctx) -> ast::AbstractLiteral;
    [[nodiscard]] auto makeAggregate(vhdlParser::AggregateContext *ctx) -> ast::Aggregate;
    [[nodiscard]] auto makeAllocator(vhdlParser::AllocatorContext *ctx) -> ast::Allocator;
    [[nodiscard]] auto makeExpression(vhdlParser::ExpressionContext *ctx) -> ast::Expression;
    [[nodiscard]] auto makeFactor(vhdlParser::FactorContext *ctx) -> ast::Factor;
    [[nodiscard]] auto makeLiteral(vhdlParser::LiteralContext *ctx) -> ast::Literal;
    [[nodiscard]] auto makeNumericLiteral(vhdlParser::Numeric_literalContext *ctx) -> ast::NumericLiteral;
    [[nodiscard]] auto makePhysicalLiteral(vhdlParser::Physical_literalContext *ctx) -> ast::PhysicalLiteral;
    [[nodiscard]] auto makePrimary(vhdlParser::PrimaryContext *ctx) -> ast::Primary;
    [[nodiscard]] auto makeRelation(vhdlParser::RelationContext *ctx) -> ast::Relation;
    [[nodiscard]] auto makeShiftExpression(vhdlParser::Shift_expressionContext *ctx) -> ast::ShiftExpression;
    [[nodiscard]] auto makeSimpleExpression(vhdlParser::Simple_expressionContext *ctx) -> ast::SimpleExpression;
    [[nodiscard]] auto makeTerm(vhdlParser::TermContext *ctx) -> ast::Term;

    // Types & Subtypes
    [[nodiscard]] auto makeAccessTypeDefinition(vhdlParser::Access_type_definitionContext *ctx) -> ast::AccessTypeDefinition;
    [[nodiscard]] auto makeArrayNatureDefinition(vhdlParser::Array_nature_definitionContext *ctx) -> ast::ArrayNatureDefinition;
    [[nodiscard]] auto makeArrayTypeDefinition(vhdlParser::Array_type_definitionContext *ctx) -> ast::ArrayTypeDefinition;
    [[nodiscard]] auto makeCompositeNatureDefinition(vhdlParser::Composite_nature_definitionContext *ctx) -> ast::CompositeNatureDefinition;
    [[nodiscard]] auto makeCompositeTypeDefinition(vhdlParser::Composite_type_definitionContext *ctx) -> ast::CompositeTypeDefinition;
    [[nodiscard]] auto makeConstrainedArrayDefinition(vhdlParser::Constrained_array_definitionContext *ctx) -> ast::ConstrainedArrayDefinition;
    [[nodiscard]] auto makeConstrainedNatureDefinition(vhdlParser::Constrained_nature_definitionContext *ctx) -> ast::ConstrainedNatureDefinition;
    [[nodiscard]] auto makeElementSubnatureDefinition(vhdlParser::Element_subnature_definitionContext *ctx) -> ast::ElementSubnatureDefinition;
    [[nodiscard]] auto makeElementSubtypeDefinition(vhdlParser::Element_subtype_definitionContext *ctx) -> ast::ElementSubtypeDefinition;
    [[nodiscard]] auto makeEnumerationTypeDefinition(vhdlParser::Enumeration_type_definitionContext *ctx) -> ast::EnumerationTypeDefinition;
    [[nodiscard]] auto makeFileTypeDefinition(vhdlParser::File_type_definitionContext *ctx) -> ast::FileTypeDefinition;
    [[nodiscard]] auto makeNatureDefinition(vhdlParser::Nature_definitionContext *ctx) -> ast::NatureDefinition;
    [[nodiscard]] auto makePhysicalTypeDefinition(vhdlParser::Physical_type_definitionContext *ctx) -> ast::PhysicalTypeDefinition;
    [[nodiscard]] auto makeRecordNatureDefinition(vhdlParser::Record_nature_definitionContext *ctx) -> ast::RecordNatureDefinition;
    [[nodiscard]] auto makeRecordTypeDefinition(vhdlParser::Record_type_definitionContext *ctx) -> ast::RecordTypeDefinition;
    [[nodiscard]] auto makeScalarNatureDefinition(vhdlParser::Scalar_nature_definitionContext *ctx) -> ast::ScalarNatureDefinition;
    [[nodiscard]] auto makeScalarTypeDefinition(vhdlParser::Scalar_type_definitionContext *ctx) -> ast::ScalarTypeDefinition;
    [[nodiscard]] auto makeSubnatureIndication(vhdlParser::Subnature_indicationContext *ctx) -> ast::SubnatureIndication;
    [[nodiscard]] auto makeSubtypeIndication(vhdlParser::Subtype_indicationContext *ctx) -> ast::SubtypeIndication;
    [[nodiscard]] auto makeTypeDefinition(vhdlParser::Type_definitionContext *ctx) -> ast::TypeDefinition;
    [[nodiscard]] auto makeUnconstrainedArrayDefinition(vhdlParser::Unconstrained_array_definitionContext *ctx) -> ast::UnconstrainedArrayDefinition;
    [[nodiscard]] auto makeUnconstrainedNatureDefinition(vhdlParser::Unconstrained_nature_definitionContext *ctx) -> ast::UnconstrainedNatureDefinition;

    // Names & References
    [[nodiscard]] auto makeAliasDesignator(vhdlParser::Alias_designatorContext *ctx) -> ast::AliasDesignator;
    [[nodiscard]] auto makeAliasIndication(vhdlParser::Alias_indicationContext *ctx) -> ast::AliasIndication;
    [[nodiscard]] auto makeAttributeDesignator(vhdlParser::Attribute_designatorContext *ctx) -> ast::AttributeDesignator;
    [[nodiscard]] auto makeAttributeNamePart(ast::Expression base, vhdlParser::Attribute_name_partContext *ctx) -> ast::AttributeNamePart;
    [[nodiscard]] auto makeDesignator(vhdlParser::DesignatorContext *ctx) -> ast::Designator;
    [[nodiscard]] auto makeEntityDesignator(vhdlParser::Entity_designatorContext *ctx) -> ast::EntityDesignator;
    [[nodiscard]] auto makeEntityTag(vhdlParser::Entity_tagContext *ctx) -> ast::EntityTag;
    [[nodiscard]] auto makeEnumerationLiteral(vhdlParser::Enumeration_literalContext *ctx) -> ast::EnumerationLiteral;
    [[nodiscard]] auto makeFunctionCallOrIndexedNamePart(ast::Expression base, vhdlParser::Function_call_or_indexed_name_partContext *ctx) -> ast::FunctionCallOrIndexedNamePart;
    [[nodiscard]] auto makeIdentifier(vhdlParser::IdentifierContext *ctx) -> ast::Identifier;
    [[nodiscard]] auto makeIdentifierList(vhdlParser::Identifier_listContext *ctx) -> ast::IdentifierList;
    [[nodiscard]] auto makeName(vhdlParser::NameContext *ctx) -> ast::Name;
    [[nodiscard]] auto makeNamePart(vhdlParser::Name_partContext *ctx) -> ast::NamePart;
    [[nodiscard]] auto makeSelectedName(ast::Expression base, vhdlParser::Selected_nameContext *ctx) -> ast::SelectedName;
    [[nodiscard]] auto makeSelectedNamePart(ast::Expression base, vhdlParser::Selected_name_partContext *ctx) -> ast::SelectedNamePart;
    [[nodiscard]] auto makeSliceNamePart(ast::Expression base, vhdlParser::Slice_name_partContext *ctx) -> ast::SliceNamePart;
    [[nodiscard]] auto makeSuffix(vhdlParser::SuffixContext *ctx) -> ast::Suffix;
    [[nodiscard]] auto makeTarget(vhdlParser::TargetContext *ctx) -> ast::Target;

    // Aspects & Specifications
    [[nodiscard]] auto makeAcrossAspect(vhdlParser::Across_aspectContext *ctx) -> ast::AcrossAspect;
    [[nodiscard]] auto makeAttributeSpecification(vhdlParser::Attribute_specificationContext *ctx) -> ast::AttributeSpecification;
    [[nodiscard]] auto makeBindingIndication(vhdlParser::Binding_indicationContext *ctx) -> ast::BindingIndication;
    [[nodiscard]] auto makeBlockSpecification(vhdlParser::Block_specificationContext *ctx) -> ast::BlockSpecification;
    [[nodiscard]] auto makeComponentSpecification(vhdlParser::Component_specificationContext *ctx) -> ast::ComponentSpecification;
    [[nodiscard]] auto makeConfigurationSpecification(vhdlParser::Configuration_specificationContext *ctx) -> ast::ConfigurationSpecification;
    [[nodiscard]] auto makeEntityAspect(vhdlParser::Entity_aspectContext *ctx) -> ast::EntityAspect;
    [[nodiscard]] auto makeEntitySpecification(vhdlParser::Entity_specificationContext *ctx) -> ast::EntitySpecification;
    [[nodiscard]] auto makeGenericMapAspect(vhdlParser::Generic_map_aspectContext *ctx) -> ast::GenericMapAspect;
    [[nodiscard]] auto makePortMapAspect(vhdlParser::Port_map_aspectContext *ctx) -> ast::PortMapAspect;
    [[nodiscard]] auto makeQuantitySpecification(vhdlParser::Quantity_specificationContext *ctx) -> ast::QuantitySpecification;
    [[nodiscard]] auto makeSourceAspect(vhdlParser::Source_aspectContext *ctx) -> ast::SourceAspect;
    [[nodiscard]] auto makeTerminalAspect(vhdlParser::Terminal_aspectContext *ctx) -> ast::TerminalAspect;
    [[nodiscard]] auto makeThroughAspect(vhdlParser::Through_aspectContext *ctx) -> ast::ThroughAspect;
    [[nodiscard]] auto makeToleranceAspect(vhdlParser::Tolerance_aspectContext *ctx) -> ast::ToleranceAspect;

    // Constraints & Ranges
    [[nodiscard]] auto makeConstraint(vhdlParser::ConstraintContext *ctx) -> std::vector<ast::BinaryExpr>;
    [[nodiscard]] auto makeDiscreteRange(vhdlParser::Discrete_rangeContext *ctx) -> ast::DiscreteRange;
    [[nodiscard]] auto makeExplicitRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::ExplicitRange;
    [[nodiscard]] auto makeIndexConstraint(vhdlParser::Index_constraintContext *ctx) -> std::vector<ast::BinaryExpr>;
    [[nodiscard]] auto makeIndexSpecification(vhdlParser::Index_specificationContext *ctx) -> ast::IndexSpecification;
    [[nodiscard]] auto makeIndexSubtypeDefinition(vhdlParser::Index_subtype_definitionContext *ctx) -> ast::IndexSubtypeDefinition;
    [[nodiscard]] auto makeRangeConstraint(vhdlParser::Range_constraintContext *ctx) -> std::vector<ast::BinaryExpr>;
    [[nodiscard]] auto makeRangeDecl(vhdlParser::Range_declContext *ctx) -> ast::RangeDecl;
    [[nodiscard]] auto makeStepLimitSpecification(vhdlParser::Step_limit_specificationContext *ctx) -> ast::StepLimitSpecification;

    // Lists & Collections
    [[nodiscard]] auto makeAssociationElement(vhdlParser::Association_elementContext *ctx) -> ast::AssociationElement;
    [[nodiscard]] auto makeAssociationList(vhdlParser::Association_listContext *ctx) -> ast::AssociationList;
    [[nodiscard]] auto makeBreakElement(vhdlParser::Break_elementContext *ctx) -> ast::BreakElement;
    [[nodiscard]] auto makeBreakList(vhdlParser::Break_listContext *ctx) -> ast::BreakList;
    [[nodiscard]] auto makeBreakSelectorClause(vhdlParser::Break_selector_clauseContext *ctx) -> ast::BreakSelectorClause;
    [[nodiscard]] auto makeChoices(vhdlParser::ChoiceContext *ctx) -> ast::Choices;
    [[nodiscard]] auto makeEntityClassEntry(vhdlParser::Entity_class_entryContext *ctx) -> ast::EntityClassEntry;
    [[nodiscard]] auto makeEntityClassEntryList(vhdlParser::Entity_class_entry_listContext *ctx) -> ast::EntityClassEntryList;
    [[nodiscard]] auto makeEntityNameList(vhdlParser::Entity_name_listContext *ctx) -> ast::EntityNameList;
    [[nodiscard]] auto makeFormalParameterList(vhdlParser::Formal_parameter_listContext *ctx) -> ast::FormalParameterList;
    [[nodiscard]] auto makeGenericList(vhdlParser::Generic_listContext *ctx) -> ast::GenericList;
    [[nodiscard]] auto makeGroupConstituent(vhdlParser::Group_constituentContext *ctx) -> ast::GroupConstituent;
    [[nodiscard]] auto makeGroupConstituentList(vhdlParser::Group_constituent_listContext *ctx) -> ast::GroupConstituentList;
    [[nodiscard]] auto makeInstantiationList(vhdlParser::Instantiation_listContext *ctx) -> ast::InstantiationList;
    [[nodiscard]] auto makeInterfaceElement(vhdlParser::Interface_elementContext *ctx) -> ast::InterfaceElement;
    [[nodiscard]] auto makeInterfaceList(vhdlParser::Interface_listContext *ctx) -> ast::InterfaceList;
    [[nodiscard]] auto makeInterfacePortList(vhdlParser::Interface_port_listContext *ctx) -> ast::InterfacePortList;
    [[nodiscard]] auto makeInterfaceSignalList(vhdlParser::Interface_signal_listContext *ctx) -> ast::InterfaceSignalList;
    [[nodiscard]] auto makeLogicalNameList(vhdlParser::Logical_name_listContext *ctx) -> ast::LogicalNameList;
    [[nodiscard]] auto makeQuantityList(vhdlParser::Quantity_listContext *ctx) -> ast::QuantityList;
    [[nodiscard]] auto makeSignalList(vhdlParser::Signal_listContext *ctx) -> ast::SignalList;

    // Clauses & Mechanisms
    [[nodiscard]] auto makeConditionClause(vhdlParser::Condition_clauseContext *ctx) -> ast::ConditionClause;
    [[nodiscard]] auto makeContextClause(vhdlParser::Context_clauseContext *ctx) -> ast::ContextClause;
    [[nodiscard]] auto makeContextItem(vhdlParser::Context_itemContext *ctx) -> ast::ContextItem;
    [[nodiscard]] auto makeDelayMechanism(vhdlParser::Delay_mechanismContext *ctx) -> ast::DelayMechanism;
    [[nodiscard]] auto makeGenericClause(vhdlParser::Generic_clauseContext *ctx) -> ast::GenericClause;
    [[nodiscard]] auto makeGuardedSignalSpecification(vhdlParser::Guarded_signal_specificationContext *ctx) -> ast::GuardedSignalSpecification;
    [[nodiscard]] auto makeLibraryClause(vhdlParser::Library_clauseContext *ctx) -> ast::LibraryClause;
    [[nodiscard]] auto makePortClause(vhdlParser::Port_clauseContext *ctx) -> ast::PortClause;
    [[nodiscard]] auto makeSensitivityClause(vhdlParser::Sensitivity_clauseContext *ctx) -> ast::SensitivityClause;
    [[nodiscard]] auto makeSensitivityList(vhdlParser::Sensitivity_listContext *ctx) -> ast::SensitivityList;
    [[nodiscard]] auto makeTimeoutClause(vhdlParser::Timeout_clauseContext *ctx) -> ast::TimeoutClause;
    [[nodiscard]] auto makeUseClause(vhdlParser::Use_clauseContext *ctx) -> ast::UseClause;

    // Operators
    [[nodiscard]] auto makeAddingOperator(vhdlParser::Adding_operatorContext *ctx) -> ast::AddingOperator;
    [[nodiscard]] auto makeLogicalOperator(vhdlParser::Logical_operatorContext *ctx) -> ast::LogicalOperator;
    [[nodiscard]] auto makeMultiplyingOperator(vhdlParser::Multiplying_operatorContext *ctx) -> ast::MultiplyingOperator;
    [[nodiscard]] auto makeRelationalOperator(vhdlParser::Relational_operatorContext *ctx) -> ast::RelationalOperator;
    [[nodiscard]] auto makeShiftOperator(vhdlParser::Shift_operatorContext *ctx) -> ast::ShiftOperator;

    // Architecture / Package / Subprogram Structure
    [[nodiscard]] auto makeArchitectureBody(vhdlParser::Architecture_bodyContext *ctx) -> ast::ArchitectureBody;
    [[nodiscard]] auto makeArchitectureDeclarativePart(vhdlParser::Architecture_declarative_partContext *ctx) -> ast::ArchitectureDeclarativePart;
    [[nodiscard]] auto makeArchitectureStatementPart(vhdlParser::Architecture_statement_partContext *ctx) -> ast::ArchitectureStatementPart;
    [[nodiscard]] auto makePackageBody(vhdlParser::Package_bodyContext *ctx) -> ast::PackageBody;
    [[nodiscard]] auto makePackageBodyDeclarativeItem(vhdlParser::Package_body_declarative_itemContext *ctx) -> ast::PackageBodyDeclarativeItem;
    [[nodiscard]] auto makePackageBodyDeclarativePart(vhdlParser::Package_body_declarative_partContext *ctx) -> ast::PackageBodyDeclarativePart;
    [[nodiscard]] auto makePackageDeclarativeItem(vhdlParser::Package_declarative_itemContext *ctx) -> ast::PackageDeclarativeItem;
    [[nodiscard]] auto makePackageDeclarativePart(vhdlParser::Package_declarative_partContext *ctx) -> ast::PackageDeclarativePart;
    [[nodiscard]] auto makeSubprogramBody(vhdlParser::Subprogram_bodyContext *ctx) -> ast::SubprogramBody;
    [[nodiscard]] auto makeSubprogramDeclarativeItem(vhdlParser::Subprogram_declarative_itemContext *ctx) -> ast::SubprogramDeclarativeItem;
    [[nodiscard]] auto makeSubprogramDeclarativePart(vhdlParser::Subprogram_declarative_partContext *ctx) -> ast::SubprogramDeclarativePart;
    [[nodiscard]] auto makeSubprogramKind(vhdlParser::Subprogram_kindContext *ctx) -> ast::SubprogramKind;
    [[nodiscard]] auto makeSubprogramSpecification(vhdlParser::Subprogram_specificationContext *ctx) -> ast::SubprogramSpecification;
    [[nodiscard]] auto makeSubprogramStatementPart(vhdlParser::Subprogram_statement_partContext *ctx) -> ast::SubprogramStatementPart;

    // Configuration & Block Structure
    [[nodiscard]] auto makeBlockConfiguration(vhdlParser::Block_configurationContext *ctx) -> ast::BlockConfiguration;
    [[nodiscard]] auto makeBlockDeclarativeItem(vhdlParser::Block_declarative_itemContext *ctx) -> ast::BlockDeclarativeItem;
    [[nodiscard]] auto makeBlockDeclarativePart(vhdlParser::Block_declarative_partContext *ctx) -> ast::BlockDeclarativePart;
    [[nodiscard]] auto makeBlockHeader(vhdlParser::Block_headerContext *ctx) -> ast::BlockHeader;
    [[nodiscard]] auto makeBlockStatementPart(vhdlParser::Block_statement_partContext *ctx) -> ast::BlockStatementPart;
    [[nodiscard]] auto makeComponentConfiguration(vhdlParser::Component_configurationContext *ctx) -> ast::ComponentConfiguration;
    [[nodiscard]] auto makeConfigurationDeclarativeItem(vhdlParser::Configuration_declarative_itemContext *ctx) -> ast::ConfigurationDeclarativeItem;
    [[nodiscard]] auto makeConfigurationDeclarativePart(vhdlParser::Configuration_declarative_partContext *ctx) -> ast::ConfigurationDeclarativePart;
    [[nodiscard]] auto makeConfigurationItem(vhdlParser::Configuration_itemContext *ctx) -> ast::ConfigurationItem;

    // Process / Procedural Structure
    [[nodiscard]] auto makeProcessDeclarativeItem(vhdlParser::Process_declarative_itemContext *ctx) -> ast::ProcessDeclarativeItem;
    [[nodiscard]] auto makeProcessDeclarativePart(vhdlParser::Process_declarative_partContext *ctx) -> ast::ProcessDeclarativePart;
    [[nodiscard]] auto makeProcessStatementPart(vhdlParser::Process_statement_partContext *ctx) -> ast::ProcessStatementPart;
    [[nodiscard]] auto makeProceduralDeclarativeItem(vhdlParser::Procedural_declarative_itemContext *ctx) -> ast::ProceduralDeclarativeItem;
    [[nodiscard]] auto makeProceduralDeclarativePart(vhdlParser::Procedural_declarative_partContext *ctx) -> ast::ProceduralDeclarativePart;
    [[nodiscard]] auto makeProceduralStatementPart(vhdlParser::Procedural_statement_partContext *ctx) -> ast::ProceduralStatementPart;

    // Simultaneous / Waveform Constructs
    [[nodiscard]] auto makeConditionalWaveforms(vhdlParser::Conditional_waveformsContext *ctx) -> ast::ConditionalWaveforms;
    [[nodiscard]] auto makeSelectedWaveforms(vhdlParser::Selected_waveformsContext *ctx) -> ast::SelectedWaveforms;
    [[nodiscard]] auto makeSimultaneousAlternative(vhdlParser::Simultaneous_alternativeContext *ctx) -> ast::SimultaneousAlternative;
    [[nodiscard]] auto makeSimultaneousStatementPart(vhdlParser::Simultaneous_statement_partContext *ctx) -> ast::SimultaneousStatementPart;
    [[nodiscard]] auto makeWaveform(vhdlParser::WaveformContext *ctx) -> ast::Waveform;
    [[nodiscard]] auto makeWaveformElement(vhdlParser::Waveform_elementContext *ctx) -> ast::WaveformElement;

    // Entity Structure
    [[nodiscard]] auto makeEntityClass(vhdlParser::Entity_classContext *ctx) -> ast::EntityClass;
    [[nodiscard]] auto makeEntityDeclarativeItem(vhdlParser::Entity_declarative_itemContext *ctx) -> ast::EntityDeclarativeItem;
    [[nodiscard]] auto makeEntityDeclarativePart(vhdlParser::Entity_declarative_partContext *ctx) -> ast::EntityDeclarativePart;
    [[nodiscard]] auto makeEntityHeader(vhdlParser::Entity_headerContext *ctx) -> ast::EntityHeader;
    [[nodiscard]] auto makeEntityStatementPart(vhdlParser::Entity_statement_partContext *ctx) -> ast::EntityStatementPart;
    [[nodiscard]] auto makePortList(vhdlParser::Port_listContext *ctx) -> ast::PortList;

    // Case / If / Generate Alternatives
    [[nodiscard]] auto makeCaseStatementAlternative(vhdlParser::Case_statement_alternativeContext *ctx) -> ast::CaseStatementAlternative;
    [[nodiscard]] auto makeGenerationScheme(vhdlParser::Generation_schemeContext *ctx) -> ast::GenerationScheme;
    [[nodiscard]] auto makeIterationScheme(vhdlParser::Iteration_schemeContext *ctx) -> ast::IterationScheme;
    [[nodiscard]] auto makeLabelColon(vhdlParser::Label_colonContext *ctx) -> ast::LabelColon;
    [[nodiscard]] auto makeParameterSpecification(vhdlParser::Parameter_specificationContext *ctx) -> ast::ParameterSpecification;

    // Design Units & Files
    [[nodiscard]] auto makeDesignFile(vhdlParser::Design_fileContext *ctx) -> ast::DesignFile;
    [[nodiscard]] auto makeDesignUnit(vhdlParser::Design_unitContext *ctx) -> ast::DesignUnit;
    [[nodiscard]] auto makeFileLogicalName(vhdlParser::File_logical_nameContext *ctx) -> ast::FileLogicalName;
    [[nodiscard]] auto makeFileOpenInformation(vhdlParser::File_open_informationContext *ctx) -> ast::FileOpenInformation;
    [[nodiscard]] auto makeInstantiatedUnit(vhdlParser::Instantiated_unitContext *ctx) -> ast::InstantiatedUnit;
    [[nodiscard]] auto makeLibraryUnit(vhdlParser::Library_unitContext *ctx) -> ast::LibraryUnit;
    [[nodiscard]] auto makeLogicalName(vhdlParser::Logical_nameContext *ctx) -> ast::LogicalName;
    [[nodiscard]] auto makePrimaryUnit(vhdlParser::Primary_unitContext *ctx) -> ast::PrimaryUnit;
    [[nodiscard]] auto makeSecondaryUnit(vhdlParser::Secondary_unitContext *ctx) -> ast::SecondaryUnit;

    // Procedure/Function Specifications
    [[nodiscard]] auto makeFunctionSpecification(vhdlParser::Function_specificationContext *ctx) -> ast::FunctionSpecification;
    [[nodiscard]] auto makeProcedureCall(vhdlParser::Procedure_callContext *ctx) -> ast::ProcedureCall;
    [[nodiscard]] auto makeProcedureSpecification(vhdlParser::Procedure_specificationContext *ctx) -> ast::ProcedureSpecification;

    // 19. Parameters & Associations
    [[nodiscard]] auto makeActualDesignator(vhdlParser::Actual_designatorContext *ctx) -> ast::ActualDesignator;
    [[nodiscard]] auto makeActualParameterPart(vhdlParser::Actual_parameter_partContext *ctx) -> ast::ActualParameterPart;
    [[nodiscard]] auto makeActualPart(vhdlParser::Actual_partContext *ctx) -> ast::ActualPart;
    [[nodiscard]] auto makeElementAssociation(vhdlParser::Element_associationContext *ctx) -> ast::ElementAssociation;
    [[nodiscard]] auto makeFormalPart(vhdlParser::Formal_partContext *ctx) -> ast::FormalPart;

    // 20. Modes & Kinds
    [[nodiscard]] auto makeDirection(vhdlParser::DirectionContext *ctx) -> ast::Direction;
    [[nodiscard]] auto makeSignalKind(vhdlParser::Signal_kindContext *ctx) -> ast::SignalKind;
    [[nodiscard]] auto makeSignalMode(vhdlParser::Signal_modeContext *ctx) -> ast::SignalMode;

    // 21. Miscellaneous
    [[nodiscard]] auto makeChoice(vhdlParser::ChoiceContext *ctx) -> ast::Choice;
    [[nodiscard]] auto makeCondition(vhdlParser::ConditionContext *ctx) -> ast::Condition;
    [[nodiscard]] auto makeOpts(vhdlParser::OptsContext *ctx) -> ast::Opts;
    [[nodiscard]] auto makeRuleConstraint(vhdlParser::RuleConstraint *ctx) -> ast::RuleConstraint;
    [[nodiscard]] auto makeSignature(vhdlParser::SignatureContext *ctx) -> ast::Signature;

    // clang-format on

    /// @brief Helper to create a boxed expression
    template<typename T = ast::Expression>
    [[nodiscard]]
    constexpr auto box(T &&expr) -> ast::Box<T>
    {
        return std::make_unique<T>(std::forward<T>(expr));
    }

    /// @brief Helper to create and bind an AST node with trivia
    template<typename T, typename Ctx>
    [[nodiscard]]
    auto make(const Ctx *ctx) -> T
    {
        T node;
        trivia_.bind(node, ctx);
        return node;
    }

    /// @brief Helper to create binary expressions
    template<typename Ctx>
    [[nodiscard]]
    auto makeBinary(const Ctx *ctx,
                    std::string_view op,
                    ast::Expression left,
                    ast::Expression right) -> ast::Expression
    {
        ast::BinaryExpr bin;
        trivia_.bind(bin, ctx);
        bin.op = op;
        bin.left = box(std::move(left));
        bin.right = box(std::move(right));
        return bin;
    }

    /// @brief Helper to create unary expressions
    template<typename Ctx>
    [[nodiscard]]
    auto makeUnary(const Ctx *ctx, std::string_view op, ast::Expression value) -> ast::Expression
    {
        ast::UnaryExpr un;
        trivia_.bind(un, ctx);
        un.op = op;
        un.value = box(std::move(value));
        return un;
    }

    /// @brief Helper to create token expressions
    template<typename Ctx>
    [[nodiscard]]
    auto makeToken(const Ctx *ctx, std::string_view text) -> ast::Expression
    {
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = text;
        return tok;
    }
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
