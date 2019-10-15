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

#include "Topology.h"

#include <list>

#include <TopoDS_CompSolid.hxx>

namespace TopologicCore
{
	class Cell;
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;

	class CellComplexGUID
	{
	public:
		static std::string Get()
		{
			return std::string("4ec9904b-dc01-42df-9647-2e58c2e08e78");
		}
	};

	/// <summary>
	/// A CellComplex is a contiguous collection of Cells where adjacent Cells are connected by shared Faces. It is non-manifold.
	/// </summary>
	class CellComplex : public Topology
	{
	public:
		typedef std::shared_ptr<CellComplex> Ptr;

	public:
		CellComplex(const TopoDS_CompSolid& rkOcctCompSolid, const std::string& rkGuid = "");

		virtual ~CellComplex();

		/// <summary>
		/// Returns the Cells constituent to the CellComplex.
		/// </summary>
		/// <param name="rCells">A list of Cells constituent to the CellComplex</param>
		TOPOLOGIC_API void Cells(std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// Returns the Faces constituent to the CellComplex.
		/// </summary>
		/// <param name="rCells">A list of Faces constituent to the CellComplex</param>
		TOPOLOGIC_API void Faces(std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// Returns the Shells constituent to the CellComplex.
		/// </summary>
		/// <returns name="rShells">A list of Shells constituent to the CellComplex</returns>
		TOPOLOGIC_API void Shells(std::list<std::shared_ptr<Shell>>& rShells) const;

		/// <summary>
		/// Returns the Edges constituent to the CellComplex.
		/// </summary>
		/// <param name="rEdges">A list of Edges constituent to the CellComplex</param>
		TOPOLOGIC_API void Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// Returns the Vertices constituent to the CellComplex.
		/// </summary>
		/// <param name="rVertices">A list of Vertices constituent to the CellComplex</param>
		TOPOLOGIC_API void Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// Returns the Wires constituent to the CellComplex.
		/// </summary>
		/// <param name="rWires">A list of Wires constituent to the CellComplex</param>
		TOPOLOGIC_API void Wires(std::list<std::shared_ptr<Wire>>& rWires) const;

		/// <summary>
		/// Creates a CellComplex by a set of Cells.
		/// </summary>
		/// <param name="rkCells">A set of Cells</param>
		/// <returns name="CellComplex">The created CellComplex</returns>
		TOPOLOGIC_API static std::shared_ptr<CellComplex> ByCells(const std::list<std::shared_ptr<Cell>>& rkCells);

		/// <summary>
		/// Creates an OCCT CompSolid by a set of OCCT Solids.
		/// </summary>
		/// <param name="rkOcctSolids">A set of OCCT Solids</param>
		/// <returns name="CellComplex">The created OCCT CompSolid</returns>
		static TopoDS_CompSolid ByOcctSolids(const TopTools_ListOfShape& rkOcctSolids);

		/// <summary>
		/// Creates a CellComplex from the space enclosed by a set of Faces. Parts of the Faces which do not enclose any space will be discarded.
		/// </summary>
		/// <param name="rkFaces">A set of Faces</param>
		/// <returns name="CellComplex">The created CellComplex</returns> 
		TOPOLOGIC_API static std::shared_ptr<CellComplex> ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces, const double kTolerance);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Cell> ExternalBoundary() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkInternalFaces"></param>
		/// <returns></returns>
		TOPOLOGIC_API void InternalBoundaries(std::list<std::shared_ptr<Face>>& rInternalFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual bool IsManifold() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rNonManifoldFaces"></param>
		/// <returns></returns>
		TOPOLOGIC_API void NonManifoldFaces(std::list<std::shared_ptr<Face>>& rNonManifoldFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Shape& GetOcctShape();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Shape& GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_CompSolid& GetOcctCompSolid();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_CompSolid& GetOcctCompSolid() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctCompSolid"></param>
		void SetOcctCompSolid(const TopoDS_CompSolid& rkOcctCompSolid);

		/// <summary>
		/// 
		/// </summary>
		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		static TopoDS_Vertex CenterOfMass(const TopoDS_CompSolid& rkOcctCompSolid);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CELLCOMPLEX; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return CellComplexGUID::Get();
		}

		static TOPOLOGIC_API int Type() { return TopologicCore::TOPOLOGY_CELLCOMPLEX; }

		virtual bool IsContainerType() { return true; }

	protected:
		/// <summary>
		/// The underlying OCCT cell complex.
		/// </summary>
		TopoDS_CompSolid m_occtCompSolid;
	};
}
