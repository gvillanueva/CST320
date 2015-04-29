#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

class TokenList;
class SymbolTable;

class Preprocessor
{
public:
    Preprocessor(SymbolTable symbols);
    void process(const TokenList &tokens);
};

#endif // PREPROCESSOR_H
