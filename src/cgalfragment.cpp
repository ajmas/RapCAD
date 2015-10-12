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
#if USE_CGAL
#include "cgalfragment.h"
#include "rmath.h"

CGALFragment::CGALFragment(const Fragment f) : Fragment(f)
{
}

int CGALFragment::getFragments(CGAL::Scalar r)
{
	int fn=fragmentNumber;
	if(fn > 0.0) return (int)(fn >= 3 ? fn : 3);

	CGAL::Scalar fs=fragmentSize;
	CGAL::Scalar fa=fragmentAngle;
	CGAL::Scalar f=std::min(CGAL::Scalar(360.0) / fa, r*CGAL::Scalar(r_tau()) / fs);
	return std::max((int)ceil(to_decimal(f)),5);
}
#endif
