/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 5/17/2015
 *              Modified, 5/17/2015
 * \ingroup     CST320 - Lab2a
 * \file        test_syntaxanalyzer.cpp
 *
 * \brief       Defines the test procedures declared in test_syntaxanalyzer.h
 */
#include <assert.h>
#include "test_syntaxanalyzer.h"
#include "../src/syntaxanalyzer.h"
#include "../src/token.h"
#include "../src/tokenlist.h"
#include "../src/symboltable.h"
#include "../src/lex.h"
#include "../src/preprocessor.h"

void TestSyntaxAnalyzer::setUpDefaultSymbolTable(SymbolTable &symbolTable)
{
    symbolTable.addSymbol("bool", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("break", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("else", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("false", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("if", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("int", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("return", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("true", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("void", ET_VOID, EU_KEYWORD, NULL);
    symbolTable.addSymbol("while", ET_VOID, EU_KEYWORD, NULL);
}

void TestSyntaxAnalyzer::test_parameterList()
{
    SymbolTable symbols;
    setUpDefaultSymbolTable(symbols);

    TokenList tokens;
    tokens.add(new Token("(", "("));
    tokens.add(new Token("a", "ID"));
    tokens.add(new Token(",", ","));
    tokens.add(new Token("b", "ID"));
    tokens.add(new Token(",", ","));
    tokens.add(new Token("c", "ID"));
    tokens.add(new Token(")", ")"));

    int i = 0;
    SyntaxAnalyzer syntaxAnalyzer(symbols, tokens);
    assert(!syntaxAnalyzer.parameterList());
}

void TestSyntaxAnalyzer::test_identifierList()
{
    SymbolTable symbols;
    setUpDefaultSymbolTable(symbols);

    TokenList tokens;
    tokens.add(new Token("a", "ID"));
    tokens.add(new Token(",", ","));
    tokens.add(new Token("b", "ID"));
    tokens.add(new Token(",", ","));
    tokens.add(new Token("c", "ID"));

    int i = 0;
    SyntaxAnalyzer syntaxAnalyzer(symbols, tokens);
    assert(!syntaxAnalyzer.identifierList());
}

void TestSyntaxAnalyzer::test_unaryExpressionIncrement()
{
    SymbolTable symbols;
    setUpDefaultSymbolTable(symbols);

    TokenList tokens;
    tokens.add(new Token("++", "++"));
    tokens.add(new Token("a", "ID"));

    SyntaxAnalyzer syntaxAnalyzer(symbols, tokens);
    assert(!syntaxAnalyzer.unaryExpression());
}

void TestSyntaxAnalyzer::test_unaryExpressionDecrement()
{
    SymbolTable symbols;
    setUpDefaultSymbolTable(symbols);

    TokenList tokens;
    tokens.add(new Token("--", "--"));
    tokens.add(new Token("a", "ID"));

    SyntaxAnalyzer syntaxAnalyzer(symbols, tokens);
    assert(!syntaxAnalyzer.unaryExpression());
}

void TestSyntaxAnalyzer::test_test1()
{
    SymbolTable symbols;
    setUpDefaultSymbolTable(symbols);

    Lex lex(symbols);
    TokenList* tokens= lex.tokenizeFile("../tests/sources/test1.cpp");
    Preprocessor preprocessor(symbols);
    preprocessor.process(*tokens);
    SyntaxAnalyzer syntaxAnalyzer(symbols, *tokens);
    assert(syntaxAnalyzer.parse());
}

int main(int, char*[])
{
    TestSyntaxAnalyzer tester;

    //tester.test_parameterList();
    tester.test_identifierList();
    tester.test_unaryExpressionIncrement();
    tester.test_unaryExpressionDecrement();

    tester.test_test1();
    return 0;
}
