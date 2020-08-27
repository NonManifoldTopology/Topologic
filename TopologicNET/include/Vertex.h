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

#include <Topology.h>

#include <TopologicCore/include/Vertex.h>

namespace Topologic
{
	ref class Edge;

	/// <summary>
	/// A Vertex is a zero-dimensional entity equivalent to a geometry point.
	/// </summary>

	public ref class Vertex : Topology
	{
	public:
		/// <summary>
		/// Creates a Vertex by XYZ coordinates.
		/// </summary>
		/// <param name="x">The X coordinate</param>
		/// <param name="y">The Y coordinate</param>
		/// <param name="z">The Z coordinate</param>
		/// <returns name="Vertex">The created Vertex</returns>
#ifdef TOPOLOGIC_DYNAMO
		static Vertex^ ByCoordinates(
			[DefaultArgument("0")] double x, 
			[DefaultArgument("0")] double y, 
			[DefaultArgument("0")] double z);
#else
		static Vertex^ ByCoordinates(double x, double y, double z);
#endif

		/// <summary>
		/// Returns the Edges incident to the Vertex.
		/// </summary>
		/// <returns name="Edge[]">A list of the Edges incident to the Vertex</returns>
		property IEnumerable<Edge^>^ Edges
		{
			IEnumerable<Edge^>^ get();
		}

		/// <summary>
		/// Returns the X coordinate of the Vertex.
		/// </summary>
		/// <returns>The X coordinate of the Vertex</returns>
		property double X
		{
			virtual double get();
		}

		/// <summary>
		/// Returns the Y coordinate of the Vertex.
		/// </summary>
		/// <returns>The Y coordinate of the Vertex</returns>
		property double Y
		{
			virtual double get();
		}

		/// <summary>
		/// Returns the Z coordinate of the Vertex.
		/// </summary>
		/// <returns>The Z coordinate of the Vertex</returns>
		property double Z
		{
			virtual double get();
		}

		/// <summary>
		/// Returns the coordinates of the Vertex.
		/// </summary>
		/// <returns name="double[]">The coordinates of the Vertex</returns>
		property IEnumerable<double>^ Coordinates
		{
			virtual IEnumerable<double>^ get();
		}

		/// <summary>
		/// Creates a geometry from Vertex.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ BasicGeometry
		{
			virtual Object^ get() override;	
		}

		/// <summary>
		/// Returns the type associated to Vertex.
		/// </summary>
		/// <returns>The type associated to Vertex</returns>
		static int Type();

	public protected:
		Vertex();
		Vertex(const std::shared_ptr<TopologicCore::Vertex>& kpCoreVertex);

		/// <summary>
		/// Creates a Vertex by a point.
		/// </summary>
		/// <param name="point">A Dynamo point</param>
		/// <returns name="Vertex">The created Vertex</returns>
#ifdef TOPOLOGIC_DYNAMO
		static Vertex^ ByPoint(Autodesk::DesignScript::Geometry::Point^ point);

		Autodesk::DesignScript::Geometry::Point^ Point();
#endif

		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Vertex();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Vertex>* m_pCoreVertex;
	};
}