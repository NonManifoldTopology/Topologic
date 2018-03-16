#include <Aperture.h>
#include <Context.h>
#include <GlobalCluster.h>

#include <TopExp.hxx>

#include <assert.h>
#include <array>

namespace TopoLogicCore
{
	Aperture* Aperture::ByTopologyContext(TopoLogicCore::Topology* const kpTopology, Context* const kpContext)
	{
		return new Aperture(kpTopology, kpContext, false);
	}

	Aperture* Aperture::ByTopologyContextStatus(TopoLogicCore::Topology * const kpTopology, Context * const kpContext, const bool kOpenStatus)
	{
		return new Aperture(kpTopology, kpContext, kOpenStatus);
	}

	bool Aperture::IsOpen() const
	{
		for (std::map<TopologyPairKey, ApertureStatus>::const_iterator kApertureStatusIterator = m_apertureStatuses.begin();
			kApertureStatusIterator != m_apertureStatuses.end();
			kApertureStatusIterator++)
		{
			if (kApertureStatusIterator->second.isOpen)
			{
				return true;
			}
		}

		return false;
	}

	bool Aperture::IsOpen(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies) const
	{
		CheckQueryInputValidity(rkTopologies);
		const ApertureStatus& rkApertureStatus = FindStatus(rkTopologies, true)->second;
		return rkApertureStatus.isOpen;
	}

	void Aperture::Open()
	{
		for (std::map<TopologyPairKey, ApertureStatus>::iterator apertureStatusIterator = m_apertureStatuses.begin();
			apertureStatusIterator != m_apertureStatuses.end();
			apertureStatusIterator++)
		{
			apertureStatusIterator->second.isOpen = true;
		}
	}

	void Aperture::Open(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies)
	{
		CheckQueryInputValidity(rkTopologies);
		ApertureStatus& rkApertureStatus = FindStatus(rkTopologies, true)->second;
		rkApertureStatus.isOpen = true;
	}

	void Aperture::Close()
	{
		for (std::map<TopologyPairKey, ApertureStatus>::iterator apertureStatusIterator = m_apertureStatuses.begin();
			apertureStatusIterator != m_apertureStatuses.end();
			apertureStatusIterator++)
		{
			apertureStatusIterator->second.isOpen = false;
		}
	}

	void Aperture::Close(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies)
	{
		CheckQueryInputValidity(rkTopologies);
		ApertureStatus& rkApertureStatus = FindStatus(rkTopologies, true)->second;
		rkApertureStatus.isOpen = false;
	}

	void Aperture::Paths(std::list<std::list<TopoLogicCore::Topology*>>& rPaths) const
	{
		for (std::map<TopologyPairKey, ApertureStatus>::const_iterator kApertureStatusIterator = m_apertureStatuses.begin();
			kApertureStatusIterator != m_apertureStatuses.end();
			kApertureStatusIterator++)
		{
			if (kApertureStatusIterator->second.isOpen)
			{
				std::list<TopoLogicCore::Topology*> path;
				path.push_back(Topology::ByOcctShape(kApertureStatusIterator->first.GetTopology1()));
				path.push_back(Topology::ByOcctShape(kApertureStatusIterator->first.GetTopology2()));
				rPaths.push_back(path);
			}
		}
	}

	void Aperture::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		Topology()->Geometry(rOcctGeometries);
	}

	TopoDS_Shape* Aperture::GetOcctShape() const
	{
		return Topology()->GetOcctShape();
	}

	TopologyType Aperture::GetType() const
	{
		return Topology()->GetType();
	}

	Topology* Aperture::Topology() const
	{
		return m_pTopology;
	}

	void Aperture::CheckQueryInputValidity(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies) const
	{
		TopologyType apertureType = GetType();
		if (GetType() == TOPOLOGY_EDGE)
		{
			if (rkTopologies[0] != nullptr && rkTopologies[0]->GetType() != TOPOLOGY_FACE)
			{
				std::stringstream ssException;
				ssException << "The aperture is an edge, but the first topology is a " <<
					rkTopologies[0]->GetTypeAsString() << "; it must be a face.";
			}
			
			if(rkTopologies[1] != nullptr && rkTopologies[1]->GetType() != TOPOLOGY_FACE)
			{
				std::stringstream ssException;
				ssException << "The aperture is an edge, but the second topology is a " <<
					rkTopologies[1]->GetTypeAsString() << "; it must be a face.";
			}
		}else if (GetType() == TOPOLOGY_FACE)
		{
			if (rkTopologies[0] != nullptr && rkTopologies[0]->GetType() != TOPOLOGY_CELL)
			{
				std::stringstream ssException;
				ssException << "The aperture is a face, but the first topology is a " <<
					rkTopologies[0]->GetTypeAsString() << "; it must be a cell.";
			}

			if (rkTopologies[1] != nullptr && rkTopologies[1]->GetType() != TOPOLOGY_CELL)
			{
				std::stringstream ssException;
				ssException << "The aperture is a face, but the second topology is a " <<
					rkTopologies[1]->GetTypeAsString() << "; it must be a cell.";
			}
		}
	}

	std::map<TopologyPairKey, ApertureStatus>::const_iterator Aperture::FindStatus(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies, const bool kRaiseExceptionIfNotFound) const
	{
		std::map<TopologyPairKey, ApertureStatus>::const_iterator kApertureStatusIterator = 
			m_apertureStatuses.find(TopologyPairKey(*rkTopologies[0]->GetOcctShape(), *rkTopologies[1]->GetOcctShape()));
		if (kApertureStatusIterator == m_apertureStatuses.end() && kRaiseExceptionIfNotFound)
		{
			throw std::exception("The aperture does not border the two topologies.");
		}

		return kApertureStatusIterator;
	}

	std::map<TopologyPairKey, ApertureStatus>::iterator Aperture::FindStatus(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies, const bool kRaiseExceptionIfNotFound)
	{
		std::map<TopologyPairKey, ApertureStatus>::iterator apertureStatusIterator =
			m_apertureStatuses.find(TopologyPairKey(*rkTopologies[0]->GetOcctShape(), *rkTopologies[1]->GetOcctShape()));
		if (apertureStatusIterator == m_apertureStatuses.end() && kRaiseExceptionIfNotFound)
		{
			throw std::exception("The aperture does not border the two topologies.");
		}

		return apertureStatusIterator;
	}

	void Aperture::InitialisePairwiseStatuses(const bool kOpenStatus)
	{
		TopAbs_ShapeEnum occtTopologyType = Topology()->GetOcctShape()->ShapeType();
		TopAbs_ShapeEnum occtParentTopologyType = TopAbs_SHAPE;
		if (occtTopologyType == TopAbs_FACE)
		{
			occtParentTopologyType = TopAbs_SOLID;
		}
		else if (occtTopologyType == TopAbs_EDGE)
		{
			occtParentTopologyType = TopAbs_FACE;
		}else
		{
			throw std::exception("Invalid topology");
		}

		TopTools_IndexedDataMapOfShapeListOfShape apertureToTopologyMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), occtTopologyType, occtParentTopologyType, apertureToTopologyMap);

		const TopTools_ListOfShape& rkOcctParentTopologies = apertureToTopologyMap.FindFromKey(*Topology()->GetOcctShape());

		for(TopTools_ListOfShape::const_iterator kOcctParentIterator1 = rkOcctParentTopologies.begin();
			kOcctParentIterator1 != rkOcctParentTopologies.end();
			kOcctParentIterator1++)
		{
			for (TopTools_ListOfShape::const_iterator kOcctParentIterator2 = rkOcctParentTopologies.begin();
				kOcctParentIterator2 != rkOcctParentTopologies.end();
				kOcctParentIterator2++)
			{
				if (kOcctParentIterator1 == kOcctParentIterator2)
				{
					continue;
				}
				ApertureStatus apertureStatus; 
				apertureStatus.isOpen = kOpenStatus;
				m_apertureStatuses.insert(std::make_pair(TopologyPairKey(*kOcctParentIterator1, *kOcctParentIterator2), apertureStatus));
			}
		}
	}

	Aperture::Aperture(TopoLogicCore::Topology* const kpTopology, Context* const kpContext, const bool kOpenStatus)
		: TopoLogicCore::Topology(kpTopology->Dimensionality())
		, m_pTopology(nullptr)
	{
		if (kpTopology == nullptr)
		{
			throw std::exception("A null topology is passed.");
		}
		if (kpContext == nullptr)
		{
			throw std::exception("A null context is passed.");
		}

		// Check validity
		TopologyType topologyType = kpTopology->GetType();
		TopologyType contextType = kpContext->Topology()->GetType();
		if (topologyType == TOPOLOGY_FACE)
		{
			if (contextType != TOPOLOGY_FACE)
			{
				std::stringstream ssException;
				ssException << "The topology is a " << kpTopology->GetTypeAsString() <<
					"but the context is a " << kpContext->Topology()->GetTypeAsString() << 
					"; it must be a face too.";
				throw std::exception(ssException.str().c_str());
			}
		}
		else if (topologyType == TOPOLOGY_EDGE)
		{
			if (contextType != TOPOLOGY_EDGE)
			{
				std::stringstream ssException;
				ssException << "The topology is an " << kpTopology->GetTypeAsString() <<
					"but the context is a " << kpContext->Topology()->GetTypeAsString() <<
					"; it must be an edge too.";
				throw std::exception(ssException.str().c_str());
			}
		}
		else
		{
			std::stringstream ssException;
			ssException << "The topology is a " << kpTopology->GetTypeAsString() << "; it must either be an edge or a face.";
			throw std::exception(ssException.str().c_str());
		}

		m_pTopology = kpTopology;
		AddContext(kpContext);

		InitialisePairwiseStatuses(kOpenStatus);
	}

	Aperture::~Aperture()
	{
		// Does not delete the contents; if the aperture disappears, its contents continue to exist.
	}
}