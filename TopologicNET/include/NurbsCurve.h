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