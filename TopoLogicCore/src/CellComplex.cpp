#include <CellComplex.h>
#include <GlobalCluster.h>
#include <Face.h>
#include <Cell.h>

#include <BRep_Builder.hxx>
#include <Geom_Surface.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <TopTools_MapOfShape.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	Cell* CellComplex::BoundingCell() const
	{
		throw std::exception("Not implemented yet");
	}

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
			rCells.push_back(new Cell(new TopoDS_Solid(TopoDS::Solid(*kOcctShapeIterator))));
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
			rFaces.push_back(new Face(new TopoDS_Face(TopoDS::Face(*kOcctShapeIterator))));
		}
	}

	bool CellComplex::IsClosed() const
	{
		throw std::exception("Not implemented yet");
	}

	CellComplex* CellComplex::ByCells(const std::list<Cell*>& rkCells)
	{
		std::list<Topology*> topologyList;
		for (std::list<Cell*>::const_iterator rkCellIterator = rkCells.begin();
			rkCellIterator != rkCells.end();
			rkCellIterator++)
		{
			topologyList.push_back(*rkCellIterator);
		}
		Topology* pMergeTopology = Topology::Merge(topologyList, true);

		// Should get us a CellComplex, otherwise an exception.

		CellComplex* pMergeCellComplex = dynamic_cast<CellComplex*>(pMergeTopology);
		if (pMergeCellComplex == nullptr)
		{
			throw std::exception("CellComplex::ByCells(): Merge operation is not giving a cell complex");
		}
		return pMergeCellComplex;
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

	CellComplex::CellComplex(TopoDS_CompSolid * const kpOcctCompSolid)
		: Topology(3)
		, m_pOcctCompSolid(kpOcctCompSolid)
	{
		GlobalCluster::GetInstance().Add(this);
	}

	CellComplex::~CellComplex()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctCompSolid;
	}
}
