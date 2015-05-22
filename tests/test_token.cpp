#include "test_token.h"
#include "../src/token.h"
#include <assert.h>

void TestToken::runAllTests()
{
    test_token_getLexeme();
    test_token_getType();
    test_token_getSource();
    test_token_getLine();
    test_token_getColumn();
}

void TestToken::test_token_getLexeme()
{
    Token token("if", "if");
    assert(token.lexeme() == "if");
}

void TestToken::test_token_getType()
{
    Token token("<=", "RELOP");
    assert(token.type() == "RELOP");
}

void TestToken::test_token_getSource()
{
    Token token("<=", "RELOP", "test.cpp");
    assert(token.source() == "test.cpp");
}

void TestToken::test_token_getLine()
{
    Token token("<=", "RELOP", "test.cpp", 2412);
    assert(token.line() == 2412);
}

void TestToken::test_token_getColumn()
{
    Token token("<=", "RELOP", "test.cpp", 2142, 45);
    assert(token.column() == 45);
}

