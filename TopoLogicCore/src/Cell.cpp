#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <CellComplex.h>
#include <CellFactory.h>

#include <BRep_Builder.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepCheck_Shell.hxx>
#include <BRepClass3d.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepGProp.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <GProp_GProps.hxx>
#include <ShapeFix_Solid.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <Message_ProgressIndicator.hxx>

#include <TopTools_MapOfShape.hxx>
#include <BOPTools_AlgoTools.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Cell::AdjacentCells(const Topology::Ptr& kpParentTopology, std::list<Cell::Ptr>& rCells) const
	{
		// Iterate through the faces and find the incident cells which are not this cell.
		TopTools_IndexedDataMapOfShapeListOfShape occtFaceSolidMap;
		TopExp::MapShapesAndUniqueAncestors(kpParentTopology->GetOcctShape(), TopAbs_FACE, TopAbs_SOLID, occtFaceSolidMap);

		// Find the constituent faces
		TopTools_MapOfShape occtFaces;
		for (TopExp_Explorer occtExplorer(GetOcctShape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtFaces.Contains(occtCurrent))
			{
				occtFaces.Add(occtCurrent);
			}
		}

		const TopoDS_Solid& rkOcctSolid = GetOcctSolid();
		for (TopTools_MapOfShape::const_iterator kOcctFaceIterator = occtFaces.cbegin();
			kOcctFaceIterator != occtFaces.cend();
			kOcctFaceIterator++)
		{
			const TopoDS_Shape& rkOcctFace = *kOcctFaceIterator;
			const TopTools_ListOfShape& rkIncidentCells = occtFaceSolidMap.FindFromKey(rkOcctFace);

			for (TopTools_ListOfShape::const_iterator kOcctCellIterator = rkIncidentCells.cbegin();
				kOcctCellIterator != rkIncidentCells.cend();
				kOcctCellIterator++)
			{
				const TopoDS_Shape& rkIncidentCell = *kOcctCellIterator;
				if (!rkOcctSolid.IsSame(rkIncidentCell))
				{
					rCells.push_back(std::make_shared<Cell>(TopoDS::Solid(rkIncidentCell)));
				}
			}
		}
	}

	void Cell::CellComplexes(const Topology::Ptr& kpParentTopology, std::list<std::shared_ptr<TopologicCore::CellComplex>>& rCellComplexes) const
	{
		UpwardNavigation(kpParentTopology, rCellComplexes);
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

	double Cell::Volume() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(GetOcctShape(), occtShapeProperties);
		return occtShapeProperties.Mass();
	}

	Vertex::Ptr Cell::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	Cell::Ptr Cell::ByFaces(const std::list<Face::Ptr>& rkFaces)
	{
		if (rkFaces.empty())
		{
			throw std::exception("No face is passed.");
		}

		Shell::Ptr pShell = Shell::ByFaces(rkFaces);
		Cell::Ptr pCell = ByShell(pShell);
		return pCell;
	}

	Cell::Ptr Cell::ByShell(const Shell::Ptr& kpShell)
	{
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
		Cell::Ptr pCell = std::make_shared<Cell>(occtMakeSolid);

		return pCell;
	}

	Cell::Ptr Cell::ByCuboid(Handle(Geom_CartesianPoint) pOcctCentroid, const double kXDimension, const double kYDimension, const double kZDimension)
	{
		gp_Pnt occtLowCorner(
			pOcctCentroid->X() - kXDimension / 2.0,
			pOcctCentroid->Y() - kYDimension / 2.0,
			pOcctCentroid->Z() - kZDimension / 2.0
			);
		BRepPrimAPI_MakeBox occtMakeBox(occtLowCorner, kXDimension, kYDimension, kZDimension);
		occtMakeBox.Build();

		return std::make_shared<Cell>(occtMakeBox.Solid());
	}

	Cell::Ptr Cell::BySphere(const double kCenterX, const double kCenterY, const double kCenterZ, const double kRadius)
	{
		BRepPrimAPI_MakeSphere occtMakeSphere(gp_Pnt(kCenterX, kCenterY, kCenterZ), kRadius);
		return std::make_shared<Cell>(occtMakeSphere.Solid());
	}

	Cell::Ptr Cell::ByCylinder(const double kReferencePointX, const double kReferencePointY, const double kReferencePointZ,
		const double kNormalX, const double kNormalY, const double kNormalZ,
		const double kXDirectionX, const double kXDirectionY, const double kDirectionZ,
		const double kRadius, const double kHeight)
	{
		BRepPrimAPI_MakeCylinder occtMakeCylinder(
			gp_Ax2(
				gp_Pnt(kReferencePointX, kReferencePointY, kReferencePointZ),
				gp_Dir(kNormalX, kNormalY, kNormalZ),
				gp_Dir(kXDirectionX, kXDirectionY, kDirectionZ)), 
			kRadius, kHeight);
		return std::make_shared<Cell>(occtMakeCylinder.Solid());
	}

	std::shared_ptr<Cell> Cell::ByCone(
		const double kReferencePointX, const double kReferencePointY, const double kReferencePointZ,
		const double kNormalX, const double kNormalY, const double kNormalZ,
		const double kXDirectionX, const double kXDirectionY, const double kDirectionZ,
		const double kRadius1, const double kRadius2, const double kHeight)
	{
		BRepPrimAPI_MakeCone occtMakeCone(
			gp_Ax2(
				gp_Pnt(kReferencePointX, kReferencePointY, kReferencePointZ), 
				gp_Dir(kNormalX, kNormalY, kNormalZ), 
				gp_Dir(kXDirectionX, kXDirectionY, kDirectionZ)),
			kRadius1, kRadius2, kHeight);
		return std::make_shared<Cell>(occtMakeCone.Solid());
	}

	Cell::Ptr Cell::ByVerticesFaceIndices(const std::vector<Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkFaceIndices)
	{
		if (rkVertices.empty())
		{
			throw std::exception("No vertex is passed.");
		}

		std::list<Face::Ptr> faces;
		for (const std::list<int>& rkVertexIndices : rkFaceIndices)
		{
			BRepBuilderAPI_MakeWire occtMakeWire;

			std::list<int>::const_iterator kSecondFromLastVertexIterator = --rkVertexIndices.end();
			for (std::list<int>::const_iterator kVertexIterator = rkVertexIndices.begin();
				kVertexIterator != kSecondFromLastVertexIterator;
				kVertexIterator++)
			{
				int vertexIndex = *kVertexIterator;

				std::list<int>::const_iterator kNextVertexIterator = kVertexIterator;
				kNextVertexIterator++;
				int nextVertexIndex = *kNextVertexIterator;

				occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
					rkVertices[vertexIndex]->GetOcctVertex(),
					rkVertices[nextVertexIndex]->GetOcctVertex())
				);
			}
			occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
				rkVertices[*--rkVertexIndices.end()]->GetOcctVertex(),
				rkVertices[*rkVertexIndices.begin()]->GetOcctVertex())
			);

			const TopoDS_Wire& rkOcctWire = occtMakeWire.Wire();
			BRepBuilderAPI_MakeFace occtMakeFace(rkOcctWire);
			faces.push_back(std::make_shared<Face>(occtMakeFace));
		}
		Cell::Ptr pCell = ByFaces(faces);

		// Only add the vertices; the faces are dealt with in ByFaces()
		/*for (std::vector<Vertex::Ptr>::const_iterator kVertexIterator = rkVertices.begin();
			kVertexIterator != rkVertices.end();
			kVertexIterator++)
		{
			const Vertex::Ptr& kpVertex = *kVertexIterator;
			kpVertex->AddIngredientTo(pCell);
		}*/
		return pCell;
	}

	Cell::Ptr Cell::ByLoft(const std::list<Wire::Ptr>& rkWires)
	{
		BRepOffsetAPI_ThruSections occtLoft(true);
		for (const Wire::Ptr& kpWire : rkWires)
		{
			occtLoft.AddWire(kpWire->GetOcctWire());
		};
		try {
			occtLoft.Build();
		}
		catch (...)
		{
			throw std::exception("Loft error");
		}
		return std::make_shared<Cell>(TopoDS::Solid(occtLoft.Shape()));
	}

	void Cell::SharedEdges(const Cell::Ptr& kpAnotherCell, std::list<Edge::Ptr>& rEdges) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_ListOfShape occtEdges1;
		for (TopExp_Explorer occtExplorer(rkOcctShape1, TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges1.Contains(occtCurrent))
			{
				occtEdges1.Append(occtCurrent);
			}
		}

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_ListOfShape occtEdges2;
		for (TopExp_Explorer occtExplorer(rkOcctShape2, TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges2.Contains(occtCurrent))
			{
				occtEdges2.Append(occtCurrent);
			}
		}

		for (TopTools_ListIteratorOfListOfShape occtEdgeIterator1(occtEdges1);
			occtEdgeIterator1.More();
			occtEdgeIterator1.Next())
		{
			for (TopTools_ListIteratorOfListOfShape occtEdgeIterator2(occtEdges2);
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
		TopTools_ListOfShape occtFaces1;
		for (TopExp_Explorer occtExplorer(rkOcctShape1, TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtFaces1.Contains(occtCurrent))
			{
				occtFaces1.Append(occtCurrent);
			}
		}

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_ListOfShape occtFaces2;
		for (TopExp_Explorer occtExplorer(rkOcctShape2, TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtFaces2.Contains(occtCurrent))
			{
				occtFaces2.Append(occtCurrent);
			}
		}

		for (TopTools_ListIteratorOfListOfShape occtFaceIterator1(occtFaces1);
			occtFaceIterator1.More();
			occtFaceIterator1.Next())
		{
			for (TopTools_ListIteratorOfListOfShape occtFaceIterator2(occtFaces2);
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
		TopTools_ListOfShape occtVertices1;
		for (TopExp_Explorer occtExplorer(rkOcctShape1, TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices1.Contains(occtCurrent))
			{
				occtVertices1.Append(occtCurrent);
			}
		}

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_ListOfShape occtVertices2;
		for (TopExp_Explorer occtExplorer(rkOcctShape2, TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices2.Contains(occtCurrent))
			{
				occtVertices2.Append(occtCurrent);
			}
		}

		for (TopTools_ListIteratorOfListOfShape occtVertexIterator1(occtVertices1);
			occtVertexIterator1.More();
			occtVertexIterator1.Next())
		{
			for (TopTools_ListIteratorOfListOfShape occtVertexIterator2(occtVertices2);
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

	Shell::Ptr Cell::OuterBoundary() const
	{
		TopoDS_Shell occtOuterShell = BRepClass3d::OuterShell(TopoDS::Solid(GetOcctShape()));
		return std::make_shared<Shell>(occtOuterShell);
	}

	bool Cell::DoesContain(const Vertex::Ptr& kpVertex) const
	{
		ShapeFix_Solid occtSolidFix(GetOcctSolid());
		occtSolidFix.Perform();
		BRepClass3d_SolidClassifier occtSolidClassifier(occtSolidFix.Solid(), kpVertex->Point()->Pnt(), Precision::Confusion());
		TopAbs_State occtState = occtSolidClassifier.State();
		return (occtState == TopAbs_IN || occtState == TopAbs_ON);
	}

	void Cell::InnerBoundaries(std::list<Shell::Ptr>& rShells) const
	{
		Shell::Ptr pOuterBoundary = OuterBoundary();

		std::list<Shell::Ptr> shells;
		DownwardNavigation(shells);
		for (const Shell::Ptr& kpShell : shells)
		{
			if (!kpShell->IsSame(pOuterBoundary))
			{
				rShells.push_back(kpShell);
			}
		}
	}

	bool Cell::IsManifold(TopologicCore::Topology const * const kpkParentTopology) const
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
			throw std::exception("Cell::m_occtSolid is null.");
		}

		return m_occtSolid;
	}

	const TopoDS_Solid& Cell::GetOcctSolid() const
	{
		assert(!m_occtSolid.IsNull() && "Cell::m_occtSolid is null.");
		if (m_occtSolid.IsNull())
		{
			throw std::exception("Cell::m_occtSolid is null.");
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
