/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/11/2015
 *              Modified, 5/18/2015
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

/*!
 * \brief The TokenIterator class iterates over a TokenList. It reduces the
 *        complexity and tediousness of manually iterating over the list of
 *        tokens.
 */
class TokenIterator
{
public:
    /// Creates a new token iterator
    TokenIterator(TokenList& tokens);

    /// Non-fatally processes the current token.
    bool acceptType(std::string type);

    /// Fatally processes the current token.
    bool expectType(std::string type);

    /// Gets whether the last token was accepted or generated a syntax error.
    bool acceptedLast();

private:
    /// The current index of the iterator.
    int m_CurrentIndex;

    /// Flag indicating whether the last token was accepted of was a syntax error.
    bool m_AcceptedLast;

    /// A reference to the list of tokens this object iterates over.
    TokenList& m_TokenList;
};

/*!
 * \brief The SyntaxAnalyzer class evaluates postprocessed tokens from the
 *        Lexical Anaylzer for proper LLC syntax.
 */
class SyntaxAnalyzer
{
    /// Befriend the test class for unit tests as necessary
    friend class TestSyntaxAnalyzer;

public:
    /// Creates a new SyntaxAnalyzer object.
    SyntaxAnalyzer(SymbolTable &symbolTable, TokenList& tokenList);

    /// Process a list of tokens, evaluating its syntax.
    bool parse();

    /// The following methods represent the production rules of the LLC grammar.
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

    /// An interator for our input list of tokens.
    TokenIterator m_Iter;
};

#endif // SYNTAXANALYZER_H
