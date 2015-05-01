/***********************************************************************************
* Author:					Giancarlo Villanueva
* Date Created:				May 1, 2010
* Last Modification Date:	June 5, 2010
* Lab Number:				CST 320 Lab 1
* Filename:					main.cpp
*
* Overview:
*   The Lexical Analyzer breaks source code into meaningful units. The Lexical
* Analyzer will only recognize tokens/lexemes that are included in the C- language.
*
* Input:
*   The input to the Lexical Analyzer is obtained via the first command line
* argument. This should be a file path, and the file should contain the source to
* to a C- program, which should contain no lexical errors. However, if any of these
* conditions are not met, an error message will be generated.
*
* Output:
*   Output from the Lexical Analyzer is written locally to lex.txt in the CWD.
* If there are any lexical errors in the source file or if the source file cannot be
* opened, error messages will appear in lex.txt. Recognized tokens on their own
* line, preceeded by their lexeme formatted to a width of 12, padded by whitespace.
***********************************************************************************/
#include <stdio.h>
#include "../src/lex.h"
#include "../src/token.h"
#include "../src/symboltable.h"
#include "../src/preprocessor.h"
#include <iostream>
#include <iomanip>

#define OUTPUT_WIDTH    12 ///Formats output, pads lexemes to 12 chars

void printTokenList(TokenList *tokenList)
{
    for (int i = 0; i < tokenList->length(); i++)
    {
        Token *token = (*tokenList)[i];
        std::cout << std::setw(OUTPUT_WIDTH) << token->value() << token->type() << std::endl;
        //printf("%-*c%c\n", OUTPUT_WIDTH, token->value(), token->type());
    }
}

void initSymbolTable(SymbolTable &symbolTable)
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

int main (int argc, char* argv[])
{
    FILE *fo = 0;
    TokenList *tokenList = NULL;

    // Create and populate symbol table for compiler components
    SymbolTable symbolTable;
    initSymbolTable(symbolTable);

    // Create a lexical analyzer and pass the symbol table to it
    Lex lex(symbolTable);

    ///Redirect STDOUT to lex.txt
//    if((fo = freopen("lex.txt", "w", stdout)) != NULL)
        if (argc > 0)
            tokenList = lex.tokenizeFile(argv[1]);
        else
            printf("Usage: lex.exe [filename]\n");
//    else
//        printf("Couldn't redirect stdout to log.txt.\n");

    Preprocessor preprocessor(symbolTable);
    preprocessor.process(*tokenList);

    //Print tokens and clean up
    if (tokenList)
    {
        printTokenList(tokenList);
        delete tokenList;
    }

    return 0;
}
