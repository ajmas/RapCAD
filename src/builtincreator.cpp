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

#include "builtincreator.h"

#include "module/echomodule.h"
#include "module/cubemodule.h"
#include "module/squaremodule.h"
#include "module/cylindermodule.h"
#include "module/conemodule.h"
#include "module/cylindersurfacemodule.h"
#include "module/prismmodule.h"
#include "module/circlemodule.h"
#include "module/polyhedronmodule.h"
#include "module/polygonmodule.h"
#include "module/polylinemodule.h"
#include "module/beziersurfacemodule.h"

#include "module/differencemodule.h"
#include "module/unionmodule.h"
#include "module/groupmodule.h"
#include "module/intersectionmodule.h"
#include "module/translatemodule.h"
#include "module/symmetricdifferencemodule.h"
#include "module/multmatrixmodule.h"
#include "module/minkowskimodule.h"
#include "module/glidemodule.h"
#include "module/linearextrudemodule.h"
#include "module/rotateextrudemodule.h"
#include "module/hullmodule.h"
#include "module/rotatemodule.h"
#include "module/mirrormodule.h"
#include "module/scalemodule.h"
#include "module/shearmodule.h"
#include "module/spheremodule.h"
#include "module/childmodule.h"
#include "module/boundsmodule.h"
#include "module/subdivisionmodule.h"
#include "module/offsetmodule.h"
#include "module/outlinemodule.h"
#include "module/importmodule.h"
#include "module/resizemodule.h"
#include "module/centermodule.h"
#include "module/pointmodule.h"
#include "module/slicemodule.h"
#include "module/projectionmodule.h"
#include "module/decomposemodule.h"
#include "module/alignmodule.h"
#include "module/complementmodule.h"
#include "module/radialsmodule.h"
#include "module/volumesmodule.h"

#include "function/lengthfunction.h"
#include "function/sqrtfunction.h"
#include "function/sumfunction.h"
#include "function/randfunction.h"
#include "function/versionfunction.h"
#include "function/strfunction.h"
#include "function/sinfunction.h"
#include "function/cosfunction.h"
#include "function/concatfunction.h"
#include "function/tanfunction.h"
#include "function/absfunction.h"
#include "function/signfunction.h"
#include "function/minfunction.h"
#include "function/maxfunction.h"
#include "function/roundfunction.h"
#include "function/ceilfunction.h"
#include "function/floorfunction.h"
#include "function/powfunction.h"
#include "function/expfunction.h"
#include "function/asinfunction.h"
#include "function/acosfunction.h"
#include "function/atanfunction.h"
#include "function/atan2function.h"
#include "function/coshfunction.h"
#include "function/sinhfunction.h"
#include "function/tanhfunction.h"
#include "function/lnfunction.h"
#include "function/logfunction.h"
#include "function/radfunction.h"
#include "function/degfunction.h"
#include "function/numfunction.h"

#include "module/writemodule.h"
#include "module/writelnmodule.h"

#include "treeprinter.h"

BuiltinCreator::BuiltinCreator(QTextStream& output)
{
	builtins.append(new AlignModule());
	builtins.append(new BezierSurfaceModule());
	builtins.append(new BoundsModule());
	builtins.append(new CenterModule());
	builtins.append(new ChildModule());
	builtins.append(new CircleModule());
	builtins.append(new ComplementModule());
	builtins.append(new ConeModule());
	builtins.append(new CubeModule());
	builtins.append(new CylinderModule());
	builtins.append(new CylinderSurfaceModule());
	builtins.append(new DecomposeModule());
	builtins.append(new DifferenceModule());
	builtins.append(new EchoModule(output));
	builtins.append(new GlideModule());
	builtins.append(new GroupModule());
	builtins.append(new HullModule());
	builtins.append(new IntersectionModule());
	builtins.append(new LinearExtrudeModule());
	builtins.append(new MinkowskiModule());
	builtins.append(new MirrorModule());
	builtins.append(new MultMatrixModule());
	builtins.append(new OffsetModule());
	builtins.append(new OutlineModule());
	builtins.append(new PointModule());
	builtins.append(new PolygonModule());
	builtins.append(new PolyhedronModule());
	builtins.append(new PolylineModule());
	builtins.append(new PrismModule());
	builtins.append(new ProjectionModule());
	builtins.append(new RadialsModule());
	builtins.append(new ResizeModule());
	builtins.append(new RotateExtrudeModule());
	builtins.append(new RotateModule());
	builtins.append(new ScaleModule());
	builtins.append(new ShearModule());
	builtins.append(new SliceModule());
	builtins.append(new SphereModule());
	builtins.append(new SquareModule());
	builtins.append(new SubDivisionModule());
	builtins.append(new SymmetricDifferenceModule());
	builtins.append(new TranslateModule());
	builtins.append(new UnionModule());
	builtins.append(new VolumesModule());
	builtins.append(new WriteLnModule(output));
	builtins.append(new WriteModule(output));

	builtins.append(new AbsFunction());
	builtins.append(new AcosFunction());
	builtins.append(new AsinFunction());
	builtins.append(new Atan2Function());
	builtins.append(new AtanFunction());
	builtins.append(new CeilFunction());
	builtins.append(new ConcatFunction());
	builtins.append(new CosFunction());
	builtins.append(new CoshFunction());
	builtins.append(new DegFunction());
	builtins.append(new ExpFunction());
	builtins.append(new FloorFunction());
	builtins.append(new LengthFunction());
	builtins.append(new LnFunction());
	builtins.append(new LogFunction());
	builtins.append(new MaxFunction());
	builtins.append(new MinFunction());
	builtins.append(new NumFunction());
	builtins.append(new PowFunction());
	builtins.append(new RadFunction());
	builtins.append(new RandFunction());
	builtins.append(new RoundFunction());
	builtins.append(new SignFunction());
	builtins.append(new SinFunction());
	builtins.append(new SinhFunction());
	builtins.append(new SqrtFunction());
	builtins.append(new StrFunction());
	builtins.append(new SumFunction());
	builtins.append(new TanFunction());
	builtins.append(new TanhFunction());
	builtins.append(new VersionFunction());
}

BuiltinCreator* BuiltinCreator::instance=NULL;

BuiltinCreator* BuiltinCreator::getInstance(QTextStream& output)
{
	if(!instance)
		instance = new BuiltinCreator(output);

	return instance;
}

/**
  Add the builtins to a script.
*/
void BuiltinCreator::initBuiltins(Script* sc)
{
	foreach(Declaration* d,builtins)
		sc->addDeclaration(d);
}

/**
  To ensure that the builtins do not get deleted when the script
  is deleted we remove them from the script.
*/
void BuiltinCreator::saveBuiltins(Script* sc)
{
	foreach(Declaration* d,builtins)
		sc->removeDeclaration(d);
}

void BuiltinCreator::generateDocs(QTextStream& output)
{
	TreePrinter p(output);
	foreach(Declaration* d,builtins)
		d->accept(p);
}
