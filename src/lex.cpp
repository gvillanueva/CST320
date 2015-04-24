#include "lex.h"
#include <cstring>
#include <cstdio>

/***********************************************************************************
*	  Entry:	Empty symbol table.
*	   Exit:	Symbol table populated with C- keywords.
*	Purpose:	Adds C- keywords to symbol table. Indicates whether or not a parsed
*               identifier is a keyword.
***********************************************************************************/
Lex::Lex()
{
    SymbolTable["bool"] = "keyword";
    SymbolTable["break"] = "keyword";
    SymbolTable["else"] = "keyword";
    SymbolTable["false"] = "keyword";
    SymbolTable["if"] = "keyword";
    SymbolTable["int"] = "keyword";
    SymbolTable["return"] = "keyword";
    SymbolTable["true"] = "keyword";
    SymbolTable["void"] = "keyword";
    SymbolTable["while"] = "keyword";
}

/***********************************************************************************
*	  Entry:	n/a
*	   Exit:	Default destructors for data members completed.
*	Purpose:	Default destructor, added for completeness.
***********************************************************************************/
Lex::~Lex()
{
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
int Lex::Analyze(const char *filename)
{
    LEX_STATE state = START;
    int line = 1; ///Variables to track the current line and column position
    int ch   = 0; ///Current symbol being parsed by the lexical analyzer
    int ret  = 0; ///Return value, 0 for success, anything else for errors

    FILE *fi = 0; ///File handler for reading source code

    std::string token; ///Token being parsed
    std::string expansion; ///#define macro expansion
    std::string macro; ///#define macro name
    std::string error; ///Error string

    if (filename)
	{
		fi = fopen(filename,"r");
		for (int i = 0; i < IncludeStack.size(); i++)
			if (IncludeStack[i] == filename)
				fi = 0;
		IncludeStack.push_back(filename);
		if (!fi)
		{
			printf("Error: Unable to open %s.\n", filename);
			ret = 1;
		}
		else
		{
			ch = fgetc(fi);
			while (!feof(fi))
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
							printf("%-*c%c\n", OUTPUT_WIDTH, ch, ch);
						else if (ch == '*' ||
								 ch == '%')
							printf("%-*c%s\n", OUTPUT_WIDTH, ch, "MULOP");
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
	#ifndef TWO_OR_THREE_PART_OPS
					///Operator tokens that may or may not be more than one character
					case OP_ADD:
					{
						if (ch == '=')
							printf("%-*s+=\n", OUTPUT_WIDTH, "ADDASSIGN");
						else
						{
							printf("%-*s+\n", OUTPUT_WIDTH, "ADDOP");
							fseek(fi, -1, SEEK_CUR);
						}
						state = START;
					}
					break;
					case OP_SUB:
					{
						if (ch == '=')
							printf("%-*s-=\n", OUTPUT_WIDTH, "SUBASSIGN");
						else
						{
							printf("%-*s-\n", OUTPUT_WIDTH, "ADDOP");
							fseek(fi, -1, SEEK_CUR);
						}
						state = START;
					}
					break;
					case OP_DIV:
					{
						if (ch == '/')
							state = COMMENTS_SINGLE_LINE;
						else
						{
							printf("%-*s/\n", OUTPUT_WIDTH, "MULOP");
							fseek(fi, -1, SEEK_CUR);
							state = START;
						}
					}
					break;
					case OP_AND:
					{
						if (ch == '&')
							printf("%-*s&&\n", OUTPUT_WIDTH, "LOGICOP");
						else
						{
							printf("Encountered illegal token &. %s(%i)\n", filename, line);
							fseek(fi, -1, SEEK_CUR);
						}
						state = START;
					}
					break;
					case OP_OR:
					{
						if (ch == '|')
							printf("%-*s||\n", OUTPUT_WIDTH, "LOGICOP");
						else
						{
							printf("Encountered illegal token |. %s(%i)\n", filename, line);
							fseek(fi, -1, SEEK_CUR);
						}
						state = START;
					}
					break;
					case OP_LEFT:
					{
						if (ch == '=')
							printf("%-*s<=\n", OUTPUT_WIDTH, "RELOP");
						else
						{
							printf("%-*s<\n", OUTPUT_WIDTH, "RELOP");
							fseek(fi, -1, SEEK_CUR);
						}
						state = START;
					}
					break;
					case OP_RIGHT:
					{
						if (ch == '=')
							printf("%-*s>=\n", OUTPUT_WIDTH, "RELOP");
						else
						{
							printf("%-*s>\n", OUTPUT_WIDTH, "RELOP");
							fseek(fi, -1, SEEK_CUR);
						}
						state = START;
					}
					break;
					case OP_NOT:
					{
						if (ch == '=')
							printf("%-*s!=\n", OUTPUT_WIDTH, "RELOP");
						else
						{
							printf("%-*s!\n", OUTPUT_WIDTH, "UNARYOP");
							fseek(fi, -1, SEEK_CUR);
						}
						state = START;
					}
					break;
					case OP_ASSIGN:
					{
						if (ch == '=')
							printf("%-*s==\n", OUTPUT_WIDTH, "RELOP");
						else
						{
							printf("%-*s=\n", OUTPUT_WIDTH, "ASSIGNOP");
							fseek(fi, -1, SEEK_CUR);
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
							///#define token, need to get the expansion
							if (token == "define")
							{
								while (isspace(ch))
									ch = fgetc(fi);
								fseek(fi, -1, SEEK_CUR);
								state = PREPROCESSOR_DEFINE;
							}
							///#include token, need to get the filename/search path
							else if (token == "include")
							{
								while ((ch != '\"' || ch != '<') && isspace(ch) && ch != '\n')
									ch = fgetc(fi);
								if (ch == '\"')
									state = PREPROCESSOR_LOCAL_INCLUDE;
								else
								{
									printf("#include expects \"FILENAME\". %s(%i)\n", filename, line);
									state = START;
								}
							}
							///#ifdef token, get macro name
							else if (token == "ifdef")
							{
								while (isspace(ch) && ch != '\n')
									ch = fgetc(fi);
								while (isalnum(ch) || ch == '_')
								{
									macro.push_back(ch);
									ch = fgetc(fi);
								}
								///If macro exists, continue happily
								if (SymbolTable["#"+macro] == "PREPROCESSOR_FLAG")
									state = RUN_TO_ENDLINE;
								else
								///Run until #endif
									state = PREPROCESSOR_EXCLUDE;
								macro.clear();
							}
							///#ifndef token, get macro name
							else if (token == "ifndef")
							{
								while (isspace(ch) && ch != '\n')
									ch = fgetc(fi);
								while (isalnum(ch) || ch == '_')
								{
									macro.push_back(ch);
									ch = fgetc(fi);
								}
								///If macro doesn't exists, continue happily
								if (SymbolTable["#"+macro] == "PREPROCESSOR_FLAG")
									state = PREPROCESSOR_EXCLUDE;
								else
								///Run until #endif
									state = RUN_TO_ENDLINE;
								macro.clear();
							}
							///#endif, run to endline
							else if (token == "endif")
							{
								printf("WARNING: #endif with no beginning #ifdef or #ifndef\n");
								state = RUN_TO_ENDLINE;
							}
							else
							{
								printf("Invalid preprocessing directive %s. %s(%i)\n", token.c_str(), filename, line);
								state = RUN_TO_ENDLINE;
							}
							token.clear();
						}
					break;
					case PREPROCESSOR_DEFINE:
						if (isalnum(ch) || ch == '_')
							macro.push_back(ch);
						else
							if (ch == '\n')
							{
								SymbolTable["#"+macro] = "PREPROCESSOR_FLAG";
								macro.clear();
								state = START;
							}
							else if (!isspace(ch))
							{
								printf("Macro names must be identifiers. %s(%i)\n", filename, line);
								macro.clear();
								state = RUN_TO_ENDLINE;
							}
							else
							{
								while (isspace(ch))
									ch = fgetc(fi);
								state = PREPROCESSOR_MACRO;
							}
						break;
					///Currently allows only object-like macros
					case PREPROCESSOR_MACRO:
						if (isalnum(ch))
							expansion.push_back(ch);
						else
						{
							if (isspace(ch))
							{
								SymbolTable[macro] = expansion;
								fseek(fi, -1, SEEK_CUR);
							}
							else
								printf("Macro expansions may only be alphanumeric. %s(%i)\n", filename, line);
							macro.clear();
							expansion.clear();
							state = RUN_TO_ENDLINE;
						}
						break;
					///Concatenate the current file path to the #include filename
					case PREPROCESSOR_LOCAL_INCLUDE:
						if (ch == '\"')
						{
							std::string inclFile;
							char *directory = const_cast<char*>(strrchr(filename, '\\'));
							if (directory)
							{
								*directory = '\0';
								inclFile = directory;
								*directory = '\\';
							}
							inclFile += filename;
							Analyze(token.c_str());
							token.clear();
							state = RUN_TO_ENDLINE;
						}
						else if (ch == '\n')
						{
							printf("#include expects \"FILENAME\". %s(%i)\n", filename, line);
							token.clear();
							state = START;
						}
						else
							token.push_back(ch);
						break;
					///Exclude source code until #endif
					case PREPROCESSOR_EXCLUDE:
						if (ch != '#')
						{
							while (!isspace(ch))
							{
								ch = fgetc(fi);
								token.push_back(ch);
							}
							if (token == "endif")
								state = RUN_TO_ENDLINE;
							else
								token.clear();
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
								if (SymbolTable.find(token) == SymbolTable.end())
									printf("%-*s%s\n", OUTPUT_WIDTH, "ID", token.c_str());
								else
									printf("%-*s%s\n", OUTPUT_WIDTH, "KEYWORD", token.c_str());
								///Reanalyze the non-alphanumeric
								token.clear();
								fseek(fi, -1, SEEK_CUR);
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
								printf("%-*s%s\n", OUTPUT_WIDTH, "ID", token.c_str());
							token.clear();
							fseek(fi, -1, SEEK_CUR);
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
								printf("%-*s%s\n", OUTPUT_WIDTH, "CONSTANT", token.c_str());
							token.clear();
							fseek(fi, -1, SEEK_CUR);
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
				///After all that hoopla, we simply get the next character
				ch = fgetc(fi);
			}
			///If we're not in START at EOF, we have a token that failed to be recognized
			if (state != START)
				switch (state)
				{
					default:
						ret = 1;
						break;
				}
		}
	}
    if (fi)
        fclose(fi);
	if (!IncludeStack.empty())
		IncludeStack.pop_back();
    return ret;
}
