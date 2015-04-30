#include "lex.h"
#include "token.h"
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

/***********************************************************************************
*	  Entry:	Empty symbol table.
*	   Exit:	Symbol table populated with C- keywords.
*	Purpose:	Adds C- keywords to symbol table. Indicates whether or not a parsed
*               identifier is a keyword.
***********************************************************************************/
Lex::Lex(const SymbolTable &symbolTable)
    :m_SymbolTable(symbolTable)
{
}

/***********************************************************************************
*	  Entry:	n/a
*	   Exit:	Default destructors for data members completed.
*	Purpose:	Default destructor, added for completeness.
***********************************************************************************/
Lex::~Lex()
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

    for (size_t i = 0; i < IncludeStack.size(); i++)
        if (IncludeStack[i] == filename)
            return NULL;

    IncludeStack.push_back(filename);
    std::ifstream stream(filename);

    TokenList *tokens = Analyze(stream, filename);

    if (!IncludeStack.empty())
        IncludeStack.pop_back();

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
                    state = PREPROCESSOR;
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
                    if (token == "define" || token == "include" ||
                        token == "ifdef" || token == "ifndef" ||
                        token == "undef" || token == "endif")
                    {
                        tokens->add(new Token(token, "PREPROCESSOR"));
                        token.clear();
                        state = START;
                    }

                    ///#define token, need to get the expansion
//                    if (token == "define")
//                    {
//                        while (isspace(ch))
//                            istream.get(ch);
//                        istream.putback(ch);
//                        state = PREPROCESSOR_DEFINE;
//                    }
//                    ///#include token, need to get the filename/search path
//                    else if (token == "include")
//                    {
//                        while ((ch != '\"' || ch != '<') && isspace(ch) && ch != '\n')
//                            istream.get(ch);
//                        if (ch == '\"')
//                            state = PREPROCESSOR_LOCAL_INCLUDE;
//                        else
//                        {
//                            printf("#include expects \"FILENAME\". %s(%i)\n", filename, line);
//                            state = START;
//                        }
//                    }
//                    ///#ifdef token, get macro name
//                    else if (token == "ifdef")
//                    {
//                        while (isspace(ch) && ch != '\n')
//                            istream.get(ch);
//                        while (isalnum(ch) || ch == '_')
//                        {
//                            macro.push_back(ch);
//                            istream.get(ch);
//                        }
//                        ///If macro exists, continue happily
//                        if (SymbolTable["#"+macro] == "PREPROCESSOR_FLAG")
//                            state = RUN_TO_ENDLINE;
//                        else
//                        ///Run until #endif
//                            state = PREPROCESSOR_EXCLUDE;
//                        macro.clear();
//                    }
//                    ///#ifndef token, get macro name
//                    else if (token == "ifndef")
//                    {
//                        while (isspace(ch) && ch != '\n')
//                            istream.get(ch);
//                        while (isalnum(ch) || ch == '_')
//                        {
//                            macro.push_back(ch);
//                            istream.get(ch);
//                        }
//                        ///If macro doesn't exists, continue happily
//                        if (SymbolTable["#"+macro] == "PREPROCESSOR_FLAG")
//                            state = PREPROCESSOR_EXCLUDE;
//                        else
//                        ///Run until #endif
//                            state = RUN_TO_ENDLINE;
//                        macro.clear();
//                    }
//                    ///#endif, run to endline
//                    else if (token == "endif")
//                    {
//                        printf("WARNING: #endif with no beginning #ifdef or #ifndef\n");
//                        state = RUN_TO_ENDLINE;
//                    }
//                    else
//                    {
//                        printf("Invalid preprocessing directive %s. %s(%i)\n", token.c_str(), filename, line);
//                        state = RUN_TO_ENDLINE;
//                    }
//                    token.clear();
                }
            break;
//            case PREPROCESSOR_DEFINE:
//                if (isalnum(ch) || ch == '_')
//                    macro.push_back(ch);
//                else
//                    if (ch == '\n')
//                    {
//                        SymbolTable["#"+macro] = "PREPROCESSOR_FLAG";
//                        macro.clear();
//                        state = START;
//                    }
//                    else if (!isspace(ch))
//                    {
//                        printf("Macro names must be identifiers. %s(%i)\n", filename, line);
//                        macro.clear();
//                        state = RUN_TO_ENDLINE;
//                    }
//                    else
//                    {
//                        while (isspace(ch))
//                            istream.get(ch);
//                        state = PREPROCESSOR_MACRO;
//                    }
//                break;
//            ///Currently allows only object-like macros
//            case PREPROCESSOR_MACRO:
//                if (isalnum(ch))
//                    expansion.push_back(ch);
//                else
//                {
//                    if (isspace(ch))
//                    {
//                        SymbolTable[macro] = expansion;
//                        istream.putback(ch);
//                    }
//                    else
//                        printf("Macro expansions may only be alphanumeric. %s(%i)\n", filename, line);
//                    macro.clear();
//                    expansion.clear();
//                    state = RUN_TO_ENDLINE;
//                }
//                break;
//            ///Concatenate the current file path to the #include filename
//            case PREPROCESSOR_LOCAL_INCLUDE:
//                if (ch == '\"')
//                {
//                    char *directory = NULL;

//                    // If filename undefined, use the current working directory
//                    if (!filename)
//                        directory = getcwd(NULL, 0);
//                    else
//                    {
//                        directory = const_cast<char*>(strrchr(filename, '\\'));
//                        if (directory)
//                        {
//                            *directory = '\0';
//                            //inclFile = directory;
//                            *directory = '\\';
//                        }
//                    }
//                    TokenList *includedTokens = tokenizeFile(token.c_str());
//                    includedTokens->move(tokens);
//                    delete includedTokens;
//                    token.clear();
//                    state = RUN_TO_ENDLINE;
//                }
//                else if (ch == '\n')
//                {
//                    printf("#include expects \"FILENAME\". %s(%i)\n", filename, line);
//                    token.clear();
//                    state = START;
//                }
//                else
//                    token.push_back(ch);
//                break;
//            ///Exclude source code until #endif
//            case PREPROCESSOR_EXCLUDE:
//                if (ch != '#')
//                {
//                    while (!isspace(ch))
//                    {
//                        istream.get(ch);
//                        token.push_back(ch);
//                    }
//                    if (token == "endif")
//                        state = RUN_TO_ENDLINE;
//                    else
//                        token.clear();
//                }
//                break;
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
