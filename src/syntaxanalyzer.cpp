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

/*!
 * \brief Instantiates a new token iterator object.
 * \param tokens List of tokens to iterate through.
 */
TokenIterator::TokenIterator(TokenList &tokens)
    :m_CurrentIndex(0), m_AcceptedLast(false), m_TokenList(tokens)
{
}

/*!
 * \brief Non-fatal consumption of a token; will not increment if \p type
 *        doesn't match the current token's type.
 * \param type  A generic token type.
 * \return True if token type matches input \p type.  Otherwise false.
 */
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

/*!
 * \brief Fatal consumption of a token. \p type must match current token's type.
 * \param type  A generic token type.
 * \return True if token type matches input \p type.  Otherwise false.
 *
 * This function will set the iterator's acceptedLast property to false if \p
 * type does not match the token's type.
 */
bool TokenIterator::expectType(std::string type)
{
    if (acceptType(type))
        return true;
    m_AcceptedLast = false;
    return false;
}

/*!
 * \brief TokenIterator::acceptedLast
 * \return True if last token was accepted.  Otherwise false.
 *
 * This function allows production rules to determine whether a test within them
 * failed. If acceptedLast is unset, it is reset when this property is accessed.
 */
bool TokenIterator::acceptedLast()
{
    // Set accepted flag and return true
    if (!m_AcceptedLast) {
        m_AcceptedLast = true;
        return false;
    }

    return true;
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

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::definition()
{
    if (m_Iter.acceptType("int"))
        definitionN();
    else if (functionHeader()) {
        functionBody();
    }
    else
        return false;

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::definitionN()
{
    if (m_Iter.expectType("ID"))
        definitionNN();

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::definitionNN()
{
    if (parameterList())
    {
        m_Iter.expectType("{");
        while(dataDefinition());
        while(statement());
        m_Iter.expectType("}");
    }
    else
    {
        while (m_Iter.acceptType(","))
            m_Iter.expectType("ID");
        m_Iter.expectType(";");
    }
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

    return m_Iter.acceptedLast();
}


bool SyntaxAnalyzer::functionDefinition()
{
    m_Iter.acceptType("int");
    functionHeader();
    functionBody();

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::functionHeader()
{
    if (m_Iter.expectType("int"))
        parameterList();

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::parameterList()
{
    if (m_Iter.expectType("(")) {
        identifierList();
        if (m_Iter.expectType(")"))
            while (parameterDeclaration());
    }

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::identifierList()
{
    do {
        m_Iter.expectType("ID");
    } while (m_Iter.acceptType(","));

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::parameterDeclaration()
{
    if (m_Iter.expectType("int")) {
        do {
            m_Iter.expectType("ID");
        } while (m_Iter.acceptType(","));
        m_Iter.expectType(";");
    }

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::functionBody()
{
    if (m_Iter.expectType("{")) {
        while (dataDefinition());
        while (statement());
        m_Iter.expectType("}");
    }

    return m_Iter.acceptedLast();
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

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::expression()
{
    if (m_Iter.acceptType("ID"))
    {
        if (m_Iter.acceptType("MULOP") || m_Iter.acceptType("MULASSIGN") ||
            m_Iter.acceptType("ADDOP") || m_Iter.acceptType("ADDASSIGN") ||
            m_Iter.acceptType("SUBASSIGN") || m_Iter.acceptType("MODASSIGN") ||
            m_Iter.acceptType("LOGICOP") || m_Iter.acceptType("RELOP") ||
            m_Iter.expectType("ASSIGNOP"))
        expression();
    }
    else
        unaryExpression();

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::unaryExpression()
{
    if (m_Iter.acceptType("++") || m_Iter.acceptType("--"))
        m_Iter.expectType("ID");
    else
        return primary();

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::primary()
{
    if (m_Iter.acceptType("("))
    {
        expression();
        m_Iter.expectType(")");
    }
    else if (m_Iter.acceptType("CONSTANT")) { ; }
    else if (m_Iter.acceptType("ID")) {
        if (m_Iter.acceptType("("))
        {
            argumentList();
            m_Iter.expectType(")");
        }
    }

    return m_Iter.acceptedLast();
}

bool SyntaxAnalyzer::argumentList()
{
    do {
        expression();
    } while (m_Iter.acceptType(","));

    return m_Iter.acceptedLast();
}
