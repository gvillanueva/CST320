#include "test_lex.h"
#include "../src/lex.h"

#include <stdio.h>
#include <vector>
#include "../src/token.h"
#include <assert.h>

/// Tests that a NULL string will return an empty token list
void TestLex::test_tokenizeString_nullString_emptyVector()
{
    Lex lex;
    TokenList *tokens = lex.tokenizeString(NULL);
    assert(tokens->length() == 0);
}

void TestLex::test_tokenizeString_if_keywordToken()
{
    Lex lex;
    TokenList *tokens = lex.tokenizeString("if");
    assert(tokens->length() == 1);
    assert((*tokens)[0]->value() == "if");
    assert((*tokens)[0]->type() == "keyword");
}

void TestLex::test_tokenizeFile_nullString_emptyVector()
{
    Lex lex;
    TokenList *tokens = lex.tokenizeFile(NULL);
    assert(tokens->length() == 0);
}

void TestLex::test_tokenizefile_test1()
{
    Lex lex;
    TokenList *tokens = lex.tokenizeFile("test1.cpp");
}

void TestLex::test_tokenizefile_test2()
{
    Lex lex;
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
