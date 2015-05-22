#ifndef TEST_LEX_H
#define TEST_LEX_H
#include "../src/symboltable.h"

class TestLex
{
public:
    void runAllTests();
public:
    void setUp();
    void test_tokenizeString_nullString_returnNull();
    void test_tokenizeString_if_keywordToken();
    void test_tokenizeFile_nullString_returnNull();
    void test_tokenizefile_test1();
    void test_tokenizefile_test2();

private:
    SymbolTable m_SymbolTable;
};

#endif // TEST_LEX_H
