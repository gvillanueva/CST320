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

Token::Token(std::string chars, std::string type)
    :m_Chars(chars), m_Type(type)
{
}

/*!
 * \brief Gets the value of the token's characters.
 * \return
 */
std::string Token::chars() const
{
    return m_Chars;
}

std::string Token::type() const
{
    return m_Type;
}
