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

#include <Geom2d_Transformation.hxx>

#include <Utilities.h>

#include <memory>

namespace TopologicUtilities
{
	class TransformationMatrix2D
	{
	public:
		typedef std::shared_ptr<TransformationMatrix2D> Ptr;

	public:
		TransformationMatrix2D(const Handle(Geom2d_Transformation) kpOcctTransformationMatrix2D);
		~TransformationMatrix2D();

	protected:
		Handle(Geom2d_Transformation) m_pOcctTransformationMatrix2D;
	};
}