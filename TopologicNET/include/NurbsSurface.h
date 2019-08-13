#pragma once

#include "Geometry.h"

#include <TopologicCore/include/NurbsSurface.h>

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// </summary>
namespace Topologic {
	ref class Vertex;

	/// <summary>
	/// </summary>
	public ref class NurbsSurface : Geometry
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