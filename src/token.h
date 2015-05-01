/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/23/2015
 *              Modified, 4/23/2015
 * \ingroup     CST320 - Lab1c
 * \file        token.h
 *
 * \brief       Declares the structure of the Token class.
 */
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

/*!
 * \brief   The Token class contains a token's value and type.
 */
class Token
{
public:
    Token(std::string value, std::string type);

    /// Gets the value of the token's characters.
    std::string value() const;

    /// Gets the token's type.
    std::string type() const;

private:
    /// The characters composing a token.
    std::string m_Value;

    /// The type of token (keyword, preprocessor, etc).
    std::string m_Type;
};

#endif // TOKEN_H
