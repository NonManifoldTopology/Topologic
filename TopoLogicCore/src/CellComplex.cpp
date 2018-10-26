#include "CellComplex.h"
#include "Cluster.h"
#include "Face.h"
#include "Cell.h"
#include "Vertex.h"
#include "Edge.h"
#include "Wire.h"
#include "Shell.h"
#include "CellComplexFactory.h"
#include "GlobalCluster.h"

#include <BRep_Builder.hxx>
#include <BRepGProp.hxx>
#include <Geom_Surface.hxx>
#include <GProp_GProps.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <TopTools_MapOfShape.hxx>

#include <BOPTools_AlgoTools.hxx>
#include <IntTools_Context.hxx>

#include <BOPAlgo_MakerVolume.hxx>
#include <algorithm>

#include <assert.h>

namespace TopologicCore
{
	void CellComplex::Cells(std::list<Cell::Ptr>& rCells) const
	{
		DownwardNavigation(rCells);
	}

	void CellComplex::Faces(std::list<Face::Ptr>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	void CellComplex::Shells(std::list<Shell::Ptr>& rShells) const
	{
		DownwardNavigation(rShells);
	}

	void CellComplex::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void CellComplex::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void CellComplex::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	CellComplex::Ptr CellComplex::ByCells(const std::list<Cell::Ptr>& rkCells)
	{
		TopTools_ListOfShape occtShapes;
		for (const Cell::Ptr& kpCell : rkCells)
		{
			Cell::Ptr pCopyCell = std::dynamic_pointer_cast<Cell>(kpCell);
			occtShapes.Append(pCopyCell->GetOcctShape());
		}

		TopoDS_CompSolid occtCompSolid = ByOcctSolids(occtShapes);
		CellComplex::Ptr pCellComplex = std::make_shared<CellComplex>(occtCompSolid);
		GlobalCluster::GetInstance().AddTopology(pCellComplex->GetOcctCompSolid());
		return pCellComplex;
	}

	TopoDS_CompSolid CellComplex::ByOcctSolids(const TopTools_ListOfShape & rkOcctSolids)
	{
		if (rkOcctSolids.IsEmpty())
		{
			throw std::exception("No cell is passed.");
		}

		TopoDS_CompSolid occtCompSolid;
		BRep_Builder occtBuilder;
		occtBuilder.MakeCompSolid(occtCompSolid);

		TopTools_ListOfShape::iterator occtSolidIterator = rkOcctSolids.begin();
		if (rkOcctSolids.Size() == 1)
		{
			// Return a cell complex with only that cells
			try {
				occtBuilder.Add(occtCompSolid, *occtSolidIterator);
			}
			catch (TopoDS_FrozenShape&)
			{
				throw std::exception("The cell is not free and cannot be modified.");
			}
			catch (TopoDS_UnCompatibleShapes&)
			{
				throw std::exception("The cell and face are not compatible.");
			}
		}
		else
		{
			// Merge the first cell with the rest.
			Topology::Ptr firstTopology = Topology::ByOcctShape(*occtSolidIterator, "");
			std::list<std::shared_ptr<Topology>> topologies;
			// Start from the second.
			occtSolidIterator++;
			for (; occtSolidIterator != rkOcctSolids.end(); occtSolidIterator++)
			{
				topologies.push_back(Topology::ByOcctShape(*occtSolidIterator, ""));
			}
			std::shared_ptr<Cluster> otherCellsAsCluster = Cluster::ByTopologies(topologies);
			std::shared_ptr<Topology> pMergeTopology = firstTopology->Merge(otherCellsAsCluster);

			std::list<Cell::Ptr> cells;
			pMergeTopology->DownwardNavigation(cells);
			for (const Cell::Ptr& kpCell : cells)
			{
				try {
					occtBuilder.Add(occtCompSolid, kpCell->GetOcctShape());
				}
				catch (TopoDS_FrozenShape&)
				{
					throw std::exception("The cell is not free and cannot be modified.");
				}
				catch (TopoDS_UnCompatibleShapes&)
				{
					throw std::exception("The cell and face are not compatible.");
				}
			}
		}

		// Should get us a CellComplex, otherwise an exception.
		return occtCompSolid;
	}

	CellComplex::Ptr CellComplex::ByFaces(const std::list<Face::Ptr>& rkFaces)
	{
		BOPAlgo_MakerVolume occtMakerVolume;
		BOPCol_ListOfShape occtShapes;
		for (const Face::Ptr& kpFace: rkFaces)
		{
			occtShapes.Append(kpFace->GetOcctShape());
		}
		bool isParallel = false; /* parallel or single mode (the default value is FALSE)*/
		bool doesIntersection = true; /* intersect or not the arguments (the default value is TRUE)*/
		double tolerance = 0.0; /* fuzzy option (default value is 0)*/
								  //
		occtMakerVolume.SetArguments(occtShapes);
		occtMakerVolume.SetRunParallel(isParallel);
		occtMakerVolume.SetIntersect(doesIntersection);
		occtMakerVolume.SetFuzzyValue(tolerance);
		//
		occtMakerVolume.Perform(); //perform the operation
		if (occtMakerVolume.HasErrors()) { //check error status
			throw std::exception("Cannot create a cell complex from the cells.");
		}

		//
		const TopoDS_Shape& rkOcctResult = occtMakerVolume.Shape();

		std::list<Cell::Ptr> cells;
		// The result is either:
		// - A solid
		if (rkOcctResult.ShapeType() == TopAbs_SOLID)
		{
			// Create a cell complex with only this
			cells.push_back(std::make_shared<Cell>(TopoDS::Solid(rkOcctResult)));
		}
		// - A compound, collect the solids
		else if (rkOcctResult.ShapeType() == TopAbs_COMPOUND)
		{
			TopTools_MapOfShape occtShapes;
			for (TopExp_Explorer occtExplorer(rkOcctResult, TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& occtCurrent = occtExplorer.Current();
				if (!occtShapes.Contains(occtCurrent))
				{
					occtShapes.Add(occtCurrent);
					cells.push_back(std::make_shared<Cell>(TopoDS::Solid(occtCurrent)));
				}
			}
		}

		return ByCells(cells);
	}

	Cell::Ptr CellComplex::OuterBoundary() const
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		std::list<Cell::Ptr> cells;
		Cells(cells);
		for (const Cell::Ptr& kpCell : cells)
		{
			occtCellsBuildersArguments.Append(kpCell->GetOcctShape());
		}

		BOPAlgo_CellsBuilder occtCellsBuilder;
		occtCellsBuilder.SetArguments(occtCellsBuildersArguments);

		occtCellsBuilder.Perform();

		if (occtCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;
		for (BOPCol_ListIteratorOfListOfShape occtShapeIterator(occtCellsBuildersArguments);
			occtShapeIterator.More();
			occtShapeIterator.Next())
		{
			occtListToTake.Clear();
			occtListToTake.Append(occtShapeIterator.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}

		// A cell complex is a contiguous shape, so there can be at maximum only one envelope cell.
		TopoDS_Shape occtEnvelopeShape = occtCellsBuilder.Shape();
		ShapeAnalysis_ShapeContents occtShapeAnalysis;
		occtShapeAnalysis.Perform(occtEnvelopeShape);
		int numberOfSolids = occtShapeAnalysis.NbSharedSolids();
		std::stringstream ssErrorMessage;
		ssErrorMessage << "There can be only 0 or 1 envelope cell, but this cell complex has " << numberOfSolids << " cells.";
		assert(numberOfSolids < 2 && ssErrorMessage.str().c_str());

		for (TopExp_Explorer occtExplorer(occtEnvelopeShape, TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
		{
			return std::make_shared<Cell>(TopoDS::Solid(occtExplorer.Current()));
		}
		return nullptr;
	}

	void CellComplex::InnerBoundaries(std::list<Face::Ptr>& rInternalFaces) const
	{
		Cell::Ptr pEnvelopeCell = OuterBoundary();

		std::list<Face::Ptr> envelopeFaces;
		pEnvelopeCell->Faces(envelopeFaces);

		std::list<Face::Ptr> faces;
		Faces(faces);

		Handle(IntTools_Context) pOcctIntToolsContext = new IntTools_Context();
		for (const Face::Ptr& kpFace : faces)
		{
			bool isEnvelopeFace = false;
			for (const Face::Ptr& kpEnvelopeFace : envelopeFaces)
			{
				if(BOPTools_AlgoTools::CheckSameGeom(kpFace->GetOcctFace(), kpEnvelopeFace->GetOcctFace(), pOcctIntToolsContext))
				{
					isEnvelopeFace = true;
					break;
				}
			}

			if (!isEnvelopeFace)
			{
				rInternalFaces.push_back(kpFace);
			}
		}
	}

	bool CellComplex::IsManifold() const
	{
		throw std::exception("Not implemented yet");
	}

	void CellComplex::NonManifoldFaces(std::list<Face::Ptr>& rNonManifoldFaces) const
	{
		std::list<Face::Ptr> faces;
		Faces(faces);

		for (const Face::Ptr& kpFace : faces)
		{
			if(!kpFace->IsManifold())
			{
				rNonManifoldFaces.push_back(kpFace);
			}
		}
	}

	TopoDS_Shape& CellComplex::GetOcctShape()
	{
		return GetOcctCompSolid();
	}

	const TopoDS_Shape& CellComplex::GetOcctShape() const
	{
		return GetOcctCompSolid();
	}

	TopoDS_CompSolid& CellComplex::GetOcctCompSolid()
	{
		assert(!m_occtCompSolid.IsNull() && "CellComplex::m_occtCompSolid is null.");
		if (m_occtCompSolid.IsNull())
		{
			throw std::exception("CellComplex::m_occtCompSolid is null.");
		}

		return m_occtCompSolid;
	}

	const TopoDS_CompSolid& CellComplex::GetOcctCompSolid() const
	{
		assert(!m_occtCompSolid.IsNull() && "CellComplex::m_occtCompSolid is null.");
		if (m_occtCompSolid.IsNull())
		{
			throw std::exception("CellComplex::m_occtCompSolid is null.");
		}

		return m_occtCompSolid;
	}

	void CellComplex::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		SetOcctCompSolid(TopoDS::CompSolid(rkOcctShape));
	}

	void CellComplex::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		std::list<Face::Ptr> faces;
		Faces(faces);
		for (const Face::Ptr& kpFace : faces)
		{
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	std::string CellComplex::GetTypeAsString() const
	{
		return std::string("CellComplex");
	}

	void CellComplex::SetOcctCompSolid(const TopoDS_CompSolid & rkOcctCompSolid)
	{
		m_occtCompSolid = rkOcctCompSolid;
	}

	Vertex::Ptr CellComplex::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	CellComplex::CellComplex(const TopoDS_CompSolid& rkOcctCompSolid, const std::string& rkGuid)
		: Topology(3, rkOcctCompSolid, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtCompSolid(rkOcctCompSolid)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<CellComplexFactory>());
	}

	CellComplex::~CellComplex()
	{

	}
}
