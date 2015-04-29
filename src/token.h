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

class TokenNode
{
public:
    TokenNode(Token *token)
        :m_Token(token), m_PrevTokenNode(0), m_NextTokenNode(0)
    {
    }

    Token *token() const
    {
        return m_Token;
    }

    TokenNode* prevTokenNode() const
    {
        return m_PrevTokenNode;
    }

    void setPrevTokenNode(TokenNode *tokenNode)
    {
        m_PrevTokenNode = tokenNode;
    }

    TokenNode* nextTokenNode() const
    {
        return m_NextTokenNode;
    }

    void setNextTokenNode(TokenNode *tokenNode)
    {
        m_NextTokenNode = tokenNode;
    }

private:
    Token *m_Token;
    TokenNode *m_PrevTokenNode;
    TokenNode *m_NextTokenNode;
};

/*!
 * \brief A self-cleaning list of Tokens.
 */
class TokenList
{
public:
    TokenList()
        :m_FirstTokenNode(0), m_LastTokenNode(0), m_Length(0)
    {
    }

    ~TokenList()
    {
        TokenNode *curTokenNode = m_FirstTokenNode;
        TokenNode *nextTokenNode = NULL;
        while (curTokenNode)
        {
            nextTokenNode = curTokenNode->nextTokenNode();
            delete curTokenNode;
            curTokenNode = nextTokenNode;
        }
        m_Length = 0;
    }

    int length() const
    {
        return m_Length;
    }

    void add(Token *token)
    {
        if (!token)
            return;

        TokenNode *newTokenNode = new TokenNode(token);

        if (!m_FirstTokenNode)
        {
            m_FirstTokenNode = newTokenNode;
            m_LastTokenNode = newTokenNode;
        }
        else
        {
            newTokenNode->setPrevTokenNode(m_LastTokenNode);
            m_LastTokenNode->setNextTokenNode(newTokenNode);
            m_LastTokenNode = newTokenNode;
        }
        m_Length++;
    }

    /*!
     * \brief Moves the contents of this token list to another.
     * \param destTokenList The destination for the tokens.
     *
     * Moves all tokens from this list into \p destTokenList. Tokens are
     * inserted before the \p before token in \p destTokenList. If \p before is
     * NULL, tokens are appended to the end of destTokenList.
     */
    void move(TokenList *destTokenList, Token *before=NULL)
    {
        // Ensure destination list and index exist
        // and that this list contains tokens
        if (!destTokenList || !before || !m_FirstTokenNode)
            return;

        // Insert at tail of the list
        if (!before)
        {
            // If destination list is empty, simply set first/last token nodes
            if (!destTokenList->m_FirstTokenNode)
            {
                destTokenList->m_FirstTokenNode = m_FirstTokenNode;
                destTokenList->m_LastTokenNode = m_LastTokenNode;
            }
            else
                destTokenList->m_LastTokenNode->setNextTokenNode(m_FirstTokenNode);
        }
        else
        {
            TokenNode *beforeTokenNode = destTokenList->find(before);
            TokenNode *prevTokenNode = beforeTokenNode->prevTokenNode();
            prevTokenNode->setNextTokenNode(m_FirstTokenNode);
            beforeTokenNode->setPrevTokenNode(m_LastTokenNode);
        }

        // Transfer ownership of tokens to destination
        m_FirstTokenNode = NULL;
        m_LastTokenNode = NULL;

        m_Length += destTokenList->m_Length;
    }

    void remove(Token *token)
    {
        if (!token)
            return;

        TokenNode *tokenNode = find(token);

        if (tokenNode)// Token found, remove it
        {
            TokenNode *prevTokenNode = tokenNode->prevTokenNode(),
                      *nextTokenNode = tokenNode->nextTokenNode();
            prevTokenNode->setNextTokenNode(nextTokenNode);
            nextTokenNode->setPrevTokenNode(prevTokenNode);
            delete tokenNode;
        }
        m_Length--;
    }

    void insertAfter(Token *token, Token *after)
    {
        if (!token || !after)
            return;

        TokenNode *newTokenNode = new TokenNode(token);
        TokenNode *afterNode = find(after);

        if (afterNode)
        {
            TokenNode *nextTokenNode = afterNode->nextTokenNode();
            if (nextTokenNode)
            {
                nextTokenNode->setPrevTokenNode(newTokenNode);
                newTokenNode->setNextTokenNode(nextTokenNode);
            }
            newTokenNode->setPrevTokenNode(afterNode);
            afterNode->setNextTokenNode(newTokenNode);
        }
        m_Length++;
    }

    void insertBefore(Token *token, Token *before)
    {
        if (!token || !before)
            return;

        TokenNode *newTokenNode = new TokenNode(token);
        TokenNode *beforeNode = find(before);

        if (beforeNode)
        {
            TokenNode *prevTokenNode = beforeNode->prevTokenNode();
            if (prevTokenNode)
            {
                prevTokenNode->setNextTokenNode(newTokenNode);
                newTokenNode->setPrevTokenNode(prevTokenNode);
            }
            newTokenNode->setNextTokenNode(beforeNode);
            beforeNode->setPrevTokenNode(newTokenNode);
        }
        m_Length++;
    }

    Token* operator[](int index)
    {
        if (index < 0 || index > m_Length)
            return NULL;

        TokenNode *tokenNode = m_FirstTokenNode;
        for (int i = 0; i < index; i++)
            tokenNode = m_FirstTokenNode->nextTokenNode();
        return tokenNode->token();
    }

private:
    TokenNode* find(Token *token)
    {
        TokenNode *curTokenNode = m_FirstTokenNode;
        while (curTokenNode && curTokenNode->token() != token)
            curTokenNode = curTokenNode->nextTokenNode();
        return curTokenNode;
    }

    TokenNode *m_FirstTokenNode;
    TokenNode *m_LastTokenNode;
    int m_Length;
};

#endif // TOKEN_H
