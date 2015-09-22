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

#include "textvalue.h"
#include "numbervalue.h"
#include "booleanvalue.h"

TextValue::TextValue(QString value)
{
	this->text=value;
	this->defined=true;
}

QString TextValue::getValueString() const
{
	return this->text;
}

TextValue* TextValue::toText()
{
	return this;
}

Value* TextValue::toNumber()
{
	bool ok;
	decimal n=to_decimal(text,&ok);
	if(ok)
		return new NumberValue(n);
	else
		return new Value();
}

bool TextValue::isTrue() const
{
	return !this->text.isEmpty();
}

Value* TextValue::operation(Value& v,Expression::Operator_e e)
{
	TextValue* that=dynamic_cast<TextValue*>(&v);
	if(that) {
		if(isComparison(e)) {
			return new BooleanValue(operation(this,e,that));
		} else {
			return new TextValue(operation(this->text,e,that->text));
		}
	}

	NumberValue* num=dynamic_cast<NumberValue*>(&v);
	if(num)
		if(e==Expression::Index)
			return new TextValue(this->text.at(num->toInteger()));

	return Value::operation(v,e);
}

QString TextValue::operation(QString left, Expression::Operator_e e, QString right)
{
	switch(e) {
	case Expression::Concatenate:
		return left.append(right);
	default:
		return this->text;
	}
}

bool TextValue::operation(TextValue* left, Expression::Operator_e e, TextValue* right)
{
	switch(e) {
	case Expression::Equal:
		return left->text==right->text;
	case Expression::NotEqual:
		return left->text!=right->text;
	default:
		return basicOperation<bool,bool>(left->isTrue(),e,right->isTrue());
	}
}
