#ifndef BUILDER_ADAPTER_BASE_VOID_VISITOR_HPP
#define BUILDER_ADAPTER_BASE_VOID_VISITOR_HPP

#include "antlr_void_adapter.hpp"
#include "tree/ParseTree.h"
#include "vhdlParser.h"
#include "void_visitor.hpp"

namespace builder::adapter {

/// @brief Base visitor for CST traversals.
///
/// Provides generic traversal utilities used by ANTLR-generated visitors.
/// Wraps recursive walking and dispatching through an `AntlrVoidAdapter`
/// that forwards calls into this visitor.
class BaseVoidVisitor : public VoidVisitor
{
  protected:
    /// @brief Dispatch a single node to this visitor.
    ///
    /// Creates an `AntlrVoidAdapter` to adapt between ANTLR's visitor API
    /// and the concrete visitor implementation, then invokes `accept()`
    /// on the provided node.
    void dispatch(antlr4::tree::ParseTree *node)
    {
        if (node == nullptr) {
            return;
        }
        AntlrVoidAdapter adapter(*this);
        node->accept(&adapter);
    }

    /// @brief Walk all children of a node.
    ///
    /// Iterates over each child in the parse tree and recursively
    /// dispatches them using `AntlrVoidAdapter`.
    void walk(antlr4::tree::ParseTree *node)
    {
        if (node == nullptr) {
            return;
        }
        AntlrVoidAdapter adapter(*this);
        for (auto *child : node->children) {
            if (child != nullptr) {
                child->accept(&adapter);
            }
        }
    }

  public:
    void visitAbstractLiteral(vhdlParser::Abstract_literalContext *context) override { walk(context); }
    void visitAccessTypeDefinition(vhdlParser::Access_type_definitionContext *context) override { walk(context); }
    void visitAcrossAspect(vhdlParser::Across_aspectContext *context) override { walk(context); }
    void visitActualDesignator(vhdlParser::Actual_designatorContext *context) override { walk(context); }
    void visitActualParameterPart(vhdlParser::Actual_parameter_partContext *context) override { walk(context); }
    void visitActualPart(vhdlParser::Actual_partContext *context) override { walk(context); }
    void visitAddingOperator(vhdlParser::Adding_operatorContext *context) override { walk(context); }
    void visitAggregate(vhdlParser::AggregateContext *context) override { walk(context); }
    void visitAliasDeclaration(vhdlParser::Alias_declarationContext *context) override { walk(context); }
    void visitAliasDesignator(vhdlParser::Alias_designatorContext *context) override { walk(context); }
    void visitAliasIndication(vhdlParser::Alias_indicationContext *context) override { walk(context); }
    void visitAllocator(vhdlParser::AllocatorContext *context) override { walk(context); }
    void visitArchitectureBody(vhdlParser::Architecture_bodyContext *context) override { walk(context); }
    void visitArchitectureDeclarativePart(vhdlParser::Architecture_declarative_partContext *context) override { walk(context); }
    void visitArchitectureStatement(vhdlParser::Architecture_statementContext *context) override { walk(context); }
    void visitArchitectureStatementPart(vhdlParser::Architecture_statement_partContext *context) override { walk(context); }
    void visitArrayNatureDefinition(vhdlParser::Array_nature_definitionContext *context) override { walk(context); }
    void visitArrayTypeDefinition(vhdlParser::Array_type_definitionContext *context) override { walk(context); }
    void visitAssertion(vhdlParser::AssertionContext *context) override { walk(context); }
    void visitAssertionStatement(vhdlParser::Assertion_statementContext *context) override { walk(context); }
    void visitAssociationElement(vhdlParser::Association_elementContext *context) override { walk(context); }
    void visitAssociationList(vhdlParser::Association_listContext *context) override { walk(context); }
    void visitAttributeDeclaration(vhdlParser::Attribute_declarationContext *context) override { walk(context); }
    void visitAttributeDesignator(vhdlParser::Attribute_designatorContext *context) override { walk(context); }
    void visitAttributeNamePart(vhdlParser::Attribute_name_partContext *context) override { walk(context); }
    void visitAttributeSpecification(vhdlParser::Attribute_specificationContext *context) override { walk(context); }
    void visitBaseUnitDeclaration(vhdlParser::Base_unit_declarationContext *context) override { walk(context); }
    void visitBindingIndication(vhdlParser::Binding_indicationContext *context) override { walk(context); }
    void visitBlockConfiguration(vhdlParser::Block_configurationContext *context) override { walk(context); }
    void visitBlockDeclarativeItem(vhdlParser::Block_declarative_itemContext *context) override { walk(context); }
    void visitBlockDeclarativePart(vhdlParser::Block_declarative_partContext *context) override { walk(context); }
    void visitBlockHeader(vhdlParser::Block_headerContext *context) override { walk(context); }
    void visitBlockSpecification(vhdlParser::Block_specificationContext *context) override { walk(context); }
    void visitBlockStatement(vhdlParser::Block_statementContext *context) override { walk(context); }
    void visitBlockStatementPart(vhdlParser::Block_statement_partContext *context) override { walk(context); }
    void visitBranchQuantityDeclaration(vhdlParser::Branch_quantity_declarationContext *context) override { walk(context); }
    void visitBreakElement(vhdlParser::Break_elementContext *context) override { walk(context); }
    void visitBreakList(vhdlParser::Break_listContext *context) override { walk(context); }
    void visitBreakSelectorClause(vhdlParser::Break_selector_clauseContext *context) override { walk(context); }
    void visitBreakStatement(vhdlParser::Break_statementContext *context) override { walk(context); }
    void visitCaseStatement(vhdlParser::Case_statementContext *context) override { walk(context); }
    void visitCaseStatementAlternative(vhdlParser::Case_statement_alternativeContext *context) override { walk(context); }
    void visitChoice(vhdlParser::ChoiceContext *context) override { walk(context); }
    void visitChoices(vhdlParser::ChoicesContext *context) override { walk(context); }
    void visitComponentConfiguration(vhdlParser::Component_configurationContext *context) override { walk(context); }
    void visitComponentDeclaration(vhdlParser::Component_declarationContext *context) override { walk(context); }
    void visitComponentInstantiationStatement(vhdlParser::Component_instantiation_statementContext *context) override { walk(context); }
    void visitComponentSpecification(vhdlParser::Component_specificationContext *context) override { walk(context); }
    void visitCompositeNatureDefinition(vhdlParser::Composite_nature_definitionContext *context) override { walk(context); }
    void visitCompositeTypeDefinition(vhdlParser::Composite_type_definitionContext *context) override { walk(context); }
    void visitConcurrentAssertionStatement(vhdlParser::Concurrent_assertion_statementContext *context) override { walk(context); }
    void visitConcurrentBreakStatement(vhdlParser::Concurrent_break_statementContext *context) override { walk(context); }
    void visitConcurrentProcedureCallStatement(vhdlParser::Concurrent_procedure_call_statementContext *context) override { walk(context); }
    void visitConcurrentSignalAssignmentStatement(vhdlParser::Concurrent_signal_assignment_statementContext *context) override { walk(context); }
    void visitCondition(vhdlParser::ConditionContext *context) override { walk(context); }
    void visitConditionClause(vhdlParser::Condition_clauseContext *context) override { walk(context); }
    void visitConditionalSignalAssignment(vhdlParser::Conditional_signal_assignmentContext *context) override { walk(context); }
    void visitConditionalWaveforms(vhdlParser::Conditional_waveformsContext *context) override { walk(context); }
    void visitConfigurationDeclaration(vhdlParser::Configuration_declarationContext *context) override { walk(context); }
    void visitConfigurationDeclarativeItem(vhdlParser::Configuration_declarative_itemContext *context) override { walk(context); }
    void visitConfigurationDeclarativePart(vhdlParser::Configuration_declarative_partContext *context) override { walk(context); }
    void visitConfigurationItem(vhdlParser::Configuration_itemContext *context) override { walk(context); }
    void visitConfigurationSpecification(vhdlParser::Configuration_specificationContext *context) override { walk(context); }
    void visitConstantDeclaration(vhdlParser::Constant_declarationContext *context) override { walk(context); }
    void visitConstrainedArrayDefinition(vhdlParser::Constrained_array_definitionContext *context) override { walk(context); }
    void visitConstrainedNatureDefinition(vhdlParser::Constrained_nature_definitionContext *context) override { walk(context); }
    void visitConstraint(vhdlParser::ConstraintContext *context) override { walk(context); }
    void visitContextClause(vhdlParser::Context_clauseContext *context) override { walk(context); }
    void visitContextItem(vhdlParser::Context_itemContext *context) override { walk(context); }
    void visitDelayMechanism(vhdlParser::Delay_mechanismContext *context) override { walk(context); }
    void visitDesignFile(vhdlParser::Design_fileContext *context) override { walk(context); }
    void visitDesignUnit(vhdlParser::Design_unitContext *context) override { walk(context); }
    void visitDesignator(vhdlParser::DesignatorContext *context) override { walk(context); }
    void visitDirection(vhdlParser::DirectionContext *context) override { walk(context); }
    void visitDisconnectionSpecification(vhdlParser::Disconnection_specificationContext *context) override { walk(context); }
    void visitDiscreteRange(vhdlParser::Discrete_rangeContext *context) override { walk(context); }
    void visitElementAssociation(vhdlParser::Element_associationContext *context) override { walk(context); }
    void visitElementDeclaration(vhdlParser::Element_declarationContext *context) override { walk(context); }
    void visitElementSubnatureDefinition(vhdlParser::Element_subnature_definitionContext *context) override { walk(context); }
    void visitElementSubtypeDefinition(vhdlParser::Element_subtype_definitionContext *context) override { walk(context); }
    void visitEntityAspect(vhdlParser::Entity_aspectContext *context) override { walk(context); }
    void visitEntityClass(vhdlParser::Entity_classContext *context) override { walk(context); }
    void visitEntityClassEntry(vhdlParser::Entity_class_entryContext *context) override { walk(context); }
    void visitEntityClassEntryList(vhdlParser::Entity_class_entry_listContext *context) override { walk(context); }
    void visitEntityDeclaration(vhdlParser::Entity_declarationContext *context) override { walk(context); }
    void visitEntityDeclarativeItem(vhdlParser::Entity_declarative_itemContext *context) override { walk(context); }
    void visitEntityDeclarativePart(vhdlParser::Entity_declarative_partContext *context) override { walk(context); }
    void visitEntityDesignator(vhdlParser::Entity_designatorContext *context) override { walk(context); }
    void visitEntityHeader(vhdlParser::Entity_headerContext *context) override { walk(context); }
    void visitEntityNameList(vhdlParser::Entity_name_listContext *context) override { walk(context); }
    void visitEntitySpecification(vhdlParser::Entity_specificationContext *context) override { walk(context); }
    void visitEntityStatement(vhdlParser::Entity_statementContext *context) override { walk(context); }
    void visitEntityStatementPart(vhdlParser::Entity_statement_partContext *context) override { walk(context); }
    void visitEntityTag(vhdlParser::Entity_tagContext *context) override { walk(context); }
    void visitEnumerationLiteral(vhdlParser::Enumeration_literalContext *context) override { walk(context); }
    void visitEnumerationTypeDefinition(vhdlParser::Enumeration_type_definitionContext *context) override { walk(context); }
    void visitExitStatement(vhdlParser::Exit_statementContext *context) override { walk(context); }
    void visitExplicitRange(vhdlParser::Explicit_rangeContext *context) override { walk(context); }
    void visitExpression(vhdlParser::ExpressionContext *context) override { walk(context); }
    void visitFactor(vhdlParser::FactorContext *context) override { walk(context); }
    void visitFileDeclaration(vhdlParser::File_declarationContext *context) override { walk(context); }
    void visitFileLogicalName(vhdlParser::File_logical_nameContext *context) override { walk(context); }
    void visitFileOpenInformation(vhdlParser::File_open_informationContext *context) override { walk(context); }
    void visitFileTypeDefinition(vhdlParser::File_type_definitionContext *context) override { walk(context); }
    void visitFormalParameterList(vhdlParser::Formal_parameter_listContext *context) override { walk(context); }
    void visitFormalPart(vhdlParser::Formal_partContext *context) override { walk(context); }
    void visitFreeQuantityDeclaration(vhdlParser::Free_quantity_declarationContext *context) override { walk(context); }
    void visitFunctionCallOrIndexedNamePart(vhdlParser::Function_call_or_indexed_name_partContext *context) override { walk(context); }
    void visitFunctionSpecification(vhdlParser::Function_specificationContext *context) override { walk(context); }
    void visitGenerateStatement(vhdlParser::Generate_statementContext *context) override { walk(context); }
    void visitGenerationScheme(vhdlParser::Generation_schemeContext *context) override { walk(context); }
    void visitGenericClause(vhdlParser::Generic_clauseContext *context) override { walk(context); }
    void visitGenericList(vhdlParser::Generic_listContext *context) override { walk(context); }
    void visitGenericMapAspect(vhdlParser::Generic_map_aspectContext *context) override { walk(context); }
    void visitGroupConstituent(vhdlParser::Group_constituentContext *context) override { walk(context); }
    void visitGroupConstituentList(vhdlParser::Group_constituent_listContext *context) override { walk(context); }
    void visitGroupDeclaration(vhdlParser::Group_declarationContext *context) override { walk(context); }
    void visitGroupTemplateDeclaration(vhdlParser::Group_template_declarationContext *context) override { walk(context); }
    void visitGuardedSignalSpecification(vhdlParser::Guarded_signal_specificationContext *context) override { walk(context); }
    void visitIdentifier(vhdlParser::IdentifierContext *context) override { walk(context); }
    void visitIdentifierList(vhdlParser::Identifier_listContext *context) override { walk(context); }
    void visitIfStatement(vhdlParser::If_statementContext *context) override { walk(context); }
    void visitIndexConstraint(vhdlParser::Index_constraintContext *context) override { walk(context); }
    void visitIndexSpecification(vhdlParser::Index_specificationContext *context) override { walk(context); }
    void visitIndexSubtypeDefinition(vhdlParser::Index_subtype_definitionContext *context) override { walk(context); }
    void visitInstantiatedUnit(vhdlParser::Instantiated_unitContext *context) override { walk(context); }
    void visitInstantiationList(vhdlParser::Instantiation_listContext *context) override { walk(context); }
    void visitInterfaceConstantDeclaration(vhdlParser::Interface_constant_declarationContext *context) override { walk(context); }
    void visitInterfaceDeclaration(vhdlParser::Interface_declarationContext *context) override { walk(context); }
    void visitInterfaceElement(vhdlParser::Interface_elementContext *context) override { walk(context); }
    void visitInterfaceFileDeclaration(vhdlParser::Interface_file_declarationContext *context) override { walk(context); }
    void visitInterfaceList(vhdlParser::Interface_listContext *context) override { walk(context); }
    void visitInterfacePortDeclaration(vhdlParser::Interface_port_declarationContext *context) override { walk(context); }
    void visitInterfacePortList(vhdlParser::Interface_port_listContext *context) override { walk(context); }
    void visitInterfaceQuantityDeclaration(vhdlParser::Interface_quantity_declarationContext *context) override { walk(context); }
    void visitInterfaceSignalDeclaration(vhdlParser::Interface_signal_declarationContext *context) override { walk(context); }
    void visitInterfaceSignalList(vhdlParser::Interface_signal_listContext *context) override { walk(context); }
    void visitInterfaceTerminalDeclaration(vhdlParser::Interface_terminal_declarationContext *context) override { walk(context); }
    void visitInterfaceVariableDeclaration(vhdlParser::Interface_variable_declarationContext *context) override { walk(context); }
    void visitIterationScheme(vhdlParser::Iteration_schemeContext *context) override { walk(context); }
    void visitLabelColon(vhdlParser::Label_colonContext *context) override { walk(context); }
    void visitLibraryClause(vhdlParser::Library_clauseContext *context) override { walk(context); }
    void visitLibraryUnit(vhdlParser::Library_unitContext *context) override { walk(context); }
    void visitLiteral(vhdlParser::LiteralContext *context) override { walk(context); }
    void visitLogicalName(vhdlParser::Logical_nameContext *context) override { walk(context); }
    void visitLogicalNameList(vhdlParser::Logical_name_listContext *context) override { walk(context); }
    void visitLogicalOperator(vhdlParser::Logical_operatorContext *context) override { walk(context); }
    void visitLoopStatement(vhdlParser::Loop_statementContext *context) override { walk(context); }
    void visitMultiplyingOperator(vhdlParser::Multiplying_operatorContext *context) override { walk(context); }
    void visitName(vhdlParser::NameContext *context) override { walk(context); }
    void visitNamePart(vhdlParser::Name_partContext *context) override { walk(context); }
    void visitNatureDeclaration(vhdlParser::Nature_declarationContext *context) override { walk(context); }
    void visitNatureDefinition(vhdlParser::Nature_definitionContext *context) override { walk(context); }
    void visitNatureElementDeclaration(vhdlParser::Nature_element_declarationContext *context) override { walk(context); }
    void visitNextStatement(vhdlParser::Next_statementContext *context) override { walk(context); }
    void visitNumericLiteral(vhdlParser::Numeric_literalContext *context) override { walk(context); }
    void visitObjectDeclaration(vhdlParser::Object_declarationContext *context) override { walk(context); }
    void visitOpts(vhdlParser::OptsContext *context) override { walk(context); }
    void visitPackageBody(vhdlParser::Package_bodyContext *context) override { walk(context); }
    void visitPackageBodyDeclarativeItem(vhdlParser::Package_body_declarative_itemContext *context) override { walk(context); }
    void visitPackageBodyDeclarativePart(vhdlParser::Package_body_declarative_partContext *context) override { walk(context); }
    void visitPackageDeclaration(vhdlParser::Package_declarationContext *context) override { walk(context); }
    void visitPackageDeclarativeItem(vhdlParser::Package_declarative_itemContext *context) override { walk(context); }
    void visitPackageDeclarativePart(vhdlParser::Package_declarative_partContext *context) override { walk(context); }
    void visitParameterSpecification(vhdlParser::Parameter_specificationContext *context) override { walk(context); }
    void visitPhysicalLiteral(vhdlParser::Physical_literalContext *context) override { walk(context); }
    void visitPhysicalTypeDefinition(vhdlParser::Physical_type_definitionContext *context) override { walk(context); }
    void visitPortClause(vhdlParser::Port_clauseContext *context) override { walk(context); }
    void visitPortList(vhdlParser::Port_listContext *context) override { walk(context); }
    void visitPortMapAspect(vhdlParser::Port_map_aspectContext *context) override { walk(context); }
    void visitPrimary(vhdlParser::PrimaryContext *context) override { walk(context); }
    void visitPrimaryUnit(vhdlParser::Primary_unitContext *context) override { walk(context); }
    void visitProceduralDeclarativeItem(vhdlParser::Procedural_declarative_itemContext *context) override { walk(context); }
    void visitProceduralDeclarativePart(vhdlParser::Procedural_declarative_partContext *context) override { walk(context); }
    void visitProceduralStatementPart(vhdlParser::Procedural_statement_partContext *context) override { walk(context); }
    void visitProcedureCall(vhdlParser::Procedure_callContext *context) override { walk(context); }
    void visitProcedureCallStatement(vhdlParser::Procedure_call_statementContext *context) override { walk(context); }
    void visitProcedureSpecification(vhdlParser::Procedure_specificationContext *context) override { walk(context); }
    void visitProcessDeclarativeItem(vhdlParser::Process_declarative_itemContext *context) override { walk(context); }
    void visitProcessDeclarativePart(vhdlParser::Process_declarative_partContext *context) override { walk(context); }
    void visitProcessStatement(vhdlParser::Process_statementContext *context) override { walk(context); }
    void visitProcessStatementPart(vhdlParser::Process_statement_partContext *context) override { walk(context); }
    void visitQualifiedExpression(vhdlParser::Qualified_expressionContext *context) override { walk(context); }
    void visitQuantityDeclaration(vhdlParser::Quantity_declarationContext *context) override { walk(context); }
    void visitQuantityList(vhdlParser::Quantity_listContext *context) override { walk(context); }
    void visitQuantitySpecification(vhdlParser::Quantity_specificationContext *context) override { walk(context); }
    void visitRangeConstraint(vhdlParser::Range_constraintContext *context) override { walk(context); }
    void visitRangeDecl(vhdlParser::Range_declContext *context) override { walk(context); }
    void visitRecordNatureDefinition(vhdlParser::Record_nature_definitionContext *context) override { walk(context); }
    void visitRecordTypeDefinition(vhdlParser::Record_type_definitionContext *context) override { walk(context); }
    void visitRelation(vhdlParser::RelationContext *context) override { walk(context); }
    void visitRelationalOperator(vhdlParser::Relational_operatorContext *context) override { walk(context); }
    void visitReportStatement(vhdlParser::Report_statementContext *context) override { walk(context); }
    void visitReturnStatement(vhdlParser::Return_statementContext *context) override { walk(context); }
    void visitScalarNatureDefinition(vhdlParser::Scalar_nature_definitionContext *context) override { walk(context); }
    void visitScalarTypeDefinition(vhdlParser::Scalar_type_definitionContext *context) override { walk(context); }
    void visitSecondaryUnit(vhdlParser::Secondary_unitContext *context) override { walk(context); }
    void visitSecondaryUnitDeclaration(vhdlParser::Secondary_unit_declarationContext *context) override { walk(context); }
    void visitSelectedName(vhdlParser::Selected_nameContext *context) override { walk(context); }
    void visitSelectedNamePart(vhdlParser::Selected_name_partContext *context) override { walk(context); }
    void visitSelectedSignalAssignment(vhdlParser::Selected_signal_assignmentContext *context) override { walk(context); }
    void visitSelectedWaveforms(vhdlParser::Selected_waveformsContext *context) override { walk(context); }
    void visitSensitivityClause(vhdlParser::Sensitivity_clauseContext *context) override { walk(context); }
    void visitSensitivityList(vhdlParser::Sensitivity_listContext *context) override { walk(context); }
    void visitSequenceOfStatements(vhdlParser::Sequence_of_statementsContext *context) override { walk(context); }
    void visitSequentialStatement(vhdlParser::Sequential_statementContext *context) override { walk(context); }
    void visitShiftExpression(vhdlParser::Shift_expressionContext *context) override { walk(context); }
    void visitShiftOperator(vhdlParser::Shift_operatorContext *context) override { walk(context); }
    void visitSignalAssignmentStatement(vhdlParser::Signal_assignment_statementContext *context) override { walk(context); }
    void visitSignalDeclaration(vhdlParser::Signal_declarationContext *context) override { walk(context); }
    void visitSignalKind(vhdlParser::Signal_kindContext *context) override { walk(context); }
    void visitSignalList(vhdlParser::Signal_listContext *context) override { walk(context); }
    void visitSignalMode(vhdlParser::Signal_modeContext *context) override { walk(context); }
    void visitSignature(vhdlParser::SignatureContext *context) override { walk(context); }
    void visitSimpleExpression(vhdlParser::Simple_expressionContext *context) override { walk(context); }
    void visitSimpleSimultaneousStatement(vhdlParser::Simple_simultaneous_statementContext *context) override { walk(context); }
    void visitSimultaneousAlternative(vhdlParser::Simultaneous_alternativeContext *context) override { walk(context); }
    void visitSimultaneousCaseStatement(vhdlParser::Simultaneous_case_statementContext *context) override { walk(context); }
    void visitSimultaneousIfStatement(vhdlParser::Simultaneous_if_statementContext *context) override { walk(context); }
    void visitSimultaneousProceduralStatement(vhdlParser::Simultaneous_procedural_statementContext *context) override { walk(context); }
    void visitSimultaneousStatement(vhdlParser::Simultaneous_statementContext *context) override { walk(context); }
    void visitSimultaneousStatementPart(vhdlParser::Simultaneous_statement_partContext *context) override { walk(context); }
    void visitSliceNamePart(vhdlParser::Slice_name_partContext *context) override { walk(context); }
    void visitSourceAspect(vhdlParser::Source_aspectContext *context) override { walk(context); }
    void visitSourceQuantityDeclaration(vhdlParser::Source_quantity_declarationContext *context) override { walk(context); }
    void visitStepLimitSpecification(vhdlParser::Step_limit_specificationContext *context) override { walk(context); }
    void visitSubnatureDeclaration(vhdlParser::Subnature_declarationContext *context) override { walk(context); }
    void visitSubnatureIndication(vhdlParser::Subnature_indicationContext *context) override { walk(context); }
    void visitSubprogramBody(vhdlParser::Subprogram_bodyContext *context) override { walk(context); }
    void visitSubprogramDeclaration(vhdlParser::Subprogram_declarationContext *context) override { walk(context); }
    void visitSubprogramDeclarativeItem(vhdlParser::Subprogram_declarative_itemContext *context) override { walk(context); }
    void visitSubprogramDeclarativePart(vhdlParser::Subprogram_declarative_partContext *context) override { walk(context); }
    void visitSubprogramKind(vhdlParser::Subprogram_kindContext *context) override { walk(context); }
    void visitSubprogramSpecification(vhdlParser::Subprogram_specificationContext *context) override { walk(context); }
    void visitSubprogramStatementPart(vhdlParser::Subprogram_statement_partContext *context) override { walk(context); }
    void visitSubtypeDeclaration(vhdlParser::Subtype_declarationContext *context) override { walk(context); }
    void visitSubtypeIndication(vhdlParser::Subtype_indicationContext *context) override { walk(context); }
    void visitSuffix(vhdlParser::SuffixContext *context) override { walk(context); }
    void visitTarget(vhdlParser::TargetContext *context) override { walk(context); }
    void visitTerm(vhdlParser::TermContext *context) override { walk(context); }
    void visitTerminalAspect(vhdlParser::Terminal_aspectContext *context) override { walk(context); }
    void visitTerminalDeclaration(vhdlParser::Terminal_declarationContext *context) override { walk(context); }
    void visitThroughAspect(vhdlParser::Through_aspectContext *context) override { walk(context); }
    void visitTimeoutClause(vhdlParser::Timeout_clauseContext *context) override { walk(context); }
    void visitToleranceAspect(vhdlParser::Tolerance_aspectContext *context) override { walk(context); }
    void visitTypeDeclaration(vhdlParser::Type_declarationContext *context) override { walk(context); }
    void visitTypeDefinition(vhdlParser::Type_definitionContext *context) override { walk(context); }
    void visitUnconstrainedArrayDefinition(vhdlParser::Unconstrained_array_definitionContext *context) override { walk(context); }
    void visitUnconstrainedNatureDefinition(vhdlParser::Unconstrained_nature_definitionContext *context) override { walk(context); }
    void visitUseClause(vhdlParser::Use_clauseContext *context) override { walk(context); }
    void visitVariableAssignmentStatement(vhdlParser::Variable_assignment_statementContext *context) override { walk(context); }
    void visitVariableDeclaration(vhdlParser::Variable_declarationContext *context) override { walk(context); }
    void visitWaitStatement(vhdlParser::Wait_statementContext *context) override { walk(context); }
    void visitWaveform(vhdlParser::WaveformContext *context) override { walk(context); }
    void visitWaveformElement(vhdlParser::Waveform_elementContext *context) override { walk(context); }
};

} // namespace builder::adapter

#endif /* BUILDER_ADAPTER_BASE_VOID_VISITOR_HPP */
