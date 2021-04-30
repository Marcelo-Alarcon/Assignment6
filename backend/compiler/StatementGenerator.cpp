#include <string>
#include <vector>
#include <map>

#include "PascalBaseVisitor.h"
#include "antlr4-runtime.h"

#include "intermediate/symtab/Predefined.h"
#include "Compiler.h"
#include "StatementGenerator.h"

namespace backend { namespace compiler {

using namespace std;
using namespace intermediate;

void StatementGenerator::emitAssignment(PascalParser::AssignmentStatementContext *ctx)
{
    PascalParser::VariableContext *varCtx  = ctx->lhs()->variable();
    PascalParser::ExpressionContext *exprCtx = ctx->rhs()->expression();
    SymtabEntry *varId = varCtx->entry;
    Typespec *varType  = varCtx->type;
    Typespec *exprType = exprCtx->type;

    // The last modifier, if any, is the variable's last subscript or field.
    int modifierCount = varCtx->modifier().size();
    PascalParser::ModifierContext *lastModCtx = modifierCount == 0
                            ? nullptr : varCtx->modifier()[modifierCount - 1];

    // The target variable has subscripts and/or fields.
    if (modifierCount > 0)
    {
        lastModCtx = varCtx->modifier()[modifierCount - 1];
        compiler->visit(varCtx);
    }

    // Emit code to evaluate the expression.
    compiler->visit(exprCtx);

    // float variable := integer constant
    if (   (varType == Predefined::realType)
        && (exprType->baseType() == Predefined::integerType)) emit(I2F);

    // Emit code to store the expression value into the target variable.
    // The target variable has no subscripts or fields.
    if (lastModCtx == nullptr) emitStoreValue(varId, varId->getType());

    // The target variable is a field.
    else if (lastModCtx->field() != nullptr)
    {
        emitStoreValue(lastModCtx->field()->entry, lastModCtx->field()->type);
    }

    // The target variable is an array element.
    else
    {
        emitStoreValue(nullptr, varType);
    }
}

void StatementGenerator::emitIf(PascalParser::IfStatementContext *ctx)
{
	Label *trueLabel = new Label();
	Label *falseLabel = new Label();

	// IF or IF/ELSE
	if (ctx->ELSE() == nullptr){
		// Generate IF statement
		compiler->visit(ctx->expression());
		emit(IFEQ, falseLabel);
		compiler->visit(ctx->trueStatement());
		emitLabel(falseLabel);
	} else {
		// Generate IF/ELSE statement
		compiler->visit(ctx->expression());
		emit(IFEQ, falseLabel);
		compiler->visit(ctx->trueStatement());
		emit(GOTO, trueLabel);
		emitLabel(falseLabel);
		compiler->visit(ctx->falseStatement());
		emitLabel(trueLabel);
	}
}

void StatementGenerator::emitCase(PascalParser::CaseStatementContext *ctx)
{
	 int bcount = 0;
	PascalParser::ExpressionContext *exprCtx = ctx->expression();
	PascalParser::CaseBranchListContext *branchListCtx = ctx->caseBranchList();

	int branches = (branchListCtx->children.size()/2)+1;
	Label branchLabels[branches];
	Label *exitcase = new Label();

	compiler->visit(exprCtx);

	emit(LOOKUPSWITCH);
	struct count{
		int val;
		int branchnum;
	};

	count pair[100];
	int size = 0;

	if(branchListCtx != nullptr)
	{
		for(PascalParser::CaseBranchContext *branchCtx :branchListCtx->caseBranch())
		{
			if(branchCtx->caseConstantList() != NULL)
			{
				PascalParser::CaseConstantListContext *constListCtx =branchCtx->caseConstantList();
				for (PascalParser::CaseConstantContext *caseConstCtx : constListCtx->caseConstant())
				{
					if(caseConstCtx != NULL)
					{
						int v = caseConstCtx->value;
						pair[size].val = v;
						pair[size].branchnum = bcount;
						size++;
					}
				}
				bcount++;
			}
		}
	}

	int k, j, min;

	for(k = 0; k< size-1; k++)
	{
		min = k;
		for(j = k+1; j<size; j++)
		{
			if(pair[j].val < pair[min].val)
			{
				min = j;
			}
		}

		count temp = pair[min];
		pair[min] = pair[k];
		pair[k] = temp;
	}

	for(int i = 0; i< size; i++)
	{
		emitLabel(pair[i].val, &branchLabels[pair[i].branchnum]);
	}

	emitLabel("default", exitcase);
	int i = 0;

	if(branchListCtx != nullptr)
	{
		for(PascalParser::CaseBranchContext *branchCtx :branchListCtx->caseBranch())
		{
			if(branchCtx->statement() != nullptr)
			{
				emitLabel(&branchLabels[i]);
				compiler->visit(branchCtx->statement());
				emit(GOTO, exitcase);
			}
			i++;
		}
		emitLabel(exitcase);
	}

}

void StatementGenerator::emitRepeat(PascalParser::RepeatStatementContext *ctx)
{
    Label *loopTopLabel  = new Label();
    Label *loopExitLabel = new Label();

    emitLabel(loopTopLabel);

    compiler->visit(ctx->statementList());
    compiler->visit(ctx->expression());
    emit(IFNE, loopExitLabel);
    emit(GOTO, loopTopLabel);

    emitLabel(loopExitLabel);
}

void StatementGenerator::emitWhile(PascalParser::WhileStatementContext *ctx)
{
	Label *loopTopLabel  = new Label();
	Label *loopExitLabel = new Label();
	string op;

	// Extract the operator and the test condition
	op = ctx->expression()->relOp()->getText();
	emitLabel(loopTopLabel);
	compiler->visit(ctx->expression());

	// Generate expression based the operation type
	if (op=="<=")
		emit(IFLE, loopExitLabel);
	else if (op==">=")
		emit(IFGE, loopExitLabel);
	else if (op=="<")
		emit(IFLT, loopExitLabel);
	else if (op==">")
		emit(IFGT, loopExitLabel);
	else if (op=="=")
		emit(IFEQ, loopExitLabel);
	else if (op=="<>")
		emit(IFNE, loopExitLabel);

	// Generate loop statements
	compiler->visit(ctx->statement());
	emit(GOTO, loopTopLabel);
	emitLabel(loopExitLabel);
}

void StatementGenerator::emitFor(PascalParser::ForStatementContext *ctx)
{
	Label *loopTopLabel  = new Label();
	Label *loopExitLabel = new Label();

	// Extract the value from the expression
	PascalParser::VariableContext *var = ctx->variable();
	string value1 = ctx->expression()[0]->getText();
	int c = 0;

	// Determine the data type of the value
	if(ctx->expression()[0]->type == Predefined::integerType)
		emitLoadConstant(stoi(value1));
	else if(ctx->expression()[0]->type == Predefined::realType)
		emitLoadConstant(stof(value1));
	else if(ctx->expression()[0]->type == Predefined::charType){
		c = value1[1];
		emitLoadConstant(c);
	} else
		return;


	// Get test conditions
	emitStoreValue(var->variableIdentifier()->entry, var->type);
	emitLabel(loopTopLabel);
	compiler->visit(var);
	compiler->visit(ctx->expression()[1]);

	// Determine direction of the loop
	bool TO = (ctx->TO() != nullptr);
	if(TO)
		emit(IF_ICMPGT, loopExitLabel);
	else
		emit(IF_ICMPLT, loopExitLabel);


	// Generate Increment/decrement
	compiler->visit(ctx->statement());
	emitLoadValue(var->variableIdentifier()->entry);
	emitLoadConstant(1);
	if(TO)
		emit(IADD);
	else
		emit(ISUB);


	// Generate Reassignment and loop
	emitStoreValue(var->variableIdentifier()->entry, var->type);
	emit(GOTO, loopTopLabel);
	emitLabel(loopExitLabel);
}

void StatementGenerator::emitProcedureCall(PascalParser::ProcedureCallStatementContext *ctx)
{
    string name = ctx->procedureName()->getText();
    string arg;
    bool no_args = ctx->argumentList() == nullptr;
    string call = programName + "/" + name;
    if(no_args){
    	call = call + "()";
    	call = call + typeDescriptor(ctx->procedureName()->entry);
    	emit(INVOKESTATIC, call);
    	return;
    }


    vector<SymtabEntry *> *parmIds = ctx->procedureName()->entry->getRoutineParameters();

    int index = 0;

    if (parmIds != nullptr)
        {
            for (SymtabEntry *parmId : *parmIds)
            {
            	compiler->visit(ctx->argumentList()->argument()[index]->expression());
            	if(typeDescriptor(parmId) == "F" && ctx->argumentList()->argument()[index]->expression()->type == Predefined::integerType){
					emit(I2F);
				}
                index++;
                localStack->increase(1);
                    ++count;
            }
        }

    emitCall(ctx->procedureName()->entry, ctx->argumentList());

}

void StatementGenerator::emitFunctionCall(PascalParser::FunctionCallContext *ctx)
{
	string name = ctx->functionName()->getText();
	string arg;
	bool no_args = ctx->argumentList() == nullptr;
	string call = programName + "/" + name;
	if(no_args){
		call = call + "()";
		call = call + typeDescriptor(ctx->functionName()->entry);
		emit(INVOKESTATIC, call);
		return;
	}

	vector<SymtabEntry *> *parmIds = ctx->functionName()->entry->getRoutineParameters();

	int index = 0;

	if (parmIds != nullptr)
		{
			for (SymtabEntry *parmId : *parmIds)
			{
				compiler->visit(ctx->argumentList()->argument()[index]->expression());
				if(typeDescriptor(parmId) == "F" && ctx->argumentList()->argument()[index]->expression()->type == Predefined::integerType){
					emit(I2F);
				}
				index++;
			}
		}

	emitCall(ctx->functionName()->entry, ctx->argumentList());

}

void StatementGenerator::emitCall(SymtabEntry *routineId,
                                  PascalParser::ArgumentListContext *argListCtx)
{
	string name = routineId->getName();
	string call = programName + "/" + name + "(";
	vector<SymtabEntry *> *parmIds = routineId->getRoutineParameters();

	if (parmIds != nullptr)
	{
		for (SymtabEntry *parmId : *parmIds)
		{
			call += typeDescriptor(parmId);
		}
	}


	call = call + ")";
	call = call + typeDescriptor(routineId);

	emit(INVOKESTATIC, call);
}

void StatementGenerator::emitWrite(PascalParser::WriteStatementContext *ctx)
{
    emitWrite(ctx->writeArguments(), false);
}

void StatementGenerator::emitWriteln(PascalParser::WritelnStatementContext *ctx)
{
    emitWrite(ctx->writeArguments(), true);
}

void StatementGenerator::emitWrite(PascalParser::WriteArgumentsContext *argsCtx,
                      bool needLF)
{
    emit(GETSTATIC, "java/lang/System/out", "Ljava/io/PrintStream;");

    // WRITELN with no arguments.
    if (argsCtx == nullptr)
    {
        emit(INVOKEVIRTUAL, "java/io/PrintStream.println()V");
        localStack->decrease(1);
    }

    // Generate code for the arguments.
    else
    {
        string format;
        int exprCount = createWriteFormat(argsCtx, format, needLF);

        // Load the format string.
        emit(LDC, format);

        // Emit the arguments array.
       if (exprCount > 0)
        {
            emitArgumentsArray(argsCtx, exprCount);

            emit(INVOKEVIRTUAL,
                        string("java/io/PrintStream/printf(Ljava/lang/String;")
                      + string("[Ljava/lang/Object;)")
                      + string("Ljava/io/PrintStream;"));
            localStack->decrease(2);
            emit(POP);
        }
        else
        {
            emit(INVOKEVIRTUAL,
                 "java/io/PrintStream/print(Ljava/lang/String;)V");
            localStack->decrease(2);
        }
    }
}

int StatementGenerator::createWriteFormat(
                                PascalParser::WriteArgumentsContext *argsCtx,
                                string& format, bool needLF)
{
    int exprCount = 0;
    format += "\"";

    // Loop over the write arguments.
    for (PascalParser::WriteArgumentContext *argCtx : argsCtx->writeArgument())
    {
        Typespec *type = argCtx->expression()->type;
        string argText = argCtx->getText();

        // Append any literal strings.
        if (argText[0] == '\'') format += convertString(argText, true);

        // For any other expressions, append a field specifier.
        else
        {
            exprCount++;
            format.append("%");

            PascalParser::FieldWidthContext *fwCtx = argCtx->fieldWidth();
            if (fwCtx != nullptr)
            {
                string sign = (   (fwCtx->sign() != nullptr)
                               && (fwCtx->sign()->getText() == "-")) ? "-" : "";
                format += sign + fwCtx->integerConstant()->getText();

                PascalParser::DecimalPlacesContext *dpCtx =
                                                        fwCtx->decimalPlaces();
                if (dpCtx != nullptr)
                {
                    format += "." + dpCtx->integerConstant()->getText();
                }
            }

            string typeFlag = type == Predefined::integerType ? "d"
                            : type == Predefined::realType    ? "f"
                            : type == Predefined::booleanType ? "b"
                            : type == Predefined::charType    ? "c"
                            :                                  "s";
            format += typeFlag;
        }
    }

    format += needLF ? "\\n\"" : "\"";

    return exprCount;
}

void StatementGenerator::emitArgumentsArray(
                    PascalParser::WriteArgumentsContext *argsCtx, int exprCount)
{
    // Create the arguments array.
    emitLoadConstant(exprCount);
    emit(ANEWARRAY, "java/lang/Object");

    int index = 0;

    // Loop over the write arguments to fill the arguments array.
    for (PascalParser::WriteArgumentContext *argCtx :
                                                argsCtx->writeArgument())
    {
        string argText = argCtx->getText();
        PascalParser::ExpressionContext *exprCtx = argCtx->expression();
        Typespec *type = exprCtx->type->baseType();

        // Skip string constants, which were made part of
        // the format string.
        if (argText[0] != '\'')
        {
            emit(DUP);
            emitLoadConstant(index++);

            compiler->visit(exprCtx);

            Form form = type->getForm();
            if (    ((form == SCALAR) || (form == ENUMERATION))
                 && (type != Predefined::stringType))
            {
                emit(INVOKESTATIC, valueOfSignature(type));
            }

            // Store the value into the array.
            emit(AASTORE);
        }
    }
}

void StatementGenerator::emitRead(PascalParser::ReadStatementContext *ctx)
{
    emitRead(ctx->readArguments(), false);
}

void StatementGenerator::emitReadln(PascalParser::ReadlnStatementContext *ctx)
{
    emitRead(ctx->readArguments(), true);
}

void StatementGenerator::emitRead(PascalParser::ReadArgumentsContext *argsCtx,
                                  bool needSkip)
{
    int size = argsCtx->variable().size();

    // Loop over read arguments.
    for (int i = 0; i < size; i++)
    {
        PascalParser::VariableContext *varCtx = argsCtx->variable()[i];
        Typespec *varType = varCtx->type;

        if (varType == Predefined::integerType)
        {
            emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
            emit(INVOKEVIRTUAL, "java/util/Scanner/nextInt()I");
            emitStoreValue(varCtx->entry, nullptr);
        }
        else if (varType == Predefined::realType)
        {
            emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
            emit(INVOKEVIRTUAL, "java/util/Scanner/nextFloat()F");
            emitStoreValue(varCtx->entry, nullptr);
        }
        else if (varType == Predefined::booleanType)
        {
            emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
            emit(INVOKEVIRTUAL, "java/util/Scanner/nextBoolean()Z");
            emitStoreValue(varCtx->entry, nullptr);
        }
        else if (varType == Predefined::charType)
        {
            emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
            emit(LDC, "\"\"");
            emit(INVOKEVIRTUAL,
                 string("java/util/Scanner/useDelimiter(Ljava/lang/String;)") +
                 string("Ljava/util/Scanner;"));
            emit(POP);
            emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
            emit(INVOKEVIRTUAL, "java/util/Scanner/next()Ljava/lang/String;");
            emit(ICONST_0);
            emit(INVOKEVIRTUAL, "java/lang/String/charAt(I)C");
            emitStoreValue(varCtx->entry, nullptr);

            emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
            emit(INVOKEVIRTUAL, "java/util/Scanner/reset()Ljava/util/Scanner;");

        }
        else  // string
        {
            emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
            emit(INVOKEVIRTUAL, "java/util/Scanner/next()Ljava/lang/String;");
            emitStoreValue(varCtx->entry, nullptr);
        }
    }

    // READLN: Skip the rest of the input line.
    if (needSkip)
    {
        emit(GETSTATIC, programName + "/_sysin Ljava/util/Scanner;");
        emit(INVOKEVIRTUAL, "java/util/Scanner/nextLine()Ljava/lang/String;");
        emit(POP);
    }
}

}} // namespace backend::compiler
