#include "test_lex.h"
#include "../src/lex.h"

#include <stdio.h>
#include <vector>
#include "../src/token.h"
#include "../src/tokenlist.h"
#include <assert.h>

void TestLex::runAllTests()
{
    setUp();

    test_tokenizeString_nullString_returnNull();
    test_tokenizeFile_nullString_returnNull();
    test_tokenizeString_if_keywordToken();
    test_tokenizeString_correctColNumbers();
    test_tokenizeString_correctLineNumbers();
    test_tokenizefile_test1();
    test_tokenizefile_test2();

    //tearDown();
}

void TestLex::setUp()
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
void TestLex::test_tokenizeString_nullString_returnNull()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeString(NULL);
    assert(tokens == NULL);
}

void TestLex::test_tokenizeFile_nullString_returnNull()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeFile(NULL);
    assert(tokens == NULL);
}

void TestLex::test_tokenizeString_if_keywordToken()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeString("if");
    assert(tokens->length() == 1);
    assert((*tokens)[0]->lexeme() == "if");
    assert((*tokens)[0]->type() == "if");
}

void TestLex::test_tokenizeString_correctColNumbers()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeString("if (a == b)");
    assert(tokens->length() == 6);
    assert((*tokens)[0]->column() == 1);
    assert((*tokens)[1]->column() == 4);
    assert((*tokens)[2]->column() == 5);
    assert((*tokens)[3]->column() == 7);
    assert((*tokens)[4]->column() == 10);
    assert((*tokens)[5]->column() == 11);
}

void TestLex::test_tokenizeString_correctLineNumbers()
{
    Lex lex(m_SymbolTable);
    TokenList *tokens = lex.tokenizeString("if\n(\na==b\n)");
    assert(tokens->length() == 6);
    assert((*tokens)[0]->line() == 1);
    assert((*tokens)[1]->line() == 2);
    assert((*tokens)[2]->line() == 3);
    assert((*tokens)[3]->line() == 3);
    assert((*tokens)[4]->line() == 3);
    assert((*tokens)[5]->line() == 4);
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
