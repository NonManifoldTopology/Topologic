#include <Cluster.h>
#include <GlobalCluster.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>

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

namespace TopologicCore
{
	std::shared_ptr<Cluster> Cluster::ByTopology(const std::list<std::shared_ptr<Topology>>& rkTopologies)
	{
		if (rkTopologies.empty())
		{
			throw std::exception("No topology is passed.");
		}

		//std::shared_ptr<Cluster> pCluster = std::make_shared<Cluster>(true);
		std::shared_ptr<Cluster> pCluster = std::make_shared<Cluster>();
		for(const std::shared_ptr<Topology>& kpTopology : rkTopologies)
		{
			pCluster->AddTopology(kpTopology.get());
		}
		return pCluster;
	}

	// Use raw pointer because 1) called from the individual instance, 2) does not need a smart pointer
	bool Cluster::AddTopology(Topology const * const kpkTopology, const bool kCheckIfInside)
	{
		if (kCheckIfInside)
		{
			if (IsInside(kpkTopology))
			{
				return false;
			}
		}

		// If this cluster is not the global cluster, it must have been inside the global cluster.
		// (Added during initialisation.) The free flag is therefore at this point false.
		// To add a new member to this cluster, unfreeze it first/set the flag to true.
		/*std::shared_ptr<Cluster> pGlobalCluster = GlobalCluster::GetInstance().GetCluster();
		if (GetOcctShape().IsNotEqual(pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape().Free(true);
		}*/

		// Do the addition
		bool returnValue = true;
		try {
			m_occtBuilder.Add(GetOcctShape(), kpkTopology->GetOcctShape());
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
		/*if (GetOcctShape().IsNotEqual(pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape().Free(false);
		}*/

		return returnValue;
	}

	bool Cluster::RemoveTopology(Topology const * const kpkTopology)
	{
		// If this cluster is not the global cluster, it must have been inside the global cluster.
		// (Added during initialisation.) The free flag is therefore at this point false.
		// To remove a new member to this cluster, unfreeze it first/set the flag to true.
		/*std::shared_ptr<Cluster> pGlobalCluster = GlobalCluster::GetInstance().GetCluster();
		if (GetOcctShape().IsNotEqual(pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape().Free(true);
		}*/

		try {
			m_occtBuilder.Remove(GetOcctShape(), kpkTopology->GetOcctShape());

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
		/*if (GetOcctShape().IsNotEqual(pGlobalCluster->GetOcctShape()))
		{
			GetOcctShape().Free(false);
		}*/
	}

	TopoDS_Shape& Cluster::GetOcctShape()
	{
		return GetOcctCompound();
	}

	const TopoDS_Shape& Cluster::GetOcctShape() const
	{
		return GetOcctCompound();
	}

	TopoDS_Compound& Cluster::GetOcctCompound()
	{
		assert(!m_occtCompound.IsNull() && "Cluster::m_occtCompound is null.");
		if (m_occtCompound.IsNull())
		{
			throw std::exception("Cluster::m_occtCompound is null.");
		}

		return m_occtCompound;
	}

	const TopoDS_Compound& Cluster::GetOcctCompound() const
	{
		assert(!m_occtCompound.IsNull() && "Cluster::m_occtCompound is null.");
		if (m_occtCompound.IsNull())
		{
			throw std::exception("Cluster::m_occtCompound is null.");
		}

		return m_occtCompound;
	}

	void Cluster::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		throw std::exception("No implementation for Cluster entity");
	}

	Cluster::Cluster()
		: Topology(3)
		, m_occtCompound(TopoDS_Compound())
		, m_occtBuilder(TopoDS_Builder())
	{
		m_occtBuilder.MakeCompound(m_occtCompound);

		/*if (kAddToGlobalCluster)
		{
			GlobalCluster::GetInstance().Add(this);
		}*/
	}

	Cluster::Cluster(const TopoDS_Compound& rkOcctCompound)
		: Topology(3)
		, m_occtCompound(rkOcctCompound)
	{
		// This constructor does not initialise the compound with MakeCompound.

		/*if (kAddToGlobalCluster)
		{
			GlobalCluster::GetInstance().Add(this);
		}*/
	}

	Cluster::~Cluster()
	{
		/*GlobalCluster::GetInstance().Remove(this);
		DecreaseCounter();*/
	}

	void Cluster::Shells(std::list<std::shared_ptr<Shell>>& rShells) const
	{
		DownwardNavigation(rShells);
	}

	void Cluster::Edges(std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Cluster::Faces(std::list<std::shared_ptr<Face>>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	void Cluster::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Cluster::Wires(std::list<std::shared_ptr<Wire>>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	void Cluster::Cells(std::list<std::shared_ptr<Cell>>& rCells) const
	{
		DownwardNavigation(rCells);
	}

	void Cluster::CellComplexes(std::list<std::shared_ptr<CellComplex>>& rCellComplexes) const
	{
		DownwardNavigation(rCellComplexes);
	}

	//Cluster::Ptr Cluster::Flatten()
	//{
	//	std::list<Topology::Ptr> allMembers;
	//	std::list<Topology::Ptr> thisClusterMembers;
	//	BOPCol_ListOfShape occtClusters;
	//	occtClusters.Append(GetOcctCompound());
	//	
	//	// Non-recursive breadth first search
	//	// Iterate through the members
	//	ImmediateMembers(thisClusterMembers);

	//	return Cluster::ByTopology(topologies);
	//}

	bool Cluster::IsInside(Topology const * const kpkTopology) const
	{
		const TopoDS_Shape& rkOcctAddedShape = kpkTopology->GetOcctShape();
		TopTools_MapOfShape occtShapes;
		for (TopExp_Explorer occtExplorer(GetOcctShape(), rkOcctAddedShape.ShapeType()); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& rkOcctCurrent = occtExplorer.Current();
			if (rkOcctAddedShape.IsSame(rkOcctCurrent))
			{
				return true;
			}
		}

		return false;
	}
}