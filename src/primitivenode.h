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

#ifndef PRIMITIVENODE_H
#define PRIMITIVENODE_H

#include <QVector>
#include <QString>
#include "abstractnode.h"
#include "point.h"

class PrimitiveNode : public AbstractNode
{
public:
	typedef QVector<Point> Polygon;

	PrimitiveNode();
	void createPolygon();
	void appendVertex(double x, double y, double z);
	void appendVertex(Point p);
	QString toString();
private:
	QVector<Polygon> polygons;
};

#endif // PRIMITIVENODE_H