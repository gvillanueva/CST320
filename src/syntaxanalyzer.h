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

class TokenList;
class SymbolTable;

/*!
 * \brief The SyntaxAnalyzer class evaluates postprocessed tokens from the
 *        Lexical Anaylzer for proper LLC syntax.
 */
class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer(SymbolTable &symbolTable);

    /// Process a list of tokens, evaluating its syntax.
    void process(TokenList &tokens);

private:
    /// A reference to a populated symbol table.
    SymbolTable &m_SymbolTable;
};

#endif // SYNTAXANALYZER_H
