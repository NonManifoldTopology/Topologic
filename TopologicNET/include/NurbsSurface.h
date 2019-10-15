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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "Surface.h"

#include <TopologicCore/include/NurbsSurface.h>

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// </summary>
namespace Topologic {
	ref class Vertex;

	/// <summary>
	/// </summary>
	public ref class NurbsSurface : Surface
	{
	public:

		property int UDegree
		{
			int get();
		}

		property int VDegree
		{
			int get();
		}
		
		property bool IsURational
		{
			bool get();
		}

		property bool IsVRational
		{
			bool get();
		}

		property int NumOfUControlVertices
		{
			int get();
		}

		property int NumOfVControlVertices
		{
			int get();
		}

		Vertex^ ControlVertex(int u, int v);

		property List<double>^ UKnots
		{
			List<double>^ get();
		}

		property List<double>^ VKnots
		{
			List<double>^ get();
		}

		/*property bool IsPeriodic
		{
			bool get();
		}

		property List<Vertex^>^ ControlVertices
		{
			List<Vertex^>^ get();
		}*/

	public protected:
		NurbsSurface(const std::shared_ptr<TopologicCore::NurbsSurface>& kpCoreNurbsSurface);
		virtual ~NurbsSurface();

	protected:
		std::shared_ptr<TopologicCore::NurbsSurface>* m_pCoreNurbsSurface;
	};
}