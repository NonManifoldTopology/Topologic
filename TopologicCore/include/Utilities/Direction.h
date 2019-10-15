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
#include "Vector.h"

#include <Geom_Direction.hxx>

#include <memory>

namespace TopologicUtilities
{
	class Direction
	{
	public:
		typedef std::shared_ptr<Direction> Ptr;

	public:
		Direction(const Handle(Geom_Direction) kpOcctVector);
		~Direction();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kX"></param>
		/// <param name="kY"></param>
		/// <param name="kZ"></param>
		/// <returns></returns>
		TOPOLOGIC_API static Direction::Ptr ByCoordinates(const double kX, const double kY, const double kZ);

		TOPOLOGIC_API static Direction::Ptr ByVector(const Vector::Ptr& kpVector);

	protected:

		Handle(Geom_Direction) m_pOcctDirection;
	};
}