#include "Cluster.h"
#include "CellComplex.h"
#include "Cell.h"
#include "Vertex.h"
#include "Edge.h"
#include "Wire.h"
#include "Face.h"
#include "Shell.h"
#include "ClusterFactory.h"
#include "GlobalCluster.h"
#include "AttributeManager.h"

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

#include <assert.h>

namespace TopologicCore
{
	Cluster::Ptr Cluster::ByTopologies(const std::list<Topology::Ptr>& rkTopologies)
	{
		if (rkTopologies.empty())
		{
			return nullptr;
		}

		TopoDS_Compound occtCompound;
		TopoDS_Builder occtBuilder;
		occtBuilder.MakeCompound(occtCompound);
		Cluster::Ptr pCluster = std::make_shared<Cluster>(occtCompound);
		for(const Topology::Ptr& kpTopology : rkTopologies)
		{
			//Topology::Ptr pCopyTopology = kpTopology->DeepCopy();
			pCluster->AddTopology(kpTopology.get());
		}

		Cluster::Ptr pCopyCluster = std::dynamic_pointer_cast<Cluster>(pCluster->DeepCopy());
		for (const Topology::Ptr& kpTopology : rkTopologies)
		{
			AttributeManager::GetInstance().CopyAttributes(kpTopology->GetOcctShape(), pCopyCluster->GetOcctCompound());
		}

		GlobalCluster::GetInstance().AddTopology(pCopyCluster->GetOcctCompound());
		return pCopyCluster;
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

		return returnValue;
	}

	bool Cluster::RemoveTopology(Topology const * const kpkTopology)
	{
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
			throw std::exception("A null Cluster is encountered.");
		}

		return m_occtCompound;
	}

	const TopoDS_Compound& Cluster::GetOcctCompound() const
	{
		assert(!m_occtCompound.IsNull() && "Cluster::m_occtCompound is null.");
		if (m_occtCompound.IsNull())
		{
			throw std::exception("A null Cluster is encountered.");
		}

		return m_occtCompound;
	}

	void Cluster::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		SetOcctShape(TopoDS::Compound(rkOcctShape));
	}

	void Cluster::SetOcctCompound(const TopoDS_Compound & rkOcctCompound)
	{
		m_occtCompound = rkOcctCompound;
	}

	void Cluster::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		throw std::exception("No implementation for Cluster entity");
	}

	Cluster::Cluster(const TopoDS_Compound& rkOcctCompound, const std::string& rkGuid)
		: Topology(3, rkOcctCompound, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtCompound(rkOcctCompound)
	{
		// This constructor does not initialise the compound with MakeCompound.
		RegisterFactory(GetClassGUID(), std::make_shared<ClusterFactory>());
	}

	Cluster::~Cluster()
	{

	}

	void Cluster::Shells(std::list<Shell::Ptr>& rShells) const
	{
		DownwardNavigation(rShells);
	}

	void Cluster::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Cluster::Faces(std::list<Face::Ptr>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	void Cluster::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Cluster::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	void Cluster::Cells(std::list<Cell::Ptr>& rCells) const
	{
		DownwardNavigation(rCells);
	}

	void Cluster::CellComplexes(std::list<CellComplex::Ptr>& rCellComplexes) const
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

	Vertex::Ptr Cluster::CenterOfMass() const
	{
		throw std::exception("Not implemented yet");
	}

	TopoDS_Vertex Cluster::CenterOfMass(const TopoDS_Compound & rkOcctCompound)
	{
		throw std::exception("Not implemented yet");
	}

	bool Cluster::IsManifold() const
	{
		throw std::exception("Not implemented yet");
	}

	std::string Cluster::GetTypeAsString() const
	{
		return std::string("Cluster");
	}

}