#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

// #define Nnn [nnn]
// #endif
// #ifdef/ifndef Nnn
// #include "nnnnnn"/<aaaaa>
// #undef Nnn
//

class TokenList;
class SymbolTable;

class Preprocessor
{
public:
    Preprocessor(SymbolTable symbols);
    void process(const TokenList &tokens);
};

#endif // PREPROCESSOR_H
