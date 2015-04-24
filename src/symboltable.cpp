/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/12/2015
 *              Modified, 4/13/2015
 * \ingroup     CST320 - Lab1a
 * \file        symboltable.cpp
 *
 * \brief       Defines the methods of the SymbolTable class.
 */
#include "symboltable.h"
#include <string.h>

/*!
 * \brief   Instantiates a new SymbolTable object.
 *
 * Creates a symbol table with an initial, global scope to contain symbols.
 */
SymbolTable::SymbolTable()
    :m_pHeadScope(0), m_Scope(0)
{
    pushScope();
}

/*!
 * \brief   Destroys a symbol table, taking allocated memory with it.
 */
SymbolTable::~SymbolTable()
{
    SymbolTableScope *scope = m_pHeadScope, *nextScope = 0;

    while (scope)
    {
        nextScope = scope->m_pNextScope;
        delete scope;
        scope = nextScope;
    }
}

/*!
 * \brief   Inserts new scope at head of scope list.
 * \return  Integer representing current scope depth of the symbol table.
 */
int SymbolTable::pushScope()
{
    SymbolTableScope *oldHead = m_pHeadScope;
    m_pHeadScope = new SymbolTableScope;
    m_pHeadScope->m_pNextScope = oldHead;
    return ++m_Scope;
}

/*!
 * \brief   Pops the local-most scope from the symbol table.
 */
void SymbolTable::popScope()
{
    SymbolTableScope *oldHead = m_pHeadScope;
    m_pHeadScope = m_pHeadScope->m_pNextScope;
    delete oldHead;
    m_Scope--;
}

/*!
 * \brief   Gets the current scope depth.
 * \return  Integer representing current scope depth of the symbol table.
 */
int SymbolTable::scope()
{
    return m_Scope;
}

/*!
 * \brief   Adds a symbol to the symbol table's current scope.
 * \param   name    The name of the new symbol.
 * \param   type    The type of the new symbol.
 * \param   use     The purpose of the new symbol.
 * \param   data    The constant data for the new symbol.
 * \return  true if symbol was added, otherwise false.
 */
bool SymbolTable::addSymbol(const char *name, E_TYPE type, E_USE use, void *data)
{
    return m_pHeadScope->addSymbol(name, type, use, data);
}

/*!
 * \brief   Removes a symbol from the symbol table.
 * \param   symbolName  The name of the symbol to be removed.
 */
void SymbolTable::removeSymbol(const char *symbolName)
{
    SymbolTableScope *scope = m_pHeadScope;

    // Iterate through the list of scopes until exhausted
    while (scope)
    {
        if (scope->removeSymbol(symbolName))
            break;
        scope = scope->m_pNextScope;
    }
}

/*!
 * \brief   Finds a symbol who name matches /p symbolName.
 * \param   symbolName The name of the symbol to search for.
 * \return  A wrapper to the symbol whose name matches /p symbolName, or a
 *          wrapper around NULL, if no match is found.
 *
 * findSymbol performs string comparisons against entries in the symbol table at
 * the current scope. If no matching symbol is found in the current scope, the
 * next scope is searched. This continues until all scopes are exhausted. Due to
 * this search algorithm, a matching symbol from the most local scope is always
 * returned.
 *
 * If no symbol in any scope matches /p symbolName, the function returns a
 * wrapper around NULL.
 */
SymbolPtr SymbolTable::findSymbol(const char *symbolName)
{
    SymbolTableScope *scope = m_pHeadScope;
    SymbolPtr symbol(0);

    while (scope && symbol.isNull())
    {
        symbol = scope->findSymbol(symbolName);
        scope = scope->m_pNextScope;
    }

    return symbol;
}



/*!
 * \brief   Instantiates a new SymbolTableScope object.
 */
SymbolTable::SymbolTableScope::SymbolTableScope()
    :m_pHeadSymbol(0), m_pNextScope(0)
{
}

/*!
 * \brief Deletes a symbol table scope, taking allocated memory with it.
 */
SymbolTable::SymbolTableScope::~SymbolTableScope()
{
    Symbol *symbol = m_pHeadSymbol, *nextSymbol = 0;

    while (symbol)
    {
        nextSymbol = symbol->nextSymbol();
        delete symbol;
        symbol = nextSymbol;
    }
}

/*!
 * \brief   Adds a symbol to the scope.
 * \param   name    The name of the new symbol.
 * \param   type    The type of the new symbol.
 * \param   use     The purpose of the new symbol.
 * \param   data    The constant data for the new symbol.
 * \return  true if symbol was added, otherwise false.
 */
bool SymbolTable::SymbolTableScope::addSymbol(const char *name, E_TYPE type,
                                              E_USE use, void *data)
{
    if (!findSymbol(name).isNull())
        return false;

    Symbol *newHead = new Symbol(name, type, use, data);
    Symbol *oldHead = m_pHeadSymbol;
    m_pHeadSymbol = newHead;
    m_pHeadSymbol->setNextSymbol(oldHead);

    // If previous head existed, set it's previous symbol to new head
    if (oldHead)
        oldHead->setPrevSymbol(m_pHeadSymbol);

    return true;
}

/*!
 * \brief   Removes a symbol from the scope.
 * \param   symbolName  The name of the symbol to be removed.
 * \return  true if symbol was removed, otherwise false.
 */
bool SymbolTable::SymbolTableScope::removeSymbol(const char* symbolName)
{
    Symbol *symbol = findSymbolInternal(symbolName);

    // If symbol doesn't exist in this scope
    if (!symbol)
        return false;

    if (symbol == m_pHeadSymbol)
        m_pHeadSymbol = symbol->nextSymbol();

    delete symbol;
    return true;
}

/*!
 * \brief   Finds a symbol in the current scope, returning it.
 * \param   symbolName  The name of the symbol to be retrieved.
 * \return  A wrapper around the matching symbol, if found. Otherwise a wrapper
 *          around NULL.
 */
SymbolPtr SymbolTable::SymbolTableScope::findSymbol(const char *symbolName)
{
    Symbol *symbol = findSymbolInternal(symbolName);
    return SymbolPtr(symbol);
}

/*!
 * \brief   Finds a symbol in the scope by matching names.
 * \param   symbolName  The name of the symbol to find.
 * \return  Pointer to matching symbol, or NULL.
 */
Symbol* SymbolTable::SymbolTableScope::findSymbolInternal(const char *symbolName)
{
    Symbol *symbol = m_pHeadSymbol;

    while (symbol)
    {
        if (!strcmp(symbolName, symbol->name()))
            return symbol;
        symbol = symbol->nextSymbol();
    }

    return symbol;
}
