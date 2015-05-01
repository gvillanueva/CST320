/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/12/2015
 *              Modified, 4/13/2015
 * \ingroup     CST320 - Lab1a
 * \file        symbol.h
 *
 * \brief       Declares the structure of the Symbol and SymbolPtr classes.
 */
#ifndef SYMBOL_H
#define SYMBOL_H

/*!
 * \brief The E_TYPE enum identifies the type of symbol.
 */
enum E_TYPE
{
    ET_VOID,
    ET_CHAR,
    ET_SHORT,
    ET_INTEGER,
    ET_LONG
};

/*!
 * \brief The E_USE enum identifies the use for the symbol.
 */
enum E_USE
{
    EU_KEYWORD,
    EU_VARIABLE,
    EU_FUNCTION,
    EU_TYPE,
    EU_CONSTANT,
    EU_MACRO
};

/*!
 * \brief   The Symbol class contains data to represent a symbol in the symbol
 *          table. Do NOT delete a pointer to a symbol once it has been added to
 *          a symbol table or scope. This may corrupt the scope.
 */
class Symbol
{
public:
    /// Creates a new symbol object.
    Symbol(const char *name, E_TYPE type, E_USE use, const char *constData);

    /// Deletes a symbol object.
    ~Symbol();

    /// Getters and setters
    char* name() const;
    E_TYPE type() const;
    E_USE use() const;
    char* constData() const;
    Symbol *prevSymbol() const;
    void setPrevSymbol(Symbol *prevSymbol);
    Symbol *nextSymbol() const;
    void setNextSymbol(Symbol *nextSymbol);

private:
    /// Pointer to the next symbol in the list
    Symbol *m_pNextSymbol;

    /// Pointer to the previous symbol in the list.
    Symbol *m_pPrevSymbol;

    /// The name of the symbol.
    char *m_pName;

    /// The type of the symbol.
    E_TYPE m_Type;

    /// The use for the symbol.
    E_USE  m_Use;

    /// The constant data the symbol may contain.
    char *m_ConstData;
};

/*!
 * \brief The SymbolPtr class wraps a pointer for the symbol type.
 *
 * To prevent the user from messing anything up, the SymbolPtr class wraps the
 * actual Symbol class. This keeps actual symbols within the implementation of
 * the symbol table, and prevents any tom foolery like deleting a raw pointer to
 * a symbol and mucking up the whole symbol table.
 */
class SymbolPtr
{
public:
    SymbolPtr(Symbol *pSymbol)
        :m_Symbol(pSymbol)
    {
    }

    /// Getters
    char *name() { return m_Symbol->name(); }
    E_TYPE type() { return m_Symbol->type(); }
    E_USE use() { return m_Symbol->use(); }
    void *constData() { return m_Symbol->constData(); }

    /// Used to determine if the object is a wrapper around NULL.
    bool isNull() { return m_Symbol == 0 ? true : false; }

    /// Returns whether this wrapper's pointer matches /p b's.
    bool operator ==(const SymbolPtr& b)
    {
        return this->m_Symbol == b.m_Symbol;
    }

    /// Returns whether this wrapper's point does not match /p b's.
    bool operator !=(const SymbolPtr& b)
    {
        return this->m_Symbol != b.m_Symbol;
    }

private:
    /// Pointer to the symbol being wrapped.
    Symbol* m_Symbol;
};

#endif//SYMBOL_H
