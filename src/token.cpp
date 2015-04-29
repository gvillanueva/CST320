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

Token::Token(std::string value, std::string type)
    :m_Value(value), m_Type(type)
{
}

/*!
 * \brief Gets the value of the token's characters.
 * \return String representing the token characters.
 */
std::string Token::value() const
{
    return m_Value;
}

/*!
 * \brief Gets the token's type.
 * \return String identifying the token type.
 */
std::string Token::type() const
{
    return m_Type;
}
