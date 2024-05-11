#ifndef GSOLVER_H
#define GSOLVER_H

// GSolver is a lightweight logical floating-point constraint solver
// 

typedef void* GSolver;
typedef void* GSolverSymbol;
typedef void* GSolverEquation;

typedef struct {
    float value;
    bool isConstant;
    #ifdef DEBUG
        const char* name;
    #endif
} GSolverConstantDef;

typedef struct {
    float value;
    float defaultValue;
    bool isConstant;
    bool hasVariableBeenEvaluated;
    #ifdef DEBUG
        const char* name;
    #endif
} GSolverSymbol;

GSolver GSolver_Alloc();
void GSolver_Free();

GSolverSymbol GSolver_RegisterConstant(GSolver solver, const char* name, float value);
GSolverSymbol GSolver_RegisterVariable(GSolver solver, const char* name, float defaultValue);

GSolverEquation GSolver_RegisterEquation(GSolver solver, GSolverSymbol output, GSolverSymbol inputA, float multiplier, float offset, GSolverSymbol inputB);

void GSolver_SetConstantValue(GSolver solver, GSolverSymbol constant, float newValue);
float GSolver_GetConstantValue(GSolver solver, GSolverSymbol constant);

void GSolver_SetVariableDefaultValue(GSolver solver, GSolverSymbol variable, float newDefaultValue);
float GSolver_GetVariableValue(GSolver solver, GSolverSymbol variable);

void GSolver_Evaluate();


#endif // GSOLVER_H