/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/21/2015
 *              Modified, 4/30/2015
 * \ingroup     CST320 - Lab1c
 * \file        preprocessor.cpp
 *
 * \brief       Defines the structure of the Preprocessor class.
 */
#include "preprocessor.h"
#include "symboltable.h"
#include "token.h"
#include "tokenlist.h"
#include <stdio.h>
#include <string.h>
#include "lex.h"

/*!
 * \brief Creates a preprocessor object, maintaining reference to \p symbolTable.
 * \param symbolTable   A reference to an instantiated symbol table.
 */
Preprocessor::Preprocessor(SymbolTable &symbolTable)
    :m_SymbolTable(symbolTable), m_IfPreproc(false)
{
}

/*!
 * \brief Processes preprocessor directives in the given token list.
 * \param tokens    A list of tokens which will be modified according to the
 *                  preprocessor directives it contains.
 */
void Preprocessor::process(TokenList &tokens)
{
    // int implies a unit of program maximum length of 2 billion tokens
    for (int i = 0; i < tokens.length(); i++)
    {
        if (tokens[i]->type() != "PREPROCESSOR")
        {
            if (tokens[i]->type() != "ID")
                continue;

            // Replace defined macros
            SymbolPtr symbol = m_SymbolTable.findSymbol(tokens[i]->lexeme().c_str());
            if (!symbol.isNull())
            {
                Token *before = tokens[i];
                Token *replacement = new Token(symbol.constData(),
                                         symbol.use() == EU_ID ? "ID" : "CONSTANT");
                tokens.insertBefore(replacement, before);
                tokens.remove(before);
                delete before;
            }
            continue;
        }

        // Maintain pointer to token and remove proprocessor token from list
        Token *token = tokens[i];
        tokens.remove(token);

        if (token->lexeme() == "#define")
        {
            // Look an identifier to name the macro
            if (tokens[i]->type() != "ID")
            {
                printf("no macro name given in #define directive\n");
                continue;
            }
            Token *macro = tokens[i];
            tokens.remove(macro);

            // Look for an identifier or constant to use as the macro's value
            Token *value = NULL;
            if (tokens[i]->type() == "ID" || tokens[i]->type() == "CONSTANT")
            {
                value = tokens[i];
                tokens.remove(value);
            }

            E_USE use = EU_MACRO;
            if (value) {
                if (value->type() == "ID")
                    use = EU_ID;
                else
                    use = EU_CONSTANT;
            }
            // Add a preprocessor macro symbol, with const value if value exists
            m_SymbolTable.addSymbol(macro->lexeme().c_str(), ET_VOID, use,
                                    value ? value->lexeme().c_str() : NULL);
            delete macro;
            if (value) delete value;
            i--;
        }
        else if (token->lexeme() == "#undef")
        {
            // Look an identifier to name the macro
            if (tokens[i]->type() != "ID")
            {
                printf("no macro name given in #define directive\n");
                continue;
            }
            Token *macro = tokens[i];
            tokens.remove(macro);
            m_SymbolTable.removeSymbol(macro->lexeme().c_str());
            delete macro;
            i--;
        }
        else if (token->lexeme() == "#ifdef" ||
                 token->lexeme() == "#ifndef")
        {
            bool nullPasses = token->lexeme() == "#ifndef";

            // Look an identifier to name the macro
            if (tokens[i]->type() != "ID")
            {
                printf("no macro name given in %s directive\n", token->lexeme().c_str());
                continue;
            }
            Token *macro = tokens[i];
            tokens.remove(macro);

            // Look for a defined macro symbol
            SymbolPtr symbol = m_SymbolTable.findSymbol(macro->lexeme().c_str());

            // Depending on directive value, ignore tokens until #endif
            if (symbol.isNull() ^ nullPasses) {
                if (!removeTokensUntilEndif(i, tokens))
                    printf("unterminated %s\n", token->lexeme().c_str());
            }
            else
                m_IfPreproc = true;
            delete macro;
        }
        else if (token->lexeme() == "#endif")
        {
            if (!m_IfPreproc)
                printf("#endif without #if\n");
            m_IfPreproc = false;
        }
        else if (token->lexeme() == "#include")
        {
            if (tokens[i]->type() != "STRING")
            {
                printf("#include expects \"FILENAME\"\n");
                continue;
            }

            // Get include path token
            Token *path = tokens[i];
            tokens.remove(path);

            // Use lexical analyzer to tokenize included file
            Lex lex(m_SymbolTable);
            const char *filename = path->lexeme().substr(1, path->lexeme().length() - 2).c_str();
            delete path;

            for (size_t j = 0; j < IncludeStack.size(); j++)
                if (IncludeStack[j] == filename)
                {
                    printf("prevented duplicate include of %s\n", filename);
                    continue;
                }

            IncludeStack.push_back(filename);
            TokenList *includedTokens = lex.tokenizeFile(filename);
            process(*includedTokens);
            includedTokens->move(&tokens, tokens[i]);

            if (!IncludeStack.empty())
                IncludeStack.pop_back();
        }

        delete token;
    }
}

/*!
 * \brief   Private method removes tokens until and #endif token is encountered.
 * \param i         The index of the current token.
 * \param tokens    The list of tokens.
 * \return  true if endif encountered; otherwise false.
 */
bool Preprocessor::removeTokensUntilEndif(int i, TokenList &tokens)
{
    bool bEndif = tokens[i]->lexeme() == "#endif";
    while (i < tokens.length() && !bEndif)
    {
        Token *token = tokens[i];
        tokens.remove(tokens[i]);
        delete token;
        bEndif = tokens[i]->lexeme() == "#endif";
    }

    if (bEndif)
    {
        Token *endif = tokens[i];
        tokens.remove(tokens[i]);
        delete endif;
    }

    return bEndif;
}
