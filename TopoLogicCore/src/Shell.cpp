#include <Shell.h>
#include <GlobalCluster.h>
#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
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
	Cell* Shell::Cell() const
	{
		TopTools_IndexedDataMapOfShapeListOfShape occtShellToCellsMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_SHELL, TopAbs_SOLID, occtShellToCellsMap);

		const TopTools_ListOfShape& rkOcctCells = occtShellToCellsMap.FindFromKey(*GetOcctShape());

		assert(rkOcctCells.Size() < 2);

		for (TopTools_ListOfShape::const_iterator kIterator = rkOcctCells.cbegin();
			kIterator != rkOcctCells.cend();
			kIterator++)
		{
			if (kIterator->ShapeType() == TopAbs_SOLID)
			{
				const TopoDS_Solid& rkOcctSolid = TopoDS::Solid(*kIterator);
				return new TopoLogicCore::Cell(new TopoDS_Solid(rkOcctSolid));
			}
		}

		return nullptr;
	}

	void Shell::Edges(std::list<Edge*>& rEdges) const
	{
		TopTools_MapOfShape occtEdges;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges.Contains(occtCurrent))
			{
				occtEdges.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtEdges.cbegin();
			kOcctShapeIterator != occtEdges.cend();
			kOcctShapeIterator++)
		{
			rEdges.push_back(new Edge(new TopoDS_Edge(TopoDS::Edge(*kOcctShapeIterator))));
		}
	}

	void Shell::Faces(std::list<Face*>& rFaces) const
	{
		TopTools_MapOfShape occtFaces;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtFaces.Contains(occtCurrent))
			{
				occtFaces.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtFaces.cbegin();
			kOcctShapeIterator != occtFaces.cend();
			kOcctShapeIterator++)
		{
			rFaces.push_back(new Face(new TopoDS_Face(TopoDS::Face(*kOcctShapeIterator))));
		}
	}

	bool Shell::IsClosed() const
	{
		return (BRepCheck_Shell(TopoDS::Shell(*GetOcctShape())).Closed() == BRepCheck_NoError);
	}

	void Shell::Vertices(std::list<Vertex*>& rVertices) const
	{
		TopTools_MapOfShape occtVertices;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices.Contains(occtCurrent))
			{
				occtVertices.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtVertices.cbegin();
			kOcctShapeIterator != occtVertices.cend();
			kOcctShapeIterator++)
		{
			rVertices.push_back(new Vertex(new TopoDS_Vertex(TopoDS::Vertex(*kOcctShapeIterator))));
		}
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
		TopoDS_Shell* pOcctShell = new TopoDS_Shell(TopoDS::Shell(occtSewing.SewedShape()));
		ShapeFix_Shell occtShapeFixShell;
		occtShapeFixShell.Init(*pOcctShell);
		occtShapeFixShell.Perform();

		return new Shell(pOcctShell);
	}

	/*Shell * Shell::ByPolySurface(TopoDS_Shell const * const kpkOcctShell)
	{
		return nullptr;
	}*/

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
			faces.push_back(new Face(new TopoDS_Face(occtMakeFace)));
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

	Shell::Shell(TopoDS_Shell * const kpOcctShell)
		: Topology(2)
		, m_pOcctShell(kpOcctShell)
	{
		GlobalCluster::GetInstance().Add(this);
	}

	Shell::~Shell()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctShell;
	}
}
