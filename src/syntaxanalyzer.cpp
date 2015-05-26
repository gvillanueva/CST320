/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/12/2015
 *              Modified, 5/25/2015
 * \ingroup     CST320 - Lab2a
 * \file        syntaxanalyzer.cpp
 *
 * \brief       Defines the structure of the SyntaxAnalyzer class.
 */
#include "syntaxanalyzer.h"
#include "tokenlist.h"
#include "token.h"
#include "symboltable.h"

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

/*!
 * \brief Returns the pointer to the iterator's current Token.
 * \return Pointer to the current Token object.
 */
Token* TokenIterator::operator*() const
{
    if (m_CurrentIndex >= m_TokenList.length())
        return NULL;

    return m_TokenList[m_CurrentIndex];
}

/*!
 * \brief Returns the pointer to the iterator's previous Token.
 * \return Pointer to the previous Token object.
 */
Token* TokenIterator::previousToken() const
{
    if (m_CurrentIndex >= m_TokenList.length() + 1)
        return NULL;

    return m_TokenList[m_CurrentIndex - 1];
}



/*!
 * \brief Instantiates a new SyntaxError object.
 * \param message   The message associated with the syntax error.
 * \param token     The Token at the site of the syntax error.
 */
SyntaxError::SyntaxError(std::string message, const Token* token)
    :m_Message(message), m_Token(token)
{
}

/*!
 * \brief Instantiates a new SyntaxError object as a copy of an existing object.
 * \param The SyntaxError object to copy.
 */
SyntaxError::SyntaxError(const SyntaxError& copy)
    :m_Message(copy.m_Message), m_Token(copy.m_Token)
{
}

/*!
 * \brief Gets the message associated with the syntax error.
 * \return The message associated with the syntax error.
 */
std::string SyntaxError::message() const
{
    return m_Message;
}

/*!
 * \brief Gets the Token associated with the syntax error.
 * \return A pointer to the Token associated with the syntax error.
 */
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

/*!
 * \brief Gets a list of errors from the last syntax analysis.
 * \return A std::list of SyntaxError objects.
 */
std::list<SyntaxError> SyntaxAnalyzer::GetLastErrors() const
{
    return m_ErrorList;
}

/*!
 * \brief Gets a list of matched production rules from the last analysis.
 * \return A std::list of production rule names.
 */
std::list<std::string> SyntaxAnalyzer::GetProductionRules() const
{
    return m_MatchedRules;
}

/*!
 * \brief Parses the source tokens until an error is encountered or the tokens
 *        are exhausted.
 * \return true if parsed successfully.  error if syntax error encountered.
 */
bool SyntaxAnalyzer::parse()
{
    m_ErrorList.clear();
    m_MatchedRules.clear();
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
        while(*m_Iter != 0 && definition());
        m_MatchedRules.push_back("program");
        return true;
    }

    return false;
}

bool SyntaxAnalyzer::definition()
{
    if (m_Iter.acceptType("int")) {
        if (definitionN()) {
            m_MatchedRules.push_back("definition");
            return true;
        }
    }
    else if (m_Iter.expectType("ID")) {
        if (!m_SymbolTable.addSymbol(m_Iter.previousToken()->lexeme().c_str(), ET_INTEGER, EU_FUNCTION, NULL))
            AddError("Reclaration of symbol", *m_Iter);
        if (parameterList())
            if (functionBody()) {
                m_MatchedRules.push_back("definition");
                return true;
            }
    }

    return false;
}

bool SyntaxAnalyzer::definitionN()
{
    if (m_Iter.expectType("ID")) {
        if (!m_SymbolTable.addSymbol(m_Iter.previousToken()->lexeme().c_str(), ET_INTEGER, EU_FUNCTION, NULL))
            AddError("Reclaration of symbol", *m_Iter);
        if (definitionNN()) {
            m_MatchedRules.push_back("definitionN");
            return true;
        }
    }

    return false;
}

bool SyntaxAnalyzer::definitionNN()
{
    // Last symbol was function declaration
    if (m_Iter.acceptType("(")) {
        if (identifierList())
            if (m_Iter.expectType(")")) {
                while (parameterDeclaration());
                if (functionBody()) {
                    m_MatchedRules.push_back("definitionNN");
                    return true;
                }
        }
    }
    // Last symbol was a variable declaration
    else
    {
        while (m_Iter.expectType(","))
            if (!m_Iter.expectType("ID"))
                return false;
        if (m_Iter.expectType(";")) {
            m_MatchedRules.push_back("definitionNN");
            return true;
        }
    }

    return false;
}

bool SyntaxAnalyzer::dataDefinition()
{
    if (m_Iter.expectType("int")) {
        if (m_Iter.expectType("ID")) {
            if (!m_SymbolTable.addSymbol(m_Iter.previousToken()->lexeme().c_str(), ET_INTEGER, EU_VARIABLE, NULL))
                AddError("Reclaration of symbol", *m_Iter);
            while (m_Iter.acceptType(","))  {
                if (!m_Iter.expectType("ID"))
                    return false;
                else {
                    if (!m_SymbolTable.addSymbol(m_Iter.previousToken()->lexeme().c_str(), ET_INTEGER, EU_VARIABLE, NULL))
                        AddError("Reclaration of symbol", *m_Iter);
                }
            }
            if (m_Iter.acceptType(";")) {
                m_MatchedRules.push_back("dataDefinition");
                return true;
            }
        }
    }

    return false;
}

bool SyntaxAnalyzer::parameterList()
{
    if (m_Iter.expectType("(")) {
        identifierList();
        if (m_Iter.expectType(")")) {
            while (parameterDeclaration());
            m_MatchedRules.push_back("parameterList");
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
        m_MatchedRules.push_back("identifierList");
        return true;
    }

    return false;
}

bool SyntaxAnalyzer::parameterDeclaration()
{
    if (m_Iter.expectType("int")) {
        if (m_Iter.expectType("ID")) {
            if (!m_SymbolTable.addSymbol(m_Iter.previousToken()->lexeme().c_str(), ET_INTEGER, EU_VARIABLE, NULL))
                AddError("Reclaration of symbol", *m_Iter);
            while (m_Iter.acceptType(",")) {
                if (!m_Iter.expectType("ID")) {
                    return false;
                }
                else {
                    if (!m_SymbolTable.addSymbol(m_Iter.previousToken()->lexeme().c_str(), ET_INTEGER, EU_VARIABLE, NULL))
                        AddError("Reclaration of symbol", *m_Iter);
                }
            }
            if (m_Iter.expectType(";")) {
                m_MatchedRules.push_back("parameterDeclaration");
                return true;
            }
        }
    }
\
    return false;
}

bool SyntaxAnalyzer::functionBody()
{
    if (m_Iter.expectType("{")) {
        m_SymbolTable.pushScope();
        while (dataDefinition());
        while (statement());
        if (m_Iter.expectType("}")) {
            m_SymbolTable.popScope();
            m_MatchedRules.push_back("functionBody");
            return true;
        }
        m_SymbolTable.popScope();
    }

    return false;
}

bool SyntaxAnalyzer::statement()
{
    if (m_Iter.acceptType("{")) {
        m_SymbolTable.pushScope();
        while (dataDefinition());
        while (statement());
        if (m_Iter.expectType("}")) {
            m_SymbolTable.popScope();
            m_MatchedRules.push_back("statement");
            return true;
        }
        m_SymbolTable.popScope();
    }
    else if (m_Iter.acceptType("if"))
    {
        if (m_Iter.expectType("(")) {
            if (expression())
                if (m_Iter.expectType(")"))
                    if (statement()) {
                        if (m_Iter.acceptType("else")) {
                            if (statement()) {
                                m_MatchedRules.push_back("statement");
                                return true;
                            }
                        }
                        m_MatchedRules.push_back("statement");
                        return true;
                    }
        }
    }
    else if (m_Iter.acceptType("while"))
    {
        if (m_Iter.expectType("("))
            if (expression())
                if (m_Iter.expectType(")"))
                    if (statement()) {
                        m_MatchedRules.push_back("statement");
                        return true;
                    }
    }
    else if (m_Iter.acceptType("break")) {
        if (m_Iter.expectType(";")) {
            m_MatchedRules.push_back("statement");
            return true;
        }
    }
    else if (m_Iter.acceptType("continue")) {
        if (m_Iter.expectType(";")) {
            m_MatchedRules.push_back("statement");
            return true;
        }
    }
    else if (m_Iter.acceptType("return")) {
        expression();
        if (m_Iter.expectType(";")) {
            m_MatchedRules.push_back("statement");
            return true;
        }
    }
    else {
        expression();
        if (m_Iter.expectType(";")) {
            m_MatchedRules.push_back("statement");
            return true;
        }
    }

    return false;
}

bool SyntaxAnalyzer::expression()
{
    if (m_Iter.acceptType("ID"))
    {
        if (m_SymbolTable.findSymbol(m_Iter.previousToken()->lexeme().c_str()).isNull())
            AddError("Symbol not declared", *m_Iter);
        if (expressionN()) {
            m_MatchedRules.push_back("expression");
            return true;
        }
        else {
            m_MatchedRules.push_back("expression");
            return true;
        }
    }
    else if (m_Iter.acceptType("++") || m_Iter.acceptType("--") ||
             m_Iter.expectType("!")) {
        if (m_Iter.expectType("ID")) {
            if (m_SymbolTable.findSymbol(m_Iter.previousToken()->lexeme().c_str()).isNull())
                AddError("Symbol not declared", *m_Iter);
            m_MatchedRules.push_back("expression");
            return true;
        }
    }
    else if (m_Iter.acceptType("CONSTANT"))  {
        m_MatchedRules.push_back("expression");
        return true;
    }
    else if (m_Iter.acceptType("("))
        if (expression())
            if (m_Iter.acceptType(")")) {
                m_MatchedRules.push_back("expression");
                return true;
            }

    return false;
}

bool SyntaxAnalyzer::expressionN()
{
    if (m_Iter.acceptType("MULOP") || m_Iter.acceptType("MULASSIGN") ||
        m_Iter.acceptType("ADDOP") || m_Iter.acceptType("ADDASSIGN") ||
        m_Iter.acceptType("SUBASSIGN") || m_Iter.acceptType("MODASSIGN") ||
        m_Iter.acceptType("LOGICOP") || m_Iter.acceptType("RELOP") ||
        m_Iter.expectType("ASSIGNOP")) {
        if (expression()) {
            m_MatchedRules.push_back("expressionN");
            return true;
        }
    }
    else if (m_Iter.expectType("(")) {
        if (argumentList())
            if (m_Iter.expectType(")")) {
                m_MatchedRules.push_back("expressionN");
                return true;
            }
    }

    return false;
}

bool SyntaxAnalyzer::argumentList()
{
    if (expression()) {
        while (m_Iter.acceptType(","))
            if (!expression())
                return false;

        m_MatchedRules.push_back("argumentList");
        return true;
    }

    return false;
}
