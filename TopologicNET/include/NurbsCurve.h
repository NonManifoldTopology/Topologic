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