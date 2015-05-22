#ifndef TEST_TOKEN_H
#define TEST_TOKEN_H

class TestToken
{
public:
    void runAllTests();

public:
    void test_token_getLexeme();
    void test_token_getType();
    void test_token_getSource();
    void test_token_getLine();
    void test_token_getColumn();
};

#endif // TEST_TOKEN_H
