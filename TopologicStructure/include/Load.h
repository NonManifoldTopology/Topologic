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