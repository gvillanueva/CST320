/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/13/2015
 *              Modified, 4/13/2015
 * \ingroup     CST320 - Lab1a
 * \file        test_symboltable.cpp
 *
 * \brief       Defines the test procedures declared in test_symboltable.h
 */
#include <assert.h>
#include <string.h>
#include "test_symboltable.h"
#include "../src/symboltable.h"
#include "../src/symbol.h"

///Scope tests
/*!
 * \brief   Tests that pushing scope at the global scope will result in a scope
 *          depth of two.
 */
void TestSymbolTable::test_pushScope_atGlobal_scopeEqualsTwo()
{
    SymbolTable st;
    st.pushScope();
    assert(st.scope() == 2);
}

/*!
 * \brief   Tests that popping scope at the global scope will result in a scope
 *          depth of zero.
 */
void TestSymbolTable::test_popScope_atGlobal_scopeEqualsZero()
{
    SymbolTable st;
    st.popScope();
    assert(st.scope() == 0);
}


///Symbol addition tests
/*!
 * \brief   Tests that a symbol can be added to the current scope.
 */
void TestSymbolTable::test_addSymbol_atGlobal_newSymbol()
{
    SymbolTable st;
    st.addSymbol("temp", ET_INTEGER, EU_VARIABLENAME, 0);
    assert(st.findSymbol("temp").isNull() != true);
}

/*!
 * \brief   Tests that a symbol whose name matches an existing symbol in the
 *          current scope cannot be added.
 */
void TestSymbolTable::test_addSymbol_atGlobal_duplicateSymbolName()
{
    SymbolTable st;
    st.addSymbol("temp", ET_INTEGER, EU_VARIABLENAME, 0);
    assert(st.addSymbol("temp", ET_INTEGER, EU_VARIABLENAME, 0) == false);
}

/*!
 * \brief   Tests that a symbol whose name matches an existing symbol in a
 *          different scope can be added.
 */
void TestSymbolTable::test_addSymbol_differentScopes_allowed()
{
    SymbolTable st;
    st.addSymbol("test", ET_INTEGER, EU_VARIABLENAME, 0);
    SymbolPtr test1 = st.findSymbol("test");
    st.pushScope();
    st.addSymbol("test", ET_CHAR, EU_VARIABLENAME, 0);
    SymbolPtr test2 = st.findSymbol("test");

    // Ensure symbol names are identical
    assert(!strcmp(test1.name(), test2.name()));

    // Ensure symbol is not the same reference
    assert(test1 != test2);
}


///Symbol removal tests
/*!
 * \brief   Tests that a symbol can be removed from the global scope.
 */
void TestSymbolTable::test_removeSymbol_fromGlobalScope()
{
    SymbolTable st;
    st.addSymbol("temp", ET_INTEGER, EU_VARIABLENAME, 0);
    SymbolPtr symbol = st.findSymbol("temp");
    assert(symbol.isNull() != true);// Ensure the symbol has been successfully added
    st.removeSymbol(symbol.name());
    symbol = st.findSymbol("temp");
    assert(symbol.isNull());// Ensure the symbol has been successfully removed
}

/*!
 * \brief   Tests that a symbol can be removed from the global scope when it is
 *          not the current scope.
 */
void TestSymbolTable::test_removeSymbol_fromGlobalScope_atDifferentScope()
{
    SymbolTable st;
    st.addSymbol("temp", ET_INTEGER, EU_VARIABLENAME, 0);
    SymbolPtr symbol = st.findSymbol("temp");

    // Ensure the symbol has been successfully added
    assert(symbol.isNull() != true);

    st.pushScope();
    st.removeSymbol(symbol.name());
    symbol = st.findSymbol("temp");

    // Ensure the symbol has been successfully removed
    assert(symbol.isNull());
}

/*!
 * \brief   Tests that a removing a symbol redeclared in a lower scope removes
 *          only the lower-scoped symbol.
 */
void TestSymbolTable::test_removeSymbol_fromScope2_reusedSymbolName()
{
    SymbolTable st;
    st.addSymbol("temp", ET_INTEGER, EU_VARIABLENAME, 0);
    SymbolPtr temp1 = st.findSymbol("temp");

    // Ensure the symbol has been successfully added
    assert(temp1.isNull() != true);

    st.pushScope();
    st.addSymbol("temp", ET_CHAR, EU_VARIABLENAME, 0);
    SymbolPtr temp2 = st.findSymbol("temp");

    // Ensure temp symbol in new scope is created
    assert(temp2.isNull() != true);

    // Ensure symbols are not the same
    assert(temp1 != temp2);

    st.removeSymbol(temp2.name());
    temp2 = st.findSymbol("temp");

    // Ensure symbols are the same after removal of lower-scoped symbol
    assert(temp1 == temp2);
}


///Symbol search tests
/*!
 * \brief   Tests that an added symbol can be retrieved.
 */
void TestSymbolTable::test_findSymbol_atGlobal_succeeds()
{
    SymbolTable st;
    st.addSymbol("test", ET_INTEGER, EU_VARIABLENAME, 0);
    SymbolPtr symbol = st.findSymbol("test");

    // Ensure all the data matches
    assert(!strcmp(symbol.name(), "test"));
    assert(symbol.type() == ET_INTEGER);
    assert(symbol.use() == EU_VARIABLENAME);
    assert(symbol.constData() == 0);
}

/*!
 * \brief   Tests that an undeclared symbol cannot be found.
 */
void TestSymbolTable::test_findSymbol_atGlobal_undeclaredSymbol()
{
    SymbolTable st;
    assert(st.findSymbol("foo").isNull());
}

/*!
 * \brief   Tests that symbols with matching names, but on different scopes
 *          are not equal.
 */
void TestSymbolTable::test_findSymbol_differentScopes_notEqual()
{
    SymbolTable st;
    st.addSymbol("test", ET_INTEGER, EU_VARIABLENAME, 0);
    SymbolPtr test1 = st.findSymbol("test");
    st.pushScope();
    st.addSymbol("test", ET_INTEGER, EU_VARIABLENAME, 0);
    SymbolPtr test2 = st.findSymbol("test");

    assert(test1 != test2);
}
