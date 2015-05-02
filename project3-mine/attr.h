/**********************************************
        CS415  Project 2
        Spring  2015
        Student Version
**********************************************/

#ifndef ATTR_H
#define ATTR_H

struct Node {
    struct Node *prev;
    struct Node *next;
    void *data;
};

typedef struct Node* Node;

struct Dequeue{
    Node left;
    Node right;
};

typedef struct Dequeue* Dequeue;

void initDequeue(Dequeue);

int rinsert(Dequeue, void*);
int linsert(Dequeue, void*);

void* rdequeue(Dequeue);
void* ldequeue(Dequeue);

typedef struct {
    int num;
    int cl;
    int sz;
    char *str;
    struct Dequeue dequeue;
} tokentype;

typedef enum type_expression {
    TYPE_INT=0,
    TYPE_BOOL,
    TYPE_ERROR
} Type_Expression;

typedef enum class_expression {
    CL_SCALAR=0,
    CL_ARR
} Class_Expression;

struct arrDeps {
    int is_arr;
    int is_constant;
    int has_c;
    int c;
    int has_a;
    int a;
    char iName[64];
    char arrName[64];
};

typedef struct regInfo {
    Type_Expression type;
    int targetRegister;
    int endLabel;
    int controlLabel;
    int canVector;
    int vectLabel;
    int startRange;
    int endRange;
    struct arrDeps deps;
    struct regInfo *lhs;
    struct regInfo *rhs;
    char varName[64];
    struct Dequeue vars;
    struct Dequeue arrExprs;
    struct regInfo *rhsDq;
} regInfo;

typedef struct {
    int controlLabel;
    int bodyLabel;
    int postLabel;
} loopcontrol;

#define ERR_INDUCTION_NOT_DECLARED "\n*** ERROR ***: Induction variable %s not declared.\n"
#define ERR_INDUCTION_NOT_SCALAR "\n*** ERROR ***: Induction variable %s not a scalar of type integer.\n"
#define ERR_ASSIGNMENT_TYPES_MATCH "\n*** ERROR ***: Assignment types do not match.\n"
#define ERR_VARIABLE_NOT_DECLARED "\n*** ERROR ***: Variable %s not declared.\n"
#define ERR_VARIABLE_NOT_SCALAR "\n*** ERROR ***: Variable %s is not a scalar variable.\n"
#define ERR_VARIABLE_NOT_ARRAY "\n*** ERROR ***: Variable %s is not an array variable.\n"
#define ERR_INDEX_NOT_INTEGER "\n*** ERROR ***: Array variable %s index type must be integer.\n"
#define ERR_OPERAND_NOT_INTEGER "\n*** ERROR ***: Operand type must be integer.\n"
#define ERR_OPERAND_NOT_BOOLEAN "\n*** ERROR ***: Operand type must be boolean.\n"
#define ERR_EQUALS_WITH_DIFFERENT_TYPES "\n*** ERROR ***: == or != operator with different types.\n"
#define ERR_RELATIONAL_ILLEGAL_TYPE "\n*** ERROR ***: Relational operator with illegal type.\n"

#define APPEND_DEQ(temp, indequeue, outdequeue) \
    while ( (temp = rdequeue(outdequeue)) != NULL ) \
        linsert(indequeue, temp);

#endif
