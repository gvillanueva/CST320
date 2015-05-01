#include "preprocessor.h"
#include "symboltable.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include "lex.h"

Preprocessor::Preprocessor(SymbolTable &symbolTable)
    :m_SymbolTable(symbolTable), m_IfPreproc(false)
{
}

/*!
 * \brief Processes preprocessor directives within \p tokens.
 * \param tokens    A list of tokens which will be modified according to the
 *                  preprocessor directives it contains.
 */
void Preprocessor::process(TokenList &tokens)
{
    // int implies a unit of program maximum length of 2 billion tokens
    for (int i = 0; i < tokens.length(); i++)
    {
        // Maintain pointer to token and remove proprocessor token from list
        Token *token = tokens[i];
        tokens.remove(token);

        if (token->value() == "#define")
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

            // Add a preprocessor macro symbol, with const value if value exists
            m_SymbolTable.addSymbol(macro->value().c_str(), ET_VOID, EU_MACRO,
                                    value ? value->value().c_str() : NULL);
            delete macro;
            if (value) delete value;
            i--;
        }
        else if (token->value() == "#undef")
        {
            // Look an identifier to name the macro
            if (tokens[i]->type() != "ID")
            {
                printf("no macro name given in #define directive\n");
                continue;
            }
            Token *macro = tokens[i];
            tokens.remove(macro);
            m_SymbolTable.removeSymbol(macro->value().c_str());
            delete macro;
            i--;
        }
        else if (token->value() == "#ifdef" ||
                 token->value() == "#ifndef")
        {
            bool nullPasses = token->value() == "#ifndef";

            // Look an identifier to name the macro
            if (tokens[i]->type() != "ID")
            {
                printf("no macro name given in %s directive\n", token->value().c_str());
                continue;
            }
            Token *macro = tokens[i];
            tokens.remove(macro);

            SymbolPtr symbol = m_SymbolTable.findSymbol(macro->value().c_str());
            if (symbol.isNull() ^ nullPasses) {
                if (!removeTokensUntilEndif(i, tokens))
                    printf("unterminated %s\n", token->value().c_str());
            }
            else
                m_IfPreproc = true;
            delete macro;
        }
        else if (token->value() == "#endif")
        {
            if (!m_IfPreproc)
                printf("#endif without #if\n");
            m_IfPreproc = false;
        }
        else if (token->value() == "#include")
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
            const char *filename = path->value().substr(1, path->value().length() - 2).c_str();
            delete path;

            for (size_t i = 0; i < IncludeStack.size(); i++)
                if (IncludeStack[i] == filename)
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
        else
        {
            // Replace defined macros
            SymbolPtr symbol = m_SymbolTable.findSymbol(token->value().c_str());
            if (!symbol.isNull())
            {
                token->type()
            }
        }

        delete token;
    }
}

bool Preprocessor::removeTokensUntilEndif(int i, TokenList &tokens)
{
    bool bEndif = tokens[i]->value() == "#endif";
    while (i < tokens.length() && !bEndif)
    {
        Token *token = tokens[i];
        tokens.remove(tokens[i]);
        delete token;
        bEndif = tokens[i]->value() == "#endif";
    }

    if (bEndif)
    {
        Token *endif = tokens[i];
        tokens.remove(tokens[i]);
        delete endif;
    }

    return bEndif;
}
