#pragma once
#include "vhdlBaseVisitor.h"
#include "void_visitor.hpp"

namespace builder::adapter {
class AntlrVoidAdapter : public vhdlBaseVisitor {
    VoidVisitor &impl;
public:
    explicit AntlrVoidAdapter(VoidVisitor &v) : impl(v) {}

    std::any visitAbstract_literal(vhdlParser::Abstract_literalContext *context) override {
        impl.visitAbstract_literal(context);
        return {};
    }

    std::any visitAccess_type_definition(vhdlParser::Access_type_definitionContext *context) override {
        impl.visitAccess_type_definition(context);
        return {};
    }

    std::any visitAcross_aspect(vhdlParser::Across_aspectContext *context) override {
        impl.visitAcross_aspect(context);
        return {};
    }

    std::any visitActual_designator(vhdlParser::Actual_designatorContext *context) override {
        impl.visitActual_designator(context);
        return {};
    }

    std::any visitActual_parameter_part(vhdlParser::Actual_parameter_partContext *context) override {
        impl.visitActual_parameter_part(context);
        return {};
    }

    std::any visitActual_part(vhdlParser::Actual_partContext *context) override {
        impl.visitActual_part(context);
        return {};
    }

    std::any visitAdding_operator(vhdlParser::Adding_operatorContext *context) override {
        impl.visitAdding_operator(context);
        return {};
    }

    std::any visitAggregate(vhdlParser::AggregateContext *context) override {
        impl.visitAggregate(context);
        return {};
    }

    std::any visitAlias_declaration(vhdlParser::Alias_declarationContext *context) override {
        impl.visitAlias_declaration(context);
        return {};
    }

    std::any visitAlias_designator(vhdlParser::Alias_designatorContext *context) override {
        impl.visitAlias_designator(context);
        return {};
    }

    std::any visitAlias_indication(vhdlParser::Alias_indicationContext *context) override {
        impl.visitAlias_indication(context);
        return {};
    }

    std::any visitAllocator(vhdlParser::AllocatorContext *context) override {
        impl.visitAllocator(context);
        return {};
    }

    std::any visitArchitecture_body(vhdlParser::Architecture_bodyContext *context) override {
        impl.visitArchitecture_body(context);
        return {};
    }

    std::any visitArchitecture_declarative_part(vhdlParser::Architecture_declarative_partContext *context) override {
        impl.visitArchitecture_declarative_part(context);
        return {};
    }

    std::any visitArchitecture_statement(vhdlParser::Architecture_statementContext *context) override {
        impl.visitArchitecture_statement(context);
        return {};
    }

    std::any visitArchitecture_statement_part(vhdlParser::Architecture_statement_partContext *context) override {
        impl.visitArchitecture_statement_part(context);
        return {};
    }

    std::any visitArray_nature_definition(vhdlParser::Array_nature_definitionContext *context) override {
        impl.visitArray_nature_definition(context);
        return {};
    }

    std::any visitArray_type_definition(vhdlParser::Array_type_definitionContext *context) override {
        impl.visitArray_type_definition(context);
        return {};
    }

    std::any visitAssertion(vhdlParser::AssertionContext *context) override {
        impl.visitAssertion(context);
        return {};
    }

    std::any visitAssertion_statement(vhdlParser::Assertion_statementContext *context) override {
        impl.visitAssertion_statement(context);
        return {};
    }

    std::any visitAssociation_element(vhdlParser::Association_elementContext *context) override {
        impl.visitAssociation_element(context);
        return {};
    }

    std::any visitAssociation_list(vhdlParser::Association_listContext *context) override {
        impl.visitAssociation_list(context);
        return {};
    }

    std::any visitAttribute_declaration(vhdlParser::Attribute_declarationContext *context) override {
        impl.visitAttribute_declaration(context);
        return {};
    }

    std::any visitAttribute_designator(vhdlParser::Attribute_designatorContext *context) override {
        impl.visitAttribute_designator(context);
        return {};
    }

    std::any visitAttribute_specification(vhdlParser::Attribute_specificationContext *context) override {
        impl.visitAttribute_specification(context);
        return {};
    }

    std::any visitBase_unit_declaration(vhdlParser::Base_unit_declarationContext *context) override {
        impl.visitBase_unit_declaration(context);
        return {};
    }

    std::any visitBinding_indication(vhdlParser::Binding_indicationContext *context) override {
        impl.visitBinding_indication(context);
        return {};
    }

    std::any visitBlock_configuration(vhdlParser::Block_configurationContext *context) override {
        impl.visitBlock_configuration(context);
        return {};
    }

    std::any visitBlock_declarative_item(vhdlParser::Block_declarative_itemContext *context) override {
        impl.visitBlock_declarative_item(context);
        return {};
    }

    std::any visitBlock_declarative_part(vhdlParser::Block_declarative_partContext *context) override {
        impl.visitBlock_declarative_part(context);
        return {};
    }

    std::any visitBlock_header(vhdlParser::Block_headerContext *context) override {
        impl.visitBlock_header(context);
        return {};
    }

    std::any visitBlock_specification(vhdlParser::Block_specificationContext *context) override {
        impl.visitBlock_specification(context);
        return {};
    }

    std::any visitBlock_statement(vhdlParser::Block_statementContext *context) override {
        impl.visitBlock_statement(context);
        return {};
    }

    std::any visitBlock_statement_part(vhdlParser::Block_statement_partContext *context) override {
        impl.visitBlock_statement_part(context);
        return {};
    }

    std::any visitBranch_quantity_declaration(vhdlParser::Branch_quantity_declarationContext *context) override {
        impl.visitBranch_quantity_declaration(context);
        return {};
    }

    std::any visitBreak_element(vhdlParser::Break_elementContext *context) override {
        impl.visitBreak_element(context);
        return {};
    }

    std::any visitBreak_list(vhdlParser::Break_listContext *context) override {
        impl.visitBreak_list(context);
        return {};
    }

    std::any visitBreak_selector_clause(vhdlParser::Break_selector_clauseContext *context) override {
        impl.visitBreak_selector_clause(context);
        return {};
    }

    std::any visitBreak_statement(vhdlParser::Break_statementContext *context) override {
        impl.visitBreak_statement(context);
        return {};
    }

    std::any visitCase_statement(vhdlParser::Case_statementContext *context) override {
        impl.visitCase_statement(context);
        return {};
    }

    std::any visitCase_statement_alternative(vhdlParser::Case_statement_alternativeContext *context) override {
        impl.visitCase_statement_alternative(context);
        return {};
    }

    std::any visitChoice(vhdlParser::ChoiceContext *context) override {
        impl.visitChoice(context);
        return {};
    }

    std::any visitChoices(vhdlParser::ChoicesContext *context) override {
        impl.visitChoices(context);
        return {};
    }

    std::any visitComponent_configuration(vhdlParser::Component_configurationContext *context) override {
        impl.visitComponent_configuration(context);
        return {};
    }

    std::any visitComponent_declaration(vhdlParser::Component_declarationContext *context) override {
        impl.visitComponent_declaration(context);
        return {};
    }

    std::any visitComponent_instantiation_statement(vhdlParser::Component_instantiation_statementContext *context) override {
        impl.visitComponent_instantiation_statement(context);
        return {};
    }

    std::any visitComponent_specification(vhdlParser::Component_specificationContext *context) override {
        impl.visitComponent_specification(context);
        return {};
    }

    std::any visitComposite_nature_definition(vhdlParser::Composite_nature_definitionContext *context) override {
        impl.visitComposite_nature_definition(context);
        return {};
    }

    std::any visitComposite_type_definition(vhdlParser::Composite_type_definitionContext *context) override {
        impl.visitComposite_type_definition(context);
        return {};
    }

    std::any visitConcurrent_assertion_statement(vhdlParser::Concurrent_assertion_statementContext *context) override {
        impl.visitConcurrent_assertion_statement(context);
        return {};
    }

    std::any visitConcurrent_break_statement(vhdlParser::Concurrent_break_statementContext *context) override {
        impl.visitConcurrent_break_statement(context);
        return {};
    }

    std::any visitConcurrent_procedure_call_statement(vhdlParser::Concurrent_procedure_call_statementContext *context) override {
        impl.visitConcurrent_procedure_call_statement(context);
        return {};
    }

    std::any visitConcurrent_signal_assignment_statement(vhdlParser::Concurrent_signal_assignment_statementContext *context) override {
        impl.visitConcurrent_signal_assignment_statement(context);
        return {};
    }

    std::any visitCondition(vhdlParser::ConditionContext *context) override {
        impl.visitCondition(context);
        return {};
    }

    std::any visitCondition_clause(vhdlParser::Condition_clauseContext *context) override {
        impl.visitCondition_clause(context);
        return {};
    }

    std::any visitConditional_signal_assignment(vhdlParser::Conditional_signal_assignmentContext *context) override {
        impl.visitConditional_signal_assignment(context);
        return {};
    }

    std::any visitConditional_waveforms(vhdlParser::Conditional_waveformsContext *context) override {
        impl.visitConditional_waveforms(context);
        return {};
    }

    std::any visitConfiguration_declaration(vhdlParser::Configuration_declarationContext *context) override {
        impl.visitConfiguration_declaration(context);
        return {};
    }

    std::any visitConfiguration_declarative_item(vhdlParser::Configuration_declarative_itemContext *context) override {
        impl.visitConfiguration_declarative_item(context);
        return {};
    }

    std::any visitConfiguration_declarative_part(vhdlParser::Configuration_declarative_partContext *context) override {
        impl.visitConfiguration_declarative_part(context);
        return {};
    }

    std::any visitConfiguration_item(vhdlParser::Configuration_itemContext *context) override {
        impl.visitConfiguration_item(context);
        return {};
    }

    std::any visitConfiguration_specification(vhdlParser::Configuration_specificationContext *context) override {
        impl.visitConfiguration_specification(context);
        return {};
    }

    std::any visitConstant_declaration(vhdlParser::Constant_declarationContext *context) override {
        impl.visitConstant_declaration(context);
        return {};
    }

    std::any visitConstrained_array_definition(vhdlParser::Constrained_array_definitionContext *context) override {
        impl.visitConstrained_array_definition(context);
        return {};
    }

    std::any visitConstrained_nature_definition(vhdlParser::Constrained_nature_definitionContext *context) override {
        impl.visitConstrained_nature_definition(context);
        return {};
    }

    std::any visitConstraint(vhdlParser::ConstraintContext *context) override {
        impl.visitConstraint(context);
        return {};
    }

    std::any visitContext_clause(vhdlParser::Context_clauseContext *context) override {
        impl.visitContext_clause(context);
        return {};
    }

    std::any visitContext_item(vhdlParser::Context_itemContext *context) override {
        impl.visitContext_item(context);
        return {};
    }

    std::any visitDelay_mechanism(vhdlParser::Delay_mechanismContext *context) override {
        impl.visitDelay_mechanism(context);
        return {};
    }

    std::any visitDesign_file(vhdlParser::Design_fileContext *context) override {
        impl.visitDesign_file(context);
        return {};
    }

    std::any visitDesign_unit(vhdlParser::Design_unitContext *context) override {
        impl.visitDesign_unit(context);
        return {};
    }

    std::any visitDesignator(vhdlParser::DesignatorContext *context) override {
        impl.visitDesignator(context);
        return {};
    }

    std::any visitDirection(vhdlParser::DirectionContext *context) override {
        impl.visitDirection(context);
        return {};
    }

    std::any visitDisconnection_specification(vhdlParser::Disconnection_specificationContext *context) override {
        impl.visitDisconnection_specification(context);
        return {};
    }

    std::any visitDiscrete_range(vhdlParser::Discrete_rangeContext *context) override {
        impl.visitDiscrete_range(context);
        return {};
    }

    std::any visitElement_association(vhdlParser::Element_associationContext *context) override {
        impl.visitElement_association(context);
        return {};
    }

    std::any visitElement_declaration(vhdlParser::Element_declarationContext *context) override {
        impl.visitElement_declaration(context);
        return {};
    }

    std::any visitElement_subnature_definition(vhdlParser::Element_subnature_definitionContext *context) override {
        impl.visitElement_subnature_definition(context);
        return {};
    }

    std::any visitElement_subtype_definition(vhdlParser::Element_subtype_definitionContext *context) override {
        impl.visitElement_subtype_definition(context);
        return {};
    }

    std::any visitEntity_aspect(vhdlParser::Entity_aspectContext *context) override {
        impl.visitEntity_aspect(context);
        return {};
    }

    std::any visitEntity_class(vhdlParser::Entity_classContext *context) override {
        impl.visitEntity_class(context);
        return {};
    }

    std::any visitEntity_class_entry(vhdlParser::Entity_class_entryContext *context) override {
        impl.visitEntity_class_entry(context);
        return {};
    }

    std::any visitEntity_class_entry_list(vhdlParser::Entity_class_entry_listContext *context) override {
        impl.visitEntity_class_entry_list(context);
        return {};
    }

    std::any visitEntity_declaration(vhdlParser::Entity_declarationContext *context) override {
        impl.visitEntity_declaration(context);
        return {};
    }

    std::any visitEntity_declarative_item(vhdlParser::Entity_declarative_itemContext *context) override {
        impl.visitEntity_declarative_item(context);
        return {};
    }

    std::any visitEntity_declarative_part(vhdlParser::Entity_declarative_partContext *context) override {
        impl.visitEntity_declarative_part(context);
        return {};
    }

    std::any visitEntity_designator(vhdlParser::Entity_designatorContext *context) override {
        impl.visitEntity_designator(context);
        return {};
    }

    std::any visitEntity_header(vhdlParser::Entity_headerContext *context) override {
        impl.visitEntity_header(context);
        return {};
    }

    std::any visitEntity_name_list(vhdlParser::Entity_name_listContext *context) override {
        impl.visitEntity_name_list(context);
        return {};
    }

    std::any visitEntity_specification(vhdlParser::Entity_specificationContext *context) override {
        impl.visitEntity_specification(context);
        return {};
    }

    std::any visitEntity_statement(vhdlParser::Entity_statementContext *context) override {
        impl.visitEntity_statement(context);
        return {};
    }

    std::any visitEntity_statement_part(vhdlParser::Entity_statement_partContext *context) override {
        impl.visitEntity_statement_part(context);
        return {};
    }

    std::any visitEntity_tag(vhdlParser::Entity_tagContext *context) override {
        impl.visitEntity_tag(context);
        return {};
    }

    std::any visitEnumeration_literal(vhdlParser::Enumeration_literalContext *context) override {
        impl.visitEnumeration_literal(context);
        return {};
    }

    std::any visitEnumeration_type_definition(vhdlParser::Enumeration_type_definitionContext *context) override {
        impl.visitEnumeration_type_definition(context);
        return {};
    }

    std::any visitExit_statement(vhdlParser::Exit_statementContext *context) override {
        impl.visitExit_statement(context);
        return {};
    }

    std::any visitExpression(vhdlParser::ExpressionContext *context) override {
        impl.visitExpression(context);
        return {};
    }

    std::any visitFactor(vhdlParser::FactorContext *context) override {
        impl.visitFactor(context);
        return {};
    }

    std::any visitFile_declaration(vhdlParser::File_declarationContext *context) override {
        impl.visitFile_declaration(context);
        return {};
    }

    std::any visitFile_logical_name(vhdlParser::File_logical_nameContext *context) override {
        impl.visitFile_logical_name(context);
        return {};
    }

    std::any visitFile_open_information(vhdlParser::File_open_informationContext *context) override {
        impl.visitFile_open_information(context);
        return {};
    }

    std::any visitFile_type_definition(vhdlParser::File_type_definitionContext *context) override {
        impl.visitFile_type_definition(context);
        return {};
    }

    std::any visitFormal_parameter_list(vhdlParser::Formal_parameter_listContext *context) override {
        impl.visitFormal_parameter_list(context);
        return {};
    }

    std::any visitFormal_part(vhdlParser::Formal_partContext *context) override {
        impl.visitFormal_part(context);
        return {};
    }

    std::any visitFree_quantity_declaration(vhdlParser::Free_quantity_declarationContext *context) override {
        impl.visitFree_quantity_declaration(context);
        return {};
    }

    std::any visitGenerate_statement(vhdlParser::Generate_statementContext *context) override {
        impl.visitGenerate_statement(context);
        return {};
    }

    std::any visitGeneration_scheme(vhdlParser::Generation_schemeContext *context) override {
        impl.visitGeneration_scheme(context);
        return {};
    }

    std::any visitGeneric_clause(vhdlParser::Generic_clauseContext *context) override {
        impl.visitGeneric_clause(context);
        return {};
    }

    std::any visitGeneric_list(vhdlParser::Generic_listContext *context) override {
        impl.visitGeneric_list(context);
        return {};
    }

    std::any visitGeneric_map_aspect(vhdlParser::Generic_map_aspectContext *context) override {
        impl.visitGeneric_map_aspect(context);
        return {};
    }

    std::any visitGroup_constituent(vhdlParser::Group_constituentContext *context) override {
        impl.visitGroup_constituent(context);
        return {};
    }

    std::any visitGroup_constituent_list(vhdlParser::Group_constituent_listContext *context) override {
        impl.visitGroup_constituent_list(context);
        return {};
    }

    std::any visitGroup_declaration(vhdlParser::Group_declarationContext *context) override {
        impl.visitGroup_declaration(context);
        return {};
    }

    std::any visitGroup_template_declaration(vhdlParser::Group_template_declarationContext *context) override {
        impl.visitGroup_template_declaration(context);
        return {};
    }

    std::any visitGuarded_signal_specification(vhdlParser::Guarded_signal_specificationContext *context) override {
        impl.visitGuarded_signal_specification(context);
        return {};
    }

    std::any visitIdentifier(vhdlParser::IdentifierContext *context) override {
        impl.visitIdentifier(context);
        return {};
    }

    std::any visitIdentifier_list(vhdlParser::Identifier_listContext *context) override {
        impl.visitIdentifier_list(context);
        return {};
    }

    std::any visitIf_statement(vhdlParser::If_statementContext *context) override {
        impl.visitIf_statement(context);
        return {};
    }

    std::any visitIndex_constraint(vhdlParser::Index_constraintContext *context) override {
        impl.visitIndex_constraint(context);
        return {};
    }

    std::any visitIndex_specification(vhdlParser::Index_specificationContext *context) override {
        impl.visitIndex_specification(context);
        return {};
    }

    std::any visitIndex_subtype_definition(vhdlParser::Index_subtype_definitionContext *context) override {
        impl.visitIndex_subtype_definition(context);
        return {};
    }

    std::any visitInstantiated_unit(vhdlParser::Instantiated_unitContext *context) override {
        impl.visitInstantiated_unit(context);
        return {};
    }

    std::any visitInstantiation_list(vhdlParser::Instantiation_listContext *context) override {
        impl.visitInstantiation_list(context);
        return {};
    }

    std::any visitInterface_constant_declaration(vhdlParser::Interface_constant_declarationContext *context) override {
        impl.visitInterface_constant_declaration(context);
        return {};
    }

    std::any visitInterface_declaration(vhdlParser::Interface_declarationContext *context) override {
        impl.visitInterface_declaration(context);
        return {};
    }

    std::any visitInterface_element(vhdlParser::Interface_elementContext *context) override {
        impl.visitInterface_element(context);
        return {};
    }

    std::any visitInterface_file_declaration(vhdlParser::Interface_file_declarationContext *context) override {
        impl.visitInterface_file_declaration(context);
        return {};
    }

    std::any visitInterface_signal_list(vhdlParser::Interface_signal_listContext *context) override {
        impl.visitInterface_signal_list(context);
        return {};
    }

    std::any visitInterface_port_list(vhdlParser::Interface_port_listContext *context) override {
        impl.visitInterface_port_list(context);
        return {};
    }

    std::any visitInterface_list(vhdlParser::Interface_listContext *context) override {
        impl.visitInterface_list(context);
        return {};
    }

    std::any visitInterface_quantity_declaration(vhdlParser::Interface_quantity_declarationContext *context) override {
        impl.visitInterface_quantity_declaration(context);
        return {};
    }

    std::any visitInterface_port_declaration(vhdlParser::Interface_port_declarationContext *context) override {
        impl.visitInterface_port_declaration(context);
        return {};
    }

    std::any visitInterface_signal_declaration(vhdlParser::Interface_signal_declarationContext *context) override {
        impl.visitInterface_signal_declaration(context);
        return {};
    }

    std::any visitInterface_terminal_declaration(vhdlParser::Interface_terminal_declarationContext *context) override {
        impl.visitInterface_terminal_declaration(context);
        return {};
    }

    std::any visitInterface_variable_declaration(vhdlParser::Interface_variable_declarationContext *context) override {
        impl.visitInterface_variable_declaration(context);
        return {};
    }

    std::any visitIteration_scheme(vhdlParser::Iteration_schemeContext *context) override {
        impl.visitIteration_scheme(context);
        return {};
    }

    std::any visitLabel_colon(vhdlParser::Label_colonContext *context) override {
        impl.visitLabel_colon(context);
        return {};
    }

    std::any visitLibrary_clause(vhdlParser::Library_clauseContext *context) override {
        impl.visitLibrary_clause(context);
        return {};
    }

    std::any visitLibrary_unit(vhdlParser::Library_unitContext *context) override {
        impl.visitLibrary_unit(context);
        return {};
    }

    std::any visitLiteral(vhdlParser::LiteralContext *context) override {
        impl.visitLiteral(context);
        return {};
    }

    std::any visitLogical_name(vhdlParser::Logical_nameContext *context) override {
        impl.visitLogical_name(context);
        return {};
    }

    std::any visitLogical_name_list(vhdlParser::Logical_name_listContext *context) override {
        impl.visitLogical_name_list(context);
        return {};
    }

    std::any visitLogical_operator(vhdlParser::Logical_operatorContext *context) override {
        impl.visitLogical_operator(context);
        return {};
    }

    std::any visitLoop_statement(vhdlParser::Loop_statementContext *context) override {
        impl.visitLoop_statement(context);
        return {};
    }

    std::any visitSignal_mode(vhdlParser::Signal_modeContext *context) override {
        impl.visitSignal_mode(context);
        return {};
    }

    std::any visitMultiplying_operator(vhdlParser::Multiplying_operatorContext *context) override {
        impl.visitMultiplying_operator(context);
        return {};
    }

    std::any visitName(vhdlParser::NameContext *context) override {
        impl.visitName(context);
        return {};
    }

    std::any visitName_part(vhdlParser::Name_partContext *context) override {
        impl.visitName_part(context);
        return {};
    }

    std::any visitSelected_name(vhdlParser::Selected_nameContext *context) override {
        impl.visitSelected_name(context);
        return {};
    }

    std::any visitSelected_name_part(vhdlParser::Selected_name_partContext *context) override {
        impl.visitSelected_name_part(context);
        return {};
    }

    std::any visitFunction_call_or_indexed_name_part(vhdlParser::Function_call_or_indexed_name_partContext *context) override {
        impl.visitFunction_call_or_indexed_name_part(context);
        return {};
    }

    std::any visitSlice_name_part(vhdlParser::Slice_name_partContext *context) override {
        impl.visitSlice_name_part(context);
        return {};
    }

    std::any visitAttribute_name_part(vhdlParser::Attribute_name_partContext *context) override {
        impl.visitAttribute_name_part(context);
        return {};
    }

    std::any visitNature_declaration(vhdlParser::Nature_declarationContext *context) override {
        impl.visitNature_declaration(context);
        return {};
    }

    std::any visitNature_definition(vhdlParser::Nature_definitionContext *context) override {
        impl.visitNature_definition(context);
        return {};
    }

    std::any visitNature_element_declaration(vhdlParser::Nature_element_declarationContext *context) override {
        impl.visitNature_element_declaration(context);
        return {};
    }

    std::any visitNext_statement(vhdlParser::Next_statementContext *context) override {
        impl.visitNext_statement(context);
        return {};
    }

    std::any visitNumeric_literal(vhdlParser::Numeric_literalContext *context) override {
        impl.visitNumeric_literal(context);
        return {};
    }

    std::any visitObject_declaration(vhdlParser::Object_declarationContext *context) override {
        impl.visitObject_declaration(context);
        return {};
    }

    std::any visitOpts(vhdlParser::OptsContext *context) override {
        impl.visitOpts(context);
        return {};
    }

    std::any visitPackage_body(vhdlParser::Package_bodyContext *context) override {
        impl.visitPackage_body(context);
        return {};
    }

    std::any visitPackage_body_declarative_item(vhdlParser::Package_body_declarative_itemContext *context) override {
        impl.visitPackage_body_declarative_item(context);
        return {};
    }

    std::any visitPackage_body_declarative_part(vhdlParser::Package_body_declarative_partContext *context) override {
        impl.visitPackage_body_declarative_part(context);
        return {};
    }

    std::any visitPackage_declaration(vhdlParser::Package_declarationContext *context) override {
        impl.visitPackage_declaration(context);
        return {};
    }

    std::any visitPackage_declarative_item(vhdlParser::Package_declarative_itemContext *context) override {
        impl.visitPackage_declarative_item(context);
        return {};
    }

    std::any visitPackage_declarative_part(vhdlParser::Package_declarative_partContext *context) override {
        impl.visitPackage_declarative_part(context);
        return {};
    }

    std::any visitParameter_specification(vhdlParser::Parameter_specificationContext *context) override {
        impl.visitParameter_specification(context);
        return {};
    }

    std::any visitPhysical_literal(vhdlParser::Physical_literalContext *context) override {
        impl.visitPhysical_literal(context);
        return {};
    }

    std::any visitPhysical_type_definition(vhdlParser::Physical_type_definitionContext *context) override {
        impl.visitPhysical_type_definition(context);
        return {};
    }

    std::any visitPort_clause(vhdlParser::Port_clauseContext *context) override {
        impl.visitPort_clause(context);
        return {};
    }

    std::any visitPort_list(vhdlParser::Port_listContext *context) override {
        impl.visitPort_list(context);
        return {};
    }

    std::any visitPort_map_aspect(vhdlParser::Port_map_aspectContext *context) override {
        impl.visitPort_map_aspect(context);
        return {};
    }

    std::any visitPrimary(vhdlParser::PrimaryContext *context) override {
        impl.visitPrimary(context);
        return {};
    }

    std::any visitPrimary_unit(vhdlParser::Primary_unitContext *context) override {
        impl.visitPrimary_unit(context);
        return {};
    }

    std::any visitProcedural_declarative_item(vhdlParser::Procedural_declarative_itemContext *context) override {
        impl.visitProcedural_declarative_item(context);
        return {};
    }

    std::any visitProcedural_declarative_part(vhdlParser::Procedural_declarative_partContext *context) override {
        impl.visitProcedural_declarative_part(context);
        return {};
    }

    std::any visitProcedural_statement_part(vhdlParser::Procedural_statement_partContext *context) override {
        impl.visitProcedural_statement_part(context);
        return {};
    }

    std::any visitProcedure_call(vhdlParser::Procedure_callContext *context) override {
        impl.visitProcedure_call(context);
        return {};
    }

    std::any visitProcedure_call_statement(vhdlParser::Procedure_call_statementContext *context) override {
        impl.visitProcedure_call_statement(context);
        return {};
    }

    std::any visitProcess_declarative_item(vhdlParser::Process_declarative_itemContext *context) override {
        impl.visitProcess_declarative_item(context);
        return {};
    }

    std::any visitProcess_declarative_part(vhdlParser::Process_declarative_partContext *context) override {
        impl.visitProcess_declarative_part(context);
        return {};
    }

    std::any visitProcess_statement(vhdlParser::Process_statementContext *context) override {
        impl.visitProcess_statement(context);
        return {};
    }

    std::any visitProcess_statement_part(vhdlParser::Process_statement_partContext *context) override {
        impl.visitProcess_statement_part(context);
        return {};
    }

    std::any visitQualified_expression(vhdlParser::Qualified_expressionContext *context) override {
        impl.visitQualified_expression(context);
        return {};
    }

    std::any visitQuantity_declaration(vhdlParser::Quantity_declarationContext *context) override {
        impl.visitQuantity_declaration(context);
        return {};
    }

    std::any visitQuantity_list(vhdlParser::Quantity_listContext *context) override {
        impl.visitQuantity_list(context);
        return {};
    }

    std::any visitQuantity_specification(vhdlParser::Quantity_specificationContext *context) override {
        impl.visitQuantity_specification(context);
        return {};
    }

    std::any visitRange_decl(vhdlParser::Range_declContext *context) override {
        impl.visitRange_decl(context);
        return {};
    }

    std::any visitExplicit_range(vhdlParser::Explicit_rangeContext *context) override {
        impl.visitExplicit_range(context);
        return {};
    }

    std::any visitRange_constraint(vhdlParser::Range_constraintContext *context) override {
        impl.visitRange_constraint(context);
        return {};
    }

    std::any visitRecord_nature_definition(vhdlParser::Record_nature_definitionContext *context) override {
        impl.visitRecord_nature_definition(context);
        return {};
    }

    std::any visitRecord_type_definition(vhdlParser::Record_type_definitionContext *context) override {
        impl.visitRecord_type_definition(context);
        return {};
    }

    std::any visitRelation(vhdlParser::RelationContext *context) override {
        impl.visitRelation(context);
        return {};
    }

    std::any visitRelational_operator(vhdlParser::Relational_operatorContext *context) override {
        impl.visitRelational_operator(context);
        return {};
    }

    std::any visitReport_statement(vhdlParser::Report_statementContext *context) override {
        impl.visitReport_statement(context);
        return {};
    }

    std::any visitReturn_statement(vhdlParser::Return_statementContext *context) override {
        impl.visitReturn_statement(context);
        return {};
    }

    std::any visitScalar_nature_definition(vhdlParser::Scalar_nature_definitionContext *context) override {
        impl.visitScalar_nature_definition(context);
        return {};
    }

    std::any visitScalar_type_definition(vhdlParser::Scalar_type_definitionContext *context) override {
        impl.visitScalar_type_definition(context);
        return {};
    }

    std::any visitSecondary_unit(vhdlParser::Secondary_unitContext *context) override {
        impl.visitSecondary_unit(context);
        return {};
    }

    std::any visitSecondary_unit_declaration(vhdlParser::Secondary_unit_declarationContext *context) override {
        impl.visitSecondary_unit_declaration(context);
        return {};
    }

    std::any visitSelected_signal_assignment(vhdlParser::Selected_signal_assignmentContext *context) override {
        impl.visitSelected_signal_assignment(context);
        return {};
    }

    std::any visitSelected_waveforms(vhdlParser::Selected_waveformsContext *context) override {
        impl.visitSelected_waveforms(context);
        return {};
    }

    std::any visitSensitivity_clause(vhdlParser::Sensitivity_clauseContext *context) override {
        impl.visitSensitivity_clause(context);
        return {};
    }

    std::any visitSensitivity_list(vhdlParser::Sensitivity_listContext *context) override {
        impl.visitSensitivity_list(context);
        return {};
    }

    std::any visitSequence_of_statements(vhdlParser::Sequence_of_statementsContext *context) override {
        impl.visitSequence_of_statements(context);
        return {};
    }

    std::any visitSequential_statement(vhdlParser::Sequential_statementContext *context) override {
        impl.visitSequential_statement(context);
        return {};
    }

    std::any visitShift_expression(vhdlParser::Shift_expressionContext *context) override {
        impl.visitShift_expression(context);
        return {};
    }

    std::any visitShift_operator(vhdlParser::Shift_operatorContext *context) override {
        impl.visitShift_operator(context);
        return {};
    }

    std::any visitSignal_assignment_statement(vhdlParser::Signal_assignment_statementContext *context) override {
        impl.visitSignal_assignment_statement(context);
        return {};
    }

    std::any visitSignal_declaration(vhdlParser::Signal_declarationContext *context) override {
        impl.visitSignal_declaration(context);
        return {};
    }

    std::any visitSignal_kind(vhdlParser::Signal_kindContext *context) override {
        impl.visitSignal_kind(context);
        return {};
    }

    std::any visitSignal_list(vhdlParser::Signal_listContext *context) override {
        impl.visitSignal_list(context);
        return {};
    }

    std::any visitSignature(vhdlParser::SignatureContext *context) override {
        impl.visitSignature(context);
        return {};
    }

    std::any visitSimple_expression(vhdlParser::Simple_expressionContext *context) override {
        impl.visitSimple_expression(context);
        return {};
    }

    std::any visitSimple_simultaneous_statement(vhdlParser::Simple_simultaneous_statementContext *context) override {
        impl.visitSimple_simultaneous_statement(context);
        return {};
    }

    std::any visitSimultaneous_alternative(vhdlParser::Simultaneous_alternativeContext *context) override {
        impl.visitSimultaneous_alternative(context);
        return {};
    }

    std::any visitSimultaneous_case_statement(vhdlParser::Simultaneous_case_statementContext *context) override {
        impl.visitSimultaneous_case_statement(context);
        return {};
    }

    std::any visitSimultaneous_if_statement(vhdlParser::Simultaneous_if_statementContext *context) override {
        impl.visitSimultaneous_if_statement(context);
        return {};
    }

    std::any visitSimultaneous_procedural_statement(vhdlParser::Simultaneous_procedural_statementContext *context) override {
        impl.visitSimultaneous_procedural_statement(context);
        return {};
    }

    std::any visitSimultaneous_statement(vhdlParser::Simultaneous_statementContext *context) override {
        impl.visitSimultaneous_statement(context);
        return {};
    }

    std::any visitSimultaneous_statement_part(vhdlParser::Simultaneous_statement_partContext *context) override {
        impl.visitSimultaneous_statement_part(context);
        return {};
    }

    std::any visitSource_aspect(vhdlParser::Source_aspectContext *context) override {
        impl.visitSource_aspect(context);
        return {};
    }

    std::any visitSource_quantity_declaration(vhdlParser::Source_quantity_declarationContext *context) override {
        impl.visitSource_quantity_declaration(context);
        return {};
    }

    std::any visitStep_limit_specification(vhdlParser::Step_limit_specificationContext *context) override {
        impl.visitStep_limit_specification(context);
        return {};
    }

    std::any visitSubnature_declaration(vhdlParser::Subnature_declarationContext *context) override {
        impl.visitSubnature_declaration(context);
        return {};
    }

    std::any visitSubnature_indication(vhdlParser::Subnature_indicationContext *context) override {
        impl.visitSubnature_indication(context);
        return {};
    }

    std::any visitSubprogram_body(vhdlParser::Subprogram_bodyContext *context) override {
        impl.visitSubprogram_body(context);
        return {};
    }

    std::any visitSubprogram_declaration(vhdlParser::Subprogram_declarationContext *context) override {
        impl.visitSubprogram_declaration(context);
        return {};
    }

    std::any visitSubprogram_declarative_item(vhdlParser::Subprogram_declarative_itemContext *context) override {
        impl.visitSubprogram_declarative_item(context);
        return {};
    }

    std::any visitSubprogram_declarative_part(vhdlParser::Subprogram_declarative_partContext *context) override {
        impl.visitSubprogram_declarative_part(context);
        return {};
    }

    std::any visitSubprogram_kind(vhdlParser::Subprogram_kindContext *context) override {
        impl.visitSubprogram_kind(context);
        return {};
    }

    std::any visitSubprogram_specification(vhdlParser::Subprogram_specificationContext *context) override {
        impl.visitSubprogram_specification(context);
        return {};
    }

    std::any visitProcedure_specification(vhdlParser::Procedure_specificationContext *context) override {
        impl.visitProcedure_specification(context);
        return {};
    }

    std::any visitFunction_specification(vhdlParser::Function_specificationContext *context) override {
        impl.visitFunction_specification(context);
        return {};
    }

    std::any visitSubprogram_statement_part(vhdlParser::Subprogram_statement_partContext *context) override {
        impl.visitSubprogram_statement_part(context);
        return {};
    }

    std::any visitSubtype_declaration(vhdlParser::Subtype_declarationContext *context) override {
        impl.visitSubtype_declaration(context);
        return {};
    }

    std::any visitSubtype_indication(vhdlParser::Subtype_indicationContext *context) override {
        impl.visitSubtype_indication(context);
        return {};
    }

    std::any visitSuffix(vhdlParser::SuffixContext *context) override {
        impl.visitSuffix(context);
        return {};
    }

    std::any visitTarget(vhdlParser::TargetContext *context) override {
        impl.visitTarget(context);
        return {};
    }

    std::any visitTerm(vhdlParser::TermContext *context) override {
        impl.visitTerm(context);
        return {};
    }

    std::any visitTerminal_aspect(vhdlParser::Terminal_aspectContext *context) override {
        impl.visitTerminal_aspect(context);
        return {};
    }

    std::any visitTerminal_declaration(vhdlParser::Terminal_declarationContext *context) override {
        impl.visitTerminal_declaration(context);
        return {};
    }

    std::any visitThrough_aspect(vhdlParser::Through_aspectContext *context) override {
        impl.visitThrough_aspect(context);
        return {};
    }

    std::any visitTimeout_clause(vhdlParser::Timeout_clauseContext *context) override {
        impl.visitTimeout_clause(context);
        return {};
    }

    std::any visitTolerance_aspect(vhdlParser::Tolerance_aspectContext *context) override {
        impl.visitTolerance_aspect(context);
        return {};
    }

    std::any visitType_declaration(vhdlParser::Type_declarationContext *context) override {
        impl.visitType_declaration(context);
        return {};
    }

    std::any visitType_definition(vhdlParser::Type_definitionContext *context) override {
        impl.visitType_definition(context);
        return {};
    }

    std::any visitUnconstrained_array_definition(vhdlParser::Unconstrained_array_definitionContext *context) override {
        impl.visitUnconstrained_array_definition(context);
        return {};
    }

    std::any visitUnconstrained_nature_definition(vhdlParser::Unconstrained_nature_definitionContext *context) override {
        impl.visitUnconstrained_nature_definition(context);
        return {};
    }

    std::any visitUse_clause(vhdlParser::Use_clauseContext *context) override {
        impl.visitUse_clause(context);
        return {};
    }

    std::any visitVariable_assignment_statement(vhdlParser::Variable_assignment_statementContext *context) override {
        impl.visitVariable_assignment_statement(context);
        return {};
    }

    std::any visitVariable_declaration(vhdlParser::Variable_declarationContext *context) override {
        impl.visitVariable_declaration(context);
        return {};
    }

    std::any visitWait_statement(vhdlParser::Wait_statementContext *context) override {
        impl.visitWait_statement(context);
        return {};
    }

    std::any visitWaveform(vhdlParser::WaveformContext *context) override {
        impl.visitWaveform(context);
        return {};
    }

    std::any visitWaveform_element(vhdlParser::Waveform_elementContext *context) override {
        impl.visitWaveform_element(context);
        return {};
    }

};
} // namespace builder::adapter
