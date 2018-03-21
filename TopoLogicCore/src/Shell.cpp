#include <Shell.h>
#include <GlobalCluster.h>
#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepCheck_Shell.hxx>
#include <Geom_Surface.hxx>
#include <ShapeFix_Shell.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>
#include <TopTools_MapOfShape.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	void Shell::Cells(std::list<Cell*>& rCells) const
	{
		UpwardNavigation(rCells);
	}

	void Shell::Edges(std::list<Edge*>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Shell::Wires(std::list<Wire*>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	void Shell::Faces(std::list<Face*>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	bool Shell::IsClosed() const
	{
		return (BRepCheck_Shell(TopoDS::Shell(*GetOcctShape())).Closed() == BRepCheck_NoError);
	}

	void Shell::Vertices(std::list<Vertex*>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	Shell* Shell::ByFaces(const std::list<Face*>& rkFaces)
	{
		BRepBuilderAPI_Sewing occtSewing;
		for(std::list<Face*>::const_iterator kFaceIterator = rkFaces.begin();
			kFaceIterator != rkFaces.end();
			kFaceIterator++)
		{
			Face* pFace = *kFaceIterator;
			occtSewing.Add(*pFace->GetOcctShape());
		}

		occtSewing.Perform();
		return new Shell(TopoDS::Shell(occtSewing.SewedShape()));
	}

	Shell* Shell::ByVerticesFaceIndices(const std::vector<Vertex*>& rkVertices, const std::list<std::list<int>>& rkFaceIndices)
	{
		std::list<Face*> faces;
		for(std::list<std::list<int>>::const_iterator kFaceIndexIterator = rkFaceIndices.begin();
			kFaceIndexIterator != rkFaceIndices.end();
			kFaceIndexIterator++)
		{
			const std::list<int>& rkVertexIndices = *kFaceIndexIterator;

			BRepBuilderAPI_MakeWire occtMakeWire;

			std::list<int>::const_iterator kSecondFromLastVertexIterator = --rkVertexIndices.end();
			for(std::list<int>::const_iterator kVertexIterator = rkVertexIndices.begin();
				kVertexIterator != kSecondFromLastVertexIterator;
				kVertexIterator++)
			{
				int vertexIndex = *kVertexIterator;

				std::list<int>::const_iterator kNextVertexIterator = kVertexIterator;
				kNextVertexIterator++;
				int nextVertexIndex = *kNextVertexIterator;

				occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
					TopoDS::Vertex(*rkVertices[vertexIndex]->GetOcctShape()),
					TopoDS::Vertex(*rkVertices[nextVertexIndex]->GetOcctShape())
				));
			}
			occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
				TopoDS::Vertex(*rkVertices[*--rkVertexIndices.end()]->GetOcctShape()),
				TopoDS::Vertex(*rkVertices[*rkVertexIndices.begin()]->GetOcctShape())
			));

			TopoDS_Wire* pOcctWire = new TopoDS_Wire(occtMakeWire);
			BRepBuilderAPI_MakeFace occtMakeFace(*pOcctWire);
			faces.push_back(new Face(occtMakeFace));
		}
		return ByFaces(faces);
	}

	TopoDS_Shape* Shell::GetOcctShape() const
	{
		assert(m_pOcctShell != nullptr && "Shell::m_pOcctShell is null.");
		if (m_pOcctShell == nullptr)
		{
			throw std::exception("Shell::m_pOcctShell is null.");
		}

		return m_pOcctShell;
	}

	void Shell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of faces
		std::list<Face*> faces;
		Faces(faces);
		for (std::list<Face*>::const_iterator kFaceIterator = faces.begin();
			kFaceIterator != faces.end();
			kFaceIterator++)
		{
			Face* pFace = *kFaceIterator;
			rOcctGeometries.push_back(pFace->Surface());
		}
	}

	Shell::Shell(const TopoDS_Shell& rkOcctShell)
		: Topology(2)
		, m_pOcctShell(nullptr)
	{
		ShapeFix_Shell occtShapeFixShell(rkOcctShell);
		occtShapeFixShell.Perform();
		m_pOcctShell = new TopoDS_Shell(occtShapeFixShell.Shell());
		GlobalCluster::GetInstance().Add(this);
	}

	Shell::~Shell()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctShell;
	}
}
