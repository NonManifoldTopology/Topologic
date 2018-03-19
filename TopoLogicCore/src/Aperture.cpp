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
		return !m_occtAperturePaths.empty();
	}

	bool Aperture::IsOpen(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies) const
	{
		AperturePath topologyPair(*rkTopologies[0]->GetOcctShape(), *rkTopologies[1]->GetOcctShape());
		std::list<AperturePath>::const_iterator kTopologyPairIterator = std::find(m_occtAperturePaths.begin(), m_occtAperturePaths.end(), topologyPair);
		return kTopologyPairIterator != m_occtAperturePaths.end();
	}

	void Aperture::Open()
	{
		throw std::exception("Not taking into account a null");
		TopAbs_ShapeEnum occtContextTopologyType = m_pMainContext->Topology()->GetOcctShape()->ShapeType();
		TopAbs_ShapeEnum occtParentTopologyType = TopAbs_SHAPE;
		if (occtContextTopologyType == TopAbs_VERTEX)
		{
			occtContextTopologyType = TopAbs_EDGE;
		}
		else if (occtContextTopologyType == TopAbs_EDGE)
		{
			occtParentTopologyType = TopAbs_WIRE;
		}
		else if (occtContextTopologyType == TopAbs_WIRE)
		{
			occtParentTopologyType = TopAbs_FACE;
		}
		else if (occtContextTopologyType == TopAbs_FACE)
		{
			occtParentTopologyType = TopAbs_SHELL;
		}
		else if (occtContextTopologyType == TopAbs_SHELL)
		{
			occtParentTopologyType = TopAbs_SOLID;
		}
		else if (occtContextTopologyType == TopAbs_SOLID)
		{
			occtParentTopologyType = TopAbs_COMPSOLID;
		}
		else
		{
			throw std::exception("Invalid topology");
		}

		TopTools_IndexedDataMapOfShapeListOfShape apertureToTopologyMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), occtContextTopologyType, occtParentTopologyType, apertureToTopologyMap);

		const TopTools_ListOfShape& rkOcctParentTopologies = apertureToTopologyMap.FindFromKey(*Topology()->GetOcctShape());

		for (TopTools_ListOfShape::const_iterator kOcctParentIterator1 = rkOcctParentTopologies.begin();
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

				m_occtAperturePaths.push_back(AperturePath(*kOcctParentIterator1, *kOcctParentIterator2));
			}
		}
	}

	void Aperture::Open(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies)
	{
		throw std::exception("Not taking into account a null");
		AperturePath aperturePath(*rkTopologies[0]->GetOcctShape(), *rkTopologies[1]->GetOcctShape());
		std::list<AperturePath>::const_iterator kTopologyPairIterator = std::find(m_occtAperturePaths.begin(), m_occtAperturePaths.end(), aperturePath);
		if (kTopologyPairIterator == m_occtAperturePaths.end())
		{
			m_occtAperturePaths.push_back(aperturePath);
		}
	}

	void Aperture::Close()
	{
		m_occtAperturePaths.clear();
	}

	void Aperture::Close(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies)
	{
		throw std::exception("Not taking into account a null");
		AperturePath topologyPair(*rkTopologies[0]->GetOcctShape(), *rkTopologies[1]->GetOcctShape());
		m_occtAperturePaths.remove(topologyPair);
	}

	void Aperture::Paths(std::list<std::list<TopoLogicCore::Topology*>>& rPaths) const
	{
		for (std::list<AperturePath>::const_iterator kAperturePathIterator = m_occtAperturePaths.begin();
			kAperturePathIterator != m_occtAperturePaths.end();
			kAperturePathIterator++)
		{
			std::list<TopoLogicCore::Topology*> path;
			path.push_back(Topology::ByOcctShape(kAperturePathIterator->GetTopology1()));
			path.push_back(Topology::ByOcctShape(kAperturePathIterator->GetTopology2()));
			rPaths.push_back(path);
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

	Aperture::Aperture(TopoLogicCore::Topology* const kpTopology, Context* const kpContext, const bool kOpenStatus)
		: TopoLogicCore::Topology(kpTopology->Dimensionality())
		, m_pMainContext(kpContext)
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

		m_pTopology = kpTopology;
		AddContext(kpContext);

		if (kOpenStatus)
		{
			Open();
		}
	}

	Aperture::~Aperture()
	{
		// Does not delete the contents; if the aperture disappears, its contents continue to exist.
	}
}