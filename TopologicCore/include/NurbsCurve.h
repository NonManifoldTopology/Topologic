// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

#pragma once

#include "Utilities.h"
#include "Geometry.h"

#include <Geom_BSplineCurve.hxx>
#include <TopoDS_Edge.hxx>

#include <list>

namespace TopologicCore
{
	class Vertex;

	class NurbsCurve : Geometry
	{
	public:
		typedef std::shared_ptr<NurbsCurve> Ptr;

	public:
		TOPOLOGIC_API NurbsCurve(Handle(Geom_BSplineCurve) pOcctBSplineCurve, const TopoDS_Edge& rkOcctEdge);
		virtual ~NurbsCurve();

		TOPOLOGIC_API bool IsPeriodic() const;

		TOPOLOGIC_API bool IsRational() const;

		TOPOLOGIC_API int Degree() const;

		TOPOLOGIC_API void ControlVertices(std::list<std::shared_ptr<Vertex>>& rControlVertices) const;

		TOPOLOGIC_API void Knots(std::list<double>& rKnots) const;

		TOPOLOGIC_API double FirstParameter() const;

		TOPOLOGIC_API double LastParameter() const;

	protected:
		Handle(Geom_BSplineCurve) m_pOcctBSplineCurve;
		TopoDS_Edge m_occtEdge;
	};
}