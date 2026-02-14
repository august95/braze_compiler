#include "../../source/pch.h"
#include "../codeGenerator.h"
#include "../../braze_compiler.h"
#include "../../codegen/resolver/resolverResult.h"

codeGenerator::codeGenerator()
  : m_codegen_expression(m_asm_writer, m_resolver, this),
  m_codegen_statement(m_asm_writer, m_resolver,&m_codegen_expression, &m_codegen_global_declaration, this),
  m_codegen_global_declaration(m_asm_writer, m_resolver, &m_codegen_statement, &m_codegen_expression, this)
{
}

void codeGenerator::initialize()
{
  int error = m_asm_writer.initialize(output_file);

  m_resolver.initialize();

  if (error != 0)
  {
    cerror("failed to open file");
  }
}

void codeGenerator::setFileName(std::string filename, asmWriter::WriteMode write_mode)
{
  m_input_file = filename;
  output_file = filename + ".asm";
  m_asm_writer.setWriteMode(write_mode);
}

int codeGenerator::startCodeGeneration()
{

  initialize();

  m_root_scope.init(std::make_shared<scope>(), true);

  generateDataSection();

  generateTextSection();

  generateReadOnlySection();

  m_asm_writer.close();

  return 0;
}

void codeGenerator::generateDataSection()
{
  m_asm_writer.asmGen("section .data");

  for (auto node : m_ast)
  {
    if (node->getNodeType() & NODE_TYPE_VARIABLE_DECLARATION)
    {
      m_codegen_global_declaration.generateGlobalVariable(cast_node<nodeVariableDeclaration>(node));
    }
  }
}

void codeGenerator::generateTextSection()
{
  m_asm_writer.asmGen("section .text");

  for (auto node : m_ast)
  {
    if (node->getNodeType() == NODE_TYPE_VARIABLE)
    {
      // Global variables already processed in generateDataSection
    }
    if (node->getNodeType() & NODE_TYPE_FUNCTION_DECLARATION)
    {
      m_codegen_global_declaration.generateFunctionDeclaration(cast_node<nodeFunctionDeclaration>(node));
    }
  }
}

void codeGenerator::generateReadOnlySection()
{
  m_asm_writer.asmGen("section .rodata");
  m_codegen_expression.generateWriteStrings();
}

int codeGenerator::generateLableCount(bool reset)
{
  static int count = 0;
  count++;

  // static counter lives during all unit tests, needs to be cleared after each tests
  if (reset)
  {
    count = 0;
  }
  return count;
}
