/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/30/2015
 *              Modified, 4/30/2015
 * \ingroup     CST320 - Lab1c
 * \file        tokenlist.cpp
 *
 * \brief       Defines the structure of the TokenNode and TokenList classes.
 */
#include "tokenlist.h"

/*!
 * \brief Instantiates a TokenNode object, initializes member variables.
 * \param token
 */
TokenNode::TokenNode(Token *token)
    :m_Token(token), m_PrevTokenNode(0), m_NextTokenNode(0)
{
}

/*!
 * \brief Gets the token contained in the TokenNode.
 * \return Pointer to the token containined by the token node.
 */
Token* TokenNode::token() const
{
    return m_Token;
}

/*!
 * \brief Gets a pointer to the node's previous TokenNode.
 * \return The previous TokenNode in the list. Can be NULL.
 */
TokenNode* TokenNode::prevTokenNode() const
{
    return m_PrevTokenNode;
}

/*!
 * \brief Sets the TokenNode's previous node in the list.
 * \param tokenNode The token node to be set as previous.
 */
void TokenNode::setPrevTokenNode(TokenNode *tokenNode)
{
    m_PrevTokenNode = tokenNode;
}

/*!
 * \brief Gets a pointer to the node's next TokenNode.
 * \return The next TokenNode in the list.  Can be NULL.
 */
TokenNode* TokenNode::nextTokenNode() const
{
    return m_NextTokenNode;
}

/*!
 * \brief Sets the TokenNode's next node in the list.
 * \param tokenNode The token node to set as next.
 */
void TokenNode::setNextTokenNode(TokenNode *tokenNode)
{
    m_NextTokenNode = tokenNode;
}


/*!
 * \brief Instantiates a new token list object, initializing member values.
 */
TokenList::TokenList()
    :m_FirstTokenNode(0), m_LastTokenNode(0), m_Length(0)
{
}

/*!
 * \brief Destroys a token list.
 */
TokenList::~TokenList()
{
    TokenNode *curTokenNode = m_FirstTokenNode;
    TokenNode *nextTokenNode = 0;
    while (curTokenNode)
    {
        nextTokenNode = curTokenNode->nextTokenNode();
        delete curTokenNode;
        curTokenNode = nextTokenNode;
    }
    m_Length = 0;
}

/*!
 * \brief Gets the number of tokens in the token list.
 * \return The length of the token list.
 */
int TokenList::length() const
{
    return m_Length;
}

/*!
 * \brief Adds a token to the list.
 * \param token The token to add to the list.
 */
void TokenList::add(Token *token)
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
void TokenList::move(TokenList *destTokenList, Token *before)
{
    // Ensure destination list and index exist
    // and that this list contains tokens
    if (!destTokenList || !before || !m_FirstTokenNode)
        return;

    // Ensure before token is in the destination token's list
    TokenNode *beforeTokenNode = destTokenList->find(before);
    if (!beforeTokenNode)
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
        TokenNode *prevTokenNode = beforeTokenNode->prevTokenNode();

        // Found node is head token
        if (!prevTokenNode)
            destTokenList->m_FirstTokenNode = m_FirstTokenNode;
        else
            prevTokenNode->setNextTokenNode(m_FirstTokenNode);
        m_LastTokenNode->setNextTokenNode(beforeTokenNode);
        beforeTokenNode->setPrevTokenNode(m_LastTokenNode);
    }

    // Transfer ownership of tokens to destination
    m_FirstTokenNode = 0;
    m_LastTokenNode = 0;

    destTokenList->m_Length += m_Length;
}

/*!
 * \brief Removes a token from the list.
 * \param token The token to remove.
 */
void TokenList::remove(Token *token)
{
    if (!token)
        return;

    TokenNode *tokenNode = find(token);

    if (tokenNode)// Token found, remove it
    {
        TokenNode *prevTokenNode = tokenNode->prevTokenNode(),
                  *nextTokenNode = tokenNode->nextTokenNode();

        // Found node is head token
        if (!prevTokenNode)
            m_FirstTokenNode = nextTokenNode;
        else
            prevTokenNode->setNextTokenNode(nextTokenNode);

        // Found node is tail token
        if (!nextTokenNode)
            m_LastTokenNode = prevTokenNode;
        else
            nextTokenNode->setPrevTokenNode(prevTokenNode);

        delete tokenNode;
    }
    m_Length--;
}

/*!
 * \brief Inserts a new token in the list after the given token.
 * \param token     The token to insert in the list.
 * \param before    The token to insert after.
 */
void TokenList::insertAfter(Token *token, Token *after)
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

/*!
 * \brief Inserts a new token in the list before the given token.
 * \param token     The token to insert in the list.
 * \param before    The token to insert before.
 */
void TokenList::insertBefore(Token *token, Token *before)
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

/*!
 * \brief   Returns a token in the list at index \p index.
 * \param index The index of the token to return.
 * \return  The token at \p index, or NULL.
 */
Token* TokenList::operator[](int index)
{
    if (index < 0 || index > m_Length)
        return 0;

    TokenNode *tokenNode = m_FirstTokenNode;
    for (int i = 0; i < index; i++)
        tokenNode = tokenNode->nextTokenNode();
    return tokenNode->token();
}

/*!
 * \brief   Finds a token node in the list containing \p token.
 * \param token The token to find in the list.
 * \return The node containing \p token, or NULL.
 */
TokenNode* TokenList::find(Token *token)
{
    TokenNode *curTokenNode = m_FirstTokenNode;
    while (curTokenNode && curTokenNode->token() != token)
        curTokenNode = curTokenNode->nextTokenNode();
    return curTokenNode;
}
