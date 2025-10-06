#pragma once
#include "tree/ParseTree.h"
#include "antlr_void_adapter.hpp"
#include "void_visitor.hpp"

namespace builder::adapter {
class BaseVoidVisitor : public VoidVisitor {
protected:
    void dispatch(antlr4::tree::ParseTree *node) {
        if (!node) { 
            return;
        }
        AntlrVoidAdapter adapter(*this);
        node->accept(&adapter);
    }

    void walk(antlr4::tree::ParseTree *node) {
        if (!node) { 
            return;
        }
        AntlrVoidAdapter adapter(*this);
        for (auto *child : node->children) {
            if (child) { 
                child->accept(&adapter);
            }
        }
    }

public:
    void visitAbstract_literal(vhdlParser::Abstract_literalContext *context) override { walk(context); }
    void visitAccess_type_definition(vhdlParser::Access_type_definitionContext *context) override { walk(context); }
    void visitAcross_aspect(vhdlParser::Across_aspectContext *context) override { walk(context); }
    void visitActual_designator(vhdlParser::Actual_designatorContext *context) override { walk(context); }
    void visitActual_parameter_part(vhdlParser::Actual_parameter_partContext *context) override { walk(context); }
    void visitActual_part(vhdlParser::Actual_partContext *context) override { walk(context); }
    void visitAdding_operator(vhdlParser::Adding_operatorContext *context) override { walk(context); }
    void visitAggregate(vhdlParser::AggregateContext *context) override { walk(context); }
    void visitAlias_declaration(vhdlParser::Alias_declarationContext *context) override { walk(context); }
    void visitAlias_designator(vhdlParser::Alias_designatorContext *context) override { walk(context); }
    void visitAlias_indication(vhdlParser::Alias_indicationContext *context) override { walk(context); }
    void visitAllocator(vhdlParser::AllocatorContext *context) override { walk(context); }
    void visitArchitecture_body(vhdlParser::Architecture_bodyContext *context) override { walk(context); }
    void visitArchitecture_declarative_part(vhdlParser::Architecture_declarative_partContext *context) override { walk(context); }
    void visitArchitecture_statement(vhdlParser::Architecture_statementContext *context) override { walk(context); }
    void visitArchitecture_statement_part(vhdlParser::Architecture_statement_partContext *context) override { walk(context); }
    void visitArray_nature_definition(vhdlParser::Array_nature_definitionContext *context) override { walk(context); }
    void visitArray_type_definition(vhdlParser::Array_type_definitionContext *context) override { walk(context); }
    void visitAssertion(vhdlParser::AssertionContext *context) override { walk(context); }
    void visitAssertion_statement(vhdlParser::Assertion_statementContext *context) override { walk(context); }
    void visitAssociation_element(vhdlParser::Association_elementContext *context) override { walk(context); }
    void visitAssociation_list(vhdlParser::Association_listContext *context) override { walk(context); }
    void visitAttribute_declaration(vhdlParser::Attribute_declarationContext *context) override { walk(context); }
    void visitAttribute_designator(vhdlParser::Attribute_designatorContext *context) override { walk(context); }
    void visitAttribute_specification(vhdlParser::Attribute_specificationContext *context) override { walk(context); }
    void visitBase_unit_declaration(vhdlParser::Base_unit_declarationContext *context) override { walk(context); }
    void visitBinding_indication(vhdlParser::Binding_indicationContext *context) override { walk(context); }
    void visitBlock_configuration(vhdlParser::Block_configurationContext *context) override { walk(context); }
    void visitBlock_declarative_item(vhdlParser::Block_declarative_itemContext *context) override { walk(context); }
    void visitBlock_declarative_part(vhdlParser::Block_declarative_partContext *context) override { walk(context); }
    void visitBlock_header(vhdlParser::Block_headerContext *context) override { walk(context); }
    void visitBlock_specification(vhdlParser::Block_specificationContext *context) override { walk(context); }
    void visitBlock_statement(vhdlParser::Block_statementContext *context) override { walk(context); }
    void visitBlock_statement_part(vhdlParser::Block_statement_partContext *context) override { walk(context); }
    void visitBranch_quantity_declaration(vhdlParser::Branch_quantity_declarationContext *context) override { walk(context); }
    void visitBreak_element(vhdlParser::Break_elementContext *context) override { walk(context); }
    void visitBreak_list(vhdlParser::Break_listContext *context) override { walk(context); }
    void visitBreak_selector_clause(vhdlParser::Break_selector_clauseContext *context) override { walk(context); }
    void visitBreak_statement(vhdlParser::Break_statementContext *context) override { walk(context); }
    void visitCase_statement(vhdlParser::Case_statementContext *context) override { walk(context); }
    void visitCase_statement_alternative(vhdlParser::Case_statement_alternativeContext *context) override { walk(context); }
    void visitChoice(vhdlParser::ChoiceContext *context) override { walk(context); }
    void visitChoices(vhdlParser::ChoicesContext *context) override { walk(context); }
    void visitComponent_configuration(vhdlParser::Component_configurationContext *context) override { walk(context); }
    void visitComponent_declaration(vhdlParser::Component_declarationContext *context) override { walk(context); }
    void visitComponent_instantiation_statement(vhdlParser::Component_instantiation_statementContext *context) override { walk(context); }
    void visitComponent_specification(vhdlParser::Component_specificationContext *context) override { walk(context); }
    void visitComposite_nature_definition(vhdlParser::Composite_nature_definitionContext *context) override { walk(context); }
    void visitComposite_type_definition(vhdlParser::Composite_type_definitionContext *context) override { walk(context); }
    void visitConcurrent_assertion_statement(vhdlParser::Concurrent_assertion_statementContext *context) override { walk(context); }
    void visitConcurrent_break_statement(vhdlParser::Concurrent_break_statementContext *context) override { walk(context); }
    void visitConcurrent_procedure_call_statement(vhdlParser::Concurrent_procedure_call_statementContext *context) override { walk(context); }
    void visitConcurrent_signal_assignment_statement(vhdlParser::Concurrent_signal_assignment_statementContext *context) override { walk(context); }
    void visitCondition(vhdlParser::ConditionContext *context) override { walk(context); }
    void visitCondition_clause(vhdlParser::Condition_clauseContext *context) override { walk(context); }
    void visitConditional_signal_assignment(vhdlParser::Conditional_signal_assignmentContext *context) override { walk(context); }
    void visitConditional_waveforms(vhdlParser::Conditional_waveformsContext *context) override { walk(context); }
    void visitConfiguration_declaration(vhdlParser::Configuration_declarationContext *context) override { walk(context); }
    void visitConfiguration_declarative_item(vhdlParser::Configuration_declarative_itemContext *context) override { walk(context); }
    void visitConfiguration_declarative_part(vhdlParser::Configuration_declarative_partContext *context) override { walk(context); }
    void visitConfiguration_item(vhdlParser::Configuration_itemContext *context) override { walk(context); }
    void visitConfiguration_specification(vhdlParser::Configuration_specificationContext *context) override { walk(context); }
    void visitConstant_declaration(vhdlParser::Constant_declarationContext *context) override { walk(context); }
    void visitConstrained_array_definition(vhdlParser::Constrained_array_definitionContext *context) override { walk(context); }
    void visitConstrained_nature_definition(vhdlParser::Constrained_nature_definitionContext *context) override { walk(context); }
    void visitConstraint(vhdlParser::ConstraintContext *context) override { walk(context); }
    void visitContext_clause(vhdlParser::Context_clauseContext *context) override { walk(context); }
    void visitContext_item(vhdlParser::Context_itemContext *context) override { walk(context); }
    void visitDelay_mechanism(vhdlParser::Delay_mechanismContext *context) override { walk(context); }
    void visitDesign_file(vhdlParser::Design_fileContext *context) override { walk(context); }
    void visitDesign_unit(vhdlParser::Design_unitContext *context) override { walk(context); }
    void visitDesignator(vhdlParser::DesignatorContext *context) override { walk(context); }
    void visitDirection(vhdlParser::DirectionContext *context) override { walk(context); }
    void visitDisconnection_specification(vhdlParser::Disconnection_specificationContext *context) override { walk(context); }
    void visitDiscrete_range(vhdlParser::Discrete_rangeContext *context) override { walk(context); }
    void visitElement_association(vhdlParser::Element_associationContext *context) override { walk(context); }
    void visitElement_declaration(vhdlParser::Element_declarationContext *context) override { walk(context); }
    void visitElement_subnature_definition(vhdlParser::Element_subnature_definitionContext *context) override { walk(context); }
    void visitElement_subtype_definition(vhdlParser::Element_subtype_definitionContext *context) override { walk(context); }
    void visitEntity_aspect(vhdlParser::Entity_aspectContext *context) override { walk(context); }
    void visitEntity_class(vhdlParser::Entity_classContext *context) override { walk(context); }
    void visitEntity_class_entry(vhdlParser::Entity_class_entryContext *context) override { walk(context); }
    void visitEntity_class_entry_list(vhdlParser::Entity_class_entry_listContext *context) override { walk(context); }
    void visitEntity_declaration(vhdlParser::Entity_declarationContext *context) override { walk(context); }
    void visitEntity_declarative_item(vhdlParser::Entity_declarative_itemContext *context) override { walk(context); }
    void visitEntity_declarative_part(vhdlParser::Entity_declarative_partContext *context) override { walk(context); }
    void visitEntity_designator(vhdlParser::Entity_designatorContext *context) override { walk(context); }
    void visitEntity_header(vhdlParser::Entity_headerContext *context) override { walk(context); }
    void visitEntity_name_list(vhdlParser::Entity_name_listContext *context) override { walk(context); }
    void visitEntity_specification(vhdlParser::Entity_specificationContext *context) override { walk(context); }
    void visitEntity_statement(vhdlParser::Entity_statementContext *context) override { walk(context); }
    void visitEntity_statement_part(vhdlParser::Entity_statement_partContext *context) override { walk(context); }
    void visitEntity_tag(vhdlParser::Entity_tagContext *context) override { walk(context); }
    void visitEnumeration_literal(vhdlParser::Enumeration_literalContext *context) override { walk(context); }
    void visitEnumeration_type_definition(vhdlParser::Enumeration_type_definitionContext *context) override { walk(context); }
    void visitExit_statement(vhdlParser::Exit_statementContext *context) override { walk(context); }
    void visitExpression(vhdlParser::ExpressionContext *context) override { walk(context); }
    void visitFactor(vhdlParser::FactorContext *context) override { walk(context); }
    void visitFile_declaration(vhdlParser::File_declarationContext *context) override { walk(context); }
    void visitFile_logical_name(vhdlParser::File_logical_nameContext *context) override { walk(context); }
    void visitFile_open_information(vhdlParser::File_open_informationContext *context) override { walk(context); }
    void visitFile_type_definition(vhdlParser::File_type_definitionContext *context) override { walk(context); }
    void visitFormal_parameter_list(vhdlParser::Formal_parameter_listContext *context) override { walk(context); }
    void visitFormal_part(vhdlParser::Formal_partContext *context) override { walk(context); }
    void visitFree_quantity_declaration(vhdlParser::Free_quantity_declarationContext *context) override { walk(context); }
    void visitGenerate_statement(vhdlParser::Generate_statementContext *context) override { walk(context); }
    void visitGeneration_scheme(vhdlParser::Generation_schemeContext *context) override { walk(context); }
    void visitGeneric_clause(vhdlParser::Generic_clauseContext *context) override { walk(context); }
    void visitGeneric_list(vhdlParser::Generic_listContext *context) override { walk(context); }
    void visitGeneric_map_aspect(vhdlParser::Generic_map_aspectContext *context) override { walk(context); }
    void visitGroup_constituent(vhdlParser::Group_constituentContext *context) override { walk(context); }
    void visitGroup_constituent_list(vhdlParser::Group_constituent_listContext *context) override { walk(context); }
    void visitGroup_declaration(vhdlParser::Group_declarationContext *context) override { walk(context); }
    void visitGroup_template_declaration(vhdlParser::Group_template_declarationContext *context) override { walk(context); }
    void visitGuarded_signal_specification(vhdlParser::Guarded_signal_specificationContext *context) override { walk(context); }
    void visitIdentifier(vhdlParser::IdentifierContext *context) override { walk(context); }
    void visitIdentifier_list(vhdlParser::Identifier_listContext *context) override { walk(context); }
    void visitIf_statement(vhdlParser::If_statementContext *context) override { walk(context); }
    void visitIndex_constraint(vhdlParser::Index_constraintContext *context) override { walk(context); }
    void visitIndex_specification(vhdlParser::Index_specificationContext *context) override { walk(context); }
    void visitIndex_subtype_definition(vhdlParser::Index_subtype_definitionContext *context) override { walk(context); }
    void visitInstantiated_unit(vhdlParser::Instantiated_unitContext *context) override { walk(context); }
    void visitInstantiation_list(vhdlParser::Instantiation_listContext *context) override { walk(context); }
    void visitInterface_constant_declaration(vhdlParser::Interface_constant_declarationContext *context) override { walk(context); }
    void visitInterface_declaration(vhdlParser::Interface_declarationContext *context) override { walk(context); }
    void visitInterface_element(vhdlParser::Interface_elementContext *context) override { walk(context); }
    void visitInterface_file_declaration(vhdlParser::Interface_file_declarationContext *context) override { walk(context); }
    void visitInterface_signal_list(vhdlParser::Interface_signal_listContext *context) override { walk(context); }
    void visitInterface_port_list(vhdlParser::Interface_port_listContext *context) override { walk(context); }
    void visitInterface_list(vhdlParser::Interface_listContext *context) override { walk(context); }
    void visitInterface_quantity_declaration(vhdlParser::Interface_quantity_declarationContext *context) override { walk(context); }
    void visitInterface_port_declaration(vhdlParser::Interface_port_declarationContext *context) override { walk(context); }
    void visitInterface_signal_declaration(vhdlParser::Interface_signal_declarationContext *context) override { walk(context); }
    void visitInterface_terminal_declaration(vhdlParser::Interface_terminal_declarationContext *context) override { walk(context); }
    void visitInterface_variable_declaration(vhdlParser::Interface_variable_declarationContext *context) override { walk(context); }
    void visitIteration_scheme(vhdlParser::Iteration_schemeContext *context) override { walk(context); }
    void visitLabel_colon(vhdlParser::Label_colonContext *context) override { walk(context); }
    void visitLibrary_clause(vhdlParser::Library_clauseContext *context) override { walk(context); }
    void visitLibrary_unit(vhdlParser::Library_unitContext *context) override { walk(context); }
    void visitLiteral(vhdlParser::LiteralContext *context) override { walk(context); }
    void visitLogical_name(vhdlParser::Logical_nameContext *context) override { walk(context); }
    void visitLogical_name_list(vhdlParser::Logical_name_listContext *context) override { walk(context); }
    void visitLogical_operator(vhdlParser::Logical_operatorContext *context) override { walk(context); }
    void visitLoop_statement(vhdlParser::Loop_statementContext *context) override { walk(context); }
    void visitSignal_mode(vhdlParser::Signal_modeContext *context) override { walk(context); }
    void visitMultiplying_operator(vhdlParser::Multiplying_operatorContext *context) override { walk(context); }
    void visitName(vhdlParser::NameContext *context) override { walk(context); }
    void visitName_part(vhdlParser::Name_partContext *context) override { walk(context); }
    void visitSelected_name(vhdlParser::Selected_nameContext *context) override { walk(context); }
    void visitSelected_name_part(vhdlParser::Selected_name_partContext *context) override { walk(context); }
    void visitFunction_call_or_indexed_name_part(vhdlParser::Function_call_or_indexed_name_partContext *context) override { walk(context); }
    void visitSlice_name_part(vhdlParser::Slice_name_partContext *context) override { walk(context); }
    void visitAttribute_name_part(vhdlParser::Attribute_name_partContext *context) override { walk(context); }
    void visitNature_declaration(vhdlParser::Nature_declarationContext *context) override { walk(context); }
    void visitNature_definition(vhdlParser::Nature_definitionContext *context) override { walk(context); }
    void visitNature_element_declaration(vhdlParser::Nature_element_declarationContext *context) override { walk(context); }
    void visitNext_statement(vhdlParser::Next_statementContext *context) override { walk(context); }
    void visitNumeric_literal(vhdlParser::Numeric_literalContext *context) override { walk(context); }
    void visitObject_declaration(vhdlParser::Object_declarationContext *context) override { walk(context); }
    void visitOpts(vhdlParser::OptsContext *context) override { walk(context); }
    void visitPackage_body(vhdlParser::Package_bodyContext *context) override { walk(context); }
    void visitPackage_body_declarative_item(vhdlParser::Package_body_declarative_itemContext *context) override { walk(context); }
    void visitPackage_body_declarative_part(vhdlParser::Package_body_declarative_partContext *context) override { walk(context); }
    void visitPackage_declaration(vhdlParser::Package_declarationContext *context) override { walk(context); }
    void visitPackage_declarative_item(vhdlParser::Package_declarative_itemContext *context) override { walk(context); }
    void visitPackage_declarative_part(vhdlParser::Package_declarative_partContext *context) override { walk(context); }
    void visitParameter_specification(vhdlParser::Parameter_specificationContext *context) override { walk(context); }
    void visitPhysical_literal(vhdlParser::Physical_literalContext *context) override { walk(context); }
    void visitPhysical_type_definition(vhdlParser::Physical_type_definitionContext *context) override { walk(context); }
    void visitPort_clause(vhdlParser::Port_clauseContext *context) override { walk(context); }
    void visitPort_list(vhdlParser::Port_listContext *context) override { walk(context); }
    void visitPort_map_aspect(vhdlParser::Port_map_aspectContext *context) override { walk(context); }
    void visitPrimary(vhdlParser::PrimaryContext *context) override { walk(context); }
    void visitPrimary_unit(vhdlParser::Primary_unitContext *context) override { walk(context); }
    void visitProcedural_declarative_item(vhdlParser::Procedural_declarative_itemContext *context) override { walk(context); }
    void visitProcedural_declarative_part(vhdlParser::Procedural_declarative_partContext *context) override { walk(context); }
    void visitProcedural_statement_part(vhdlParser::Procedural_statement_partContext *context) override { walk(context); }
    void visitProcedure_call(vhdlParser::Procedure_callContext *context) override { walk(context); }
    void visitProcedure_call_statement(vhdlParser::Procedure_call_statementContext *context) override { walk(context); }
    void visitProcess_declarative_item(vhdlParser::Process_declarative_itemContext *context) override { walk(context); }
    void visitProcess_declarative_part(vhdlParser::Process_declarative_partContext *context) override { walk(context); }
    void visitProcess_statement(vhdlParser::Process_statementContext *context) override { walk(context); }
    void visitProcess_statement_part(vhdlParser::Process_statement_partContext *context) override { walk(context); }
    void visitQualified_expression(vhdlParser::Qualified_expressionContext *context) override { walk(context); }
    void visitQuantity_declaration(vhdlParser::Quantity_declarationContext *context) override { walk(context); }
    void visitQuantity_list(vhdlParser::Quantity_listContext *context) override { walk(context); }
    void visitQuantity_specification(vhdlParser::Quantity_specificationContext *context) override { walk(context); }
    void visitRange_decl(vhdlParser::Range_declContext *context) override { walk(context); }
    void visitExplicit_range(vhdlParser::Explicit_rangeContext *context) override { walk(context); }
    void visitRange_constraint(vhdlParser::Range_constraintContext *context) override { walk(context); }
    void visitRecord_nature_definition(vhdlParser::Record_nature_definitionContext *context) override { walk(context); }
    void visitRecord_type_definition(vhdlParser::Record_type_definitionContext *context) override { walk(context); }
    void visitRelation(vhdlParser::RelationContext *context) override { walk(context); }
    void visitRelational_operator(vhdlParser::Relational_operatorContext *context) override { walk(context); }
    void visitReport_statement(vhdlParser::Report_statementContext *context) override { walk(context); }
    void visitReturn_statement(vhdlParser::Return_statementContext *context) override { walk(context); }
    void visitScalar_nature_definition(vhdlParser::Scalar_nature_definitionContext *context) override { walk(context); }
    void visitScalar_type_definition(vhdlParser::Scalar_type_definitionContext *context) override { walk(context); }
    void visitSecondary_unit(vhdlParser::Secondary_unitContext *context) override { walk(context); }
    void visitSecondary_unit_declaration(vhdlParser::Secondary_unit_declarationContext *context) override { walk(context); }
    void visitSelected_signal_assignment(vhdlParser::Selected_signal_assignmentContext *context) override { walk(context); }
    void visitSelected_waveforms(vhdlParser::Selected_waveformsContext *context) override { walk(context); }
    void visitSensitivity_clause(vhdlParser::Sensitivity_clauseContext *context) override { walk(context); }
    void visitSensitivity_list(vhdlParser::Sensitivity_listContext *context) override { walk(context); }
    void visitSequence_of_statements(vhdlParser::Sequence_of_statementsContext *context) override { walk(context); }
    void visitSequential_statement(vhdlParser::Sequential_statementContext *context) override { walk(context); }
    void visitShift_expression(vhdlParser::Shift_expressionContext *context) override { walk(context); }
    void visitShift_operator(vhdlParser::Shift_operatorContext *context) override { walk(context); }
    void visitSignal_assignment_statement(vhdlParser::Signal_assignment_statementContext *context) override { walk(context); }
    void visitSignal_declaration(vhdlParser::Signal_declarationContext *context) override { walk(context); }
    void visitSignal_kind(vhdlParser::Signal_kindContext *context) override { walk(context); }
    void visitSignal_list(vhdlParser::Signal_listContext *context) override { walk(context); }
    void visitSignature(vhdlParser::SignatureContext *context) override { walk(context); }
    void visitSimple_expression(vhdlParser::Simple_expressionContext *context) override { walk(context); }
    void visitSimple_simultaneous_statement(vhdlParser::Simple_simultaneous_statementContext *context) override { walk(context); }
    void visitSimultaneous_alternative(vhdlParser::Simultaneous_alternativeContext *context) override { walk(context); }
    void visitSimultaneous_case_statement(vhdlParser::Simultaneous_case_statementContext *context) override { walk(context); }
    void visitSimultaneous_if_statement(vhdlParser::Simultaneous_if_statementContext *context) override { walk(context); }
    void visitSimultaneous_procedural_statement(vhdlParser::Simultaneous_procedural_statementContext *context) override { walk(context); }
    void visitSimultaneous_statement(vhdlParser::Simultaneous_statementContext *context) override { walk(context); }
    void visitSimultaneous_statement_part(vhdlParser::Simultaneous_statement_partContext *context) override { walk(context); }
    void visitSource_aspect(vhdlParser::Source_aspectContext *context) override { walk(context); }
    void visitSource_quantity_declaration(vhdlParser::Source_quantity_declarationContext *context) override { walk(context); }
    void visitStep_limit_specification(vhdlParser::Step_limit_specificationContext *context) override { walk(context); }
    void visitSubnature_declaration(vhdlParser::Subnature_declarationContext *context) override { walk(context); }
    void visitSubnature_indication(vhdlParser::Subnature_indicationContext *context) override { walk(context); }
    void visitSubprogram_body(vhdlParser::Subprogram_bodyContext *context) override { walk(context); }
    void visitSubprogram_declaration(vhdlParser::Subprogram_declarationContext *context) override { walk(context); }
    void visitSubprogram_declarative_item(vhdlParser::Subprogram_declarative_itemContext *context) override { walk(context); }
    void visitSubprogram_declarative_part(vhdlParser::Subprogram_declarative_partContext *context) override { walk(context); }
    void visitSubprogram_kind(vhdlParser::Subprogram_kindContext *context) override { walk(context); }
    void visitSubprogram_specification(vhdlParser::Subprogram_specificationContext *context) override { walk(context); }
    void visitProcedure_specification(vhdlParser::Procedure_specificationContext *context) override { walk(context); }
    void visitFunction_specification(vhdlParser::Function_specificationContext *context) override { walk(context); }
    void visitSubprogram_statement_part(vhdlParser::Subprogram_statement_partContext *context) override { walk(context); }
    void visitSubtype_declaration(vhdlParser::Subtype_declarationContext *context) override { walk(context); }
    void visitSubtype_indication(vhdlParser::Subtype_indicationContext *context) override { walk(context); }
    void visitSuffix(vhdlParser::SuffixContext *context) override { walk(context); }
    void visitTarget(vhdlParser::TargetContext *context) override { walk(context); }
    void visitTerm(vhdlParser::TermContext *context) override { walk(context); }
    void visitTerminal_aspect(vhdlParser::Terminal_aspectContext *context) override { walk(context); }
    void visitTerminal_declaration(vhdlParser::Terminal_declarationContext *context) override { walk(context); }
    void visitThrough_aspect(vhdlParser::Through_aspectContext *context) override { walk(context); }
    void visitTimeout_clause(vhdlParser::Timeout_clauseContext *context) override { walk(context); }
    void visitTolerance_aspect(vhdlParser::Tolerance_aspectContext *context) override { walk(context); }
    void visitType_declaration(vhdlParser::Type_declarationContext *context) override { walk(context); }
    void visitType_definition(vhdlParser::Type_definitionContext *context) override { walk(context); }
    void visitUnconstrained_array_definition(vhdlParser::Unconstrained_array_definitionContext *context) override { walk(context); }
    void visitUnconstrained_nature_definition(vhdlParser::Unconstrained_nature_definitionContext *context) override { walk(context); }
    void visitUse_clause(vhdlParser::Use_clauseContext *context) override { walk(context); }
    void visitVariable_assignment_statement(vhdlParser::Variable_assignment_statementContext *context) override { walk(context); }
    void visitVariable_declaration(vhdlParser::Variable_declarationContext *context) override { walk(context); }
    void visitWait_statement(vhdlParser::Wait_statementContext *context) override { walk(context); }
    void visitWaveform(vhdlParser::WaveformContext *context) override { walk(context); }
    void visitWaveform_element(vhdlParser::Waveform_elementContext *context) override { walk(context); }
};
} // namespace builder::adapter
