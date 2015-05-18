/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/12/2015
 *              Modified, 5/12/2015
 * \ingroup     CST320 - Lab2a
 * \file        syntaxanalyzer.cpp
 *
 * \brief       Defines the structure of the SyntaxAnalyzer class.
 */
#include "syntaxanalyzer.h"
#include "tokenlist.h"
#include "token.h"

TokenIterator::TokenIterator(TokenList &tokens)
    :m_CurrentIndex(0), m_HasError(false), m_TokenList(tokens)
{
}

Token* TokenIterator::operator*()
{
    return m_TokenList[m_CurrentIndex];
}

Token* TokenIterator::current()
{
    return m_TokenList[m_CurrentIndex];
}

bool TokenIterator::acceptType(std::string type)
{
    if (m_CurrentIndex >= m_TokenList.length())
        return false;

    if (type == m_TokenList[m_CurrentIndex]->type()) {
        m_CurrentIndex++;
        return true;
    }
    else
        return false;
}

bool TokenIterator::expectType(std::string type)
{
    if (acceptType(type))
        return true;
    m_HasError = true;
    return false;
}

bool TokenIterator::hasError()
{
    // Unset error flag and return true
    if (m_HasError) {
        m_HasError = false;
        return true;
    }

    return false;
}

/*!
 * \brief Creates a SyntaxAnalyzer object, maintaining reference to \p
 *        symbolTable.
 * \param symbolTable   A reference to an instantiated symbol table.
 */
SyntaxAnalyzer::SyntaxAnalyzer(SymbolTable &symbolTable, TokenList& tokenList)
    :m_SymbolTable(symbolTable), m_Iter(tokenList)
{
}

bool SyntaxAnalyzer::parse()
{
    return program();
}

bool SyntaxAnalyzer::program()
{
    definition();
    while(definition());
    return m_Iter.hasError();
}

bool SyntaxAnalyzer::definition()
{
    if (m_Iter.acceptType("int")) {
        if (m_Iter.expectType("ID")) {
            while (m_Iter.acceptType(","))
                m_Iter.expectType("ID");
        }
        m_Iter.expectType(";");
    }
    else
    {
        m_Iter.acceptType("int");
        functionHeader();
        functionBody();
    }

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::dataDefinition()
{
    if (m_Iter.expectType("int")) {
        if (m_Iter.expectType("ID")) {
            while (m_Iter.acceptType(","))
                m_Iter.expectType("ID");
        }
        m_Iter.expectType(";");
    }

    return m_Iter.hasError();
}


bool SyntaxAnalyzer::functionDefinition()
{
    m_Iter.acceptType("int");
    functionHeader();
    functionBody();

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::functionHeader()
{
    if (m_Iter.expectType("int"))
        parameterList();

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::parameterList()
{
    if (m_Iter.expectType("(")) {
        identifierList();
        if (m_Iter.expectType(")"))
            while (parameterDeclaration());
    }

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::identifierList()
{
    do {
        m_Iter.expectType("ID");
    } while (m_Iter.acceptType(","));

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::parameterDeclaration()
{
    if (m_Iter.expectType("int")) {
        do {
            m_Iter.expectType("ID");
        } while (m_Iter.acceptType(","));
        m_Iter.expectType(";");
    }

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::functionBody()
{
    if (m_Iter.expectType("{")) {
        while (dataDefinition());
        while (statement());
        m_Iter.expectType("}");
    }

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::statement()
{
    if (m_Iter.acceptType("{")) {
        while (dataDefinition());
        while (statement());
        m_Iter.expectType("}");
    }
    else if (m_Iter.acceptType("if"))
    {
        m_Iter.expectType("(");
        expression();
        m_Iter.expectType(")");
        statement();
        if (m_Iter.acceptType("else"))
            statement();
    }
    else if (m_Iter.acceptType("while"))
    {
        m_Iter.expectType("(");
        expression();
        m_Iter.expectType(")");
        statement();
    }
    else if (m_Iter.acceptType("break"))
        m_Iter.expectType(";");
    else if (m_Iter.acceptType("continue"))
        m_Iter.expectType(";");
    else if (m_Iter.acceptType("return")) {
        expression();
        m_Iter.expectType(";");
    }
    else {
        expression();
        m_Iter.expectType(";");
    }

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::expression()
{
    if (m_Iter.acceptType("ID"))
    {
        if (m_Iter.acceptType("MULOP") || m_Iter.acceptType("MULASSIGN") ||
            m_Iter.acceptType("ADDOP") || m_Iter.acceptType("ADDASSIGN") ||
            m_Iter.acceptType("SUBASSIGN") || m_Iter.acceptType("MODASSIGN") ||
            m_Iter.acceptType("LOGICOP") || m_Iter.expectType("RELOP"))
        expression();
    }
    else
        unaryExpression();

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::unaryExpression()
{
    if (m_Iter.acceptType("++") || m_Iter.acceptType("--"))
        m_Iter.expectType("ID");
    else
        return primary();

    return m_Iter.hasError();
}

bool SyntaxAnalyzer::primary()
{
    return false;
}

bool SyntaxAnalyzer::argumentList()
{
    do {
        expression();
    } while (m_Iter.acceptType(","));

    return m_Iter.hasError();
}
