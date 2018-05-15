#pragma once

#include <memory>

using namespace System::Collections::Generic;
using namespace System;
using namespace Autodesk::DesignScript::Geometry;
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
		/// <returns name="Load"></returns>
		static Load^ ByVertex(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="edge"></param>
		/// <param name="u"></param>
		/// <param name="vector"></param>
		/// <returns></returns>
		static Load^ ByEdge(Topologic::Edge^ edge, double u, Autodesk::DesignScript::Geometry::Vector^ vector);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="face"></param>
		/// <param name="u"></param>
		/// <param name="v"></param>
		/// <param name="vector"></param>
		/// <returns></returns>
		static Load^ ByFace(Topologic::Face^ face, double u, double v, [Autodesk::DesignScript::Runtime::DefaultArgument("null")] Autodesk::DesignScript::Geometry::Vector^ vector);

		property double Magnitude
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			double get();
		}

		property Autodesk::DesignScript::Geometry::Vector^ Vector
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			Autodesk::DesignScript::Geometry::Vector^ get() { return m_pVector; }
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

	public protected:

	protected:
		Load(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector);
		virtual ~Load();

		Autodesk::DesignScript::Geometry::Vector^ m_pVector;
		TopologicStructure::Context^ m_pContext;
	};
}