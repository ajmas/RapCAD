/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
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

#include "modulecontext.h"

ModuleContext::ModuleContext()
{
}

ModuleContext::~ModuleContext()
{
    for(int i=0; i<declarations.size(); i++)
	delete declarations.at(i);
}

 void ModuleContext::setDeclarations(QVector<Declaration*> decls)
 {
     this->declarations = decls;
 }

 QVector<Declaration*> ModuleContext::getDeclarations()
 {
     return this->declarations;
 }

 QString ModuleContext::toString()
 {
     QString result;
     result.append("\nContext: (\n");
	 for(int i=0; i<declarations.size(); i++)
	     result.append(declarations.at(i)->toString());
     result.append(")\n");
     return result;
 }

