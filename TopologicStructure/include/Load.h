#pragma once

#include <memory>

#include <TopologicUtility/include/Vector.h>
#include <TopologicCore/include/Vertex.h>
//#include <TopologicCore/include/TopologicalQuery.h>

using namespace System::Collections::Generic;
using namespace System;
using namespace Autodesk::DesignScript::Geometry;
using namespace Autodesk::DesignScript::Runtime;
using namespace Topologic;

namespace TopologicStructure
{
	ref class Context;

	public ref class Load : Topologic::Vertex
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertex"></param>
		/// <param name="vector"></param>
		/// <param name="magnitude"></param>
		/// <returns name="Load"></returns>
		static Load^ ByVertex(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector, double magnitude);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="edge"></param>
		/// <param name="u"></param>
		/// <param name="vector"></param>
		/// <param name="magnitude"></param>
		/// <returns></returns>
		static Load^ ByEdge(Topologic::Edge^ edge, double u, Autodesk::DesignScript::Geometry::Vector^ vector, double magnitude);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="face"></param>
		/// <param name="u"></param>
		/// <param name="v"></param>
		/// <param name="vector"></param>
		/// <param name="magnitude"></param>
		/// <returns></returns>
		static Load^ ByFace(Topologic::Face^ face, double u, double v, [Autodesk::DesignScript::Runtime::DefaultArgument("null")] Autodesk::DesignScript::Geometry::Vector^ vector, [Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool reverseDefaultNormal, double magnitude);

		property double Magnitude
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			double get();
		}

		property Autodesk::DesignScript::Geometry::Vector^ Direction
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			Autodesk::DesignScript::Geometry::Vector^ get();
		}

		property Context^ Context
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Context"></returns>
			TopologicStructure::Context^ get() { return m_pContext; }
		}


		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Geometry"></returns>
			virtual Object^ get() override;
		}

		property Topologic::Topology^ Topology
		{
			Topologic::Topology^ get() { return m_pTopology; }
		}

	public protected:
		Load(Topologic::Vertex^ vertex, const TopologicUtility::Vector::Ptr& vector, const bool attachAttribute);

		Load(const TopologicCore::Vertex::Ptr& vertex);
	protected:
		virtual ~Load();

		TopologicUtility::Vector::Ptr* m_pVector;
		TopologicStructure::Context^ m_pContext;
		Topologic::Topology^ m_pTopology;
	};
}