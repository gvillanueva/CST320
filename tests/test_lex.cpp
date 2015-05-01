#include "test_lex.h"
#include "../src/lex.h"

#include <stdio.h>
#include <vector>
#include "../src/token.h"
#include "../src/tokenlist.h"
#include <assert.h>

void TestLex::setup()
{
    m_SymbolTable.addSymbol("bool", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("break", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("else", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("false", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("if", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("int", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("return", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("true", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("void", ET_VOID, EU_KEYWORD, NULL);
    m_SymbolTable.addSymbol("while", ET_VOID, EU_KEYWORD, NULL);
}

/// Tests that a NULL string will return an empty token list
void TestLex::test_tokenizeString_nullString_emptyVector()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeString(NULL);
    assert(tokens->length() == 0);
}

void TestLex::test_tokenizeString_if_keywordToken()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeString("if");
    assert(tokens->length() == 1);
    assert((*tokens)[0]->value() == "if");
    assert((*tokens)[0]->type() == "keyword");
}

void TestLex::test_tokenizeFile_nullString_emptyVector()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeFile(NULL);
    assert(tokens->length() == 0);
}

void TestLex::test_tokenizefile_test1()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeFile("test1.cpp");
}

void TestLex::test_tokenizefile_test2()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeFile("test2.cpp");
}

#ifdef RUN_TESTLEX

int main(int, char*[])
{
    TestLex tester;
    tester.test_tokenizeString_nullString_emptyVector();
    tester.test_tokenizeString_if_keywordToken();

    tester.test_tokenizeFile_nullString_emptyVector();
    tester.test_tokenizefile_test1();
    tester.test_tokenizefile_test2();
}

#endif
