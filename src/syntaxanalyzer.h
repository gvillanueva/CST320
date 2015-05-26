/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/11/2015
 *              Modified, 5/25/2015
 * \ingroup     CST320 - Lab2a
 * \file        syntaxanalyzer.h
 *
 * \brief       Declares the structure of the SyntaxAnalyzer class.
 */
#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H
#include <string>
#include <list>

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

    /// Gets the Token pointer at the iterator's current location.
    Token* operator*() const;

private:
    /// The current index of the iterator.
    int m_CurrentIndex;

    /// Flag indicating whether the last token was accepted of was a syntax error.
    bool m_AcceptedLast;

    /// A reference to the list of tokens this object iterates over.
    TokenList& m_TokenList;
};

/*!
 * \brief The SyntaxError class represents an error encountered during
 *        syntactical analysis.
 */
class SyntaxError
{
public:
    /// Instantiates a new SyntaxError object.
    SyntaxError(std::string message, const Token* token);

    /// Instantiates a new SyntaxError object as a copy of an existing object.
    SyntaxError(const SyntaxError& copy);

    /// Gets the message associated with the syntax error.
    std::string message() const;

    /// Gets the Token associated with the syntax error.
    const Token* token() const;

private:
    /// The message representing the error in syntax.
    std::string m_Message;

    /// The Token encountered at the site of the syntax error.
    const Token* m_Token;
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

    /// Gets a list of errors from the last syntax analysis.
    std::list<SyntaxError> GetLastErrors() const;

    /// Gets a list of matched production rules from the last analysis.
    std::list<std::string> GetProductionRules() const;

    /// Process a list of tokens, evaluating its syntax.
    bool parse();

    /// The following methods represent the production rules of the LLC grammar.
private:
    bool program();
    bool definition();
    bool definitionN();
    bool definitionNN();
    bool dataDefinition();
    bool parameterList();
    bool identifierList();
    bool parameterDeclaration();
    bool functionBody();
    bool statement();
    bool expression();
    bool expressionN();
    bool argumentList();

    /// Helper method for adding new SyntaxError objects to the list
    inline void AddError(std::string message, const Token* token);

private:
    /// A reference to a populated symbol table.
    SymbolTable &m_SymbolTable;

    /// An interator for our input list of tokens.
    TokenIterator m_Iter;

    /// The list of errors encountered during the last Syntax Analysis
    std::list<SyntaxError> m_ErrorList;

    /// The list of matched production rules, by name.
    std::list<std::string> m_MatchedRules;
};

#endif // SYNTAXANALYZER_H
