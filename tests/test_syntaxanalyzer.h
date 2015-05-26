/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/17/2015
 *              Modified, 5/17/2015
 * \ingroup     CST320 - Lab2a
 * \file        test_syntaxanalyzer.h
 *
 * \brief       Declares the test procedures for the SyntaxAnalyzer class.
 */
#ifndef TEST_SYNTAXANALYZER_H
#define TEST_SYNTAXANALYZER_H

class SymbolTable;

class TestSyntaxAnalyzer
{
public:
    void runAllTests();

public:
    /// Test parameterList rule
    void test_parameterList();
    void test_parameterList_missingOpeningParen();
    void test_parameterList_missingClosingParen();

    void test_identifierList();

    void test_unaryExpressionIncrement();
    void test_unaryExpressionDecrement();

    // Run tests on source code files
    void test_test1();

private:
    void setUpDefaultSymbolTable(SymbolTable& symbolTable);
};

#endif//TEST_SYNTAXANALYZER_H
