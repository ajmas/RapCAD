/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "syntaxtreebuilder.h"

SyntaxTreeBuilder::SyntaxTreeBuilder()
{
	script=new Script();
}

SyntaxTreeBuilder::~SyntaxTreeBuilder()
{
}

void SyntaxTreeBuilder::buildFileLocation(QString f)
{
	QFileInfo* info=new QFileInfo(f);
	script->setFileLocation(info);
}

void SyntaxTreeBuilder::buildScript(Declaration* dec)
{
	script->addDeclaration(dec);
}

void SyntaxTreeBuilder::buildScript(QList<CodeDoc*>* cdocs)
{
	script->addDocumentation(*cdocs);
}

QList<CodeDoc*>* SyntaxTreeBuilder::buildCodeDoc(QList<CodeDoc*>* cdocs)
{
	return cdocs;
}

QList<CodeDoc*>* SyntaxTreeBuilder::buildCodeDoc()
{
	return new QList<CodeDoc*>();
}

QList<CodeDoc*>* SyntaxTreeBuilder::buildCodeDoc(QString* t,QList<CodeDoc*>* cdocs)
{
	CodeDoc* cdoc = new CodeDoc();
	cdoc->setName("@description");
	cdoc->setText(*t);
	delete t;
	cdocs->prepend(cdoc);
	return cdocs;
}

QList<CodeDoc*>* SyntaxTreeBuilder::buildCodeDoc(QString* n,QString* t,QList<CodeDoc*>* cdocs)
{
	CodeDoc* cdoc = new CodeDoc();
	cdoc->setName(*n);
	delete n;
	cdoc->setText(*t);
	delete t;
	cdocs->prepend(cdoc);
	return cdocs;
}

Declaration* SyntaxTreeBuilder::buildUse(QString* imp)
{
	ScriptImport* result = new ScriptImport();
	result->setImport(*imp);
	delete imp;
	return result;
}

Declaration* SyntaxTreeBuilder::buildUse(QString* imp,QString* name)
{
	ScriptImport* result = new ScriptImport();
	result->setImport(*imp);
	delete imp;
	result->setNamespace(*name);
	delete name;
	return result;
}

Declaration* SyntaxTreeBuilder::buildImport(QString* imp,QString* name)
{
	ModuleImport* result = new ModuleImport();
	result->setImport(*imp);
	delete imp;
	result->setName(*name);
	delete name;

	return result;
}

Declaration* SyntaxTreeBuilder::buildImport(QString* imp,QString* name,QList<Parameter*>* params)
{
	ModuleImport* result = new ModuleImport();
	result->setImport(*imp);
	delete imp;
	result->setName(*name);
	delete name;
	result->setParameters(*params);
	delete params;

	return result;
}

void SyntaxTreeBuilder::buildScript(QList<Declaration*>* decls)
{
	script->setDeclarations(*decls);
	delete decls;
}

Declaration* SyntaxTreeBuilder::buildDeclaration(Declaration* dec)
{
	return dec;
}

QList<Declaration*>* SyntaxTreeBuilder::buildDeclarations()
{
	return new QList<Declaration*>();
}

QList<Declaration*>* SyntaxTreeBuilder::buildDeclarations(QList<Declaration*>* decls,Declaration* decl)
{
	decls->append(decl);
	return decls;
}

QList<Declaration*>* SyntaxTreeBuilder::buildDeclarations(QList<Declaration*>* decls)
{
	return decls;
}

QList<Declaration*>* SyntaxTreeBuilder::buildDeclarations(Declaration* decl)
{
	QList<Declaration*>* result = new QList<Declaration*>();
	result->append(decl);
	return result;
}

Statement* SyntaxTreeBuilder::buildStatement(Statement* stmt)
{
	return stmt;
}

Statement* SyntaxTreeBuilder::buildStatement(Variable* var,Expression::Operator_e op)
{
	AssignStatement* result = new AssignStatement();
	result->setVariable(var);
	result->setOperation(op);
	return result;
}

Statement* SyntaxTreeBuilder::buildStatement(Variable* var,Expression::Operator_e op,Expression* exp)
{
	AssignStatement* result = new AssignStatement();
	result->setVariable(var);
	result->setOperation(op);
	result->setExpression(exp);
	return result;
}

Statement* SyntaxTreeBuilder::buildStatement(QList<Statement*>* stmts)
{
	CompoundStatement* result = new CompoundStatement();
	result->setChildren(*stmts);
	delete stmts;
	return result;
}

Statement* SyntaxTreeBuilder::buildStatement(Variable* var,Expression* exp)
{
	AssignStatement* result = new AssignStatement();
	result->setVariable(var);
	result->setExpression(exp);
	return result;
}

Statement* SyntaxTreeBuilder::buildStatement(QString* name,Variable::Storage_e c, Expression* exp)
{
	AssignStatement* result = new AssignStatement();
	Variable* var = new Variable();
	var->setName(*name);
	delete name;
	var->setStorage(c);
	result->setVariable(var);
	result->setExpression(exp);
	return result;
}

Statement* SyntaxTreeBuilder::buildReturnStatement(Expression* exp)
{
	ReturnStatement* result = new ReturnStatement();
	result->setExpression(exp);
	return result;
}

Statement* SyntaxTreeBuilder::buildIfElseStatement(Expression* expr,Statement* stmt)
{
	IfElseStatement* result = new IfElseStatement();
	result->setExpression(expr);
	result->setTrueStatement(stmt);
	return result;
}

Statement* SyntaxTreeBuilder::buildIfElseStatement(Expression* expr,Statement* truestmt ,Statement* falsestmt)
{
	IfElseStatement* result = new IfElseStatement();
	result->setExpression(expr);
	result->setTrueStatement(truestmt);
	result->setFalseStatement(falsestmt);;
	return result;
}

Statement* SyntaxTreeBuilder::buildForStatement(QList<Argument*>* args,Statement* stmt)
{
	ForStatement* result = new ForStatement();
	result->setArguments(*args);
	delete args;
	result->setStatement(stmt);
	return result;
}

QList<Statement*>* SyntaxTreeBuilder::buildStatements()
{
	return new QList<Statement*>();
}

QList<Statement*>* SyntaxTreeBuilder::buildStatements(Statement* stmt)
{
	QList<Statement*>* result = new QList<Statement*>();
	result->append(stmt);
	return result;
}

QList<Statement*>* SyntaxTreeBuilder::buildStatements(QList<Statement*>* stmts)
{
	return stmts;
}

QList<Statement*>* SyntaxTreeBuilder::buildStatements(QList<Statement*>* stmts,Statement* stmt)
{
	stmts->append(stmt);
	return stmts;
}

Declaration* SyntaxTreeBuilder::buildModule(QString* name, QList<Parameter*>* params, Scope* scp)
{
	Module* result = new Module();
	result->setName(*name);
	delete name;
	result->setParameters(*params);
	delete params;
	result->setScope(scp);
	return result;
}

Declaration* SyntaxTreeBuilder::buildFunction(QString* name, QList<Parameter*>* params, Scope* scp)
{
	Function* result = new Function();
	result->setName(*name);
	delete name;
	result->setParameters(*params);
	delete params;
	result->setScope(scp);
	return result;
}

Scope* SyntaxTreeBuilder::buildScope(QList<Declaration*>* decls)
{
	ModuleScope* result = new ModuleScope();
	result->setDeclarations(*decls);
	delete decls;
	return result;
}

Scope* SyntaxTreeBuilder::buildScope(Instance* inst)
{
	ModuleScope* result = new ModuleScope();
	QList<Declaration*> decls;
	decls.append(inst);
	result->setDeclarations(decls);
	return result;
}

Scope* SyntaxTreeBuilder::buildScope(Expression* expr)
{
	FunctionScope* result = new FunctionScope();
	result->setExpression(expr);
	return result;
}

Scope* SyntaxTreeBuilder::buildScope(QList<Statement*>* stmts)
{
	FunctionScope* result = new FunctionScope();
	result->setStatements(*stmts);
	delete stmts;
	return result;
}

Instance* SyntaxTreeBuilder::buildInstance(Instance* inst)
{
	return inst;
}

Instance* SyntaxTreeBuilder::buildInstance(QString* name,Instance* inst)
{
	inst->setNamespace(*name);
	delete name;
	return inst;
}

Instance* SyntaxTreeBuilder::buildInstance(Instance::Type_e type,Instance* inst)
{
	inst->setType(type);
	return inst;
}

Instance* SyntaxTreeBuilder::buildInstance(Instance::Type_e type,QString* name,QList<Argument*>* args)
{
	Instance* result = new Instance();
	result->setType(type);
	result->setName(*name);
	delete name;
	result->setArguments(*args);
	delete args;
	return result;
}

Instance* SyntaxTreeBuilder::buildInstance(QString* name,QList<Argument*>* args)
{
	Instance* result = new Instance();
	result->setName(*name);
	delete name;
	result->setArguments(*args);
	delete args;
	return result;
}

Instance* SyntaxTreeBuilder::buildInstance(Instance* inst,QList<Statement*>* smts)
{
	inst->setChildren(*smts);
	delete smts;
	return inst;
}

QList<Parameter*>* SyntaxTreeBuilder::buildParameters()
{
	return new QList<Parameter*>();
}

QList<Parameter*>* SyntaxTreeBuilder::buildParameters(Parameter* param)
{
	QList<Parameter*>* result = new QList<Parameter*>();
	result->append(param);
	return result;
}

QList<Parameter*>* SyntaxTreeBuilder::buildParameters(QList<Parameter*>* params,Parameter* param)
{
	params->append(param);
	return params;
}

Parameter* SyntaxTreeBuilder::buildParameter(QString* name)
{
	Parameter* result = new Parameter();
	result->setName(*name);
	delete name;
	return result;
}

Parameter* SyntaxTreeBuilder::buildParameter(QString* name,Expression* expr)
{
	Parameter* result = new Parameter();
	result->setName(*name);
	delete name;
	result->setExpression(expr);
	return result;
}

QList<Argument*>* SyntaxTreeBuilder::buildArguments()
{
	return new QList<Argument*>();
}

QList<Argument*>* SyntaxTreeBuilder::buildArguments(Argument* arg)
{
	QList<Argument*>* result = new QList<Argument*>();
	result->append(arg);
	return result;
}

QList<Argument*>* SyntaxTreeBuilder::buildArguments(QList<Argument*>* args,unsigned int count,Argument* arg)
{
	//if we are passed an empty list then we know that the first argument was blank.
	if(args->size()==0)
		args->append(new Argument());

	for(unsigned int i=0; i<count; i++)
		args->append(new Argument());

	args->append(arg);
	return args;
}

Argument* SyntaxTreeBuilder::buildArgument(Expression* exp)
{
	Argument* result = new Argument();
	result->setExpression(exp);
	return result;
}

Argument* SyntaxTreeBuilder::buildArgument(Variable* var,Expression* exp)
{
	Argument* result = new Argument();
	result->setVariable(var);
	result->setExpression(exp);
	return result;
}

unsigned int SyntaxTreeBuilder::buildOptionalCommas()
{
	return 0;
}

unsigned int SyntaxTreeBuilder::buildOptionalCommas(unsigned int count)
{
	return ++count;
}

Expression* SyntaxTreeBuilder::buildLiteral()
{
	return new Literal();
}

Expression* SyntaxTreeBuilder::buildLiteral(bool value)
{
	Literal* result = new Literal();
	result->setValue(value);
	return result;
}

Expression* SyntaxTreeBuilder::buildLiteral(decimal* value)
{
	Literal* result = new Literal();
	result->setValue(*value);
	return result;
}

Expression* SyntaxTreeBuilder::buildLiteral(QString* value)
{
	Literal* result = new Literal();
	result->setValue(*value);
	delete value;
	return result;
}

Variable* SyntaxTreeBuilder::buildVariable(QString* name)
{
	Variable* result = new Variable();
	result->setName(*name);
	delete name;
	return result;
}

Expression* SyntaxTreeBuilder::buildVariable(Variable* var)
{
	return var;
}

Variable* SyntaxTreeBuilder::buildVariable(QString* name,Variable::Storage_e c)
{
	Variable* result = new Variable();
	result->setStorage(c);
	result->setName(*name);
	delete name;
	return result;
}

Expression* SyntaxTreeBuilder::buildExpression(Expression* exp,QString* name)
{
	BinaryExpression* result = new BinaryExpression();
	result->setLeft(exp);
	Variable* val = new Variable();
	val->setName(*name);
	delete name;
	result->setRight(val);
	result->setOp(Expression::Dot);
	return result;
}

Expression* SyntaxTreeBuilder::buildExpression(Expression* exp)
{
	return exp;
}

Expression* SyntaxTreeBuilder::buildExpression(Expression::Operator_e op,Expression* exp)
{
	UnaryExpression* result = new UnaryExpression();
	result->setExpression(exp);
	result->setOp(op);
	return result;
}

Expression* SyntaxTreeBuilder::buildExpression(Expression* left ,Expression::Operator_e op, Expression* right)
{
	BinaryExpression* result = new BinaryExpression();
	result->setLeft(left);
	result->setRight(right);
	result->setOp(op);
	return result;
}

Expression* SyntaxTreeBuilder::buildExpression(Expression* cond,Expression* trueexp ,Expression* falseexp)
{
	TernaryExpression* result = new TernaryExpression();
	result->setCondition(cond);
	result->setTrueExpression(trueexp);
	result->setFalseExpression(falseexp);
	return result;
}

Expression* SyntaxTreeBuilder::buildExpression(QList<Expression*>* exps,int count)
{
	VectorExpression* result = new VectorExpression();
	result->setChildren(*exps);
	result->setAdditionalCommas(count);
	delete exps;
	return result;
}

QList<Expression*>* SyntaxTreeBuilder::buildVector()
{
	return new QList<Expression*>();
}

QList<Expression*>* SyntaxTreeBuilder::buildVector(Expression* exp)
{
	QList<Expression*>* result = new QList<Expression*>();
	result->append(exp);
	return result;
}

QList<Expression*>* SyntaxTreeBuilder::buildVector(QList<Expression*>* exprs,unsigned int count,Expression* expr)
{
	if(exprs->size()==0)
		exprs->append(new Variable());

	for(unsigned int i=0; i<count; i++)
		exprs->append(new Variable());
	exprs->append(expr);
	return exprs;
}

Expression* SyntaxTreeBuilder::buildRange(Expression* srt,Expression* fin)
{
	RangeExpression* result = new RangeExpression();
	result->setStart(srt);
	result->setFinish(fin);
	return result;
}

Expression* SyntaxTreeBuilder::buildRange(Expression* srt,Expression* stp,Expression* fin)
{
	RangeExpression* result = new RangeExpression();
	result->setStart(srt);
	result->setFinish(fin);
	result->setStep(stp);
	return result;
}

Invocation* SyntaxTreeBuilder::buildInvocation(QString* name,QList<Argument*>* args)
{
	Invocation* result = new Invocation();
	result->setName(*name);
	delete name;
	result->setArguments(*args);
	delete args;
	return result;
}

Invocation* SyntaxTreeBuilder::buildInvocation(QString* name,Invocation* inv)
{
	inv->setNamespace(*name);
	delete name;
	return inv;
}

Script* SyntaxTreeBuilder::getResult() const
{
	return script;
}
