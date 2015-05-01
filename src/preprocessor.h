#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <vector>
#include <string>

class TokenList;
class SymbolTable;

/*!
 * \brief The Preprocessor class
 *
 * The scope of preprocessor symbols is global, so there will be no new scopes
 * to the symbol table.
 */
class Preprocessor
{
public:
    Preprocessor(SymbolTable &symbolTable);
    void process(TokenList &tokens);

private:
    bool removeTokensUntilEndif(int i, TokenList &tokens);

    SymbolTable &m_SymbolTable;
    std::vector<std::string> IncludeStack; //Prevents infinite file recursion
    bool m_IfPreproc;
};

#endif // PREPROCESSOR_H
