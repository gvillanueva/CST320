/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/30/2015
 *              Modified, 4/30/2015
 * \ingroup     CST320 - Lab1c
 * \file        tokenlist.h
 *
 * \brief       Declares the structure of the TokenNode and TokenList classes.
 */
#ifndef TOKENLIST_H
#define TOKENLIST_H

class Token;

/*!
 * \brief   The TokenNode class can be entered into a token list. It contains a
 *          pointer to the next and previous nodes in the list, as well as a
 *          pointer to the token is catalogs.
 */
class TokenNode
{
public:
    TokenNode(Token *token);
    Token *token() const;
    TokenNode* prevTokenNode() const;
    void setPrevTokenNode(TokenNode *tokenNode);
    TokenNode* nextTokenNode() const;
    void setNextTokenNode(TokenNode *tokenNode);

private:
    /// Pointer to the token indexed by the node.
    Token *m_Token;

    /// The previous token in the list.
    TokenNode *m_PrevTokenNode;

    /// The next token in the list.
    TokenNode *m_NextTokenNode;
};

/*!
 * \brief A self-cleaning list of Tokens.
 */
class TokenList
{
public:
    TokenList();
    ~TokenList();

    /// Returns the length of the list of tokens.
    int length() const;

    /// Adds a token to the list.
    void add(Token *token);

    /// Moves the entire contents of this list into another.
    void move(TokenList *destTokenList, Token *before=0);

    /// Removes a token from the list.
    void remove(Token *token);

    /// Inserts a token in the list after the given token.
    void insertAfter(Token *token, Token *after);

    /// Inserts a token in the list before the given token.
    void insertBefore(Token *token, Token *before);
    Token* operator[](int index);

private:
    /// Find a token node in the list.
    TokenNode* find(Token *token);

    /// The first token in the list.
    TokenNode *m_FirstTokenNode;

    /// The last token in the list.
    TokenNode *m_LastTokenNode;

    /// The length of the list.
    int m_Length;
};


#endif // TOKENLIST_H
