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

/*!
 * \brief   The Token class contains a token's characters and type.
 */
class Token
{
public:
    Token(std::string chars, std::string type);
    std::string chars() const;
    std::string type() const;

private:
    std::string m_Chars;
    std::string m_Type;
};

#endif // TOKEN_H
