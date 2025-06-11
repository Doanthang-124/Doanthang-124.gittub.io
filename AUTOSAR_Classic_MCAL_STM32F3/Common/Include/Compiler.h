#ifndef COMPILER_H
#define COMPILER_H

/* Compiler Abstraction Macros */

/* Keyword for volatile */
#define _VOLATILE_ volatile

/* Keyword for inline functions */
#define _INLINE_ inline

/* Keyword for static */
#define STATIC static

/* Macro for function definition */
#define FUNC(rettype, memclass) rettype

/* Macro for pointer to variable */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/* Macro for pointer to constant */
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

/* Macro for pointer to function */
#define P2FUNC(rettype, ptrclass, fctname) rettype (*fctname)

/* Macro for constant pointer to variable */
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const

/* Macro for constant pointer to constant */
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

/* Macro for constant */
#define CONST(consttype, memclass) const consttype

/* Macro for variable */
#define VAR(vartype, memclass) vartype

#endif /* COMPILER_H */
