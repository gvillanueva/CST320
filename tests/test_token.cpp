#include "test_token.h"
#include "../src/token.h"
#include <assert.h>

void TestToken::test_token_getValue()
{
    Token token("if", "keyword");
    assert(token.value() == "if");
}

void TestToken::test_token_getType()
{
    Token token("if", "keyword");
    assert(token.type() == "keyword");
}
