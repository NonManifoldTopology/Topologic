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

#include <Topology.h>

#include <TopologicCore/include/CellComplex.h>

namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Shell;
	ref class Cell;

	/// <summary>
	/// A CellComplex is a contiguous collection of Cells where adjacent Cells are connected by shared Faces. It is non-manifold.
	/// </summary>
	public ref class CellComplex : Topology
	{
	public:
		/// <summary>
		/// Creates a CellComplex by a set of Cells.
		/// </summary>
		/// <param name="cells">A set of Cells</param>
		/// <returns name="CellComplex">The created CellComplex</returns>
		static CellComplex^ ByCells(System::Collections::Generic::IEnumerable<Cell^>^ cells);

		/// <summary>
		/// Creates a CellComplex from the space enclosed by a set of Faces. Parts of the Faces which do not enclose any space will be discarded.
		/// </summary>
		/// <param name="faces">A set of Faces</param>
		/// <returns name="CellComplex">The created CellComplex</returns> 
		static CellComplex^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
		double tolerance);

		/// <summary>
		/// Returns the Cells constituent to the CellComplex.
		/// </summary>
		/// <returns name="Cell[]">A list of Cells constituent to the CellComplex</returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Returns the Faces constituent to the CellComplex.
		/// </summary>
		/// <returns name="Face[]">A list of Faces constituent to the CellComplex</returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Returns the Shells constituent to the CellComplex.
		/// </summary>
		/// <returns name="Shell[]">A list of Shells constituent to the CellComplex</returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Returns the Wires constituent to the CellComplex.
		/// </summary>
		/// <returns name="Wire[]">A list of Wires constituent to the CellComplex</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Returns the Edges constituent to the CellComplex.
		/// </summary>
		/// <returns name="Edge[]">A list of Edges constituent to the CellComplex</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Vertices constituent to the CellComplex.
		/// </summary>
		/// <returns name="Vertex[]">A list of Vertices constituent to the CellComplex</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}


		/// <summary>
		/// Returns the external boundary (Cell) of the CellComplex.
		/// </summary>
		/// <returns name="Cell">The external boundary (Cell) of the CellComplex</returns>
		property Cell^ ExternalBoundary
		{
			Cell^ get();
		}

		/// <summary>
		/// Returns the internal boundaries (Faces) of the CellComplex.
		/// </summary>
		/// <returns name="Face[]">A list of the internal boundaries (Faces) of the CellComplex</returns>
		property List<Face^>^ InternalBoundaries
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Returns the non-manifold Faces of the CellComplex.
		/// </summary>
		/// <returns name="Face[]">A list of the non-manifold Faces of the CellComplex.</returns>
		property List<Face^>^ NonManifoldFaces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Creates a geometry from CellComplex.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ BasicGeometry
		{
			virtual Object^ get() override;
		}

		/// <summary>
		/// Returns the type associated to CellComplex.
		/// </summary>
		/// <returns>The type associated to CellComplex</returns>
		static int Type();

	public protected:
		CellComplex();

		/// <summary>
		/// 
		/// </summary>
		CellComplex(const std::shared_ptr<TopologicCore::CellComplex>& kpCoreCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::CellComplex>* m_pCoreCellComplex;
	};
}