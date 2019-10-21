// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "Geometry.h"

#include <TopologicCore/include/NurbsCurve.h>

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// </summary>
namespace Topologic {
	ref class Vertex;
	/// <summary>
	/// </summary>
	public ref class NurbsCurve : Geometry
	{
	public:
		property bool IsPeriodic
		{
			bool get();
		}
		
		property bool IsRational
		{
			bool get();
		}

		property int Degree
		{
			int get();
		}

		property List<Vertex^>^ ControlVertices
		{
			List<Vertex^>^ get();
		}

		property List<double>^ Knots
		{
			List<double>^ get();
		}

		property double FirstParameter
		{
			double get();
		}

		property double LastParameter
		{
			double get();
		}

	public protected:
		NurbsCurve(const std::shared_ptr<TopologicCore::NurbsCurve>& kpCoreNurbsCurve);
		virtual ~NurbsCurve();

	protected:
		std::shared_ptr<TopologicCore::NurbsCurve>* m_pCoreNurbsCurve;
	};
}