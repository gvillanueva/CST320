/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/12/2015
 *              Modified, 5/18/2015
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
 * \return true if token type matches input \p type.  Otherwise false.
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
 * \return true if token type matches input \p type.  Otherwise false.
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
 * \return true if last token was accepted.  Otherwise false.
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

Token* TokenIterator::operator*() const
{
    if (m_CurrentIndex >= m_TokenList.length())
        return NULL;

    return m_TokenList[m_CurrentIndex];
}



SyntaxError::SyntaxError(std::string message, const Token* token)
    :m_Message(message), m_Token(token)
{
}

SyntaxError::SyntaxError(const SyntaxError& copy)
    :m_Message(copy.m_Message), m_Token(copy.m_Token)
{
}

std::string SyntaxError::message() const
{
    return m_Message;
}

const Token* SyntaxError::token() const
{
    return m_Token;
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

std::list<SyntaxError> SyntaxAnalyzer::GetLastErrors() const
{
    return m_ErrorList;
}

/*!
 * \brief Parses the source tokens until an error is encountered or the tokens
 *        are exhausted.
 * \return true if parsed successfully.  error if syntax error encountered.
 */
bool SyntaxAnalyzer::parse()
{
    m_ErrorList.clear();
    return program();
}

inline void SyntaxAnalyzer::AddError(std::string message, const Token* token)
{
    m_ErrorList.push_back(SyntaxError(message, token));
}

/****************************** Production Rules ******************************/
///   For information on production rule methods, consult the LLC grammar.   ///

bool SyntaxAnalyzer::program()
{
    if (definition()) {
        while(*m_Iter != 0 && definition());                    ///?
        return true;
    }

    return false;
}

bool SyntaxAnalyzer::definition()
{
    if (m_Iter.acceptType("int")) {
        if (definitionN())
            return true;
    }
    else if (m_Iter.expectType("ID")) {
        if (parameterList())
            if (functionBody())
                return true;
    }

    return false;
}

bool SyntaxAnalyzer::definitionN()
{
    if (m_Iter.expectType("ID"))
        if (definitionNN())
            return true;

    return false;
}

bool SyntaxAnalyzer::definitionNN()
{
    if (m_Iter.acceptType("(")) {
        if (identifierList())
            if (m_Iter.expectType(")")) {
                while (parameterDeclaration()); ///?
                if (functionBody())
                    return true;
        }
    }
    else// if (m_Iter.acceptType(","))
    {
        while (m_Iter.expectType(","))         ///?
            if (!m_Iter.expectType("ID"))             ///?
                return false;
        if (m_Iter.expectType(";"))
            return true;
    }

    return false;
}

bool SyntaxAnalyzer::dataDefinition()
{
    if (m_Iter.expectType("int")) {
        if (m_Iter.expectType("ID")) {
            while (m_Iter.acceptType(","))  {   ///?
                if (!m_Iter.expectType("ID"))   ///?
                    return false;
            }
            if (m_Iter.acceptType(";"))
                return true;
        }
    }

    return false;
}


//bool SyntaxAnalyzer::functionDefinition()
//{
//    m_Iter.acceptType("int");
//    functionHeader();
//    functionBody();

//    return m_Iter.acceptedLast();
//}

//bool SyntaxAnalyzer::functionHeader()
//{
//    if (m_Iter.expectType("int"))
//        parameterList();

//    return m_Iter.acceptedLast();
//}

bool SyntaxAnalyzer::parameterList()
{
    if (m_Iter.expectType("(")) {
        identifierList();
        if (m_Iter.expectType(")")) {
            while (parameterDeclaration());
            return true;
        }
    }

    return false;
}

bool SyntaxAnalyzer::identifierList()
{
    if (m_Iter.expectType("ID"))
    {
        while (m_Iter.expectType(","))
            if (!m_Iter.expectType("ID"))
                return false;
        return true;
    }

    return false;
}

bool SyntaxAnalyzer::parameterDeclaration()
{
    if (m_Iter.expectType("int")) {
        if (m_Iter.expectType("ID")) {
            while (m_Iter.acceptType(","))
                if (!m_Iter.expectType("ID"))
                    return false;
            if (m_Iter.expectType(";"))
                return true;
        }
    }

    return false;
}

bool SyntaxAnalyzer::functionBody()
{
    if (m_Iter.expectType("{")) {
        while (dataDefinition());
        while (statement());
        if (m_Iter.expectType("}"))
            return true;
    }

    return false;
}

bool SyntaxAnalyzer::statement()
{
    if (m_Iter.acceptType("{")) {
        while (dataDefinition());
        while (statement());
        if (m_Iter.expectType("}"))
            return true;
    }
    else if (m_Iter.acceptType("if"))
    {
        if (m_Iter.expectType("(")) {
            if (expression())
                if (m_Iter.expectType(")"))
                    if (statement()) {
                        if (m_Iter.acceptType("else")) {
                            if (statement())
                                return true;
                        }
                        return true;
                    }
        }
    }
    else if (m_Iter.acceptType("while"))
    {
        if (m_Iter.expectType("("))
            if (expression())
                if (m_Iter.expectType(")"))
                    if (statement())
                        return true;
    }
    else if (m_Iter.acceptType("break")) {
        if (m_Iter.expectType(";"))
            return true;
    }
    else if (m_Iter.acceptType("continue")) {
        if (m_Iter.expectType(";"))
            return true;
    }
    else if (m_Iter.acceptType("return")) {
        expression();
        if (m_Iter.expectType(";"))
            return true;
    }
    else {
        expression();
        if (m_Iter.expectType(";"))
            return true;
    }

    return false;
}

bool SyntaxAnalyzer::expression()
{
    if (m_Iter.acceptType("ID"))
    {
        if (expressionN())
            return true;
        else
            return true;
    }
    else if (m_Iter.acceptType("++") || m_Iter.acceptType("--") ||
             m_Iter.expectType("!")) {
        if (m_Iter.expectType("ID"))
            return true;
    }
    else if (m_Iter.acceptType("CONSTANT"))
        return true;
    else if (m_Iter.acceptType("("))
        if (expression())
            if (m_Iter.acceptType(")"))
                return true;

    return false;
}

bool SyntaxAnalyzer::expressionN()
{
    if (m_Iter.acceptType("MULOP") || m_Iter.acceptType("MULASSIGN") ||
        m_Iter.acceptType("ADDOP") || m_Iter.acceptType("ADDASSIGN") ||
        m_Iter.acceptType("SUBASSIGN") || m_Iter.acceptType("MODASSIGN") ||
        m_Iter.acceptType("LOGICOP") || m_Iter.acceptType("RELOP") ||
        m_Iter.expectType("ASSIGNOP")) {
        if (expression())
            return true;
    }
    else if (m_Iter.expectType("(")) {
        if (argumentList())
            if (m_Iter.expectType(")"))
                return true;
    }

    return false;
}

//bool SyntaxAnalyzer::unaryExpression()
//{
//    if (m_Iter.acceptType("++") || m_Iter.acceptType("--")) {
//        if (m_Iter.expectType("ID"))
//            return true;
//    }
//    else
//        return primary();

//    return false;
//}

//bool SyntaxAnalyzer::primary()
//{
//    if (m_Iter.acceptType("("))
//    {
//        expression();
//        m_Iter.expectType(")");
//    }
//    else if (m_Iter.acceptType("CONSTANT")) { ; }
//    else if (m_Iter.expectType("ID")) {
//        if (m_Iter.acceptType("("))
//        {
//            argumentList();
//            m_Iter.expectType(")");
//        }
//        return true;
//    }

//    return m_Iter.acceptedLast();
//}

bool SyntaxAnalyzer::argumentList()
{
    if (expression()) {
        while (m_Iter.acceptType(","))
            if (!expression())
                return false;
        return true;
    }

    return false;
}
