#ifndef BUILDER_ADAPTER_ANTLR_VOID_ADAPTER_HPP
#define BUILDER_ADAPTER_ANTLR_VOID_ADAPTER_HPP

#include "vhdlBaseVisitor.h"
#include "vhdlParser.h"
#include "void_visitor.hpp"

#include <any>

namespace builder::adapter {
class AntlrVoidAdapter : public vhdlBaseVisitor
{

    VoidVisitor &impl;

  public:
    explicit AntlrVoidAdapter(VoidVisitor &v) : impl(v) {}

    ~AntlrVoidAdapter() override = default;
    AntlrVoidAdapter(const AntlrVoidAdapter &) = delete;
    auto operator=(const AntlrVoidAdapter &) -> AntlrVoidAdapter & = delete;
    AntlrVoidAdapter(AntlrVoidAdapter &&) = delete;
    auto operator=(AntlrVoidAdapter &&) -> AntlrVoidAdapter & = delete;

    auto visitAbstract_literal(vhdlParser::Abstract_literalContext *context) -> std::any override
    {
        impl.visitAbstractLiteral(context);
        return {};
    }

    auto visitAccess_type_definition(vhdlParser::Access_type_definitionContext *context) -> std::any override
    {
        impl.visitAccessTypeDefinition(context);
        return {};
    }

    auto visitAcross_aspect(vhdlParser::Across_aspectContext *context) -> std::any override
    {
        impl.visitAcrossAspect(context);
        return {};
    }

    auto visitActual_designator(vhdlParser::Actual_designatorContext *context) -> std::any override
    {
        impl.visitActualDesignator(context);
        return {};
    }

    auto visitActual_parameter_part(vhdlParser::Actual_parameter_partContext *context) -> std::any override
    {
        impl.visitActualParameterPart(context);
        return {};
    }

    auto visitActual_part(vhdlParser::Actual_partContext *context) -> std::any override
    {
        impl.visitActualPart(context);
        return {};
    }

    auto visitAdding_operator(vhdlParser::Adding_operatorContext *context) -> std::any override
    {
        impl.visitAddingOperator(context);
        return {};
    }

    auto visitAggregate(vhdlParser::AggregateContext *context) -> std::any override
    {
        impl.visitAggregate(context);
        return {};
    }

    auto visitAlias_declaration(vhdlParser::Alias_declarationContext *context) -> std::any override
    {
        impl.visitAliasDeclaration(context);
        return {};
    }

    auto visitAlias_designator(vhdlParser::Alias_designatorContext *context) -> std::any override
    {
        impl.visitAliasDesignator(context);
        return {};
    }

    auto visitAlias_indication(vhdlParser::Alias_indicationContext *context) -> std::any override
    {
        impl.visitAliasIndication(context);
        return {};
    }

    auto visitAllocator(vhdlParser::AllocatorContext *context) -> std::any override
    {
        impl.visitAllocator(context);
        return {};
    }

    auto visitArchitecture_body(vhdlParser::Architecture_bodyContext *context) -> std::any override
    {
        impl.visitArchitectureBody(context);
        return {};
    }

    auto visitArchitecture_declarative_part(vhdlParser::Architecture_declarative_partContext *context) -> std::any override
    {
        impl.visitArchitectureDeclarativePart(context);
        return {};
    }

    auto visitArchitecture_statement(vhdlParser::Architecture_statementContext *context) -> std::any override
    {
        impl.visitArchitectureStatement(context);
        return {};
    }

    auto visitArchitecture_statement_part(vhdlParser::Architecture_statement_partContext *context) -> std::any override
    {
        impl.visitArchitectureStatementPart(context);
        return {};
    }

    auto visitArray_nature_definition(vhdlParser::Array_nature_definitionContext *context) -> std::any override
    {
        impl.visitArrayNatureDefinition(context);
        return {};
    }

    auto visitArray_type_definition(vhdlParser::Array_type_definitionContext *context) -> std::any override
    {
        impl.visitArrayTypeDefinition(context);
        return {};
    }

    auto visitAssertion(vhdlParser::AssertionContext *context) -> std::any override
    {
        impl.visitAssertion(context);
        return {};
    }

    auto visitAssertion_statement(vhdlParser::Assertion_statementContext *context) -> std::any override
    {
        impl.visitAssertionStatement(context);
        return {};
    }

    auto visitAssociation_element(vhdlParser::Association_elementContext *context) -> std::any override
    {
        impl.visitAssociationElement(context);
        return {};
    }

    auto visitAssociation_list(vhdlParser::Association_listContext *context) -> std::any override
    {
        impl.visitAssociationList(context);
        return {};
    }

    auto visitAttribute_declaration(vhdlParser::Attribute_declarationContext *context) -> std::any override
    {
        impl.visitAttributeDeclaration(context);
        return {};
    }

    auto visitAttribute_designator(vhdlParser::Attribute_designatorContext *context) -> std::any override
    {
        impl.visitAttributeDesignator(context);
        return {};
    }

    auto visitAttribute_specification(vhdlParser::Attribute_specificationContext *context) -> std::any override
    {
        impl.visitAttributeSpecification(context);
        return {};
    }

    auto visitBase_unit_declaration(vhdlParser::Base_unit_declarationContext *context) -> std::any override
    {
        impl.visitBaseUnitDeclaration(context);
        return {};
    }

    auto visitBinding_indication(vhdlParser::Binding_indicationContext *context) -> std::any override
    {
        impl.visitBindingIndication(context);
        return {};
    }

    auto visitBlock_configuration(vhdlParser::Block_configurationContext *context) -> std::any override
    {
        impl.visitBlockConfiguration(context);
        return {};
    }

    auto visitBlock_declarative_item(vhdlParser::Block_declarative_itemContext *context) -> std::any override
    {
        impl.visitBlockDeclarativeItem(context);
        return {};
    }

    auto visitBlock_declarative_part(vhdlParser::Block_declarative_partContext *context) -> std::any override
    {
        impl.visitBlockDeclarativePart(context);
        return {};
    }

    auto visitBlock_header(vhdlParser::Block_headerContext *context) -> std::any override
    {
        impl.visitBlockHeader(context);
        return {};
    }

    auto visitBlock_specification(vhdlParser::Block_specificationContext *context) -> std::any override
    {
        impl.visitBlockSpecification(context);
        return {};
    }

    auto visitBlock_statement(vhdlParser::Block_statementContext *context) -> std::any override
    {
        impl.visitBlockStatement(context);
        return {};
    }

    auto visitBlock_statement_part(vhdlParser::Block_statement_partContext *context) -> std::any override
    {
        impl.visitBlockStatementPart(context);
        return {};
    }

    auto visitBranch_quantity_declaration(vhdlParser::Branch_quantity_declarationContext *context) -> std::any override
    {
        impl.visitBranchQuantityDeclaration(context);
        return {};
    }

    auto visitBreak_element(vhdlParser::Break_elementContext *context) -> std::any override
    {
        impl.visitBreakElement(context);
        return {};
    }

    auto visitBreak_list(vhdlParser::Break_listContext *context) -> std::any override
    {
        impl.visitBreakList(context);
        return {};
    }

    auto visitBreak_selector_clause(vhdlParser::Break_selector_clauseContext *context) -> std::any override
    {
        impl.visitBreakSelectorClause(context);
        return {};
    }

    auto visitBreak_statement(vhdlParser::Break_statementContext *context) -> std::any override
    {
        impl.visitBreakStatement(context);
        return {};
    }

    auto visitCase_statement(vhdlParser::Case_statementContext *context) -> std::any override
    {
        impl.visitCaseStatement(context);
        return {};
    }

    auto visitCase_statement_alternative(vhdlParser::Case_statement_alternativeContext *context) -> std::any override
    {
        impl.visitCaseStatementAlternative(context);
        return {};
    }

    auto visitChoice(vhdlParser::ChoiceContext *context) -> std::any override
    {
        impl.visitChoice(context);
        return {};
    }

    auto visitChoices(vhdlParser::ChoicesContext *context) -> std::any override
    {
        impl.visitChoices(context);
        return {};
    }

    auto visitComponent_configuration(vhdlParser::Component_configurationContext *context) -> std::any override
    {
        impl.visitComponentConfiguration(context);
        return {};
    }

    auto visitComponent_declaration(vhdlParser::Component_declarationContext *context) -> std::any override
    {
        impl.visitComponentDeclaration(context);
        return {};
    }

    auto visitComponent_instantiation_statement(vhdlParser::Component_instantiation_statementContext *context) -> std::any override
    {
        impl.visitComponentInstantiationStatement(context);
        return {};
    }

    auto visitComponent_specification(vhdlParser::Component_specificationContext *context) -> std::any override
    {
        impl.visitComponentSpecification(context);
        return {};
    }

    auto visitComposite_nature_definition(vhdlParser::Composite_nature_definitionContext *context) -> std::any override
    {
        impl.visitCompositeNatureDefinition(context);
        return {};
    }

    auto visitComposite_type_definition(vhdlParser::Composite_type_definitionContext *context) -> std::any override
    {
        impl.visitCompositeTypeDefinition(context);
        return {};
    }

    auto visitConcurrent_assertion_statement(vhdlParser::Concurrent_assertion_statementContext *context) -> std::any override
    {
        impl.visitConcurrentAssertionStatement(context);
        return {};
    }

    auto visitConcurrent_break_statement(vhdlParser::Concurrent_break_statementContext *context) -> std::any override
    {
        impl.visitConcurrentBreakStatement(context);
        return {};
    }

    auto visitConcurrent_procedure_call_statement(vhdlParser::Concurrent_procedure_call_statementContext *context) -> std::any override
    {
        impl.visitConcurrentProcedureCallStatement(context);
        return {};
    }

    auto visitConcurrent_signal_assignment_statement(vhdlParser::Concurrent_signal_assignment_statementContext *context) -> std::any override
    {
        impl.visitConcurrentSignalAssignmentStatement(context);
        return {};
    }

    auto visitCondition(vhdlParser::ConditionContext *context) -> std::any override
    {
        impl.visitCondition(context);
        return {};
    }

    auto visitCondition_clause(vhdlParser::Condition_clauseContext *context) -> std::any override
    {
        impl.visitConditionClause(context);
        return {};
    }

    auto visitConditional_signal_assignment(vhdlParser::Conditional_signal_assignmentContext *context) -> std::any override
    {
        impl.visitConditionalSignalAssignment(context);
        return {};
    }

    auto visitConditional_waveforms(vhdlParser::Conditional_waveformsContext *context) -> std::any override
    {
        impl.visitConditionalWaveforms(context);
        return {};
    }

    auto visitConfiguration_declaration(vhdlParser::Configuration_declarationContext *context) -> std::any override
    {
        impl.visitConfigurationDeclaration(context);
        return {};
    }

    auto visitConfiguration_declarative_item(vhdlParser::Configuration_declarative_itemContext *context) -> std::any override
    {
        impl.visitConfigurationDeclarativeItem(context);
        return {};
    }

    auto visitConfiguration_declarative_part(vhdlParser::Configuration_declarative_partContext *context) -> std::any override
    {
        impl.visitConfigurationDeclarativePart(context);
        return {};
    }

    auto visitConfiguration_item(vhdlParser::Configuration_itemContext *context) -> std::any override
    {
        impl.visitConfigurationItem(context);
        return {};
    }

    auto visitConfiguration_specification(vhdlParser::Configuration_specificationContext *context) -> std::any override
    {
        impl.visitConfigurationSpecification(context);
        return {};
    }

    auto visitConstant_declaration(vhdlParser::Constant_declarationContext *context) -> std::any override
    {
        impl.visitConstantDeclaration(context);
        return {};
    }

    auto visitConstrained_array_definition(vhdlParser::Constrained_array_definitionContext *context) -> std::any override
    {
        impl.visitConstrainedArrayDefinition(context);
        return {};
    }

    auto visitConstrained_nature_definition(vhdlParser::Constrained_nature_definitionContext *context) -> std::any override
    {
        impl.visitConstrainedNatureDefinition(context);
        return {};
    }

    auto visitConstraint(vhdlParser::ConstraintContext *context) -> std::any override
    {
        impl.visitConstraint(context);
        return {};
    }

    auto visitContext_clause(vhdlParser::Context_clauseContext *context) -> std::any override
    {
        impl.visitContextClause(context);
        return {};
    }

    auto visitContext_item(vhdlParser::Context_itemContext *context) -> std::any override
    {
        impl.visitContextItem(context);
        return {};
    }

    auto visitDelay_mechanism(vhdlParser::Delay_mechanismContext *context) -> std::any override
    {
        impl.visitDelayMechanism(context);
        return {};
    }

    auto visitDesign_file(vhdlParser::Design_fileContext *context) -> std::any override
    {
        impl.visitDesignFile(context);
        return {};
    }

    auto visitDesign_unit(vhdlParser::Design_unitContext *context) -> std::any override
    {
        impl.visitDesignUnit(context);
        return {};
    }

    auto visitDesignator(vhdlParser::DesignatorContext *context) -> std::any override
    {
        impl.visitDesignator(context);
        return {};
    }

    auto visitDirection(vhdlParser::DirectionContext *context) -> std::any override
    {
        impl.visitDirection(context);
        return {};
    }

    auto visitDisconnection_specification(vhdlParser::Disconnection_specificationContext *context) -> std::any override
    {
        impl.visitDisconnectionSpecification(context);
        return {};
    }

    auto visitDiscrete_range(vhdlParser::Discrete_rangeContext *context) -> std::any override
    {
        impl.visitDiscreteRange(context);
        return {};
    }

    auto visitElement_association(vhdlParser::Element_associationContext *context) -> std::any override
    {
        impl.visitElementAssociation(context);
        return {};
    }

    auto visitElement_declaration(vhdlParser::Element_declarationContext *context) -> std::any override
    {
        impl.visitElementDeclaration(context);
        return {};
    }

    auto visitElement_subnature_definition(vhdlParser::Element_subnature_definitionContext *context) -> std::any override
    {
        impl.visitElementSubnatureDefinition(context);
        return {};
    }

    auto visitElement_subtype_definition(vhdlParser::Element_subtype_definitionContext *context) -> std::any override
    {
        impl.visitElementSubtypeDefinition(context);
        return {};
    }

    auto visitEntity_aspect(vhdlParser::Entity_aspectContext *context) -> std::any override
    {
        impl.visitEntityAspect(context);
        return {};
    }

    auto visitEntity_class(vhdlParser::Entity_classContext *context) -> std::any override
    {
        impl.visitEntityClass(context);
        return {};
    }

    auto visitEntity_class_entry(vhdlParser::Entity_class_entryContext *context) -> std::any override
    {
        impl.visitEntityClassEntry(context);
        return {};
    }

    auto visitEntity_class_entry_list(vhdlParser::Entity_class_entry_listContext *context) -> std::any override
    {
        impl.visitEntityClassEntryList(context);
        return {};
    }

    auto visitEntity_declaration(vhdlParser::Entity_declarationContext *context) -> std::any override
    {
        impl.visitEntityDeclaration(context);
        return {};
    }

    auto visitEntity_declarative_item(vhdlParser::Entity_declarative_itemContext *context) -> std::any override
    {
        impl.visitEntityDeclarativeItem(context);
        return {};
    }

    auto visitEntity_declarative_part(vhdlParser::Entity_declarative_partContext *context) -> std::any override
    {
        impl.visitEntityDeclarativePart(context);
        return {};
    }

    auto visitEntity_designator(vhdlParser::Entity_designatorContext *context) -> std::any override
    {
        impl.visitEntityDesignator(context);
        return {};
    }

    auto visitEntity_header(vhdlParser::Entity_headerContext *context) -> std::any override
    {
        impl.visitEntityHeader(context);
        return {};
    }

    auto visitEntity_name_list(vhdlParser::Entity_name_listContext *context) -> std::any override
    {
        impl.visitEntityNameList(context);
        return {};
    }

    auto visitEntity_specification(vhdlParser::Entity_specificationContext *context) -> std::any override
    {
        impl.visitEntitySpecification(context);
        return {};
    }

    auto visitEntity_statement(vhdlParser::Entity_statementContext *context) -> std::any override
    {
        impl.visitEntityStatement(context);
        return {};
    }

    auto visitEntity_statement_part(vhdlParser::Entity_statement_partContext *context) -> std::any override
    {
        impl.visitEntityStatementPart(context);
        return {};
    }

    auto visitEntity_tag(vhdlParser::Entity_tagContext *context) -> std::any override
    {
        impl.visitEntityTag(context);
        return {};
    }

    auto visitEnumeration_literal(vhdlParser::Enumeration_literalContext *context) -> std::any override
    {
        impl.visitEnumerationLiteral(context);
        return {};
    }

    auto visitEnumeration_type_definition(vhdlParser::Enumeration_type_definitionContext *context) -> std::any override
    {
        impl.visitEnumerationTypeDefinition(context);
        return {};
    }

    auto visitExit_statement(vhdlParser::Exit_statementContext *context) -> std::any override
    {
        impl.visitExitStatement(context);
        return {};
    }

    auto visitExpression(vhdlParser::ExpressionContext *context) -> std::any override
    {
        impl.visitExpression(context);
        return {};
    }

    auto visitFactor(vhdlParser::FactorContext *context) -> std::any override
    {
        impl.visitFactor(context);
        return {};
    }

    auto visitFile_declaration(vhdlParser::File_declarationContext *context) -> std::any override
    {
        impl.visitFileDeclaration(context);
        return {};
    }

    auto visitFile_logical_name(vhdlParser::File_logical_nameContext *context) -> std::any override
    {
        impl.visitFileLogicalName(context);
        return {};
    }

    auto visitFile_open_information(vhdlParser::File_open_informationContext *context) -> std::any override
    {
        impl.visitFileOpenInformation(context);
        return {};
    }

    auto visitFile_type_definition(vhdlParser::File_type_definitionContext *context) -> std::any override
    {
        impl.visitFileTypeDefinition(context);
        return {};
    }

    auto visitFormal_parameter_list(vhdlParser::Formal_parameter_listContext *context) -> std::any override
    {
        impl.visitFormalParameterList(context);
        return {};
    }

    auto visitFormal_part(vhdlParser::Formal_partContext *context) -> std::any override
    {
        impl.visitFormalPart(context);
        return {};
    }

    auto visitFree_quantity_declaration(vhdlParser::Free_quantity_declarationContext *context) -> std::any override
    {
        impl.visitFreeQuantityDeclaration(context);
        return {};
    }

    auto visitGenerate_statement(vhdlParser::Generate_statementContext *context) -> std::any override
    {
        impl.visitGenerateStatement(context);
        return {};
    }

    auto visitGeneration_scheme(vhdlParser::Generation_schemeContext *context) -> std::any override
    {
        impl.visitGenerationScheme(context);
        return {};
    }

    auto visitGeneric_clause(vhdlParser::Generic_clauseContext *context) -> std::any override
    {
        impl.visitGenericClause(context);
        return {};
    }

    auto visitGeneric_list(vhdlParser::Generic_listContext *context) -> std::any override
    {
        impl.visitGenericList(context);
        return {};
    }

    auto visitGeneric_map_aspect(vhdlParser::Generic_map_aspectContext *context) -> std::any override
    {
        impl.visitGenericMapAspect(context);
        return {};
    }

    auto visitGroup_constituent(vhdlParser::Group_constituentContext *context) -> std::any override
    {
        impl.visitGroupConstituent(context);
        return {};
    }

    auto visitGroup_constituent_list(vhdlParser::Group_constituent_listContext *context) -> std::any override
    {
        impl.visitGroupConstituentList(context);
        return {};
    }

    auto visitGroup_declaration(vhdlParser::Group_declarationContext *context) -> std::any override
    {
        impl.visitGroupDeclaration(context);
        return {};
    }

    auto visitGroup_template_declaration(vhdlParser::Group_template_declarationContext *context) -> std::any override
    {
        impl.visitGroupTemplateDeclaration(context);
        return {};
    }

    auto visitGuarded_signal_specification(vhdlParser::Guarded_signal_specificationContext *context) -> std::any override
    {
        impl.visitGuardedSignalSpecification(context);
        return {};
    }

    auto visitIdentifier(vhdlParser::IdentifierContext *context) -> std::any override
    {
        impl.visitIdentifier(context);
        return {};
    }

    auto visitIdentifier_list(vhdlParser::Identifier_listContext *context) -> std::any override
    {
        impl.visitIdentifierList(context);
        return {};
    }

    auto visitIf_statement(vhdlParser::If_statementContext *context) -> std::any override
    {
        impl.visitIfStatement(context);
        return {};
    }

    auto visitIndex_constraint(vhdlParser::Index_constraintContext *context) -> std::any override
    {
        impl.visitIndexConstraint(context);
        return {};
    }

    auto visitIndex_specification(vhdlParser::Index_specificationContext *context) -> std::any override
    {
        impl.visitIndexSpecification(context);
        return {};
    }

    auto visitIndex_subtype_definition(vhdlParser::Index_subtype_definitionContext *context) -> std::any override
    {
        impl.visitIndexSubtypeDefinition(context);
        return {};
    }

    auto visitInstantiated_unit(vhdlParser::Instantiated_unitContext *context) -> std::any override
    {
        impl.visitInstantiatedUnit(context);
        return {};
    }

    auto visitInstantiation_list(vhdlParser::Instantiation_listContext *context) -> std::any override
    {
        impl.visitInstantiationList(context);
        return {};
    }

    auto visitInterface_constant_declaration(vhdlParser::Interface_constant_declarationContext *context) -> std::any override
    {
        impl.visitInterfaceConstantDeclaration(context);
        return {};
    }

    auto visitInterface_declaration(vhdlParser::Interface_declarationContext *context) -> std::any override
    {
        impl.visitInterfaceDeclaration(context);
        return {};
    }

    auto visitInterface_element(vhdlParser::Interface_elementContext *context) -> std::any override
    {
        impl.visitInterfaceElement(context);
        return {};
    }

    auto visitInterface_file_declaration(vhdlParser::Interface_file_declarationContext *context) -> std::any override
    {
        impl.visitInterfaceFileDeclaration(context);
        return {};
    }

    auto visitInterface_signal_list(vhdlParser::Interface_signal_listContext *context) -> std::any override
    {
        impl.visitInterfaceSignalList(context);
        return {};
    }

    auto visitInterface_port_list(vhdlParser::Interface_port_listContext *context) -> std::any override
    {
        impl.visitInterfacePortList(context);
        return {};
    }

    auto visitInterface_list(vhdlParser::Interface_listContext *context) -> std::any override
    {
        impl.visitInterfaceList(context);
        return {};
    }

    auto visitInterface_quantity_declaration(vhdlParser::Interface_quantity_declarationContext *context) -> std::any override
    {
        impl.visitInterfaceQuantityDeclaration(context);
        return {};
    }

    auto visitInterface_port_declaration(vhdlParser::Interface_port_declarationContext *context) -> std::any override
    {
        impl.visitInterfacePortDeclaration(context);
        return {};
    }

    auto visitInterface_signal_declaration(vhdlParser::Interface_signal_declarationContext *context) -> std::any override
    {
        impl.visitInterfaceSignalDeclaration(context);
        return {};
    }

    auto visitInterface_terminal_declaration(vhdlParser::Interface_terminal_declarationContext *context) -> std::any override
    {
        impl.visitInterfaceTerminalDeclaration(context);
        return {};
    }

    auto visitInterface_variable_declaration(vhdlParser::Interface_variable_declarationContext *context) -> std::any override
    {
        impl.visitInterfaceVariableDeclaration(context);
        return {};
    }

    auto visitIteration_scheme(vhdlParser::Iteration_schemeContext *context) -> std::any override
    {
        impl.visitIterationScheme(context);
        return {};
    }

    auto visitLabel_colon(vhdlParser::Label_colonContext *context) -> std::any override
    {
        impl.visitLabelColon(context);
        return {};
    }

    auto visitLibrary_clause(vhdlParser::Library_clauseContext *context) -> std::any override
    {
        impl.visitLibraryClause(context);
        return {};
    }

    auto visitLibrary_unit(vhdlParser::Library_unitContext *context) -> std::any override
    {
        impl.visitLibraryUnit(context);
        return {};
    }

    auto visitLiteral(vhdlParser::LiteralContext *context) -> std::any override
    {
        impl.visitLiteral(context);
        return {};
    }

    auto visitLogical_name(vhdlParser::Logical_nameContext *context) -> std::any override
    {
        impl.visitLogicalName(context);
        return {};
    }

    auto visitLogical_name_list(vhdlParser::Logical_name_listContext *context) -> std::any override
    {
        impl.visitLogicalNameList(context);
        return {};
    }

    auto visitLogical_operator(vhdlParser::Logical_operatorContext *context) -> std::any override
    {
        impl.visitLogicalOperator(context);
        return {};
    }

    auto visitLoop_statement(vhdlParser::Loop_statementContext *context) -> std::any override
    {
        impl.visitLoopStatement(context);
        return {};
    }

    auto visitSignal_mode(vhdlParser::Signal_modeContext *context) -> std::any override
    {
        impl.visitSignalMode(context);
        return {};
    }

    auto visitMultiplying_operator(vhdlParser::Multiplying_operatorContext *context) -> std::any override
    {
        impl.visitMultiplyingOperator(context);
        return {};
    }

    auto visitName(vhdlParser::NameContext *context) -> std::any override
    {
        impl.visitName(context);
        return {};
    }

    auto visitName_part(vhdlParser::Name_partContext *context) -> std::any override
    {
        impl.visitNamePart(context);
        return {};
    }

    auto visitSelected_name(vhdlParser::Selected_nameContext *context) -> std::any override
    {
        impl.visitSelectedName(context);
        return {};
    }

    auto visitSelected_name_part(vhdlParser::Selected_name_partContext *context) -> std::any override
    {
        impl.visitSelectedNamePart(context);
        return {};
    }

    auto visitFunction_call_or_indexed_name_part(vhdlParser::Function_call_or_indexed_name_partContext *context) -> std::any override
    {
        impl.visitFunctionCallOrIndexedNamePart(context);
        return {};
    }

    auto visitSlice_name_part(vhdlParser::Slice_name_partContext *context) -> std::any override
    {
        impl.visitSliceNamePart(context);
        return {};
    }

    auto visitAttribute_name_part(vhdlParser::Attribute_name_partContext *context) -> std::any override
    {
        impl.visitAttributeNamePart(context);
        return {};
    }

    auto visitNature_declaration(vhdlParser::Nature_declarationContext *context) -> std::any override
    {
        impl.visitNatureDeclaration(context);
        return {};
    }

    auto visitNature_definition(vhdlParser::Nature_definitionContext *context) -> std::any override
    {
        impl.visitNatureDefinition(context);
        return {};
    }

    auto visitNature_element_declaration(vhdlParser::Nature_element_declarationContext *context) -> std::any override
    {
        impl.visitNatureElementDeclaration(context);
        return {};
    }

    auto visitNext_statement(vhdlParser::Next_statementContext *context) -> std::any override
    {
        impl.visitNextStatement(context);
        return {};
    }

    auto visitNumeric_literal(vhdlParser::Numeric_literalContext *context) -> std::any override
    {
        impl.visitNumericLiteral(context);
        return {};
    }

    auto visitObject_declaration(vhdlParser::Object_declarationContext *context) -> std::any override
    {
        impl.visitObjectDeclaration(context);
        return {};
    }

    auto visitOpts(vhdlParser::OptsContext *context) -> std::any override
    {
        impl.visitOpts(context);
        return {};
    }

    auto visitPackage_body(vhdlParser::Package_bodyContext *context) -> std::any override
    {
        impl.visitPackageBody(context);
        return {};
    }

    auto visitPackage_body_declarative_item(vhdlParser::Package_body_declarative_itemContext *context) -> std::any override
    {
        impl.visitPackageBodyDeclarativeItem(context);
        return {};
    }

    auto visitPackage_body_declarative_part(vhdlParser::Package_body_declarative_partContext *context) -> std::any override
    {
        impl.visitPackageBodyDeclarativePart(context);
        return {};
    }

    auto visitPackage_declaration(vhdlParser::Package_declarationContext *context) -> std::any override
    {
        impl.visitPackageDeclaration(context);
        return {};
    }

    auto visitPackage_declarative_item(vhdlParser::Package_declarative_itemContext *context) -> std::any override
    {
        impl.visitPackageDeclarativeItem(context);
        return {};
    }

    auto visitPackage_declarative_part(vhdlParser::Package_declarative_partContext *context) -> std::any override
    {
        impl.visitPackageDeclarativePart(context);
        return {};
    }

    auto visitParameter_specification(vhdlParser::Parameter_specificationContext *context) -> std::any override
    {
        impl.visitParameterSpecification(context);
        return {};
    }

    auto visitPhysical_literal(vhdlParser::Physical_literalContext *context) -> std::any override
    {
        impl.visitPhysicalLiteral(context);
        return {};
    }

    auto visitPhysical_type_definition(vhdlParser::Physical_type_definitionContext *context) -> std::any override
    {
        impl.visitPhysicalTypeDefinition(context);
        return {};
    }

    auto visitPort_clause(vhdlParser::Port_clauseContext *context) -> std::any override
    {
        impl.visitPortClause(context);
        return {};
    }

    auto visitPort_list(vhdlParser::Port_listContext *context) -> std::any override
    {
        impl.visitPortList(context);
        return {};
    }

    auto visitPort_map_aspect(vhdlParser::Port_map_aspectContext *context) -> std::any override
    {
        impl.visitPortMapAspect(context);
        return {};
    }

    auto visitPrimary(vhdlParser::PrimaryContext *context) -> std::any override
    {
        impl.visitPrimary(context);
        return {};
    }

    auto visitPrimary_unit(vhdlParser::Primary_unitContext *context) -> std::any override
    {
        impl.visitPrimaryUnit(context);
        return {};
    }

    auto visitProcedural_declarative_item(vhdlParser::Procedural_declarative_itemContext *context) -> std::any override
    {
        impl.visitProceduralDeclarativeItem(context);
        return {};
    }

    auto visitProcedural_declarative_part(vhdlParser::Procedural_declarative_partContext *context) -> std::any override
    {
        impl.visitProceduralDeclarativePart(context);
        return {};
    }

    auto visitProcedural_statement_part(vhdlParser::Procedural_statement_partContext *context) -> std::any override
    {
        impl.visitProceduralStatementPart(context);
        return {};
    }

    auto visitProcedure_call(vhdlParser::Procedure_callContext *context) -> std::any override
    {
        impl.visitProcedureCall(context);
        return {};
    }

    auto visitProcedure_call_statement(vhdlParser::Procedure_call_statementContext *context) -> std::any override
    {
        impl.visitProcedureCallStatement(context);
        return {};
    }

    auto visitProcess_declarative_item(vhdlParser::Process_declarative_itemContext *context) -> std::any override
    {
        impl.visitProcessDeclarativeItem(context);
        return {};
    }

    auto visitProcess_declarative_part(vhdlParser::Process_declarative_partContext *context) -> std::any override
    {
        impl.visitProcessDeclarativePart(context);
        return {};
    }

    auto visitProcess_statement(vhdlParser::Process_statementContext *context) -> std::any override
    {
        impl.visitProcessStatement(context);
        return {};
    }

    auto visitProcess_statement_part(vhdlParser::Process_statement_partContext *context) -> std::any override
    {
        impl.visitProcessStatementPart(context);
        return {};
    }

    auto visitQualified_expression(vhdlParser::Qualified_expressionContext *context) -> std::any override
    {
        impl.visitQualifiedExpression(context);
        return {};
    }

    auto visitQuantity_declaration(vhdlParser::Quantity_declarationContext *context) -> std::any override
    {
        impl.visitQuantityDeclaration(context);
        return {};
    }

    auto visitQuantity_list(vhdlParser::Quantity_listContext *context) -> std::any override
    {
        impl.visitQuantityList(context);
        return {};
    }

    auto visitQuantity_specification(vhdlParser::Quantity_specificationContext *context) -> std::any override
    {
        impl.visitQuantitySpecification(context);
        return {};
    }

    auto visitRange_decl(vhdlParser::Range_declContext *context) -> std::any override
    {
        impl.visitRangeDecl(context);
        return {};
    }

    auto visitExplicit_range(vhdlParser::Explicit_rangeContext *context) -> std::any override
    {
        impl.visitExplicitRange(context);
        return {};
    }

    auto visitRange_constraint(vhdlParser::Range_constraintContext *context) -> std::any override
    {
        impl.visitRangeConstraint(context);
        return {};
    }

    auto visitRecord_nature_definition(vhdlParser::Record_nature_definitionContext *context) -> std::any override
    {
        impl.visitRecordNatureDefinition(context);
        return {};
    }

    auto visitRecord_type_definition(vhdlParser::Record_type_definitionContext *context) -> std::any override
    {
        impl.visitRecordTypeDefinition(context);
        return {};
    }

    auto visitRelation(vhdlParser::RelationContext *context) -> std::any override
    {
        impl.visitRelation(context);
        return {};
    }

    auto visitRelational_operator(vhdlParser::Relational_operatorContext *context) -> std::any override
    {
        impl.visitRelationalOperator(context);
        return {};
    }

    auto visitReport_statement(vhdlParser::Report_statementContext *context) -> std::any override
    {
        impl.visitReportStatement(context);
        return {};
    }

    auto visitReturn_statement(vhdlParser::Return_statementContext *context) -> std::any override
    {
        impl.visitReturnStatement(context);
        return {};
    }

    auto visitScalar_nature_definition(vhdlParser::Scalar_nature_definitionContext *context) -> std::any override
    {
        impl.visitScalarNatureDefinition(context);
        return {};
    }

    auto visitScalar_type_definition(vhdlParser::Scalar_type_definitionContext *context) -> std::any override
    {
        impl.visitScalarTypeDefinition(context);
        return {};
    }

    auto visitSecondary_unit(vhdlParser::Secondary_unitContext *context) -> std::any override
    {
        impl.visitSecondaryUnit(context);
        return {};
    }

    auto visitSecondary_unit_declaration(vhdlParser::Secondary_unit_declarationContext *context) -> std::any override
    {
        impl.visitSecondaryUnitDeclaration(context);
        return {};
    }

    auto visitSelected_signal_assignment(vhdlParser::Selected_signal_assignmentContext *context) -> std::any override
    {
        impl.visitSelectedSignalAssignment(context);
        return {};
    }

    auto visitSelected_waveforms(vhdlParser::Selected_waveformsContext *context) -> std::any override
    {
        impl.visitSelectedWaveforms(context);
        return {};
    }

    auto visitSensitivity_clause(vhdlParser::Sensitivity_clauseContext *context) -> std::any override
    {
        impl.visitSensitivityClause(context);
        return {};
    }

    auto visitSensitivity_list(vhdlParser::Sensitivity_listContext *context) -> std::any override
    {
        impl.visitSensitivityList(context);
        return {};
    }

    auto visitSequence_of_statements(vhdlParser::Sequence_of_statementsContext *context) -> std::any override
    {
        impl.visitSequenceOfStatements(context);
        return {};
    }

    auto visitSequential_statement(vhdlParser::Sequential_statementContext *context) -> std::any override
    {
        impl.visitSequentialStatement(context);
        return {};
    }

    auto visitShift_expression(vhdlParser::Shift_expressionContext *context) -> std::any override
    {
        impl.visitShiftExpression(context);
        return {};
    }

    auto visitShift_operator(vhdlParser::Shift_operatorContext *context) -> std::any override
    {
        impl.visitShiftOperator(context);
        return {};
    }

    auto visitSignal_assignment_statement(vhdlParser::Signal_assignment_statementContext *context) -> std::any override
    {
        impl.visitSignalAssignmentStatement(context);
        return {};
    }

    auto visitSignal_declaration(vhdlParser::Signal_declarationContext *context) -> std::any override
    {
        impl.visitSignalDeclaration(context);
        return {};
    }

    auto visitSignal_kind(vhdlParser::Signal_kindContext *context) -> std::any override
    {
        impl.visitSignalKind(context);
        return {};
    }

    auto visitSignal_list(vhdlParser::Signal_listContext *context) -> std::any override
    {
        impl.visitSignalList(context);
        return {};
    }

    auto visitSignature(vhdlParser::SignatureContext *context) -> std::any override
    {
        impl.visitSignature(context);
        return {};
    }

    auto visitSimple_expression(vhdlParser::Simple_expressionContext *context) -> std::any override
    {
        impl.visitSimpleExpression(context);
        return {};
    }

    auto visitSimple_simultaneous_statement(vhdlParser::Simple_simultaneous_statementContext *context) -> std::any override
    {
        impl.visitSimpleSimultaneousStatement(context);
        return {};
    }

    auto visitSimultaneous_alternative(vhdlParser::Simultaneous_alternativeContext *context) -> std::any override
    {
        impl.visitSimultaneousAlternative(context);
        return {};
    }

    auto visitSimultaneous_case_statement(vhdlParser::Simultaneous_case_statementContext *context) -> std::any override
    {
        impl.visitSimultaneousCaseStatement(context);
        return {};
    }

    auto visitSimultaneous_if_statement(vhdlParser::Simultaneous_if_statementContext *context) -> std::any override
    {
        impl.visitSimultaneousIfStatement(context);
        return {};
    }

    auto visitSimultaneous_procedural_statement(vhdlParser::Simultaneous_procedural_statementContext *context) -> std::any override
    {
        impl.visitSimultaneousProceduralStatement(context);
        return {};
    }

    auto visitSimultaneous_statement(vhdlParser::Simultaneous_statementContext *context) -> std::any override
    {
        impl.visitSimultaneousStatement(context);
        return {};
    }

    auto visitSimultaneous_statement_part(vhdlParser::Simultaneous_statement_partContext *context) -> std::any override
    {
        impl.visitSimultaneousStatementPart(context);
        return {};
    }

    auto visitSource_aspect(vhdlParser::Source_aspectContext *context) -> std::any override
    {
        impl.visitSourceAspect(context);
        return {};
    }

    auto visitSource_quantity_declaration(vhdlParser::Source_quantity_declarationContext *context) -> std::any override
    {
        impl.visitSourceQuantityDeclaration(context);
        return {};
    }

    auto visitStep_limit_specification(vhdlParser::Step_limit_specificationContext *context) -> std::any override
    {
        impl.visitStepLimitSpecification(context);
        return {};
    }

    auto visitSubnature_declaration(vhdlParser::Subnature_declarationContext *context) -> std::any override
    {
        impl.visitSubnatureDeclaration(context);
        return {};
    }

    auto visitSubnature_indication(vhdlParser::Subnature_indicationContext *context) -> std::any override
    {
        impl.visitSubnatureIndication(context);
        return {};
    }

    auto visitSubprogram_body(vhdlParser::Subprogram_bodyContext *context) -> std::any override
    {
        impl.visitSubprogramBody(context);
        return {};
    }

    auto visitSubprogram_declaration(vhdlParser::Subprogram_declarationContext *context) -> std::any override
    {
        impl.visitSubprogramDeclaration(context);
        return {};
    }

    auto visitSubprogram_declarative_item(vhdlParser::Subprogram_declarative_itemContext *context) -> std::any override
    {
        impl.visitSubprogramDeclarativeItem(context);
        return {};
    }

    auto visitSubprogram_declarative_part(vhdlParser::Subprogram_declarative_partContext *context) -> std::any override
    {
        impl.visitSubprogramDeclarativePart(context);
        return {};
    }

    auto visitSubprogram_kind(vhdlParser::Subprogram_kindContext *context) -> std::any override
    {
        impl.visitSubprogramKind(context);
        return {};
    }

    auto visitSubprogram_specification(vhdlParser::Subprogram_specificationContext *context) -> std::any override
    {
        impl.visitSubprogramSpecification(context);
        return {};
    }

    auto visitProcedure_specification(vhdlParser::Procedure_specificationContext *context) -> std::any override
    {
        impl.visitProcedureSpecification(context);
        return {};
    }

    auto visitFunction_specification(vhdlParser::Function_specificationContext *context) -> std::any override
    {
        impl.visitFunctionSpecification(context);
        return {};
    }

    auto visitSubprogram_statement_part(vhdlParser::Subprogram_statement_partContext *context) -> std::any override
    {
        impl.visitSubprogramStatementPart(context);
        return {};
    }

    auto visitSubtype_declaration(vhdlParser::Subtype_declarationContext *context) -> std::any override
    {
        impl.visitSubtypeDeclaration(context);
        return {};
    }

    auto visitSubtype_indication(vhdlParser::Subtype_indicationContext *context) -> std::any override
    {
        impl.visitSubtypeIndication(context);
        return {};
    }

    auto visitSuffix(vhdlParser::SuffixContext *context) -> std::any override
    {
        impl.visitSuffix(context);
        return {};
    }

    auto visitTarget(vhdlParser::TargetContext *context) -> std::any override
    {
        impl.visitTarget(context);
        return {};
    }

    auto visitTerm(vhdlParser::TermContext *context) -> std::any override
    {
        impl.visitTerm(context);
        return {};
    }

    auto visitTerminal_aspect(vhdlParser::Terminal_aspectContext *context) -> std::any override
    {
        impl.visitTerminalAspect(context);
        return {};
    }

    auto visitTerminal_declaration(vhdlParser::Terminal_declarationContext *context) -> std::any override
    {
        impl.visitTerminalDeclaration(context);
        return {};
    }

    auto visitThrough_aspect(vhdlParser::Through_aspectContext *context) -> std::any override
    {
        impl.visitThroughAspect(context);
        return {};
    }

    auto visitTimeout_clause(vhdlParser::Timeout_clauseContext *context) -> std::any override
    {
        impl.visitTimeoutClause(context);
        return {};
    }

    auto visitTolerance_aspect(vhdlParser::Tolerance_aspectContext *context) -> std::any override
    {
        impl.visitToleranceAspect(context);
        return {};
    }

    auto visitType_declaration(vhdlParser::Type_declarationContext *context) -> std::any override
    {
        impl.visitTypeDeclaration(context);
        return {};
    }

    auto visitType_definition(vhdlParser::Type_definitionContext *context) -> std::any override
    {
        impl.visitTypeDefinition(context);
        return {};
    }

    auto visitUnconstrained_array_definition(vhdlParser::Unconstrained_array_definitionContext *context) -> std::any override
    {
        impl.visitUnconstrainedArrayDefinition(context);
        return {};
    }

    auto visitUnconstrained_nature_definition(vhdlParser::Unconstrained_nature_definitionContext *context) -> std::any override
    {
        impl.visitUnconstrainedNatureDefinition(context);
        return {};
    }

    auto visitUse_clause(vhdlParser::Use_clauseContext *context) -> std::any override
    {
        impl.visitUseClause(context);
        return {};
    }

    auto visitVariable_assignment_statement(vhdlParser::Variable_assignment_statementContext *context) -> std::any override
    {
        impl.visitVariableAssignmentStatement(context);
        return {};
    }

    auto visitVariable_declaration(vhdlParser::Variable_declarationContext *context) -> std::any override
    {
        impl.visitVariableDeclaration(context);
        return {};
    }

    auto visitWait_statement(vhdlParser::Wait_statementContext *context) -> std::any override
    {
        impl.visitWaitStatement(context);
        return {};
    }

    auto visitWaveform(vhdlParser::WaveformContext *context) -> std::any override
    {
        impl.visitWaveform(context);
        return {};
    }

    auto visitWaveform_element(vhdlParser::Waveform_elementContext *context) -> std::any override
    {
        impl.visitWaveformElement(context);
        return {};
    }
};
} // namespace builder::adapter

#endif /* BUILDER_ADAPTER_ANTLR_VOID_ADAPTER_HPP */
