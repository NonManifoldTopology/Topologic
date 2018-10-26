#include "Shell.h"
#include "Cell.h"
#include "Vertex.h"
#include "Edge.h"
#include "Wire.h"
#include "Face.h"
#include "Aperture.h"
#include "ShellFactory.h"
#include "ContentManager.h"
#include "GlobalCluster.h"

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepCheck_Shell.hxx>
#include <BRepGProp.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>
#include <GProp_GProps.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <ShapeFix_Face.hxx>
#include <ShapeFix_ShapeTolerance.hxx>
#include <ShapeFix_Shell.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>
#include <TopTools_MapOfShape.hxx>

#include <array>
#include <assert.h>

namespace TopologicCore
{
	void Shell::Cells(std::list<Cell::Ptr>& rCells) const
	{
		UpwardNavigation(rCells);
	}

	void Shell::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Shell::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	void Shell::Faces(std::list<Face::Ptr>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	bool Shell::IsClosed() const
	{
		BRepCheck_Shell occtBrepCheckShell(TopoDS::Shell(GetOcctShape()));
		return (occtBrepCheckShell.Closed() == BRepCheck_NoError);
	}

	void Shell::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	Shell::Ptr Shell::ByFaces(const std::list<Face::Ptr>& rkFaces)
	{
		if (rkFaces.empty())
		{
			throw std::exception("No face is passed.");
		}

		TopTools_ListOfShape occtShapes;
		for(const Face::Ptr& kpFace : rkFaces)
		{
			Face::Ptr pCopyFace = std::dynamic_pointer_cast<Face>(kpFace->Copy());
			occtShapes.Append(pCopyFace->GetOcctShape());
		}

		TopoDS_Shell occtShell = ByOcctFaces(occtShapes);
		Shell::Ptr pShell = std::make_shared<Shell>(TopoDS::Shell(occtShell));
		GlobalCluster::GetInstance().AddTopology(pShell->GetOcctShell());
		return pShell;
	}

	TopoDS_Shell Shell::ByOcctFaces(const TopTools_ListOfShape & rkOcctFaces)
	{
		BRepBuilderAPI_Sewing occtSewing;
		for (TopTools_ListIteratorOfListOfShape occtEdgeIterator(rkOcctFaces);
			occtEdgeIterator.More();
			occtEdgeIterator.Next())
		{
			occtSewing.Add(occtEdgeIterator.Value());
		}

		occtSewing.Perform();
		if (occtSewing.SewedShape().IsNull())
		{
			throw std::exception("A null shape is created.");
		}
		TopAbs_ShapeEnum type = occtSewing.SewedShape().ShapeType();
		if (type != TopAbs_SHELL)
		{
			throw std::exception("Fails to create a shell from faces.");
		}
		//Shell::Ptr pShell = std::make_shared<Shell>(TopoDS::Shell(occtSewing.SewedShape()));

		// HACK: add the v1 contents to the current shell faces.
		//Topology::Ptr pUpcastShell = TopologicalQuery::Upcast<Topology>(pShell);
		
		for (TopTools_ListIteratorOfListOfShape occtEdgeIterator(rkOcctFaces);
			occtEdgeIterator.More();
			occtEdgeIterator.Next())
		{
			const TopoDS_Shape& rkModifiedShape = occtSewing.Modified(occtEdgeIterator.Value());
			Topology::Ptr pChildTopology = Topology::ByOcctShape(rkModifiedShape, "");

			// Map the aperture to the modifed shell faces.
			std::list<Topology::Ptr> contents;
			ContentManager::GetInstance().Find(occtEdgeIterator.Value(), contents);
			//kShellFace->Contents(false, contents);
			for (const Topology::Ptr& rkContent : contents)
			{
				if (rkContent->GetType() != TOPOLOGY_APERTURE)
				{
					continue;
				}

				std::shared_ptr<Aperture> aperture = TopologicalQuery::Downcast<Aperture>(rkContent);

				if (aperture->Topology()->GetType() != TOPOLOGY_FACE)
				{
					continue;
				}

				Face::Ptr pApertureFace = TopologicalQuery::Downcast<Face>(aperture->Topology());
				Topology::Ptr pUpcastApertureFace = TopologicalQuery::Upcast<Topology>(pApertureFace);
			}
		}

		return TopoDS::Shell(occtSewing.SewedShape());
	}

	bool Shell::IsManifold() const
	{
		throw std::exception("Not implemented yet");
	}

	TopoDS_Shape& Shell::GetOcctShape()
	{
		return GetOcctShell();
	}

	const TopoDS_Shape& Shell::GetOcctShape() const
	{
		return GetOcctShell();
	}

	TopoDS_Shell& Shell::GetOcctShell()
	{
		assert(!m_occtShell.IsNull() && "Shell::m_occtShell is null.");
		if (m_occtShell.IsNull())
		{
			throw std::exception("Shell::m_occtShell is null.");
		}

		return m_occtShell;
	}

	const TopoDS_Shell& Shell::GetOcctShell() const
	{
		assert(!m_occtShell.IsNull() && "Shell::m_occtShell is null.");
		if (m_occtShell.IsNull())
		{
			throw std::exception("Shell::m_occtShell is null.");
		}

		return m_occtShell;
	}

	void Shell::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		SetOcctShell(TopoDS::Shell(rkOcctShape));
	}

	void Shell::SetOcctShell(const TopoDS_Shell & rkOcctShell)
	{
		m_occtShell = rkOcctShell;
	}

	Vertex::Ptr Shell::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::SurfaceProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	std::string Shell::GetTypeAsString() const
	{
		return std::string("Shell");
	}

	void Shell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of faces
		std::list<Face::Ptr> faces;
		Faces(faces);
		for (const Face::Ptr& kpFace : faces)
		{
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	Shell::Shell(const TopoDS_Shell& rkOcctShell, const std::string& rkGuid)
		: Topology(2, rkOcctShell, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtShell(rkOcctShell)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<ShellFactory>());
	}

	Shell::~Shell()
	{

	}
}
