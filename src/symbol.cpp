/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/12/2015
 *              Modified, 4/13/2015
 * \ingroup     CST320 - Lab1a
 * \file        symbol.cpp
 *
 * \brief       Defines the methods of the Symbol class.
 */
#include "symbol.h"
#include <string.h>

/*!
 * \brief   Creates a new symbol object and initializes its data members.
 * \param   name        The name of the symbol.
 * \param   type        The type of the symbol.
 * \param   use         The use for the symbol.
 * \param   constData   The constant data which the symbol may represent.
 */
Symbol::Symbol(const char *name, E_TYPE type, E_USE use, const char *constData)
    :m_pNextSymbol(0), m_pPrevSymbol(0), m_pName(0), m_ConstData(0)
{
    m_pName = new char[strlen(name) + 1];
#if defined(__GNUC__)
    strcpy(m_pName, name);
#elif defined(_MSC_VER)
    strcpy_s(m_pName, strlen(name) + 1, name);
#endif
    m_Type = type;
    m_Use = use;

    if (constData)
    {
        m_ConstData = new char[strlen(constData) + 1];
#if defined(__GNUC__)
        strcpy(m_ConstData, constData);
#elif defined(_MSC_VER)
        strcpy_s(m_ConstData, strlen(constData) + 1, constData);
#endif
    }
}

/*!
 * \brief Destroys a Symbol object.
 *
 * 3rd-party destruction of a symbol is potentially dangerous. Deletion of the
 * symbol of a list will corrupt the list. This could cause an entire scope of
 * symbols to become corrupted. Deletion of a symbol at any other position is
 * likely harmless, but could lead to costly mistakes.
 *
 * That said, the destructor frees memory allocated for the symbol. It also
 * updates its neighbors in the list of its departure. However, it cannot change
 * the list's head pointer, which is maintained by an external entity.
 */
Symbol::~Symbol()
{
    if (m_pName)
        delete []m_pName;
    if (m_ConstData)
        delete []m_ConstData;

    // Somewhere in the middle of a symbol list
    if (m_pPrevSymbol && m_pNextSymbol)
    {
        m_pPrevSymbol->setNextSymbol(m_pNextSymbol);
        m_pNextSymbol->setPrevSymbol(m_pPrevSymbol);
    }
    // At the tail of a symbol list
    else if (m_pPrevSymbol)
        m_pPrevSymbol->setNextSymbol(0);
    // At the head of a symbol list
    else if (m_pNextSymbol)
        m_pNextSymbol->setPrevSymbol(0);

    // Do nothing if only element in list, i.e. null prev and next pointers
}

/// Returns the name of the symbol.
char* Symbol::name() const
{
    return m_pName;
}

/// Returns the type of the symbol.
E_TYPE Symbol::type() const
{
    return m_Type;
}

/// Returns the use for the symbol.
E_USE Symbol::use() const
{
    return m_Use;
}

/// Returns the constant data for the symbol.
char* Symbol::constData() const
{
    return m_ConstData;
}

/// Gets the symbol's previous symbol.
Symbol* Symbol::prevSymbol() const
{
    return m_pPrevSymbol;
}

/// Sets the symbol's previous symbol.
void Symbol::setPrevSymbol(Symbol *prevSymbol)
{
    m_pPrevSymbol = prevSymbol;
}

/// Gets the symbol's next symbol.
Symbol* Symbol::nextSymbol() const
{
    return m_pNextSymbol;
}

/// Sets the symbol's next symbol.
void Symbol::setNextSymbol(Symbol *nextSymbol)
{
    m_pNextSymbol = nextSymbol;
}
