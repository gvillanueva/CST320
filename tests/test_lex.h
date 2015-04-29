#ifndef TEST_LEX_H
#define TEST_LEX_H

class TestLex
{
public:
    void test_tokenizeString_nullString_emptyVector();
    void test_tokenizeString_if_keywordToken();
    void test_tokenizeFile_nullString_emptyVector();
    void test_tokenizefile_test1();
    void test_tokenizefile_test2();
};

#endif // TEST_LEX_H
