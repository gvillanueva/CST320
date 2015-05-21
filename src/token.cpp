/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/23/2015
 *              Modified, 5/20/2015
 * \ingroup     CST320 - Lab2b
 * \file        token.cpp
 *
 * \brief       Defines the methods of the Token class.
 */
#include "token.h"

/*!
 * \brief Instantiates a Token object.
 * \param lexeme    The value of the token.
 * \param type      The generic type of the token.
 * \param source    The source file containing the token.
 * \param line      The line of the source file where the token is found.
 * \param column    The column of the source file where the token is found.
 */
Token::Token(std::string lexeme, std::string type, std::string source, int line,
             int column)
    : m_Lexeme(lexeme), m_Type(type), m_Source(source), m_Line(line),
      m_Column(column)
{
}

/*!
 * \brief Gets the specific instance of the generic token type.
 * \return String representing the token's characters.
 */
std::string Token::lexeme() const
{
    return m_Lexeme;
}

/*!
 * \brief Gets the token's generic type.
 * \return String identifying the token type.
 */
std::string Token::type() const
{
    return m_Type;
}

/*!
 * \brief Gets the source file containing the token.
 * \return String identifying the source file.
 */
std::string Token::source() const
{
    return m_Source;
}

/*!
 * \brief Gets the line of the source file where the token is found.
 * \return int identifying line number of source file where token is found.
 */
int Token::line() const
{
    return m_Line;
}

/*!
 * \brief Gets the column of the source file where the token begins.
 * \return int identifying column number of source file where token is found.
 */
int Token::column() const
{
    return m_Column;
}
