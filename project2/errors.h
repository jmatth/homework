#ifndef ERRORS_H
#define ERRORS_H 1


#define ERR_INDUCTION_NOT_DECLARED "\n*** ERROR ***: Induction variable %s not declared.\n"
#define ERR_INDUCTION_NOT_SCALAR "\n*** ERROR ***: Induction variable %s not a scalar of type integer.\n"
#define ERR_ASSIGNMENT_TYPES_MATCH "\n*** ERROR ***: Assignment types do not match.\n");
#define ERR_VARIABLE_NOT_DECLARED "\n*** ERROR ***: Variable %s not declared.\n"
#define ERR_VARIABLE_NOT_SCALAR "\n*** ERROR ***: Variable %s is not a scalar variable.\n"
#define ERR_VARIABLE_NOT_ARRAY "\n*** ERROR ***: Variable %s is not an array variable.\n"
#define ERR_INDEX_NOT_INTEGER "\n*** ERROR ***: Array variable %s index type must be integer.\n"
#define ERR_OPERAND_NOT_INTEGER "\n*** ERROR ***: Operand type must be integer.\n"
#define ERR_OPERAND_NOT_BOOLEAN "\n*** ERROR ***: Operand type must be boolean.\n"
#define ERR_EQUALS_WITH_DIFFERENT_TYPES "\n*** ERROR ***: == or != operator with different types.\n"
#define ERR_RELATIONAL_ILLEGAL_TYPE "\n*** ERROR ***: Relational operator with illegal type.\n"

#endif
