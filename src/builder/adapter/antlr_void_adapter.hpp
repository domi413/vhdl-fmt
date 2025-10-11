#ifndef BUILDER_ADAPTER_ANTLR_VOID_ADAPTER_HPP
#define BUILDER_ADAPTER_ANTLR_VOID_ADAPTER_HPP

#include "vhdlBaseVisitor.h"
#include "vhdlParser.h"
#include "void_visitor.hpp"

#include <any>

namespace builder::adapter {

class AntlrVoidAdapter : public vhdlBaseVisitor
{

    VoidVisitor &impl_;

  public:
    explicit AntlrVoidAdapter(VoidVisitor &v) : impl_(v) {}

    ~AntlrVoidAdapter() override = default;
    AntlrVoidAdapter(const AntlrVoidAdapter &) = delete;
    auto operator=(const AntlrVoidAdapter &) -> AntlrVoidAdapter & = delete;
    AntlrVoidAdapter(AntlrVoidAdapter &&) = delete;
    auto operator=(AntlrVoidAdapter &&) -> AntlrVoidAdapter & = delete;

    auto visitAbstract_literal(vhdlParser::Abstract_literalContext *context) -> std::any override
    {
        impl_.visitAbstractLiteral(context);
        return {};
    }

    auto visitAccess_type_definition(vhdlParser::Access_type_definitionContext *context) -> std::any override
    {
        impl_.visitAccessTypeDefinition(context);
        return {};
    }

    auto visitAcross_aspect(vhdlParser::Across_aspectContext *context) -> std::any override
    {
        impl_.visitAcrossAspect(context);
        return {};
    }

    auto visitActual_designator(vhdlParser::Actual_designatorContext *context) -> std::any override
    {
        impl_.visitActualDesignator(context);
        return {};
    }

    auto visitActual_parameter_part(vhdlParser::Actual_parameter_partContext *context) -> std::any override
    {
        impl_.visitActualParameterPart(context);
        return {};
    }

    auto visitActual_part(vhdlParser::Actual_partContext *context) -> std::any override
    {
        impl_.visitActualPart(context);
        return {};
    }

    auto visitAdding_operator(vhdlParser::Adding_operatorContext *context) -> std::any override
    {
        impl_.visitAddingOperator(context);
        return {};
    }

    auto visitAggregate(vhdlParser::AggregateContext *context) -> std::any override
    {
        impl_.visitAggregate(context);
        return {};
    }

    auto visitAlias_declaration(vhdlParser::Alias_declarationContext *context) -> std::any override
    {
        impl_.visitAliasDeclaration(context);
        return {};
    }

    auto visitAlias_designator(vhdlParser::Alias_designatorContext *context) -> std::any override
    {
        impl_.visitAliasDesignator(context);
        return {};
    }

    auto visitAlias_indication(vhdlParser::Alias_indicationContext *context) -> std::any override
    {
        impl_.visitAliasIndication(context);
        return {};
    }

    auto visitAllocator(vhdlParser::AllocatorContext *context) -> std::any override
    {
        impl_.visitAllocator(context);
        return {};
    }

    auto visitArchitecture_body(vhdlParser::Architecture_bodyContext *context) -> std::any override
    {
        impl_.visitArchitectureBody(context);
        return {};
    }

    auto visitArchitecture_declarative_part(vhdlParser::Architecture_declarative_partContext *context) -> std::any override
    {
        impl_.visitArchitectureDeclarativePart(context);
        return {};
    }

    auto visitArchitecture_statement(vhdlParser::Architecture_statementContext *context) -> std::any override
    {
        impl_.visitArchitectureStatement(context);
        return {};
    }

    auto visitArchitecture_statement_part(vhdlParser::Architecture_statement_partContext *context) -> std::any override
    {
        impl_.visitArchitectureStatementPart(context);
        return {};
    }

    auto visitArray_nature_definition(vhdlParser::Array_nature_definitionContext *context) -> std::any override
    {
        impl_.visitArrayNatureDefinition(context);
        return {};
    }

    auto visitArray_type_definition(vhdlParser::Array_type_definitionContext *context) -> std::any override
    {
        impl_.visitArrayTypeDefinition(context);
        return {};
    }

    auto visitAssertion(vhdlParser::AssertionContext *context) -> std::any override
    {
        impl_.visitAssertion(context);
        return {};
    }

    auto visitAssertion_statement(vhdlParser::Assertion_statementContext *context) -> std::any override
    {
        impl_.visitAssertionStatement(context);
        return {};
    }

    auto visitAssociation_element(vhdlParser::Association_elementContext *context) -> std::any override
    {
        impl_.visitAssociationElement(context);
        return {};
    }

    auto visitAssociation_list(vhdlParser::Association_listContext *context) -> std::any override
    {
        impl_.visitAssociationList(context);
        return {};
    }

    auto visitAttribute_declaration(vhdlParser::Attribute_declarationContext *context) -> std::any override
    {
        impl_.visitAttributeDeclaration(context);
        return {};
    }

    auto visitAttribute_designator(vhdlParser::Attribute_designatorContext *context) -> std::any override
    {
        impl_.visitAttributeDesignator(context);
        return {};
    }

    auto visitAttribute_specification(vhdlParser::Attribute_specificationContext *context) -> std::any override
    {
        impl_.visitAttributeSpecification(context);
        return {};
    }

    auto visitBase_unit_declaration(vhdlParser::Base_unit_declarationContext *context) -> std::any override
    {
        impl_.visitBaseUnitDeclaration(context);
        return {};
    }

    auto visitBinding_indication(vhdlParser::Binding_indicationContext *context) -> std::any override
    {
        impl_.visitBindingIndication(context);
        return {};
    }

    auto visitBlock_configuration(vhdlParser::Block_configurationContext *context) -> std::any override
    {
        impl_.visitBlockConfiguration(context);
        return {};
    }

    auto visitBlock_declarative_item(vhdlParser::Block_declarative_itemContext *context) -> std::any override
    {
        impl_.visitBlockDeclarativeItem(context);
        return {};
    }

    auto visitBlock_declarative_part(vhdlParser::Block_declarative_partContext *context) -> std::any override
    {
        impl_.visitBlockDeclarativePart(context);
        return {};
    }

    auto visitBlock_header(vhdlParser::Block_headerContext *context) -> std::any override
    {
        impl_.visitBlockHeader(context);
        return {};
    }

    auto visitBlock_specification(vhdlParser::Block_specificationContext *context) -> std::any override
    {
        impl_.visitBlockSpecification(context);
        return {};
    }

    auto visitBlock_statement(vhdlParser::Block_statementContext *context) -> std::any override
    {
        impl_.visitBlockStatement(context);
        return {};
    }

    auto visitBlock_statement_part(vhdlParser::Block_statement_partContext *context) -> std::any override
    {
        impl_.visitBlockStatementPart(context);
        return {};
    }

    auto visitBranch_quantity_declaration(vhdlParser::Branch_quantity_declarationContext *context) -> std::any override
    {
        impl_.visitBranchQuantityDeclaration(context);
        return {};
    }

    auto visitBreak_element(vhdlParser::Break_elementContext *context) -> std::any override
    {
        impl_.visitBreakElement(context);
        return {};
    }

    auto visitBreak_list(vhdlParser::Break_listContext *context) -> std::any override
    {
        impl_.visitBreakList(context);
        return {};
    }

    auto visitBreak_selector_clause(vhdlParser::Break_selector_clauseContext *context) -> std::any override
    {
        impl_.visitBreakSelectorClause(context);
        return {};
    }

    auto visitBreak_statement(vhdlParser::Break_statementContext *context) -> std::any override
    {
        impl_.visitBreakStatement(context);
        return {};
    }

    auto visitCase_statement(vhdlParser::Case_statementContext *context) -> std::any override
    {
        impl_.visitCaseStatement(context);
        return {};
    }

    auto visitCase_statement_alternative(vhdlParser::Case_statement_alternativeContext *context) -> std::any override
    {
        impl_.visitCaseStatementAlternative(context);
        return {};
    }

    auto visitChoice(vhdlParser::ChoiceContext *context) -> std::any override
    {
        impl_.visitChoice(context);
        return {};
    }

    auto visitChoices(vhdlParser::ChoicesContext *context) -> std::any override
    {
        impl_.visitChoices(context);
        return {};
    }

    auto visitComponent_configuration(vhdlParser::Component_configurationContext *context) -> std::any override
    {
        impl_.visitComponentConfiguration(context);
        return {};
    }

    auto visitComponent_declaration(vhdlParser::Component_declarationContext *context) -> std::any override
    {
        impl_.visitComponentDeclaration(context);
        return {};
    }

    auto visitComponent_instantiation_statement(vhdlParser::Component_instantiation_statementContext *context) -> std::any override
    {
        impl_.visitComponentInstantiationStatement(context);
        return {};
    }

    auto visitComponent_specification(vhdlParser::Component_specificationContext *context) -> std::any override
    {
        impl_.visitComponentSpecification(context);
        return {};
    }

    auto visitComposite_nature_definition(vhdlParser::Composite_nature_definitionContext *context) -> std::any override
    {
        impl_.visitCompositeNatureDefinition(context);
        return {};
    }

    auto visitComposite_type_definition(vhdlParser::Composite_type_definitionContext *context) -> std::any override
    {
        impl_.visitCompositeTypeDefinition(context);
        return {};
    }

    auto visitConcurrent_assertion_statement(vhdlParser::Concurrent_assertion_statementContext *context) -> std::any override
    {
        impl_.visitConcurrentAssertionStatement(context);
        return {};
    }

    auto visitConcurrent_break_statement(vhdlParser::Concurrent_break_statementContext *context) -> std::any override
    {
        impl_.visitConcurrentBreakStatement(context);
        return {};
    }

    auto visitConcurrent_procedure_call_statement(vhdlParser::Concurrent_procedure_call_statementContext *context) -> std::any override
    {
        impl_.visitConcurrentProcedureCallStatement(context);
        return {};
    }

    auto visitConcurrent_signal_assignment_statement(vhdlParser::Concurrent_signal_assignment_statementContext *context) -> std::any override
    {
        impl_.visitConcurrentSignalAssignmentStatement(context);
        return {};
    }

    auto visitCondition(vhdlParser::ConditionContext *context) -> std::any override
    {
        impl_.visitCondition(context);
        return {};
    }

    auto visitCondition_clause(vhdlParser::Condition_clauseContext *context) -> std::any override
    {
        impl_.visitConditionClause(context);
        return {};
    }

    auto visitConditional_signal_assignment(vhdlParser::Conditional_signal_assignmentContext *context) -> std::any override
    {
        impl_.visitConditionalSignalAssignment(context);
        return {};
    }

    auto visitConditional_waveforms(vhdlParser::Conditional_waveformsContext *context) -> std::any override
    {
        impl_.visitConditionalWaveforms(context);
        return {};
    }

    auto visitConfiguration_declaration(vhdlParser::Configuration_declarationContext *context) -> std::any override
    {
        impl_.visitConfigurationDeclaration(context);
        return {};
    }

    auto visitConfiguration_declarative_item(vhdlParser::Configuration_declarative_itemContext *context) -> std::any override
    {
        impl_.visitConfigurationDeclarativeItem(context);
        return {};
    }

    auto visitConfiguration_declarative_part(vhdlParser::Configuration_declarative_partContext *context) -> std::any override
    {
        impl_.visitConfigurationDeclarativePart(context);
        return {};
    }

    auto visitConfiguration_item(vhdlParser::Configuration_itemContext *context) -> std::any override
    {
        impl_.visitConfigurationItem(context);
        return {};
    }

    auto visitConfiguration_specification(vhdlParser::Configuration_specificationContext *context) -> std::any override
    {
        impl_.visitConfigurationSpecification(context);
        return {};
    }

    auto visitConstant_declaration(vhdlParser::Constant_declarationContext *context) -> std::any override
    {
        impl_.visitConstantDeclaration(context);
        return {};
    }

    auto visitConstrained_array_definition(vhdlParser::Constrained_array_definitionContext *context) -> std::any override
    {
        impl_.visitConstrainedArrayDefinition(context);
        return {};
    }

    auto visitConstrained_nature_definition(vhdlParser::Constrained_nature_definitionContext *context) -> std::any override
    {
        impl_.visitConstrainedNatureDefinition(context);
        return {};
    }

    auto visitConstraint(vhdlParser::ConstraintContext *context) -> std::any override
    {
        impl_.visitConstraint(context);
        return {};
    }

    auto visitContext_clause(vhdlParser::Context_clauseContext *context) -> std::any override
    {
        impl_.visitContextClause(context);
        return {};
    }

    auto visitContext_item(vhdlParser::Context_itemContext *context) -> std::any override
    {
        impl_.visitContextItem(context);
        return {};
    }

    auto visitDelay_mechanism(vhdlParser::Delay_mechanismContext *context) -> std::any override
    {
        impl_.visitDelayMechanism(context);
        return {};
    }

    auto visitDesign_file(vhdlParser::Design_fileContext *context) -> std::any override
    {
        impl_.visitDesignFile(context);
        return {};
    }

    auto visitDesign_unit(vhdlParser::Design_unitContext *context) -> std::any override
    {
        impl_.visitDesignUnit(context);
        return {};
    }

    auto visitDesignator(vhdlParser::DesignatorContext *context) -> std::any override
    {
        impl_.visitDesignator(context);
        return {};
    }

    auto visitDirection(vhdlParser::DirectionContext *context) -> std::any override
    {
        impl_.visitDirection(context);
        return {};
    }

    auto visitDisconnection_specification(vhdlParser::Disconnection_specificationContext *context) -> std::any override
    {
        impl_.visitDisconnectionSpecification(context);
        return {};
    }

    auto visitDiscrete_range(vhdlParser::Discrete_rangeContext *context) -> std::any override
    {
        impl_.visitDiscreteRange(context);
        return {};
    }

    auto visitElement_association(vhdlParser::Element_associationContext *context) -> std::any override
    {
        impl_.visitElementAssociation(context);
        return {};
    }

    auto visitElement_declaration(vhdlParser::Element_declarationContext *context) -> std::any override
    {
        impl_.visitElementDeclaration(context);
        return {};
    }

    auto visitElement_subnature_definition(vhdlParser::Element_subnature_definitionContext *context) -> std::any override
    {
        impl_.visitElementSubnatureDefinition(context);
        return {};
    }

    auto visitElement_subtype_definition(vhdlParser::Element_subtype_definitionContext *context) -> std::any override
    {
        impl_.visitElementSubtypeDefinition(context);
        return {};
    }

    auto visitEntity_aspect(vhdlParser::Entity_aspectContext *context) -> std::any override
    {
        impl_.visitEntityAspect(context);
        return {};
    }

    auto visitEntity_class(vhdlParser::Entity_classContext *context) -> std::any override
    {
        impl_.visitEntityClass(context);
        return {};
    }

    auto visitEntity_class_entry(vhdlParser::Entity_class_entryContext *context) -> std::any override
    {
        impl_.visitEntityClassEntry(context);
        return {};
    }

    auto visitEntity_class_entry_list(vhdlParser::Entity_class_entry_listContext *context) -> std::any override
    {
        impl_.visitEntityClassEntryList(context);
        return {};
    }

    auto visitEntity_declaration(vhdlParser::Entity_declarationContext *context) -> std::any override
    {
        impl_.visitEntityDeclaration(context);
        return {};
    }

    auto visitEntity_declarative_item(vhdlParser::Entity_declarative_itemContext *context) -> std::any override
    {
        impl_.visitEntityDeclarativeItem(context);
        return {};
    }

    auto visitEntity_declarative_part(vhdlParser::Entity_declarative_partContext *context) -> std::any override
    {
        impl_.visitEntityDeclarativePart(context);
        return {};
    }

    auto visitEntity_designator(vhdlParser::Entity_designatorContext *context) -> std::any override
    {
        impl_.visitEntityDesignator(context);
        return {};
    }

    auto visitEntity_header(vhdlParser::Entity_headerContext *context) -> std::any override
    {
        impl_.visitEntityHeader(context);
        return {};
    }

    auto visitEntity_name_list(vhdlParser::Entity_name_listContext *context) -> std::any override
    {
        impl_.visitEntityNameList(context);
        return {};
    }

    auto visitEntity_specification(vhdlParser::Entity_specificationContext *context) -> std::any override
    {
        impl_.visitEntitySpecification(context);
        return {};
    }

    auto visitEntity_statement(vhdlParser::Entity_statementContext *context) -> std::any override
    {
        impl_.visitEntityStatement(context);
        return {};
    }

    auto visitEntity_statement_part(vhdlParser::Entity_statement_partContext *context) -> std::any override
    {
        impl_.visitEntityStatementPart(context);
        return {};
    }

    auto visitEntity_tag(vhdlParser::Entity_tagContext *context) -> std::any override
    {
        impl_.visitEntityTag(context);
        return {};
    }

    auto visitEnumeration_literal(vhdlParser::Enumeration_literalContext *context) -> std::any override
    {
        impl_.visitEnumerationLiteral(context);
        return {};
    }

    auto visitEnumeration_type_definition(vhdlParser::Enumeration_type_definitionContext *context) -> std::any override
    {
        impl_.visitEnumerationTypeDefinition(context);
        return {};
    }

    auto visitExit_statement(vhdlParser::Exit_statementContext *context) -> std::any override
    {
        impl_.visitExitStatement(context);
        return {};
    }

    auto visitExpression(vhdlParser::ExpressionContext *context) -> std::any override
    {
        impl_.visitExpression(context);
        return {};
    }

    auto visitFactor(vhdlParser::FactorContext *context) -> std::any override
    {
        impl_.visitFactor(context);
        return {};
    }

    auto visitFile_declaration(vhdlParser::File_declarationContext *context) -> std::any override
    {
        impl_.visitFileDeclaration(context);
        return {};
    }

    auto visitFile_logical_name(vhdlParser::File_logical_nameContext *context) -> std::any override
    {
        impl_.visitFileLogicalName(context);
        return {};
    }

    auto visitFile_open_information(vhdlParser::File_open_informationContext *context) -> std::any override
    {
        impl_.visitFileOpenInformation(context);
        return {};
    }

    auto visitFile_type_definition(vhdlParser::File_type_definitionContext *context) -> std::any override
    {
        impl_.visitFileTypeDefinition(context);
        return {};
    }

    auto visitFormal_parameter_list(vhdlParser::Formal_parameter_listContext *context) -> std::any override
    {
        impl_.visitFormalParameterList(context);
        return {};
    }

    auto visitFormal_part(vhdlParser::Formal_partContext *context) -> std::any override
    {
        impl_.visitFormalPart(context);
        return {};
    }

    auto visitFree_quantity_declaration(vhdlParser::Free_quantity_declarationContext *context) -> std::any override
    {
        impl_.visitFreeQuantityDeclaration(context);
        return {};
    }

    auto visitGenerate_statement(vhdlParser::Generate_statementContext *context) -> std::any override
    {
        impl_.visitGenerateStatement(context);
        return {};
    }

    auto visitGeneration_scheme(vhdlParser::Generation_schemeContext *context) -> std::any override
    {
        impl_.visitGenerationScheme(context);
        return {};
    }

    auto visitGeneric_clause(vhdlParser::Generic_clauseContext *context) -> std::any override
    {
        impl_.visitGenericClause(context);
        return {};
    }

    auto visitGeneric_list(vhdlParser::Generic_listContext *context) -> std::any override
    {
        impl_.visitGenericList(context);
        return {};
    }

    auto visitGeneric_map_aspect(vhdlParser::Generic_map_aspectContext *context) -> std::any override
    {
        impl_.visitGenericMapAspect(context);
        return {};
    }

    auto visitGroup_constituent(vhdlParser::Group_constituentContext *context) -> std::any override
    {
        impl_.visitGroupConstituent(context);
        return {};
    }

    auto visitGroup_constituent_list(vhdlParser::Group_constituent_listContext *context) -> std::any override
    {
        impl_.visitGroupConstituentList(context);
        return {};
    }

    auto visitGroup_declaration(vhdlParser::Group_declarationContext *context) -> std::any override
    {
        impl_.visitGroupDeclaration(context);
        return {};
    }

    auto visitGroup_template_declaration(vhdlParser::Group_template_declarationContext *context) -> std::any override
    {
        impl_.visitGroupTemplateDeclaration(context);
        return {};
    }

    auto visitGuarded_signal_specification(vhdlParser::Guarded_signal_specificationContext *context) -> std::any override
    {
        impl_.visitGuardedSignalSpecification(context);
        return {};
    }

    auto visitIdentifier(vhdlParser::IdentifierContext *context) -> std::any override
    {
        impl_.visitIdentifier(context);
        return {};
    }

    auto visitIdentifier_list(vhdlParser::Identifier_listContext *context) -> std::any override
    {
        impl_.visitIdentifierList(context);
        return {};
    }

    auto visitIf_statement(vhdlParser::If_statementContext *context) -> std::any override
    {
        impl_.visitIfStatement(context);
        return {};
    }

    auto visitIndex_constraint(vhdlParser::Index_constraintContext *context) -> std::any override
    {
        impl_.visitIndexConstraint(context);
        return {};
    }

    auto visitIndex_specification(vhdlParser::Index_specificationContext *context) -> std::any override
    {
        impl_.visitIndexSpecification(context);
        return {};
    }

    auto visitIndex_subtype_definition(vhdlParser::Index_subtype_definitionContext *context) -> std::any override
    {
        impl_.visitIndexSubtypeDefinition(context);
        return {};
    }

    auto visitInstantiated_unit(vhdlParser::Instantiated_unitContext *context) -> std::any override
    {
        impl_.visitInstantiatedUnit(context);
        return {};
    }

    auto visitInstantiation_list(vhdlParser::Instantiation_listContext *context) -> std::any override
    {
        impl_.visitInstantiationList(context);
        return {};
    }

    auto visitInterface_constant_declaration(vhdlParser::Interface_constant_declarationContext *context) -> std::any override
    {
        impl_.visitInterfaceConstantDeclaration(context);
        return {};
    }

    auto visitInterface_declaration(vhdlParser::Interface_declarationContext *context) -> std::any override
    {
        impl_.visitInterfaceDeclaration(context);
        return {};
    }

    auto visitInterface_element(vhdlParser::Interface_elementContext *context) -> std::any override
    {
        impl_.visitInterfaceElement(context);
        return {};
    }

    auto visitInterface_file_declaration(vhdlParser::Interface_file_declarationContext *context) -> std::any override
    {
        impl_.visitInterfaceFileDeclaration(context);
        return {};
    }

    auto visitInterface_signal_list(vhdlParser::Interface_signal_listContext *context) -> std::any override
    {
        impl_.visitInterfaceSignalList(context);
        return {};
    }

    auto visitInterface_port_list(vhdlParser::Interface_port_listContext *context) -> std::any override
    {
        impl_.visitInterfacePortList(context);
        return {};
    }

    auto visitInterface_list(vhdlParser::Interface_listContext *context) -> std::any override
    {
        impl_.visitInterfaceList(context);
        return {};
    }

    auto visitInterface_quantity_declaration(vhdlParser::Interface_quantity_declarationContext *context) -> std::any override
    {
        impl_.visitInterfaceQuantityDeclaration(context);
        return {};
    }

    auto visitInterface_port_declaration(vhdlParser::Interface_port_declarationContext *context) -> std::any override
    {
        impl_.visitInterfacePortDeclaration(context);
        return {};
    }

    auto visitInterface_signal_declaration(vhdlParser::Interface_signal_declarationContext *context) -> std::any override
    {
        impl_.visitInterfaceSignalDeclaration(context);
        return {};
    }

    auto visitInterface_terminal_declaration(vhdlParser::Interface_terminal_declarationContext *context) -> std::any override
    {
        impl_.visitInterfaceTerminalDeclaration(context);
        return {};
    }

    auto visitInterface_variable_declaration(vhdlParser::Interface_variable_declarationContext *context) -> std::any override
    {
        impl_.visitInterfaceVariableDeclaration(context);
        return {};
    }

    auto visitIteration_scheme(vhdlParser::Iteration_schemeContext *context) -> std::any override
    {
        impl_.visitIterationScheme(context);
        return {};
    }

    auto visitLabel_colon(vhdlParser::Label_colonContext *context) -> std::any override
    {
        impl_.visitLabelColon(context);
        return {};
    }

    auto visitLibrary_clause(vhdlParser::Library_clauseContext *context) -> std::any override
    {
        impl_.visitLibraryClause(context);
        return {};
    }

    auto visitLibrary_unit(vhdlParser::Library_unitContext *context) -> std::any override
    {
        impl_.visitLibraryUnit(context);
        return {};
    }

    auto visitLiteral(vhdlParser::LiteralContext *context) -> std::any override
    {
        impl_.visitLiteral(context);
        return {};
    }

    auto visitLogical_name(vhdlParser::Logical_nameContext *context) -> std::any override
    {
        impl_.visitLogicalName(context);
        return {};
    }

    auto visitLogical_name_list(vhdlParser::Logical_name_listContext *context) -> std::any override
    {
        impl_.visitLogicalNameList(context);
        return {};
    }

    auto visitLogical_operator(vhdlParser::Logical_operatorContext *context) -> std::any override
    {
        impl_.visitLogicalOperator(context);
        return {};
    }

    auto visitLoop_statement(vhdlParser::Loop_statementContext *context) -> std::any override
    {
        impl_.visitLoopStatement(context);
        return {};
    }

    auto visitSignal_mode(vhdlParser::Signal_modeContext *context) -> std::any override
    {
        impl_.visitSignalMode(context);
        return {};
    }

    auto visitMultiplying_operator(vhdlParser::Multiplying_operatorContext *context) -> std::any override
    {
        impl_.visitMultiplyingOperator(context);
        return {};
    }

    auto visitName(vhdlParser::NameContext *context) -> std::any override
    {
        impl_.visitName(context);
        return {};
    }

    auto visitName_part(vhdlParser::Name_partContext *context) -> std::any override
    {
        impl_.visitNamePart(context);
        return {};
    }

    auto visitSelected_name(vhdlParser::Selected_nameContext *context) -> std::any override
    {
        impl_.visitSelectedName(context);
        return {};
    }

    auto visitSelected_name_part(vhdlParser::Selected_name_partContext *context) -> std::any override
    {
        impl_.visitSelectedNamePart(context);
        return {};
    }

    auto visitFunction_call_or_indexed_name_part(vhdlParser::Function_call_or_indexed_name_partContext *context) -> std::any override
    {
        impl_.visitFunctionCallOrIndexedNamePart(context);
        return {};
    }

    auto visitSlice_name_part(vhdlParser::Slice_name_partContext *context) -> std::any override
    {
        impl_.visitSliceNamePart(context);
        return {};
    }

    auto visitAttribute_name_part(vhdlParser::Attribute_name_partContext *context) -> std::any override
    {
        impl_.visitAttributeNamePart(context);
        return {};
    }

    auto visitNature_declaration(vhdlParser::Nature_declarationContext *context) -> std::any override
    {
        impl_.visitNatureDeclaration(context);
        return {};
    }

    auto visitNature_definition(vhdlParser::Nature_definitionContext *context) -> std::any override
    {
        impl_.visitNatureDefinition(context);
        return {};
    }

    auto visitNature_element_declaration(vhdlParser::Nature_element_declarationContext *context) -> std::any override
    {
        impl_.visitNatureElementDeclaration(context);
        return {};
    }

    auto visitNext_statement(vhdlParser::Next_statementContext *context) -> std::any override
    {
        impl_.visitNextStatement(context);
        return {};
    }

    auto visitNumeric_literal(vhdlParser::Numeric_literalContext *context) -> std::any override
    {
        impl_.visitNumericLiteral(context);
        return {};
    }

    auto visitObject_declaration(vhdlParser::Object_declarationContext *context) -> std::any override
    {
        impl_.visitObjectDeclaration(context);
        return {};
    }

    auto visitOpts(vhdlParser::OptsContext *context) -> std::any override
    {
        impl_.visitOpts(context);
        return {};
    }

    auto visitPackage_body(vhdlParser::Package_bodyContext *context) -> std::any override
    {
        impl_.visitPackageBody(context);
        return {};
    }

    auto visitPackage_body_declarative_item(vhdlParser::Package_body_declarative_itemContext *context) -> std::any override
    {
        impl_.visitPackageBodyDeclarativeItem(context);
        return {};
    }

    auto visitPackage_body_declarative_part(vhdlParser::Package_body_declarative_partContext *context) -> std::any override
    {
        impl_.visitPackageBodyDeclarativePart(context);
        return {};
    }

    auto visitPackage_declaration(vhdlParser::Package_declarationContext *context) -> std::any override
    {
        impl_.visitPackageDeclaration(context);
        return {};
    }

    auto visitPackage_declarative_item(vhdlParser::Package_declarative_itemContext *context) -> std::any override
    {
        impl_.visitPackageDeclarativeItem(context);
        return {};
    }

    auto visitPackage_declarative_part(vhdlParser::Package_declarative_partContext *context) -> std::any override
    {
        impl_.visitPackageDeclarativePart(context);
        return {};
    }

    auto visitParameter_specification(vhdlParser::Parameter_specificationContext *context) -> std::any override
    {
        impl_.visitParameterSpecification(context);
        return {};
    }

    auto visitPhysical_literal(vhdlParser::Physical_literalContext *context) -> std::any override
    {
        impl_.visitPhysicalLiteral(context);
        return {};
    }

    auto visitPhysical_type_definition(vhdlParser::Physical_type_definitionContext *context) -> std::any override
    {
        impl_.visitPhysicalTypeDefinition(context);
        return {};
    }

    auto visitPort_clause(vhdlParser::Port_clauseContext *context) -> std::any override
    {
        impl_.visitPortClause(context);
        return {};
    }

    auto visitPort_list(vhdlParser::Port_listContext *context) -> std::any override
    {
        impl_.visitPortList(context);
        return {};
    }

    auto visitPort_map_aspect(vhdlParser::Port_map_aspectContext *context) -> std::any override
    {
        impl_.visitPortMapAspect(context);
        return {};
    }

    auto visitPrimary(vhdlParser::PrimaryContext *context) -> std::any override
    {
        impl_.visitPrimary(context);
        return {};
    }

    auto visitPrimary_unit(vhdlParser::Primary_unitContext *context) -> std::any override
    {
        impl_.visitPrimaryUnit(context);
        return {};
    }

    auto visitProcedural_declarative_item(vhdlParser::Procedural_declarative_itemContext *context) -> std::any override
    {
        impl_.visitProceduralDeclarativeItem(context);
        return {};
    }

    auto visitProcedural_declarative_part(vhdlParser::Procedural_declarative_partContext *context) -> std::any override
    {
        impl_.visitProceduralDeclarativePart(context);
        return {};
    }

    auto visitProcedural_statement_part(vhdlParser::Procedural_statement_partContext *context) -> std::any override
    {
        impl_.visitProceduralStatementPart(context);
        return {};
    }

    auto visitProcedure_call(vhdlParser::Procedure_callContext *context) -> std::any override
    {
        impl_.visitProcedureCall(context);
        return {};
    }

    auto visitProcedure_call_statement(vhdlParser::Procedure_call_statementContext *context) -> std::any override
    {
        impl_.visitProcedureCallStatement(context);
        return {};
    }

    auto visitProcess_declarative_item(vhdlParser::Process_declarative_itemContext *context) -> std::any override
    {
        impl_.visitProcessDeclarativeItem(context);
        return {};
    }

    auto visitProcess_declarative_part(vhdlParser::Process_declarative_partContext *context) -> std::any override
    {
        impl_.visitProcessDeclarativePart(context);
        return {};
    }

    auto visitProcess_statement(vhdlParser::Process_statementContext *context) -> std::any override
    {
        impl_.visitProcessStatement(context);
        return {};
    }

    auto visitProcess_statement_part(vhdlParser::Process_statement_partContext *context) -> std::any override
    {
        impl_.visitProcessStatementPart(context);
        return {};
    }

    auto visitQualified_expression(vhdlParser::Qualified_expressionContext *context) -> std::any override
    {
        impl_.visitQualifiedExpression(context);
        return {};
    }

    auto visitQuantity_declaration(vhdlParser::Quantity_declarationContext *context) -> std::any override
    {
        impl_.visitQuantityDeclaration(context);
        return {};
    }

    auto visitQuantity_list(vhdlParser::Quantity_listContext *context) -> std::any override
    {
        impl_.visitQuantityList(context);
        return {};
    }

    auto visitQuantity_specification(vhdlParser::Quantity_specificationContext *context) -> std::any override
    {
        impl_.visitQuantitySpecification(context);
        return {};
    }

    auto visitRange_decl(vhdlParser::Range_declContext *context) -> std::any override
    {
        impl_.visitRangeDecl(context);
        return {};
    }

    auto visitExplicit_range(vhdlParser::Explicit_rangeContext *context) -> std::any override
    {
        impl_.visitExplicitRange(context);
        return {};
    }

    auto visitRange_constraint(vhdlParser::Range_constraintContext *context) -> std::any override
    {
        impl_.visitRangeConstraint(context);
        return {};
    }

    auto visitRecord_nature_definition(vhdlParser::Record_nature_definitionContext *context) -> std::any override
    {
        impl_.visitRecordNatureDefinition(context);
        return {};
    }

    auto visitRecord_type_definition(vhdlParser::Record_type_definitionContext *context) -> std::any override
    {
        impl_.visitRecordTypeDefinition(context);
        return {};
    }

    auto visitRelation(vhdlParser::RelationContext *context) -> std::any override
    {
        impl_.visitRelation(context);
        return {};
    }

    auto visitRelational_operator(vhdlParser::Relational_operatorContext *context) -> std::any override
    {
        impl_.visitRelationalOperator(context);
        return {};
    }

    auto visitReport_statement(vhdlParser::Report_statementContext *context) -> std::any override
    {
        impl_.visitReportStatement(context);
        return {};
    }

    auto visitReturn_statement(vhdlParser::Return_statementContext *context) -> std::any override
    {
        impl_.visitReturnStatement(context);
        return {};
    }

    auto visitScalar_nature_definition(vhdlParser::Scalar_nature_definitionContext *context) -> std::any override
    {
        impl_.visitScalarNatureDefinition(context);
        return {};
    }

    auto visitScalar_type_definition(vhdlParser::Scalar_type_definitionContext *context) -> std::any override
    {
        impl_.visitScalarTypeDefinition(context);
        return {};
    }

    auto visitSecondary_unit(vhdlParser::Secondary_unitContext *context) -> std::any override
    {
        impl_.visitSecondaryUnit(context);
        return {};
    }

    auto visitSecondary_unit_declaration(vhdlParser::Secondary_unit_declarationContext *context) -> std::any override
    {
        impl_.visitSecondaryUnitDeclaration(context);
        return {};
    }

    auto visitSelected_signal_assignment(vhdlParser::Selected_signal_assignmentContext *context) -> std::any override
    {
        impl_.visitSelectedSignalAssignment(context);
        return {};
    }

    auto visitSelected_waveforms(vhdlParser::Selected_waveformsContext *context) -> std::any override
    {
        impl_.visitSelectedWaveforms(context);
        return {};
    }

    auto visitSensitivity_clause(vhdlParser::Sensitivity_clauseContext *context) -> std::any override
    {
        impl_.visitSensitivityClause(context);
        return {};
    }

    auto visitSensitivity_list(vhdlParser::Sensitivity_listContext *context) -> std::any override
    {
        impl_.visitSensitivityList(context);
        return {};
    }

    auto visitSequence_of_statements(vhdlParser::Sequence_of_statementsContext *context) -> std::any override
    {
        impl_.visitSequenceOfStatements(context);
        return {};
    }

    auto visitSequential_statement(vhdlParser::Sequential_statementContext *context) -> std::any override
    {
        impl_.visitSequentialStatement(context);
        return {};
    }

    auto visitShift_expression(vhdlParser::Shift_expressionContext *context) -> std::any override
    {
        impl_.visitShiftExpression(context);
        return {};
    }

    auto visitShift_operator(vhdlParser::Shift_operatorContext *context) -> std::any override
    {
        impl_.visitShiftOperator(context);
        return {};
    }

    auto visitSignal_assignment_statement(vhdlParser::Signal_assignment_statementContext *context) -> std::any override
    {
        impl_.visitSignalAssignmentStatement(context);
        return {};
    }

    auto visitSignal_declaration(vhdlParser::Signal_declarationContext *context) -> std::any override
    {
        impl_.visitSignalDeclaration(context);
        return {};
    }

    auto visitSignal_kind(vhdlParser::Signal_kindContext *context) -> std::any override
    {
        impl_.visitSignalKind(context);
        return {};
    }

    auto visitSignal_list(vhdlParser::Signal_listContext *context) -> std::any override
    {
        impl_.visitSignalList(context);
        return {};
    }

    auto visitSignature(vhdlParser::SignatureContext *context) -> std::any override
    {
        impl_.visitSignature(context);
        return {};
    }

    auto visitSimple_expression(vhdlParser::Simple_expressionContext *context) -> std::any override
    {
        impl_.visitSimpleExpression(context);
        return {};
    }

    auto visitSimple_simultaneous_statement(vhdlParser::Simple_simultaneous_statementContext *context) -> std::any override
    {
        impl_.visitSimpleSimultaneousStatement(context);
        return {};
    }

    auto visitSimultaneous_alternative(vhdlParser::Simultaneous_alternativeContext *context) -> std::any override
    {
        impl_.visitSimultaneousAlternative(context);
        return {};
    }

    auto visitSimultaneous_case_statement(vhdlParser::Simultaneous_case_statementContext *context) -> std::any override
    {
        impl_.visitSimultaneousCaseStatement(context);
        return {};
    }

    auto visitSimultaneous_if_statement(vhdlParser::Simultaneous_if_statementContext *context) -> std::any override
    {
        impl_.visitSimultaneousIfStatement(context);
        return {};
    }

    auto visitSimultaneous_procedural_statement(vhdlParser::Simultaneous_procedural_statementContext *context) -> std::any override
    {
        impl_.visitSimultaneousProceduralStatement(context);
        return {};
    }

    auto visitSimultaneous_statement(vhdlParser::Simultaneous_statementContext *context) -> std::any override
    {
        impl_.visitSimultaneousStatement(context);
        return {};
    }

    auto visitSimultaneous_statement_part(vhdlParser::Simultaneous_statement_partContext *context) -> std::any override
    {
        impl_.visitSimultaneousStatementPart(context);
        return {};
    }

    auto visitSource_aspect(vhdlParser::Source_aspectContext *context) -> std::any override
    {
        impl_.visitSourceAspect(context);
        return {};
    }

    auto visitSource_quantity_declaration(vhdlParser::Source_quantity_declarationContext *context) -> std::any override
    {
        impl_.visitSourceQuantityDeclaration(context);
        return {};
    }

    auto visitStep_limit_specification(vhdlParser::Step_limit_specificationContext *context) -> std::any override
    {
        impl_.visitStepLimitSpecification(context);
        return {};
    }

    auto visitSubnature_declaration(vhdlParser::Subnature_declarationContext *context) -> std::any override
    {
        impl_.visitSubnatureDeclaration(context);
        return {};
    }

    auto visitSubnature_indication(vhdlParser::Subnature_indicationContext *context) -> std::any override
    {
        impl_.visitSubnatureIndication(context);
        return {};
    }

    auto visitSubprogram_body(vhdlParser::Subprogram_bodyContext *context) -> std::any override
    {
        impl_.visitSubprogramBody(context);
        return {};
    }

    auto visitSubprogram_declaration(vhdlParser::Subprogram_declarationContext *context) -> std::any override
    {
        impl_.visitSubprogramDeclaration(context);
        return {};
    }

    auto visitSubprogram_declarative_item(vhdlParser::Subprogram_declarative_itemContext *context) -> std::any override
    {
        impl_.visitSubprogramDeclarativeItem(context);
        return {};
    }

    auto visitSubprogram_declarative_part(vhdlParser::Subprogram_declarative_partContext *context) -> std::any override
    {
        impl_.visitSubprogramDeclarativePart(context);
        return {};
    }

    auto visitSubprogram_kind(vhdlParser::Subprogram_kindContext *context) -> std::any override
    {
        impl_.visitSubprogramKind(context);
        return {};
    }

    auto visitSubprogram_specification(vhdlParser::Subprogram_specificationContext *context) -> std::any override
    {
        impl_.visitSubprogramSpecification(context);
        return {};
    }

    auto visitProcedure_specification(vhdlParser::Procedure_specificationContext *context) -> std::any override
    {
        impl_.visitProcedureSpecification(context);
        return {};
    }

    auto visitFunction_specification(vhdlParser::Function_specificationContext *context) -> std::any override
    {
        impl_.visitFunctionSpecification(context);
        return {};
    }

    auto visitSubprogram_statement_part(vhdlParser::Subprogram_statement_partContext *context) -> std::any override
    {
        impl_.visitSubprogramStatementPart(context);
        return {};
    }

    auto visitSubtype_declaration(vhdlParser::Subtype_declarationContext *context) -> std::any override
    {
        impl_.visitSubtypeDeclaration(context);
        return {};
    }

    auto visitSubtype_indication(vhdlParser::Subtype_indicationContext *context) -> std::any override
    {
        impl_.visitSubtypeIndication(context);
        return {};
    }

    auto visitSuffix(vhdlParser::SuffixContext *context) -> std::any override
    {
        impl_.visitSuffix(context);
        return {};
    }

    auto visitTarget(vhdlParser::TargetContext *context) -> std::any override
    {
        impl_.visitTarget(context);
        return {};
    }

    auto visitTerm(vhdlParser::TermContext *context) -> std::any override
    {
        impl_.visitTerm(context);
        return {};
    }

    auto visitTerminal_aspect(vhdlParser::Terminal_aspectContext *context) -> std::any override
    {
        impl_.visitTerminalAspect(context);
        return {};
    }

    auto visitTerminal_declaration(vhdlParser::Terminal_declarationContext *context) -> std::any override
    {
        impl_.visitTerminalDeclaration(context);
        return {};
    }

    auto visitThrough_aspect(vhdlParser::Through_aspectContext *context) -> std::any override
    {
        impl_.visitThroughAspect(context);
        return {};
    }

    auto visitTimeout_clause(vhdlParser::Timeout_clauseContext *context) -> std::any override
    {
        impl_.visitTimeoutClause(context);
        return {};
    }

    auto visitTolerance_aspect(vhdlParser::Tolerance_aspectContext *context) -> std::any override
    {
        impl_.visitToleranceAspect(context);
        return {};
    }

    auto visitType_declaration(vhdlParser::Type_declarationContext *context) -> std::any override
    {
        impl_.visitTypeDeclaration(context);
        return {};
    }

    auto visitType_definition(vhdlParser::Type_definitionContext *context) -> std::any override
    {
        impl_.visitTypeDefinition(context);
        return {};
    }

    auto visitUnconstrained_array_definition(vhdlParser::Unconstrained_array_definitionContext *context) -> std::any override
    {
        impl_.visitUnconstrainedArrayDefinition(context);
        return {};
    }

    auto visitUnconstrained_nature_definition(vhdlParser::Unconstrained_nature_definitionContext *context) -> std::any override
    {
        impl_.visitUnconstrainedNatureDefinition(context);
        return {};
    }

    auto visitUse_clause(vhdlParser::Use_clauseContext *context) -> std::any override
    {
        impl_.visitUseClause(context);
        return {};
    }

    auto visitVariable_assignment_statement(vhdlParser::Variable_assignment_statementContext *context) -> std::any override
    {
        impl_.visitVariableAssignmentStatement(context);
        return {};
    }

    auto visitVariable_declaration(vhdlParser::Variable_declarationContext *context) -> std::any override
    {
        impl_.visitVariableDeclaration(context);
        return {};
    }

    auto visitWait_statement(vhdlParser::Wait_statementContext *context) -> std::any override
    {
        impl_.visitWaitStatement(context);
        return {};
    }

    auto visitWaveform(vhdlParser::WaveformContext *context) -> std::any override
    {
        impl_.visitWaveform(context);
        return {};
    }

    auto visitWaveform_element(vhdlParser::Waveform_elementContext *context) -> std::any override
    {
        impl_.visitWaveformElement(context);
        return {};
    }
};

} // namespace builder::adapter

#endif /* BUILDER_ADAPTER_ANTLR_VOID_ADAPTER_HPP */
