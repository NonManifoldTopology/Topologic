#include <CellComplex.h>
#include <GlobalCluster.h>
#include <Face.h>
#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Shell.h>

#include <BRep_Builder.hxx>
#include <Geom_Surface.hxx>
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

namespace TopoLogicCore
{
	void CellComplex::Cells(std::list<std::shared_ptr<Cell>>& rCells) const
	{
		DownwardNavigation(rCells);
	}

	void CellComplex::Faces(std::list<std::shared_ptr<Face>>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	void CellComplex::Shells(std::list<std::shared_ptr<Shell>>& rShells) const
	{
		DownwardNavigation(rShells);
	}

	void CellComplex::Edges(std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void CellComplex::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void CellComplex::Wires(std::list<std::shared_ptr<Wire>>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	std::shared_ptr<CellComplex> CellComplex::ByCells(const std::list<std::shared_ptr<Cell>>& rkCells)
	{
		if (rkCells.empty())
		{
			throw std::exception("No cell is passed.");
		}

		TopoDS_CompSolid occtCompSolid;
		BRep_Builder occtBuilder;
		occtBuilder.MakeCompSolid(occtCompSolid);

		std::list<std::shared_ptr<Cell>>::const_iterator rkCellIterator = rkCells.begin();
		if (rkCells.size() == 1)
		{
			// Return a cell complex with only that cells
			try {
				occtBuilder.Add(occtCompSolid, *(*rkCellIterator)->GetOcctShape());
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
			std::list<std::shared_ptr<Topology>> topologies;
			// Start from the second.
			rkCellIterator++;
			for (;rkCellIterator != rkCells.end(); rkCellIterator++)
			{
				topologies.push_back(*rkCellIterator);
			}
			std::shared_ptr<Cluster> otherCellsAsCluster = Cluster::ByTopology(topologies);
			std::shared_ptr<Topology> pMergeTopology = (*rkCells.begin())->Merge(otherCellsAsCluster);

			std::list<std::shared_ptr<Cell>> cells;
			pMergeTopology->DownwardNavigation(cells);
			for (const std::shared_ptr<Cell>& kpCell : cells)
			{
				try {
					occtBuilder.Add(occtCompSolid, *kpCell->GetOcctShape());
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
		std::shared_ptr<CellComplex> pMergeCellComplex = std::dynamic_pointer_cast<CellComplex>(CellComplex::ByOcctShape(occtCompSolid));
		if (pMergeCellComplex == nullptr)
		{
			throw std::exception("CellComplex::ByCells(): Merge operation is not giving a cell complex");
		}

		for (std::list<std::shared_ptr<Cell>>::const_iterator kCellIterator = rkCells.begin();
			kCellIterator != rkCells.end();
			kCellIterator++)
		{
			const std::shared_ptr<Cell>& kpCell = *kCellIterator;
			kpCell->AddIngredientTo(pMergeCellComplex);
		}
		return pMergeCellComplex;
	}

	std::shared_ptr<CellComplex> CellComplex::ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces)
	{
		BOPAlgo_MakerVolume occtMakerVolume;
		BOPCol_ListOfShape occtShapes;
		for (const std::shared_ptr<Face>& kpFace: rkFaces)
		{
			occtShapes.Append(*kpFace->GetOcctShape().get());
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

		std::list<std::shared_ptr<Cell>> cells;
		// The result is either:
		// - A solid
		if (rkOcctResult.ShapeType() == TopAbs_SOLID)
		{
			// Create a cell complex with only this
			cells.push_back(TopologicalQuery::Downcast<Cell>(Cell::ByOcctShape(rkOcctResult)));
		}
		// - A compound, collect the solids
		else if (rkOcctResult.ShapeType() == TopAbs_COMPOUND)
		{
			TopTools_MapOfShape occtShapes;
			TopExp_Explorer occtExplorer;
			for (occtExplorer.Init(rkOcctResult, TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& occtCurrent = occtExplorer.Current();
				if (!occtShapes.Contains(occtCurrent))
				{
					occtShapes.Add(occtCurrent);
					cells.push_back(Downcast<Cell>(ByOcctShape(occtCurrent)));
				}
			}
		}

		return ByCells(cells);
	}

	std::shared_ptr<Cell> CellComplex::OuterBoundary() const
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		std::list<std::shared_ptr<Cell>> cells;
		Cells(cells);
		for (std::list<std::shared_ptr<Cell>>::const_iterator kCellIterator = cells.begin();
			kCellIterator != cells.end();
			kCellIterator++)
		{
			occtCellsBuildersArguments.Append(*(*kCellIterator)->GetOcctShape());
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
		for (BOPCol_ListOfShape::const_iterator kShapeIterator = occtCellsBuildersArguments.begin();
			kShapeIterator != occtCellsBuildersArguments.end();
			kShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToTake.Append(*kShapeIterator);
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

		TopExp_Explorer occtExplorer;
		TopTools_MapOfShape occtCells;
		for (occtExplorer.Init(occtEnvelopeShape, TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
		{
			return std::make_shared<Cell>(TopoDS::Solid(occtExplorer.Current()));
		}
		return nullptr;
	}

	void CellComplex::InnerBoundaries(std::list<std::shared_ptr<Face>>& rInternalFaces) const
	{
		std::shared_ptr<Cell> pEnvelopeCell = OuterBoundary();

		std::list<std::shared_ptr<Face>> envelopeFaces;
		pEnvelopeCell->Faces(envelopeFaces);

		std::list<std::shared_ptr<Face>> faces;
		Faces(faces);

		Handle(IntTools_Context) pOcctIntToolsContext = new IntTools_Context();
		for (std::list<std::shared_ptr<Face>>::const_iterator kFaceIterator = faces.begin();
			kFaceIterator != faces.end();
			kFaceIterator++)
		{
			const std::shared_ptr<Face>& kpFace = *kFaceIterator;
			bool isEnvelopeFace = false;
			for (std::list<std::shared_ptr<Face>>::const_iterator kEnvelopeFaceIterator = envelopeFaces.begin();
				kEnvelopeFaceIterator != envelopeFaces.end() && !isEnvelopeFace;
				kEnvelopeFaceIterator++)
			{
				const std::shared_ptr<Face>& kpEnvelopeFace = *kEnvelopeFaceIterator;
				if(BOPTools_AlgoTools::CheckSameGeom(TopoDS::Face(*kpFace->GetOcctShape()), TopoDS::Face(*kpEnvelopeFace->GetOcctShape()), pOcctIntToolsContext))
				{
					isEnvelopeFace = true;
				}
			}

			if (!isEnvelopeFace)
			{
				rInternalFaces.push_back(kpFace);
			}
		}
	}

	std::shared_ptr<TopoDS_Shape> CellComplex::GetOcctShape() const
	{
		assert(m_pOcctCompSolid != nullptr && "CellComplex::m_pOcctCompSolid is null.");
		if (m_pOcctCompSolid == nullptr)
		{
			throw std::exception("CellComplex::m_pOcctCompSolid is null.");
		}

		return m_pOcctCompSolid;
	}

	void CellComplex::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		std::list<std::shared_ptr<Face>> faces;
		Faces(faces);
		for (std::list<std::shared_ptr<Face>>::const_iterator kFaceIterator = faces.begin();
			kFaceIterator != faces.end();
			kFaceIterator++)
		{
			const std::shared_ptr<Face>& kpFace = *kFaceIterator;
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	CellComplex::CellComplex(const TopoDS_CompSolid& rkOcctCompSolid)
		: Topology(3)
		, m_pOcctCompSolid(std::make_shared<TopoDS_CompSolid>(rkOcctCompSolid))
	{
		GlobalCluster::GetInstance().Add(this);
	}

	CellComplex::~CellComplex()
	{
		GlobalCluster::GetInstance().Remove(this);
	}
}
