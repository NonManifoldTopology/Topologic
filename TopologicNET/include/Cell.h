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

#include <TopologicCore/include/Cell.h>

#include "Topology.h"

namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Shell;
	ref class CellComplex;

	/// <summary>
	/// A Cell is a three-dimensional region defined by a collection of closed Shells. It may be manifold or non-manifold.
	/// </summary>
	public ref class Cell : Topology
	{
	public:
		/// <summary>
		/// Creates a Cell by a set of Faces.
		/// </summary>
		/// <param name="faces">A set of Faces</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Cell">The created Cell</returns>
#ifdef TOPOLOGIC_DYNAMO
		static Cell^ ByFaces(System::Collections::Generic::IList<Face^>^ faces, [DefaultArgument("0.0001")] double tolerance);
#else
		static Cell^ ByFaces(System::Collections::Generic::IList<Face^>^ faces, double tolerance);
#endif

		/// <summary>
		/// Creates a Cell from a Shell. The Shell must be closed, otherwise an exception is thrown.
		/// </summary>
		/// <param name="shell">A Shell</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ ByShell(Shell^ shell);

		/// <summary>
		/// Returns the CellComplexes which contain the Cell.
		/// </summary>
		/// <returns name="CellComplex[]">A list of CellComplexes containing the Cell</returns>
		property IList<CellComplex^>^ CellComplexes
		{
			IList<CellComplex^>^ get();
		}

		/// <summary>
		/// Returns the Shells constituent to the Cell.
		/// </summary>
		/// <returns name="Shell[]">A list of Shells constituent to the Cell</returns>
		property IList<Shell^>^ Shells
		{
			IList<Shell^>^ get();
		}

		/// <summary>
		/// Returns the Faces constituent to the Cell.
		/// </summary>
		/// <returns name="Face[]">A list of Faces constituent to the Cell</returns>
		property IList<Face^>^ Faces
		{
			IList<Face^>^ get();
		}

		/// <summary>
		/// Returns the Wires constituent to the Cell.
		/// </summary>
		/// <returns name="Wire[]">A list of Wires constituent to the Cell</returns>
		property IList<Wire^>^ Wires
		{
			IList<Wire^>^ get();
		}

		/// <summary>
		/// Returns the Edges constituent to the Cell.
		/// </summary>
		/// <returns name="Edge[]">A list of Edges constituent to the Cell</returns>
		property IList<Edge^>^ Edges
		{
			IList<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Vertices constituent to the Cell.
		/// </summary>
		/// <returns name="Vertex[]">A list of Vertices constituent to the Cell</returns>
		property IList<Vertex^>^ Vertices
		{
			IList<Vertex^>^ get();
		}

		/// <summary>
		/// Returns the Cells adjacent to the Cell.
		/// </summary>
		/// <returns name="Cell[]">A list of Cells adjacent to the Cell</returns>
		/// 
		property IList<Cell^>^ AdjacentCells
		{
			IList<Cell^>^ get();
		}

		/// <summary>
		/// Returns the shared Faces between two Cells. 
		/// </summary>
		/// <param name="cell">Another Cell</param>
		/// <returns name="Face[]">A list of shared Faces between two Cells</returns>
		IList<Face^>^ SharedFaces(Cell^ cell);

		/// <summary>
		/// Returns the shared Edges between two Cells. 
		/// </summary>
		/// <param name="cell">Another Cell</param>
		/// <returns name="Edge[]">A list of shared Edges between two Cells</returns>
		IList<Edge^>^ SharedEdges(Cell^ cell);

		/// <summary>
		/// Returns the shared Vertices between two Cells. 
		/// </summary>
		/// <param name="cell">Another Cell</param>
		/// <returns name="Vertex[]">A list of shared Vertices between two Cells</returns>
		IList<Vertex^>^ SharedVertices(Cell^ cell);

		/// <summary>
		/// Returns the external boundary (Shell) of the Cell.
		/// </summary>
		/// <returns name="Shell">The external boundary (Shell) of the Cell</returns>
		property Shell^ ExternalBoundary
		{
			Shell^ get();
		}

		/// <summary>
		/// Returns the internal boundaries (Shells) of the Cell.
		/// </summary>
		/// <returns name="Shell[]">A list of the internal boundaries (Shells) of the Cell</returns>
		property IList<Shell^>^ InternalBoundaries
		{
			IList<Shell^>^ get();
		}

		/// <summary>
		/// Creates a geometry from Cell.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ BasicGeometry
		{
			virtual Object^ get() override;
		}

		/// <summary>
		/// Returns the type associated to Cell.
		/// </summary>
		/// <returns name="int">The type associated to Cell</returns>
		static int Type();

	public protected:
		Cell();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreCell"></param>
		Cell(const std::shared_ptr<TopologicCore::Cell>& kpCoreCell);

#ifdef TOPOLOGIC_DYNAMO
		/// <summary>
		/// Creates a Cell from a solid.
		/// </summary>
		/// <param name="solid">A solid</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ BySolid(Autodesk::DesignScript::Geometry::Solid^ solid, [DefaultArgument("0.0001")] double tolerance);

		/// <summary>
		/// Creates a Cell from a sphere.
		/// </summary>
		/// <param name="sphere">A sphere</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ BySphere(Autodesk::DesignScript::Geometry::Sphere^ sphere);

		/// <summary>
		/// Creates a Cell from a cylinder.
		/// </summary>
		/// <param name="cylinder">A cylinder</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ ByCylinder(Autodesk::DesignScript::Geometry::Cylinder^ cylinder);

		/// <summary>
		/// Creates a Cell from a cone.
		/// </summary>
		/// <param name="cone">A cone</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ ByCone(Autodesk::DesignScript::Geometry::Cone^ cone);

		/// <summary>
		/// Creates a Cell from a cuboid.
		/// </summary>
		/// <param name="cuboid">A cuboid</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ ByCuboid(Autodesk::DesignScript::Geometry::Cuboid^ cuboid);
#endif

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Cell();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Cell>* m_pCoreCell;
	};
}