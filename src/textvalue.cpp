/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

TextValue::TextValue(QString value)
{
	this->text=value;
}

QString TextValue::getValueString()
{
	return this->text;
}

Value* TextValue::operator+(Value& v)
{
	return operation(v,Expression::Add);
}

Value* TextValue::operation(Value& v,Expression::Operator_e e)
{
	TextValue* that=dynamic_cast<TextValue*>(&v);
	if(that)
		return new TextValue(operation(this->text,e,that->text));
}

QString TextValue::operation(QString left, Expression::Operator_e e, QString right)
{
	switch(e) {
	case Expression::Add:
		return left.append(right);
	}
}
