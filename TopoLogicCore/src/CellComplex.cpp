#include <CellComplex.h>
#include <GlobalCluster.h>
#include <Face.h>
#include <Cell.h>

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

#include <assert.h>

namespace TopoLogicCore
{
	void CellComplex::Cells(std::list<Cell*>& rCells) const
	{
		TopExp_Explorer occtExplorer;
		TopTools_MapOfShape occtCells;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtCells.Contains(occtCurrent))
			{
				occtCells.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtCells.cbegin();
			kOcctShapeIterator != occtCells.cend();
			kOcctShapeIterator++)
		{
			rCells.push_back(new Cell(TopoDS::Solid(*kOcctShapeIterator)));
		}
	}

	void CellComplex::Faces(std::list<Face*>& rFaces) const
	{
		TopExp_Explorer occtExplorer;
		TopTools_MapOfShape occtFaces;
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
			rFaces.push_back(new Face(TopoDS::Face(*kOcctShapeIterator)));
		}
	}

	bool CellComplex::IsClosed() const
	{
		throw std::exception("Not implemented yet");
	}

	CellComplex* CellComplex::ByCells(const std::list<Cell*>& rkCells)
	{
		std::list<Cell*>::const_iterator rkCellIterator = rkCells.begin();
		Topology* pMergeTopology = *rkCellIterator;
		rkCellIterator++;
		for (; rkCellIterator != rkCells.end(); rkCellIterator++)
		{
			pMergeTopology = pMergeTopology->Merge(*rkCellIterator);
		}

		TopoDS_CompSolid occtCompSolid;
		BRep_Builder occtBuilder;
		occtBuilder.MakeCompSolid(occtCompSolid);

		TopExp_Explorer occtExplorer;
		TopTools_MapOfShape occtCells;
		for (occtExplorer.Init(*pMergeTopology->GetOcctShape(), TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtCells.Contains(occtCurrent))
			{
				occtCells.Add(occtCurrent);
				try {
					occtBuilder.Add(occtCompSolid, occtCurrent);
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
		CellComplex* pMergeCellComplex = dynamic_cast<CellComplex*>(CellComplex::ByOcctShape(occtCompSolid));
		if (pMergeCellComplex == nullptr)
		{
			throw std::exception("CellComplex::ByCells(): Merge operation is not giving a cell complex");
		}
		return pMergeCellComplex;
	}

	Cell* CellComplex::Envelope() const
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		std::list<Cell*> cells;
		Cells(cells);
		for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
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
			return new Cell(TopoDS::Solid(occtExplorer.Current()));
		}
		return nullptr;
	}

	void CellComplex::InternalFaces(std::list<Face*>& rInternalFaces) const
	{
		Cell* pEnvelopeCell = Envelope();

		std::list<Face*> envelopeFaces;
		pEnvelopeCell->Faces(envelopeFaces);

		std::list<Face*> faces;
		Faces(faces);

		Handle(IntTools_Context) pOcctIntToolsContext = new IntTools_Context();
		for (std::list<Face*>::const_iterator kFaceIterator = faces.begin();
			kFaceIterator != faces.end();
			kFaceIterator++)
		{
			Face* pFace = *kFaceIterator;
			bool isEnvelopeFace = false;
			for (std::list<Face*>::const_iterator kEnvelopeFaceIterator = envelopeFaces.begin();
				kEnvelopeFaceIterator != envelopeFaces.end() && !isEnvelopeFace;
				kEnvelopeFaceIterator++)
			{
				Face* pEnvelopeFace = *kEnvelopeFaceIterator;
				if(BOPTools_AlgoTools::CheckSameGeom(TopoDS::Face(*pFace->GetOcctShape()), TopoDS::Face(*pEnvelopeFace->GetOcctShape()), pOcctIntToolsContext))
				{
					isEnvelopeFace = true;
				}
			}

			if (!isEnvelopeFace)
			{
				rInternalFaces.push_back(pFace);
			}
		}

		delete pEnvelopeCell;
	}

	TopoDS_Shape* CellComplex::GetOcctShape() const
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

	CellComplex::CellComplex(const TopoDS_CompSolid& rkOcctCompSolid)
		: Topology(3)
		, m_pOcctCompSolid(new TopoDS_CompSolid(rkOcctCompSolid))
	{
		GlobalCluster::GetInstance().Add(this);
	}

	CellComplex::~CellComplex()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctCompSolid;
	}
}
