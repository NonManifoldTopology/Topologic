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

#include <Geom_Plane.hxx>
#include <TopoDS_Face.hxx>

#include <list>

namespace TopologicCore
{
	class NurbsSurface;

	class PlanarSurface : Geometry
	{
	public:
		typedef std::shared_ptr<PlanarSurface> Ptr;

	public:
		TOPOLOGIC_API PlanarSurface(Handle(Geom_Plane) pOcctPlane, const TopoDS_Face& rkOcctFace);
		virtual ~PlanarSurface();

		TOPOLOGIC_API void Coefficients(std::list<double>& rCoefficients) const;

		TOPOLOGIC_API double XMin() const;
		TOPOLOGIC_API double XMax() const;
		TOPOLOGIC_API double YMin() const;
		TOPOLOGIC_API double YMax() const;

		TOPOLOGIC_API std::shared_ptr<NurbsSurface> ToNurbsSurface() const;

	protected:
		Handle(Geom_Plane) m_pOcctPlane;
		TopoDS_Face m_occtFace;
	};
}