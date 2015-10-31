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

#include "vectorvalue.h"
#include "numbervalue.h"
#include "vectoriterator.h"
#include "rangevalue.h"
#include "booleanvalue.h"
#include "rmath.h"

VectorValue::VectorValue()
{
}

VectorValue::VectorValue(QList<Value*> values)
{
	this->children=values;
	this->defined=true;
}

QString VectorValue::getValueString() const
{
	QString result;
	result.append("[");
	for(int i=0; i<children.size(); i++) {
		if(i>0)
			result.append(",");
		result.append(children.at(i)->getValueString());
	}
	result.append("]");
	return result;
}

bool VectorValue::isTrue() const
{
	return this->children.size()>0;
}

VectorValue* VectorValue::toVector(int)
{
	return this;
}

Value* VectorValue::toNumber()
{
	if(children.size()==1)
		return children.at(0)->toNumber();
	return new Value();
}

Point VectorValue::getPoint() const
{

	NumberValue* nx=NULL,*ny=NULL,*nz=NULL;
	int s=children.size();
	if(s>0)
		nx=dynamic_cast<NumberValue*>(children.at(0));
	if(s>1)
		ny=dynamic_cast<NumberValue*>(children.at(1));
	if(s>2)
		nz=dynamic_cast<NumberValue*>(children.at(2));

	decimal x=0,y=0,z=0;
	if(nx)  {
		x=nx->getNumber();
	}
	if(ny) {
		y=ny->getNumber();
	}
	if(nz) {
		z=nz->getNumber();
	}
	return Point(x,y,z);
}

Iterator<Value*>* VectorValue::createIterator()
{
	return new VectorIterator(this->children);
}

QList<Value*> VectorValue::getChildren()
{
	return this->children;
}

Value* VectorValue::operation(Expression::Operator_e e)
{
	if(e==Expression::Length) {
		Value* v=Value::operation(this,Expression::Multiply,this);
		NumberValue* n=dynamic_cast<NumberValue*>(v);
		if(n)
			return new NumberValue(r_sqrt(n->getNumber()));
		return new Value();
	}
	QList<Value*> result;
	foreach(Value* c,children)
		result.append(Value::operation(c,e));
	return new VectorValue(result);
}

Value* VectorValue::operation(Value& v, Expression::Operator_e e)
{
	QList<Value*> result;
	VectorValue* vec=dynamic_cast<VectorValue*>(&v);
	if(vec) {
		QList<Value*> a=this->getChildren();
		QList<Value*> b=vec->getChildren();

		if(e==Expression::OuterProduct) {
			foreach(Value* c,b)
				result.append(Value::operation(this,e,c));
		} else if(e==Expression::Multiply) {
			int s=std::min(a.size(),b.size());
			if(s<=0)
				return new Value();
			Value* total=new NumberValue(0);
			for(int i=0; i<s; i++) {
				Value* r=Value::operation(a.at(i),e,b.at(i));
				total=Value::operation(total,Expression::Add,r);
			}
			return total;
		} else if(e==Expression::Divide) {
			//TODO vector division?
			return this;
		} else if(e==Expression::Concatenate) {
			result=a;
			result.append(b);
		} else if(e==Expression::Equal||e==Expression::NotEqual) {
			bool eq=(a.size()==b.size());
			if(e==Expression::NotEqual && !eq)
				return new BooleanValue(true);
			if(eq)
				for(int i=0; i<a.size(); i++) {
					Value* eqVec=Value::operation(a.at(i),e,b.at(i));
					if(e==Expression::NotEqual && eqVec->isTrue())
						return new BooleanValue(true);
					if(!eqVec->isTrue())
						eq=false;
				}
			return new BooleanValue(eq);
		} else {
			e=convertOperation(e);
			int as=a.size();
			int bs=b.size();
			for(int i=0; i<as||i<bs; i++) {
				Value* r;
				if(as<bs&&i>=as) {
					r=b.at(i);
				} else if(bs<as&&i>=bs) {
					r=a.at(i);
				} else {
					r=Value::operation(a.at(i),e,b.at(i));
				}
				result.append(r);

			}
		}
		return new VectorValue(result);
	}

	NumberValue* num = dynamic_cast<NumberValue*>(&v);
	if(num) {
		if(e==Expression::Concatenate) {
			QList<Value*> a=this->getChildren();
			result=a;
			result.append(num);
		} else if(e==Expression::Index) {
			Iterator<Value*>* it=this->createIterator();
			int i=num->toInteger();
			if(i>=0) {
				it->first();
				for(int j=0; j<i&&!it->isDone(); ++j)
					it->next();
				if(!it->isDone())
					return it->currentItem();
			}
			return new Value();
		} else {
			QList<Value*> a=this->getChildren();
			e=convertOperation(e);
			foreach(Value* c,a)
				result.append(Value::operation(c,e,num));
		}
		return new VectorValue(result);
	}

	return Value::operation(v,e);
}

Expression::Operator_e VectorValue::convertOperation(Expression::Operator_e e)
{
	switch(e) {
	case Expression::ComponentwiseMultiply:
		return Expression::Multiply;
	case Expression::ComponentwiseDivide:
		return Expression::Divide;
	case Expression::OuterProduct:
		return Expression::Multiply;
	default:
		return e;
	}
}
