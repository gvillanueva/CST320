/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/21/2015
 *              Modified, 4/30/2015
 * \ingroup     CST320 - Lab1c
 * \file        preprocessor.h
 *
 * \brief       Declares the structure of the Preprocessor class.
 */
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <vector>
#include <string>

/// Forward declarations
class TokenList;
class SymbolTable;

/*!
 * \brief The Preprocessor class
 *
 * The scope of preprocessor symbols is global, so there will be no new scopes
 * added to the symbol table.
 */
class Preprocessor
{
public:
    /// Instantiates a new preprocessor object.
    Preprocessor(SymbolTable &symbolTable);

    /// Process a list of tokens, removing and acting on preprocess directives.
    void process(TokenList &tokens);

private:
    /// Removes tokens until #endif is encountered.
    bool removeTokensUntilEndif(int i, TokenList &tokens);

    /// A reference to a populated symbol table.
    SymbolTable &m_SymbolTable;

    /// A dynamic array of included files.
    std::vector<std::string> IncludeStack;

    /// Flag to track whether we are within a preprocessor.
    bool m_IfPreproc;
};

#endif // PREPROCESSOR_H
