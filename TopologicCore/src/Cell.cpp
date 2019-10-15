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

#include "Cell.h"
#include "Vertex.h"
#include "Edge.h"
#include "Wire.h"
#include "Face.h"
#include "Shell.h"
#include "CellComplex.h"
#include "CellFactory.h"
#include "GlobalCluster.h"
#include "AttributeManager.h"

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepClass3d.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <Message_ProgressIndicator.hxx>
#include <ShapeFix_Solid.hxx>
#include <StdFail_NotDone.hxx>
#include <TopoDS.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Cell::AdjacentCells(std::list<Cell::Ptr>& rCells) const
	{
		// Iterate through the faces and find the incident cells which are not this cell.
		TopTools_IndexedDataMapOfShapeListOfShape occtFaceSolidMap;
		TopExp::MapShapesAndUniqueAncestors(GlobalCluster::GetInstance().GetOcctCompound(), TopAbs_FACE, TopAbs_SOLID, occtFaceSolidMap);

		// Find the constituent faces
		TopTools_MapOfShape occtFaces;
		DownwardNavigation(GetOcctShape(), TopAbs_FACE, occtFaces);

		const TopoDS_Solid& rkOcctSolid = GetOcctSolid();
		TopTools_MapOfShape occtAdjacentSolids;
		for (TopTools_MapOfShape::const_iterator kOcctFaceIterator = occtFaces.cbegin();
			kOcctFaceIterator != occtFaces.cend();
			kOcctFaceIterator++)
		{
			const TopoDS_Shape& rkOcctFace = *kOcctFaceIterator;
			try {
				const TopTools_ListOfShape& rkIncidentCells = occtFaceSolidMap.FindFromKey(rkOcctFace);

				for (TopTools_ListOfShape::const_iterator kOcctCellIterator = rkIncidentCells.cbegin();
					kOcctCellIterator != rkIncidentCells.cend();
					kOcctCellIterator++)
				{
					const TopoDS_Shape& rkIncidentCell = *kOcctCellIterator;
					if (!rkOcctSolid.IsSame(rkIncidentCell))
					{
						occtAdjacentSolids.Add(rkIncidentCell);
					}
				}
			}
			catch (Standard_NoSuchObject)
			{
				throw std::exception("Cannot find a Face in the global Cluster.");
			}
		}

		for (TopTools_MapIteratorOfMapOfShape occtAdjacentSolidIterator(occtAdjacentSolids); 
			occtAdjacentSolidIterator.More();
			occtAdjacentSolidIterator.Next())
		{
			rCells.push_back(std::make_shared<Cell>(TopoDS::Solid(occtAdjacentSolidIterator.Value())));
		}
	}

	void Cell::CellComplexes(std::list<std::shared_ptr<TopologicCore::CellComplex>>& rCellComplexes) const
	{
		UpwardNavigation(rCellComplexes);
	}

	void Cell::Shells(std::list<Shell::Ptr>& rShells) const
	{
		DownwardNavigation(rShells);
	}

	void Cell::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Cell::Faces(std::list<Face::Ptr>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	void Cell::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Cell::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	Vertex::Ptr Cell::CenterOfMass() const
	{
		TopoDS_Vertex occtCenterOfMass = CenterOfMass(GetOcctSolid());
		return std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtCenterOfMass));
	}

	TopoDS_Vertex Cell::CenterOfMass(const TopoDS_Solid & rkOcctSolid)
	{
		ShapeFix_Solid occtSolidFix(rkOcctSolid);
		occtSolidFix.Perform();
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(occtSolidFix.Shape(), occtShapeProperties);
		return BRepBuilderAPI_MakeVertex(occtShapeProperties.CentreOfMass());
	}

	Cell::Ptr Cell::ByFaces(const std::list<Face::Ptr>& rkFaces, double kTolerance)
	{
		if (kTolerance <= 0.0)
		{
			throw std::exception("The tolerance must have a positive value.");
		}

		if (rkFaces.empty())
		{
			throw std::exception("No face is passed.");
		}

		Shell::Ptr pShell = Shell::ByFaces(rkFaces, kTolerance);
		
		try {

			Cell::Ptr pCell = ByShell(pShell);

			for (const Face::Ptr& kpFace : rkFaces)
			{
				AttributeManager::GetInstance().CopyAttributes(kpFace->GetOcctFace(), pCell->GetOcctSolid());
			}

			for (const Face::Ptr& kpFace : rkFaces)
			{
				TransferContents(kpFace->GetOcctShape(), pCell);
			}

			return pCell;
		}
		catch (...)
		{
			throw std::exception("The input Faces do not form a closed Shell.");
		}
	}

	Cell::Ptr Cell::ByShell(const Shell::Ptr& kpShell)
	{
		if (!kpShell->IsClosed())
		{
			throw std::exception("The input Shell is open.");
		}

		BRepBuilderAPI_MakeSolid occtMakeSolid;
		try {
			occtMakeSolid = BRepBuilderAPI_MakeSolid(kpShell->GetOcctShell());
		}
		catch (StdFail_NotDone&)
		{
			throw std::exception("The solid was not built.");
		}

		// Create a cell from the shell. The faces are the same and the contents
		// are automatically passed.
		ShapeFix_Solid occtSolidFix(occtMakeSolid);
		occtSolidFix.Perform();
		Cell::Ptr pCell = std::make_shared<Cell>(TopoDS::Solid(occtSolidFix.Shape()));
		Cell::Ptr pCellCopy = std::dynamic_pointer_cast<Cell>(pCell->DeepCopy());
		GlobalCluster::GetInstance().AddTopology(pCellCopy->GetOcctSolid());
		return pCellCopy;
	}

	void Cell::SharedEdges(const Cell::Ptr& kpAnotherCell, std::list<Edge::Ptr>& rEdges) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_MapOfShape occtEdges1;
		DownwardNavigation(rkOcctShape1, TopAbs_EDGE, occtEdges1);

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_MapOfShape occtEdges2;
		DownwardNavigation(rkOcctShape2, TopAbs_EDGE, occtEdges2);

		for (TopTools_MapIteratorOfMapOfShape occtEdgeIterator1(occtEdges1);
			occtEdgeIterator1.More();
			occtEdgeIterator1.Next())
		{
			for (TopTools_MapIteratorOfMapOfShape occtEdgeIterator2(occtEdges2);
				occtEdgeIterator2.More();
				occtEdgeIterator2.Next())
			{
				if (occtEdgeIterator1.Value().IsSame(occtEdgeIterator2.Value()))
				{
					Edge::Ptr pEdge = std::make_shared<Edge>(TopoDS::Edge(occtEdgeIterator1.Value()));
					rEdges.push_back(pEdge);
				}
			}
		}
	}

	void Cell::SharedFaces(const Cell::Ptr& kpAnotherCell, std::list<Face::Ptr>& rFaces) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_MapOfShape occtFaces1;
		DownwardNavigation(rkOcctShape1, TopAbs_FACE, occtFaces1);

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_MapOfShape occtFaces2;
		DownwardNavigation(rkOcctShape2, TopAbs_FACE, occtFaces2);

		for (TopTools_MapIteratorOfMapOfShape occtFaceIterator1(occtFaces1);
			occtFaceIterator1.More();
			occtFaceIterator1.Next())
		{
			for (TopTools_MapIteratorOfMapOfShape occtFaceIterator2(occtFaces2);
				occtFaceIterator2.More();
				occtFaceIterator2.Next())
			{
				if (occtFaceIterator1.Value().IsSame(occtFaceIterator2.Value()))
				{
					Face::Ptr pFace = std::make_shared<Face>(TopoDS::Face(occtFaceIterator1.Value()));
					rFaces.push_back(pFace);
				}
			}
		}
	}

	void Cell::SharedVertices(const Cell::Ptr& kpAnotherCell, std::list<Vertex::Ptr>& rVertices) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_MapOfShape occtVertices1;
		DownwardNavigation(rkOcctShape1, TopAbs_VERTEX, occtVertices1);

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_MapOfShape occtVertices2;
		DownwardNavigation(rkOcctShape2, TopAbs_VERTEX, occtVertices2);

		for (TopTools_MapIteratorOfMapOfShape occtVertexIterator1(occtVertices1);
			occtVertexIterator1.More();
			occtVertexIterator1.Next())
		{
			for (TopTools_MapIteratorOfMapOfShape occtVertexIterator2(occtVertices2);
				occtVertexIterator2.More();
				occtVertexIterator2.Next())
			{
				if (occtVertexIterator1.Value().IsSame(occtVertexIterator2.Value()))
				{
					Vertex::Ptr pVertex = std::make_shared<Vertex>(TopoDS::Vertex(occtVertexIterator1.Value()));
					rVertices.push_back(pVertex);
				}
			}
		}
	}

	Shell::Ptr Cell::ExternalBoundary() const
	{
		TopoDS_Shell occtOuterShell = BRepClass3d::OuterShell(TopoDS::Solid(GetOcctShape()));
		return std::make_shared<Shell>(occtOuterShell);
	}

	void Cell::InternalBoundaries(std::list<Shell::Ptr>& rShells) const
	{
		Shell::Ptr pExternalBoundary = ExternalBoundary();

		std::list<Shell::Ptr> shells;
		DownwardNavigation(shells);
		for (const Shell::Ptr& kpShell : shells)
		{
			if (!kpShell->IsSame(pExternalBoundary))
			{
				rShells.push_back(kpShell);
			}
		}
	}

	bool Cell::IsManifold() const
	{
		throw std::exception("Not implemented yet");
	}

	TopoDS_Shape& Cell::GetOcctShape()
	{
		return GetOcctSolid();
	}

	const TopoDS_Shape& Cell::GetOcctShape() const
	{
		return GetOcctSolid();
	}

	TopoDS_Solid& Cell::GetOcctSolid()
	{
		assert(!m_occtSolid.IsNull() && "Cell::m_occtSolid is null.");
		if (m_occtSolid.IsNull())
		{
			throw std::exception("A null Cell is encountered.");
		}

		return m_occtSolid;
	}

	const TopoDS_Solid& Cell::GetOcctSolid() const
	{
		assert(!m_occtSolid.IsNull() && "Cell::m_occtSolid is null.");
		if (m_occtSolid.IsNull())
		{
			throw std::exception("A null Cell is encountered.");
		}

		return m_occtSolid;
	}

	void Cell::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		SetOcctSolid(TopoDS::Solid(rkOcctShape));
	}

	void Cell::SetOcctSolid(const TopoDS_Solid & rkOcctSolid)
	{
		m_occtSolid = rkOcctSolid;
	}

	void Cell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of faces
		std::list<Face::Ptr> faces;
		Faces(faces);
		for (const Face::Ptr& kpFace : faces)
		{
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	std::string Cell::GetTypeAsString() const
	{
		return std::string("Cell");
	}

	Cell::Cell(const TopoDS_Solid& rkOcctSolid, const std::string& rkGuid)
		: Topology(3, rkOcctSolid, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtSolid(rkOcctSolid)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<CellFactory>());
	}

	Cell::~Cell()
	{

	}
}
