#include <Cluster.h>
#include <GlobalCluster.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <CellComplex.h>

#include <TopoDS_Builder.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <TopoDS_FrozenShape.hxx>





#include <BRep_Builder.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>

#include <TopTools_MapOfShape.hxx>


#include <assert.h>

namespace TopoLogicCore
{
	Cluster* Cluster::ByTopology(const std::list<Topology*>& rkTopologies)
	{
		Cluster* pCluster = new Cluster(true);
		for(std::list<Topology*>::const_iterator kTopologyIterator = rkTopologies.begin();
			kTopologyIterator != rkTopologies.end();
			kTopologyIterator++)
		{
			pCluster->Add(*kTopologyIterator);
		}
		return pCluster;
	}

	bool Cluster::Add(Topology const * const kpkTopology)
	{
		// If this cluster is not the global cluster, it must have been inside the global cluster.
		// (Added during initialisation.) The free flag is therefore at this point false.
		// To add a new member to this cluster, unfreeze it first/set the flag to true.
		Cluster* pGlobalCluster = GlobalCluster::GetInstance().GetCluster();
		if (GetOcctShape()->IsNotEqual(*pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape()->Free(true);
		}

		// Do the addition
		bool returnValue = true;
		try {
			m_occtBuilder.Add(*GetOcctShape(), *kpkTopology->GetOcctShape());
		}
		catch (TopoDS_UnCompatibleShapes &)
		{
			returnValue = false;
		}
		catch (TopoDS_FrozenShape &)
		{
			returnValue = false;
		}

		// Then reset the free flag to false, i.e. freeze the cluster.
		if (GetOcctShape()->IsNotEqual(*pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape()->Free(false);
		}

		return returnValue;
	}

	bool Cluster::Remove(Topology const * const kpkTopology)
	{
		// If this cluster is not the global cluster, it must have been inside the global cluster.
		// (Added during initialisation.) The free flag is therefore at this point false.
		// To remove a new member to this cluster, unfreeze it first/set the flag to true.
		Cluster* pGlobalCluster = GlobalCluster::GetInstance().GetCluster();
		if (GetOcctShape()->IsNotEqual(*pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape()->Free(true);
		}

		try {
			m_occtBuilder.Remove(*GetOcctShape(), *kpkTopology->GetOcctShape());

			return true;
		}
		catch (TopoDS_UnCompatibleShapes &)
		{
			return false;
		}
		catch (TopoDS_FrozenShape &)
		{
			return false;
		}

		// Then reset the free flag to false, i.e. freeze the cluster.
		if (GetOcctShape()->IsNotEqual(*pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape()->Free(false);
		}
	}

	TopoDS_Shape* Cluster::GetOcctShape() const
	{
		assert(m_pOcctCompound != nullptr && "Cluster::m_pOcctCompound is null.");
		if (m_pOcctCompound == nullptr)
		{
			throw std::exception("Cluster::m_pOcctCompound is null.");
		}

		return m_pOcctCompound;
	}

	void Cluster::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		throw std::exception("No implementation for Cluster entity");
	}

	Cluster::Cluster(const bool kAddToGlobalCluster)
		: Topology(3)
		, m_pOcctCompound(new TopoDS_Compound())
		, m_occtBuilder(TopoDS_Builder())
	{
		m_occtBuilder.MakeCompound(*m_pOcctCompound);

		if (kAddToGlobalCluster)
		{
			GlobalCluster::GetInstance().Add(this);
		}
	}

	Cluster::Cluster(const TopoDS_Compound& rkOcctCompound, const bool kAddToGlobalCluster)
		: Topology(3)
		, m_pOcctCompound(new TopoDS_Compound(rkOcctCompound))
	{
		// This constructor does not initialise the compound with MakeCompound.

		if (kAddToGlobalCluster)
		{
			GlobalCluster::GetInstance().Add(this);
		}
	}

	Cluster::~Cluster()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctCompound;
	}




	void Cluster::Shells(std::list<Shell*>& rShells) const
	{
		TopTools_MapOfShape occtShells;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_SHELL); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtShells.Contains(occtCurrent))
			{
				occtShells.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtShells.cbegin();
			kOcctShapeIterator != occtShells.cend();
			kOcctShapeIterator++)
		{
			rShells.push_back(new Shell(TopoDS::Shell(*kOcctShapeIterator)));
		}
	}

	void Cluster::Edges(std::list<Edge*>& rEdges) const
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
			rEdges.push_back(new Edge(TopoDS::Edge(*kOcctShapeIterator)));
		}
	}

	void Cluster::Faces(std::list<Face*>& rFaces) const
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
			rFaces.push_back(new Face(TopoDS::Face(*kOcctShapeIterator)));
		}
	}

	void Cluster::Vertices(std::list<Vertex*>& rVertices) const
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
			rVertices.push_back(new Vertex(TopoDS::Vertex(*kOcctShapeIterator)));
		}
	}

	void Cluster::Wires(std::list<Wire*>& rWires) const
	{
		TopTools_MapOfShape occtWires;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_WIRE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtWires.Contains(occtCurrent))
			{
				occtWires.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtWires.cbegin();
			kOcctShapeIterator != occtWires.cend();
			kOcctShapeIterator++)
		{
			rWires.push_back(new Wire(TopoDS::Wire(*kOcctShapeIterator)));
		}
	}

	void Cluster::Cells(std::list<Cell*>& rCells) const
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

	void Cluster::CellComplexes(std::list<CellComplex*>& rCellComplexes) const
	{
		TopExp_Explorer occtExplorer;
		TopTools_MapOfShape occtCellComplexes;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_COMPSOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& rkOcctCurrent = occtExplorer.Current();
			if (!occtCellComplexes.Contains(rkOcctCurrent))
			{
				occtCellComplexes.Add(rkOcctCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtCellComplexes.cbegin();
			kOcctShapeIterator != occtCellComplexes.cend();
			kOcctShapeIterator++)
		{
			rCellComplexes.push_back(new CellComplex(TopoDS::CompSolid(*kOcctShapeIterator)));
		}
	}

}
