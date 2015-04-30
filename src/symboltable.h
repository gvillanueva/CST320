/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/12/2015
 *              Modified, 4/13/2015
 * \ingroup     CST320 - Lab1a
 * \file        symboltable.h
 *
 * \brief       Declares the structure of the SymbolTable class.
 */
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "symbol.h"

/*!
 * \brief   The SymbolTable class stores symbol names, type, uses, and constant
 *          data. Symbol names may not be reused on the same scope, but may be
 *          reused on different scopes.
 */
class SymbolTable
{
public:
    /// Creates a new symbol table with a global scope.
    SymbolTable();

    /// Deletes a symbol table and frees associated memory.
    ~SymbolTable();

    /// Pushes a new scope into the symbol table.
    int pushScope();

    /// Pops the current scope off the symbol table.
    void popScope();

    /// Returns the current scope depths of the symbol table.
    int scope();

    /// Adds a symbol to the symbol table at the current scope.
    bool addSymbol(const char *name, E_TYPE type, E_USE use, void *data);

    /// Removes a symbol from the symbol table.
    void removeSymbol(const char *symbolName);

    /// Finds a symbol in the symbol table and returns it.
    SymbolPtr findSymbol(const char *symbolName) const;

private:
    /*!
     * \brief   The SymbolTableScope class contains the symbols for a single
     *          scope within the symbol table. It is an implementation detail
     *          not for client use.
     */
    class SymbolTableScope
    {
    public:
        /// Creates a new symbol table scope.
        SymbolTableScope();

        /// Deletes a symbol table scope.
        ~SymbolTableScope();

        /// Adds a symbol to the scope.
        bool addSymbol(const char *name, E_TYPE type, E_USE use, void *data);

        /// Removes a symbol from the scope.
        bool removeSymbol(const char *symbolName);

        /// Finds a symbol in the scope and returns it.
        SymbolPtr findSymbol(const char *symbolName);

        /// Pointer to the next scope in the list.
        SymbolTableScope *m_pNextScope;

    private:
        /// Pointer to the first symbol in the scope.
        Symbol *m_pHeadSymbol;

        /// Internal implementation to return the actual symbol type.
        Symbol* findSymbolInternal(const char *symbolName);
    };

    /// Pointer to the first (most local) scope in the table.
    SymbolTableScope *m_pHeadScope;

    /// Counter for the number of scopes in the symbol table.
    int m_Scope;
};

#endif//SYMBOLTABLE_H
