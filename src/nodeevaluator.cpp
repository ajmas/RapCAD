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

#include <QVector>
#include "nodeevaluator.h"
#include "rmath.h"
#include "onceonly.h"
#include "decimal.h"
#include "polyhedron.h"
#include "simpletextbuilder.h"

#if USE_CGAL
#include "cgalimport.h"
#include "cgalexplorer.h"
#include "cgalprimitive.h"
#include "cgalfragment.h"
#include "cgalbuilder.h"
#endif

NodeEvaluator::NodeEvaluator(Reporter* r)
{
	reporter=r;
}

Primitive* NodeEvaluator::createPrimitive()
{
#if USE_CGAL
	return new CGALPrimitive();
#else
	return new Polyhedron();
#endif
}

void NodeEvaluator::visit(PrimitiveNode* n)
{
	Primitive* cp=createPrimitive();
	convert(n->getPrimitive(),cp);
	result=cp;
}

void NodeEvaluator::visit(PolylineNode* n)
{
	Primitive* cp=createPrimitive();
	cp->setType(Primitive::Skeleton);
	convert(n->getPrimitive(),cp);
	result=cp;
}

void NodeEvaluator::convert(Primitive* pr,Primitive* cp)
{
	foreach(Point p,pr->getPoints())
		cp->createVertex(p);

	foreach(Polygon* pg,pr->getPolygons()) {
		Polygon* np=cp->createPolygon();
		np->setIndexes(pg->getIndexes());
	}
}

void NodeEvaluator::visit(TriangulateNode* n)
{
	evaluate(n,Union);
	result=result->triangulate();
}

void NodeEvaluator::visit(UnionNode* op)
{
	evaluate(op,Union);
}

void NodeEvaluator::visit(GroupNode* op)
{
	evaluate(op,Group);
}

void NodeEvaluator::visit(DifferenceNode* op)
{
	evaluate(op,Difference);
}

void NodeEvaluator::visit(IntersectionNode* op)
{
	evaluate(op,Intersection);
}

void NodeEvaluator::visit(SymmetricDifferenceNode* op)
{
	evaluate(op,SymmetricDifference);
}

void NodeEvaluator::visit(MinkowskiNode* op)
{
	evaluate(op,Minkowski);
}

void NodeEvaluator::visit(GlideNode* op)
{
	Primitive* first=NULL;
	foreach(Node* n, op->getChildren()) {
		n->accept(*this);
		if(!first) {
#if USE_CGAL
			CGALExplorer explorer(result);
			QList<CGAL::Point3> points = explorer.getPoints();

			CGALPrimitive* cp=new CGALPrimitive();
			CGAL::Point3 fp;
			OnceOnly first_p;
			foreach(CGAL::Point3 pt,points) {
				if(first_p())
					fp=pt;
				cp->appendVertex(pt);
			}
			if(op->getClosed())
				cp->appendVertex(fp);
			first=cp;
#endif
		} else {
			first=first->minkowski(result);
		}
	}

	result=first;
}

void NodeEvaluator::visit(HullNode* n)
{
#if USE_CGAL
	QList<CGAL::Point3> points;
	foreach(Node* c,n->getChildren()) {
		c->accept(*this);
		CGALExplorer explorer(result);
		points.append(explorer.getPoints());
	}

	CGAL::Polyhedron3 hull;
	CGAL::convex_hull_3(points.begin(),points.end(),hull);

	result=new CGALPrimitive(hull);
#endif
}

#if USE_CGAL
static CGAL::Point3 flatten(const CGAL::Point3& p)
{
	return CGAL::Point3(p.x(),p.y(),0);
}

static CGAL::Point3 translate(const CGAL::Point3& p,CGAL::Scalar x,CGAL::Scalar y,CGAL::Scalar z)
{
	CGAL::AffTransformation3 t(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z, 1);
	return p.transform(t);
}

static CGAL::Point3 rotate_y(const CGAL::Point3& p,decimal phi)
{
	decimal c=cos(phi);
	decimal s=sin(phi);
	CGAL::AffTransformation3 t(
		 c, 0, s, 0,
		 0, 1, 0, 0,
		-s, 0, c, 0, 1);

	return p.transform(t);
}
#endif

void NodeEvaluator::visit(LinearExtrudeNode* op)
{
	evaluate(op,Union);
#if USE_CGAL
	CGALPrimitive* cp=new CGALPrimitive();
	if(result->isFullyDimentional()) {
		cp->setType(Primitive::Skeleton);
		cp->createPolygon();
		cp->appendVertex(CGAL::Point3(0.0,0.0,0.0));
		cp->appendVertex(CGAL::Point3(0.0,0.0,op->getHeight()));
		result=result->minkowski(cp);
	} else {
		CGAL::Scalar z=op->getHeight();
		CGALExplorer explorer(result);
		CGALPrimitive* peri=explorer.getPerimeters();
		if(!peri) return;
		CGALPrimitive* prim=explorer.getPrimitive();
		QList<CGALPolygon*> polys=prim->getCGALPolygons();

		bool up;
		foreach(CGALPolygon* pg,polys) {
			cp->createPolygon();
			up=(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				if(up)
					cp->appendVertex(pt);
				else
					cp->prependVertex(pt);
			}
		}

		foreach(CGALPolygon* pg,peri->getCGALPolygons()) {
			up=(pg->getNormal().z()>0);
			OnceOnly first;
			CGAL::Point3 pn;
			foreach(CGAL::Point3 pt, pg->getPoints()) {
				if(!first()) {
					cp->createPolygon();
					CGAL::Point3 p=pn;
					CGAL::Point3 q=pt;
					if(up) {
						cp->appendVertex(translate(p,0,0,z));
						cp->appendVertex(translate(q,0,0,z));
						cp->appendVertex(q);
						cp->appendVertex(p);
					} else {
						cp->prependVertex(translate(p,0,0,z));
						cp->prependVertex(translate(q,0,0,z));
						cp->prependVertex(q);
						cp->prependVertex(p);
					}
				}
				pn=pt;
			}
			break;
		}

		foreach(CGALPolygon* pg,polys) {
			cp->createPolygon();
			up=(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				if(up)
					cp->prependVertex(translate(pt,0,0,z));
				else
					cp->appendVertex(translate(pt,0,0,z));
			}
		}

		result=cp;
	}
#endif
}

void NodeEvaluator::visit(RotateExtrudeNode* op)
{
	evaluate(op,Union);

#if USE_CGAL
	CGAL::Scalar r=op->getRadius();
	CGALExplorer explorer(result);
	//CGALPrimitive* prim=explorer.getPrimitive();
	//QList<CGALPolygon*> polys=prim->getPolygons();
	CGALPrimitive* n = new CGALPrimitive();
	CGALFragment fg(*op->getFragments());
	CGAL::Cuboid3 b=explorer.getBounds();
	int f=fg.getFragments((b.xmax()-b.xmin())+r);

	for(int i=0; i<f; i++) {
		int j=(i+1)%f;
		decimal phi=(r_tau()*i)/f;
		decimal nphi=(r_tau()*j)/f;

		foreach(CGALExplorer::HalfEdgeHandle h, explorer.getHalfEdgePerimeter()) {
			CGAL::Point3 q=translate(h->source()->point(),r,0,0);
			CGAL::Point3 p=translate(h->target()->point(),r,0,0);
			if(q.x()<=0.0&&p.x()<=0.0)
				continue;

			n->createPolygon();
			CGAL::Point3 q1=rotate_y(q,nphi);
			CGAL::Point3 p1=rotate_y(p,nphi);
			CGAL::Point3 p2=rotate_y(p,phi);
			CGAL::Point3 q2=rotate_y(q,phi);
			n->appendVertex(q1);
			n->appendVertex(p1);
			if(p2!=p1)
				n->appendVertex(p2);
			if(q2!=q1)
				n->appendVertex(q2);
		}
	}

	result=n;
#endif
}

void NodeEvaluator::evaluate(Node* op,Operation_e type)
{
	Primitive* first=NULL;
	foreach(Node* n, op->getChildren()) {
		n->accept(*this);
		if(!first) {
			first=result;
		} else {
			switch(type) {
			case Group:
				first->add(result,false);
				break;
			case Union:
				first->add(result,true);
				break;
			case Difference:
				first=first->difference(result);
				break;
			case Intersection:
				first=first->intersection(result);
				break;
			case SymmetricDifference:
				first=first->symmetric_difference(result);
				break;
			case Minkowski:
				first=first->minkowski(result);
				break;
			}
		}
	}

	if(first)
		result=first->combine();
	else
		result=first;

}

void NodeEvaluator::visit(BoundsNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALPrimitive* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();

	decimal xmin=to_decimal(b.xmin());
	decimal ymin=to_decimal(b.ymin());
	decimal xmax=to_decimal(b.xmax());
	decimal ymax=to_decimal(b.ymax());
	decimal zmin=to_decimal(b.zmin());
	decimal zmax=to_decimal(b.zmax());

	if(zmin!=0.0) {
		QString pos=to_string(zmin,false);
		QString where = zmin<0.0?tr("below"):tr("above");
		reporter->reportWarning(tr("the model is %1 %2 the build platform.").arg(pos).arg(where));

		SimpleTextBuilder t;
		t.setText(pos);
		decimal h=t.getHeight()+0.2;
		t.setLocation(Point(xmin,ymin-h,zmin));
		Primitive* c=t.buildPrimitive();
		result->appendChild(c);
	}

	Point lower(xmin,ymin,zmin);
	Point upper(xmax,ymax,zmax);

	Polyhedron* a=new Polyhedron();
	a->setType(Primitive::Skeleton);
	a->createPolygon();
	a->createVertex(lower); //0
	a->createVertex(Point(xmax,ymin,zmin)); //1
	a->createVertex(Point(xmax,ymax,zmin)); //2
	a->createVertex(Point(xmin,ymax,zmin)); //3
	a->createVertex(Point(xmin,ymin,zmax)); //4
	a->createVertex(Point(xmax,ymin,zmax)); //5
	a->createVertex(upper); //6
	a->createVertex(Point(xmin,ymax,zmax)); //7

	//Top
	Polygon* pg=a->createPolygon();
	pg->append(0);
	pg->append(1);
	pg->append(2);
	pg->append(3);
	pg->append(0);

	pg=a->createPolygon();
	pg->append(4);
	pg->append(0);

	pg=a->createPolygon();
	pg->append(5);
	pg->append(1);

	pg=a->createPolygon();
	pg->append(6);
	pg->append(2);

	pg=a->createPolygon();
	pg->append(7);
	pg->append(3);

	//Bottom
	pg=a->createPolygon();
	pg->append(7);
	pg->append(6);
	pg->append(5);
	pg->append(4);
	pg->append(7);

	result->appendChild(a);

	reporter->reportMessage(tr("Bounds: [%1],[%2]").arg(lower.toString(false)).arg(upper.toString(false)));
#endif
}

void NodeEvaluator::visit(SubDivisionNode* n)
{
	evaluate(n,Union);
	//TODO
}

void NodeEvaluator::visit(OffsetNode* n)
{
	evaluate(n,Union);

	result=result->inset(n->getAmount());
}

void NodeEvaluator::visit(OutlineNode* op)
{
	evaluate(op,Union);

#if USE_CGAL
	CGALExplorer explorer(result);
	result=explorer.getPerimeters();
#endif
}

void NodeEvaluator::visit(ImportNode* op)
{
#if USE_CGAL
	CGALImport i(reporter);
	result=i.import(op->getImport());
#else
	result=NULL;
#endif
}

void NodeEvaluator::visit(TransformationNode* tr)
{
	evaluate(tr,Union);
#if USE_CGAL
	decimal* m=tr->matrix;
	CGAL::AffTransformation3 t(
		m[ 0], m[ 1], m[ 2], m[ 3],
		m[ 4], m[ 5], m[ 6], m[ 7],
		m[ 8], m[ 9], m[10], m[11],
	  /*m[12], m[13], m[14]*/m[15]);

	CGALPrimitive* pr=static_cast<CGALPrimitive*>(result);
	pr->transform(t);
#endif
}

void NodeEvaluator::visit(ResizeNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALPrimitive* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	Point s=n->getSize();
	decimal x1,y1,z1;
	s.getXYZ(x1,y1,z1);
	CGAL::Scalar x=x1,y=y1,z=z1,autosize=1.0;

	if(z!=0.0) {
		z/=(b.zmax()-b.zmin());
		autosize=z;
	}
	if(y!=0.0) {
		y/=(b.ymax()-b.ymin());
		autosize=y;
	}
	if(x!=0.0) {
		x/=(b.xmax()-b.xmin());
		autosize=x;
	}
	if(!n->getAutoSize())
		autosize=1.0;

	if(x==0.0) x=autosize;
	if(y==0.0) y=autosize;
	if(z==0.0) z=autosize;

	CGAL::AffTransformation3 t(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0, 1);

	pr->transform(t);
#endif
}

void NodeEvaluator::visit(AlignNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALPrimitive* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Scalar cx=0.0,cy=0.0,cz=0.0;
	CGAL::Scalar two(2.0);
	if(n->getCenter()) {
		cx=(b.xmin()+b.xmax())/two;
		cy=(b.ymin()+b.ymax())/two;
		cz=(b.zmin()+b.zmax())/two;
	} else {
		bool top=false;
		bool bottom=false;
		bool north=false;
		bool south=false;
		bool west=false;
		bool east=false;
		foreach(AlignNode::Face_t a,n->getAlign()) {
			switch(a) {
			case AlignNode::Top:
				top=true;
				cz=b.zmax();
				break;
			case AlignNode::Bottom:
				bottom=true;
				cz=b.zmin();
				break;
			case AlignNode::North:
				north=true;
				cx=b.xmax();
				break;
			case AlignNode::South:
				south=true;
				cx=b.xmin();
				break;
			case AlignNode::West:
				west=true;
				cy=b.ymax();
				break;
			case AlignNode::East:
				east=true;
				cy=b.ymin();
				break;
			}
		}
		if(top&&bottom)
			cz=(b.zmin()+b.zmax())/two;
		if(north&&south)
			cx=(b.xmin()+b.xmax())/two;
		if(east&&west)
			cy=(b.ymin()+b.ymax())/two;
	}

	CGAL::AffTransformation3 t(
				1, 0, 0, -cx,
				0, 1, 0, -cy,
				0, 0, 1, -cz, 1);

	pr->transform(t);
#endif
}

void NodeEvaluator::visit(PointNode* n)
{
	Primitive* cp=createPrimitive();
	cp->setType(Primitive::Single);
	cp->createPolygon();
	cp->createVertex(n->getPoint());
	result=cp;
}

void NodeEvaluator::visit(SliceNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALPrimitive* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();

	CGALPrimitive* cp=new CGALPrimitive();
	const CGAL::Scalar& h=n->getHeight();
	const CGAL::Scalar& xmin=b.xmin();
	const CGAL::Scalar& ymin=b.ymin();
	const CGAL::Scalar& xmax=b.xmax();
	const CGAL::Scalar& ymax=b.ymax();

	CGALBuilder bd(cp);
	bd.makeSideZ(xmin,xmax,ymin,ymax,h);

	decimal t=n->getThickness();
	if(t>0.0) {
		const CGAL::Scalar& z=h+t;
		bd.makeSideY(xmax,xmin,ymin,h,z);
		bd.makeSideX(xmax,ymax,ymin,h,z);
		bd.makeSideY(xmin,xmax,ymax,h,z);
		bd.makeSideX(xmin,ymin,ymax,h,z);

		bd.makeSideZ(xmin,xmax,ymax,ymin,z);
	}

	result=result->intersection(cp);
#endif
}

void NodeEvaluator::visit(ProductNode* p)
{
	Primitive* prim = p->getPrimitive();
	result=prim->copy();
}

void NodeEvaluator::visit(ProjectionNode* op)
{
	evaluate(op,Union);

#if USE_CGAL
	CGALExplorer explorer(result);

	bool base=op->getBase();
	if(base) {
		CGALPrimitive* r=new CGALPrimitive();
		foreach(CGALPolygon* pg,explorer.getBase()) {
			r->createPolygon();
			foreach(CGAL::Point3 pt,pg->getPoints())
				r->appendVertex(pt);
		}
		result=r;
	} else {
		CGALPrimitive* cp=explorer.getPrimitive();

		Primitive* r=new CGALPrimitive();
		foreach(CGALPolygon* p, cp->getCGALPolygons()) {
			CGAL::Vector3 normal=p->getNormal();
			if(normal.z()==0)
				continue;

			CGALPrimitive* t=new CGALPrimitive();
			t->createPolygon();
			foreach(CGAL::Point3 pt,p->getPoints()) {
				t->appendVertex(flatten(pt));
			}
			r=r->join(t);
		}
		result=r;
	}
#endif
}

void NodeEvaluator::visit(DecomposeNode* n)
{
	evaluate(n,Union);
	result=result->decompose();
}

void NodeEvaluator::visit(ComplementNode* n)
{
	evaluate(n,Union);
	result=result->complement();
}

void NodeEvaluator::visit(RadialsNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALPrimitive* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Circle3 circle=pr->getRadius();
	decimal r=inexact_sqrt(circle.squared_radius());
	QString rs=to_string(r,false);
	reporter->reportMessage(tr("Radius: %1").arg(rs));

	CGAL::Point3 c=circle.center();
	decimal a,b;
	a=to_decimal(c.x());
	b=to_decimal(c.y());

	SimpleTextBuilder t;
	t.setText(rs);
	t.setLocation(Point(a,b,0));
	Primitive* cp=t.buildPrimitive();
	result->appendChild(cp);

	Polyhedron* p = new Polyhedron();
	p->setType(Primitive::Skeleton);
	Polygon* pg=p->createPolygon();

	const int f=90;
	for(int i=0; i<=f; i++) {
		decimal phi = (r_tau()*i) / f;
		decimal x,y;
		x = a+r*cos(phi);
		y = b+r*sin(phi);

		p->createVertex(Point(x,y,0));
		pg->append(i);
	}

	result->appendChild(p);
#endif
}

void NodeEvaluator::visit(VolumesNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALPrimitive* pr=static_cast<CGALPrimitive*>(result);
	bool calcMass = n->getCalcMass();
	CGALVolume v=pr->getVolume(calcMass);

	decimal vn=to_decimal(v.getSize());
	QString vs=to_string(vn,false);
	reporter->reportMessage(tr("Volume: %1").arg(vs));

	CGAL::Point3 c=v.getCenter();
	decimal cx,cy,cz;
	cx=to_decimal(c.x());
	cy=to_decimal(c.y());
	cz=to_decimal(c.z());

	if(calcMass)
		reporter->reportMessage(tr("Center of Mass: %1").arg(Point(cx,cy,cz).toString(false)));

	CGAL::Cuboid3 b=v.getBounds();
	decimal x,y,z;
	x=to_decimal(b.xmax()+((b.xmax()-b.xmin())/10));
	y=to_decimal(b.ymax()+((b.ymax()-b.ymin())/10));
	z=to_decimal(b.zmax()+((b.zmax()-b.zmin())/10));
	Point tr(x,y,z);

	Polyhedron* p = new Polyhedron();
	p->setType(Primitive::Skeleton);
	Polygon* pg=p->createPolygon();
	p->createVertex(Point(cx,cy,cz));
	p->createVertex(tr);
	pg->append(0);
	pg->append(1);
	result->appendChild(p);

	SimpleTextBuilder t;
	t.setText(vs);
	t.setLocation(tr);
	Primitive* tp=t.buildPrimitive();
	result->appendChild(tp);
#endif
}

Primitive* NodeEvaluator::getResult() const
{
	return result;
}
