/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/13/2015
 *              Modified, 4/13/2015
 * \ingroup     CST320 - Lab1a
 * \file        test_symboltable.h
 *
 * \brief       Declares the test procedures for the SymbolTable class.
 */
#ifndef TEST_SYMBOLTABLE_H
#define TEST_SYMBOLTABLE_H

/*!
 * \brief   The TestSymbolTable class is a container of test procedures for
 *          ensuring the consistency and validity of the various functions of
 *          the symbol table.
 */
class TestSymbolTable
{
public:
    /// Scope tests
    void test_pushScope_atGlobal_scopeEqualsTwo();
    void test_popScope_atGlobal_scopeEqualsZero();

    /// Symbol addition tests
    void test_addSymbol_atGlobal_newSymbol();
    void test_addSymbol_atGlobal_duplicateSymbolName();
    void test_addSymbol_differentScopes_allowed();

    /// Symbol removal tests
    void test_removeSymbol_fromGlobalScope();
    void test_removeSymbol_fromGlobalScope_atDifferentScope();
    void test_removeSymbol_fromScope2_reusedSymbolName();

    /// Symbol retrieval tests
    void test_findSymbol_atGlobal_succeeds();
    void test_findSymbol_atGlobal_undeclaredSymbol();
    void test_findSymbol_differentScopes_notEqual();

};

#endif
