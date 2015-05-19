/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/11/2015
 *              Modified, 5/11/2015
 * \ingroup     CST320 - Lab2a
 * \file        syntaxanalyzer.h
 *
 * \brief       Declares the structure of the SyntaxAnalyzer class.
 */
#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H
#include <string>

class Token;
class TokenList;
class SymbolTable;

class TokenIterator
{
public:
    TokenIterator(TokenList& tokens);
    Token* operator*();
    Token* current();
    bool acceptType(std::string type);
    bool expectType(std::string type);
    bool acceptedLast();

private:
    int m_CurrentIndex;
    bool m_AcceptedLast;
    TokenList& m_TokenList;
};

/*!
 * \brief The SyntaxAnalyzer class evaluates postprocessed tokens from the
 *        Lexical Anaylzer for proper LLC syntax.
 */
class SyntaxAnalyzer
{
    friend class TestSyntaxAnalyzer;

public:
    SyntaxAnalyzer(SymbolTable &symbolTable, TokenList& tokenList);

    /// Process a list of tokens, evaluating its syntax.
    bool parse();

private:
    bool program();
    bool definition();
    bool definitionN();
    bool definitionNN();
    bool dataDefinition();
    bool functionDefinition();
    bool functionHeader();
    bool parameterList();
    bool identifierList();
    bool parameterDeclaration();
    bool functionBody();
    bool statement();
    bool expression();
    bool unaryExpression();
    bool primary();
    bool argumentList();

private:
    /// A reference to a populated symbol table.
    SymbolTable &m_SymbolTable;
    TokenIterator m_Iter;
};

#endif // SYNTAXANALYZER_H
