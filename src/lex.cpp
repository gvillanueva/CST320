#include "lex.h"
#include "token.h"
#include "tokenlist.h"
#include "symboltable.h"
#include <cstring>
#include <cstdio>
#include <stdio.h>
#include <sstream>
#include <fstream>

// For preprocessor #include
#ifdef _WIN32
    #include <direct.h>
    #define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
    #include <unistd.h>
#endif

/*!
 * \brief Instatiates a new Lex object with a SymbolTable reference.
 * \param symbolTable   Reference to an instantiated symbol table. Doesn't have
 *                      to be populated, but it helps.
 */
Lex::Lex(const SymbolTable &symbolTable)
    :m_SymbolTable(symbolTable)
{
}

/*!
 * \brief Tokenizes an input string.
 * \param Input string to the lexical analyzer.
 * \return A vector of tokens parsed from the input string.
 */
TokenList *Lex::tokenizeString(const char *input)
{
    // Return an empty vector if input is NULL
    if (!input)
        return NULL;

    std::istringstream stream(input);
    return Analyze(stream);
}

/*!
 * \brief Lex::tokenizeFile
 * \param filename
 * \return NULL on invalid filename or file already included.
 */
TokenList *Lex::tokenizeFile(const char *filename)
{
    // Ensure filename is defined
    if (!filename)
        return NULL;

    std::ifstream stream(filename);
    TokenList *tokens = Analyze(stream, filename);

    return tokens;
}

/***********************************************************************************
*	  Entry:	Populated symbol table
*	   Exit:	Output generated to STDOUT. If analysis is error-free, output is
*               ready to be fed into the Syntax Analyzer.
*	Purpose:	Performs lexical analysis/preprocessor directives on a given file.
*               Input from the specified filename should contain no strange Unicode
*               symbols or an error will be generated. Input should also contain no
*               lexemes outside of the C- grammar.
*               Output generated contains one token and its lexeme per line. Output
*               is formatted; lexeme comes first with a width of 12, token comes
*               second.
***********************************************************************************/
TokenList *Lex::Analyze(std::istream &istream, const char *filename)
{
    LEX_STATE state = START;
    int line = 1; ///Variables to track the current line and column position
    char ch  = 0; ///Current symbol being parsed by the lexical analyzer
    //int ret  = 0; ///Return value, 0 for success, anything else for errors

    std::string token; ///Token being parsed
    std::string expansion; ///#define macro expansion
    std::string macro; ///#define macro name
    std::string error; ///Error string
    TokenList *tokens = new TokenList;

    while (istream.get(ch))
    {
        switch (state)
        {
#ifndef START
            case START:
                ///If the  beginning of a token is a #, it is a preprocessor directive
                if (ch == '#')
                {
                    state = PREPROCESSOR;
                    token.push_back(ch);
                }
                ///If the beginning of a token is a number, it is a constant
                else if (isdigit(ch))
                {
                    state = CONSTANT;
                    token.push_back(ch);
                }
                ///If the beginning of a token is a letter, it could be a keyword or identifier
                else if (isalpha(ch))
                {
                    state = KEYWORD_OR_ID;
                    token.push_back(ch);
                }
                ///The following operator symbols are unambiguous, print immediately
                else if (ch == '(' ||
                         ch == ')' ||
                         ch == '{' ||
                         ch == '}' ||
                         ch == '[' ||
                         ch == ']' ||
                         ch == ';' ||
                         ch == ',')
                {
                    tokens->add(new Token(std::string(1, ch), std::string(1, ch)));
                    printf("%-*c%c\n", OUTPUT_WIDTH, ch, ch);
                }
                else if (ch == '*' ||
                         ch == '%')
                {
                    tokens->add(new Token(std::string(1, ch), "MULOP"));
                    printf("%-*c%s\n", OUTPUT_WIDTH, ch, "MULOP");
                }
                ///The following operator symbols are ambiguous (ex: + or +=)
                else if (ch == '+')
                    state = OP_ADD;
                else if (ch == '-')
                    state = OP_SUB;
                else if (ch == '/')
                    state = OP_DIV;
                else if (ch == '&')
                    state = OP_AND;
                else if (ch == '|')
                    state = OP_OR;
                else if (ch == '<')
                    state = OP_LEFT;
                else if (ch == '>')
                    state = OP_RIGHT;
                else if (ch == '!')
                    state = OP_NOT;
                else if (ch == '=')
                    state = OP_ASSIGN;
                else if (ch == '"')
                {
                    token.push_back(ch);
                    state = STRING;
                }
                ///Ignore whitespace, increment col counter, increment line counter on newlines
                else if (isspace(ch))
                {
                    if (ch == '\n' || ch == '\r' || ch == '\f')
                        line++;
                }
                else
                    printf("Illegal symbol %c encountered at %s(%i).\n", ch, filename, line);
            break;
#endif
            case STRING:
                if (ch == '"')
                {
                    token.push_back(ch);
                    tokens->add(new Token(token, "STRING"));
                    token.clear();
                    state = START;
                }
                else if (isprint(ch))
                {
                    token.push_back(ch);
                    if (ch == '\\')
                        state = STRING_ESCAPE;
                }
                else
                    printf("Illegal symbol %c encountered at %s(%i).\n", ch, filename, line);
            break;
            case STRING_ESCAPE:
                if (isprint(ch))
                    state = STRING;
                else
                    printf("Illegal symbol %c encountered at %s(%i).\n", ch, filename, line);
            break;
#ifndef TWO_OR_THREE_PART_OPS
            ///Operator tokens that may or may not be more than one character
            case OP_ADD:
            {
                if (ch == '=')
                {
                    tokens->add(new Token("+=", "ADDASSIGN"));
                    printf("%-*s+=\n", OUTPUT_WIDTH, "ADDASSIGN");
                }
                else
                {
                    tokens->add(new Token("+", "ADDOP"));
                    printf("%-*s+\n", OUTPUT_WIDTH, "ADDOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_SUB:
            {
                if (ch == '=')
                {
                    tokens->add(new Token("-=", "SUBASSIGN"));
                    printf("%-*s-=\n", OUTPUT_WIDTH, "SUBASSIGN");
                }
                else
                {
                    tokens->add(new Token("-", "ADDOP"));
                    printf("%-*s-\n", OUTPUT_WIDTH, "ADDOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_MUL:
            {
                if (ch == '=')
                {
                    tokens->add(new Token("*=", "MULASSIGN"));
                    printf("%-*s*=\n", OUTPUT_WIDTH, "MULASSIGN");
                }
                else
                {
                    tokens->add(new Token("*", "MULOP"));
                    printf("%-*s*\n", OUTPUT_WIDTH, "MULOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_DIV:
            {
                if (ch == '/')
                    state = COMMENTS_SINGLE_LINE;
                else if (ch == '=')
                {
                    tokens->add(new Token("/=", "MULASSIGN"));
                    printf("%-*s/=\n", OUTPUT_WIDTH, "MULASSIGN");
                }
                else
                {
                    tokens->add(new Token("/", "MULOP"));
                    printf("%-*s/\n", OUTPUT_WIDTH, "MULOP");
                    istream.putback(ch);
                    state = START;
                }
            }
            break;
            case OP_MOD:
            {
                if (ch == '=')
                {
                    tokens->add(new Token("%=", "MODASSIGN"));
                    printf("%-*s%%=\n", OUTPUT_WIDTH, "MODASSIGN");
                }
                else
                {
                    tokens->add(new Token("%", "MULOP"));
                    printf("%-*s%%\n", OUTPUT_WIDTH, "MULOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_AND:
            {
                if (ch == '&')
                {
                    tokens->add(new Token("&&", "LOGICOP"));
                    printf("%-*s&&\n", OUTPUT_WIDTH, "LOGICOP");
                }
                else
                {
                    printf("Encountered illegal token &. %s(%i)\n", filename, line);
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_OR:
            {
                if (ch == '|')
                {
                    tokens->add(new Token("||", "LOGICOP"));
                    printf("%-*s||\n", OUTPUT_WIDTH, "LOGICOP");
                }
                else
                {
                    printf("Encountered illegal token |. %s(%i)\n", filename, line);
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_LEFT:
            {
                if (ch == '=')
                {
                    tokens->add(new Token("<=", "RELOP"));
                    printf("%-*s<=\n", OUTPUT_WIDTH, "RELOP");
                }
                else
                {
                    tokens->add(new Token("<", "RELOP"));
                    printf("%-*s<\n", OUTPUT_WIDTH, "RELOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_RIGHT:
            {
                if (ch == '=')
                {
                    tokens->add(new Token(">=", "RELOP"));
                    printf("%-*s>=\n", OUTPUT_WIDTH, "RELOP");
                }
                else
                {
                    tokens->add(new Token(">", "RELOP"));
                    printf("%-*s>\n", OUTPUT_WIDTH, "RELOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_NOT:
            {
                if (ch == '=')
                {
                    tokens->add(new Token("!=", "RELOP"));
                    printf("%-*s!=\n", OUTPUT_WIDTH, "RELOP");
                }
                else
                {
                    tokens->add(new Token("!", "UNARYOP"));
                    printf("%-*s!\n", OUTPUT_WIDTH, "UNARYOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
            case OP_ASSIGN:
            {
                if (ch == '=')
                {
                    tokens->add(new Token("==", "RELOP"));
                    printf("%-*s==\n", OUTPUT_WIDTH, "RELOP");
                }
                else
                {
                    tokens->add(new Token("=", "ASSIGNOP"));
                    printf("%-*s=\n", OUTPUT_WIDTH, "ASSIGNOP");
                    istream.putback(ch);
                }
                state = START;
            }
            break;
#endif
#ifndef COMMENTS
            ///For comments we just run to the end of the line
            case COMMENTS_SINGLE_LINE:
                if (ch == '\n')
                {
                    line++;
                    state = START;
                }
                break;
#endif
#ifndef PREPROCESSOR
            ///Found a '#', now we're gonna read the preprocessor directive
            case PREPROCESSOR:
                if (isalpha(ch))
                    token.push_back(ch);
                else
                {
                    if (token == "#define" || token == "#include" ||
                        token == "#ifdef" || token == "#ifndef" ||
                        token == "#undef" || token == "#endif")
                    {
                        tokens->add(new Token(token, "PREPROCESSOR"));
                        token.clear();
                        state = START;
                    }
                }
            break;
#endif
#ifndef KEYWORD_OR_ID
            ///Until we find a number, we might still have a keyword
            case KEYWORD_OR_ID:
                if (isalpha(ch))
                {
                    if (token.length() < MAX_ID_LENGTH)
                        token.push_back(ch);
                }
                else
                    ///When we get a digit, we're looking at a number
                    if (isdigit(ch))
                    {
                        token.push_back(ch);
                        state = ID;
                    }
                    ///If anything else, it's the end of the identifier
                    else
                    {
                        ///If the token doesn't exist in the symbol table, it's a keyword
                        if (m_SymbolTable.findSymbol(token.c_str()).isNull())
                        {
                            tokens->add(new Token(token, "ID"));
                            printf("%-*s%s\n", OUTPUT_WIDTH, "ID", token.c_str());
                        }
                        else
                        {
                            tokens->add(new Token(token, "KEYWORD"));
                            printf("%-*s%s\n", OUTPUT_WIDTH, "KEYWORD", token.c_str());
                        }
                        ///Reanalyze the non-alphanumeric
                        token.clear();
                        istream.putback(ch);
                        state = START;
                    }
                break;
            ///Found a number, we know it's an identifier
            case ID:
                ///The rest of the characters must be digits
                if (isdigit(ch))
                {
                    if (token.length() < MAX_ID_LENGTH)
                        token.push_back(ch);
                }
                else if (isalpha(ch))
                {
                    error = "Letters cannot follow digits in identifier names";
                    token.push_back(ch);
                }
                else
                {
                    if (error.length())
                    {
                        printf("%s - %s. %s(%i)\n", token.c_str(), error.c_str(), filename, line);
                        error.clear();
                    }
                    else
                    {
                        tokens->add(new Token(token, "ID"));
                        printf("%-*s%s\n", OUTPUT_WIDTH, "ID", token.c_str());
                    }
                    token.clear();
                    istream.putback(ch);
                    state = START;
                }
                break;
#endif
#ifndef CONSTANT
            ///Found a number; any following character must be a digit
            case CONSTANT:
                if (isdigit(ch))
                    token.push_back(ch);
                else if (isalpha(ch))
                {
                    error = "Constants cannot contain characters";
                    token.push_back(ch);
                }
                else
                {
                    if (error.length())
                    {
                        printf("%s - %s. %s(%i)\n", token.c_str(), error.c_str(), filename, line);
                        error.clear();
                    }
                    else
                    {
                        tokens->add(new Token(token, "CONSTANT"));
                        printf("%-*s%s\n", OUTPUT_WIDTH, "CONSTANT", token.c_str());
                    }
                    token.clear();
                    istream.putback(ch);
                    state = START;
                }
                break;
#endif
            case RUN_TO_ENDLINE:
                if (ch == '\n')
                {
                    line++;
                    state = START;
                }
                break;
        }
    }
    ///If we're not in START at EOF, we have a token that failed to be recognized
    if (state != START)
        switch (state)
        {
            default:
                //ret = 1;
                break;
        }

    return tokens;
}
