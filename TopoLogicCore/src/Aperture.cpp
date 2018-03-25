#include <Aperture.h>
#include <Context.h>
#include <GlobalCluster.h>

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

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
		AperturePath aperturePath(
			rkTopologies[0] == nullptr? TopoDS_Shape() : *rkTopologies[0]->GetOcctShape(), 
			rkTopologies[1] == nullptr? TopoDS_Shape() : *rkTopologies[1]->GetOcctShape()
		);
		std::list<AperturePath>::const_iterator kTopologyPairIterator = 
			std::find(m_occtAperturePaths.begin(), m_occtAperturePaths.end(), aperturePath);
		return kTopologyPairIterator != m_occtAperturePaths.end();
	}

	void Aperture::Open()
	{
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

		m_occtAperturePaths.clear();
		TopTools_IndexedDataMapOfShapeListOfShape apertureToTopologyMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), occtContextTopologyType, occtParentTopologyType, apertureToTopologyMap);

		const TopTools_ListOfShape& rkOcctParentTopologies = apertureToTopologyMap.FindFromKey(*m_pMainContext->Topology()->GetOcctShape());

		if (rkOcctParentTopologies.IsEmpty())
		{
			m_occtAperturePaths.push_back(AperturePath(TopoDS_Shape(), TopoDS_Shape()));
		}
		else if (rkOcctParentTopologies.Size() == 1)
		{
			TopTools_ListOfShape::const_iterator kOcctParentIterator = rkOcctParentTopologies.begin();
			m_occtAperturePaths.push_back(AperturePath(TopoDS_Shape(), *kOcctParentIterator));
			m_occtAperturePaths.push_back(AperturePath(*kOcctParentIterator, TopoDS_Shape()));
		}else
		{
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
	}

	TopoDS_Shape FindSubentityAdjacentAndHigherDimensionalTo(const TopoDS_Shape& rkShape1, const TopoDS_Shape& rkShape2)
	{
		TopAbs_ShapeEnum occtShape2Type = rkShape2.ShapeType();
		TopAbs_ShapeEnum occtTypeToSearch = TopAbs_SHAPE;
		if (occtShape2Type == TopAbs_VERTEX)
		{
			occtTypeToSearch = TopAbs_EDGE;
		}
		else if (occtShape2Type == TopAbs_EDGE)
		{
			occtTypeToSearch = TopAbs_WIRE;
		}
		else if (occtShape2Type == TopAbs_WIRE)
		{
			occtTypeToSearch = TopAbs_FACE;
		}
		else if (occtShape2Type == TopAbs_FACE)
		{
			occtTypeToSearch = TopAbs_SHELL;
		}
		else if (occtShape2Type == TopAbs_SHELL)
		{
			occtTypeToSearch = TopAbs_SOLID;
		}
		else if (occtShape2Type == TopAbs_SOLID)
		{
			occtTypeToSearch = TopAbs_COMPSOLID;
		}
		else
		{
			throw std::exception("Invalid topology");
		}

		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(rkShape1, occtTypeToSearch); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			TopExp_Explorer occtCurrentExplorer;
			for (occtCurrentExplorer.Init(occtCurrent, occtShape2Type); occtCurrentExplorer.More(); occtCurrentExplorer.Next())
			{
				const TopoDS_Shape& occtCurrentChild = occtCurrentExplorer.Current();
				if (!occtCurrentChild.IsSame(rkShape2))
				{
					return occtCurrent;
				}
			}
		}

		return TopoDS_Shape(); // empty
	}

	void Aperture::Open(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies)
	{
		AperturePath aperturePath(
			rkTopologies[0] == nullptr ? TopoDS_Shape() : FindSubentityAdjacentAndHigherDimensionalTo(*rkTopologies[0]->GetOcctShape(), *m_pMainContext->Topology()->GetOcctShape()),
			rkTopologies[1] == nullptr ? TopoDS_Shape() : FindSubentityAdjacentAndHigherDimensionalTo(*rkTopologies[1]->GetOcctShape(), *m_pMainContext->Topology()->GetOcctShape())
		);
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
		AperturePath aperturePath(
			rkTopologies[0] == nullptr ? TopoDS_Shape() : *rkTopologies[0]->GetOcctShape(),
			rkTopologies[1] == nullptr ? TopoDS_Shape() : *rkTopologies[1]->GetOcctShape()
		);
		for (std::list<AperturePath>::const_iterator kAperturePathIterator = m_occtAperturePaths.begin();
			kAperturePathIterator != m_occtAperturePaths.end();
			kAperturePathIterator++)
		{
			if (*kAperturePathIterator == aperturePath)
			{
				m_occtAperturePaths.erase(kAperturePathIterator);
				break;
			}
		}
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
		return TOPOLOGY_APERTURE;
	}

	Topology* Aperture::Topology() const
	{
		assert(m_pTopology != nullptr && "The underlying topology is null.");
		if (m_pTopology == nullptr)
		{
			throw std::exception("The underlying topology is null.");
		}
		return m_pTopology;
	}

	Aperture::Aperture(TopoLogicCore::Topology* const kpTopology, Context* const kpContext, const bool kOpenStatus)
		: TopoLogicCore::Topology(kpTopology->Dimensionality())
		, m_pMainContext(kpContext)
		, m_pTopology(kpTopology)
	{
		if (kpTopology == nullptr)
		{
			throw std::exception("A null topology is passed.");
		}
		if (kpContext == nullptr)
		{
			throw std::exception("A null context is passed.");
		}

		AddContext(kpContext);
		m_pMainContext->Topology()->AddContent(this);

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