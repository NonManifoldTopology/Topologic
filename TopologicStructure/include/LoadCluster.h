#pragma once

#include <memory>

using namespace System::Collections::Generic;
using namespace System;
using namespace Autodesk::DesignScript::Geometry;
using namespace Topologic;

namespace TopologicStructure
{
	ref class Context;
	ref class Load;

	public ref class LoadCluster : Topologic::Cluster
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="loads"></param>
		/// <returns></returns>
		static LoadCluster^ ByLoads(List<Load^>^ loads);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="edge"></param>
		/// <param name="vector"></param>
		/// <param name="magnitude"></param>
		/// <param name="rows"></param>
		/// <param name="uScale"></param>
		/// <param name="uShift"></param>
		/// <returns></returns>
		static LoadCluster^ ByEdge(Topologic::Edge^ edge, Autodesk::DesignScript::Geometry::Vector^ vector, double magnitude, int rows, double uScale, double uShift);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="face"></param>
		/// <param name="vector"></param>
		/// <param name="reverseDefaultNormal"></param>
		/// <param name="magnitude"></param>
		/// <param name="rows"></param>
		/// <param name="columns"></param>
		/// <param name="uScale"></param>
		/// <param name="vScale"></param>
		/// <param name="uShift"></param>
		/// <param name="vShift"></param>
		/// <returns></returns>
		static LoadCluster^ ByFace(Topologic::Face^ face, [Autodesk::DesignScript::Runtime::DefaultArgument("null")] Autodesk::DesignScript::Geometry::Vector^ vector, [Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool reverseDefaultNormal, double magnitude, int rows, int columns, double uScale, double vScale, double uShift, double vShift);

		property List<Load^>^ Loads
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Load[]"></returns>
			List<Load^>^ get() { return m_pLoads; }
		}

		property TopologicStructure::Context^ Context
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Context"></returns>
			TopologicStructure::Context^ get() { return m_pContext; }
		}

		property int Rows
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			int get() { return m_rows; }
		}

		property int Columns
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			int get() { return m_columns; }
		}

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Geometry"></returns>
			virtual Object^ get() override;
		}

	protected:
		LoadCluster(List<Load^>^ loads, TopologicStructure::Context^ context, int rows, int columns);
		~LoadCluster();

		List<Load^>^ m_pLoads;
		TopologicStructure::Context^ m_pContext;
		int m_rows;
		int m_columns;
	};
}