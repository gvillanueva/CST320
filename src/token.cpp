/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/23/2015
 *              Modified, 4/23/2015
 * \ingroup     CST320 - Lab1c
 * \file        token.cpp
 *
 * \brief       Defines the methods of the Token class.
 */
#include "token.h"

Token::Token(std::string lexeme, std::string type)
    :m_Lexeme(lexeme), m_Type(type)
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
