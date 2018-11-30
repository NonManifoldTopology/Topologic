#include <Aperture.h>
#include <Context.h>
#include <Vertex.h>
#include <Face.h>
#include <Wire.h>
#include <ApertureFactory.h>

#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <assert.h>
#include <array>

namespace TopologicCore
{
	std::shared_ptr<Aperture> Aperture::ByTopologyContext(const Topology::Ptr& kpTopology, const std::shared_ptr<Context>& kpContext)
	{
		const bool kDefaultStatus = false;
		return ByTopologyContextStatus(kpTopology, kpContext, kDefaultStatus);
	}

	std::shared_ptr<Aperture> Aperture::ByTopologyContext(const Topology::Ptr & kpTopology, const Topology::Ptr & kpContextTopology)
	{
		const bool kDefaultStatus = false;
		const double  kDefaultParameter = 0.0;
		Topology::Ptr pClosestSimplestSubshape = kpContextTopology->ClosestSimplestSubshape(kpTopology->CenterOfMass());

		// Check aperture topology and context topology.
		// If the context is a face, and the aperture is either a wire or a face,
		// - For an aperture wire, create a hole
		// - For an aperture face, create a hole and plug it
		//if (pClosestSimplestSubshape->GetType() == TOPOLOGY_FACE)
		//{
		//	Face::Ptr pFaceContext = TopologicalQuery::Downcast<Face>(pClosestSimplestSubshape);
		//	if (kpTopology->GetType() == TOPOLOGY_WIRE)
		//	{
		//		Wire::Ptr pWireAperture = TopologicalQuery::Downcast<Wire>(kpTopology);
		//		TopoDS_Shape occtOldSubentity = pFaceContext->GetOcctFace();
		//		pFaceContext->AddInternalBoundary(pWireAperture);
		//		kpContextTopology->ReplaceSubentity(occtOldSubentity, pFaceContext->GetOcctFace());
		//	}
		//	else if (kpTopology->GetType() == TOPOLOGY_FACE)
		//	{
		//		Face::Ptr pFaceAperture = TopologicalQuery::Downcast<Face>(kpTopology);
		//		
		//		// Merge pClosestSimplestSubshape with pFaceAperture,
		//		// then pClosestSimplestSubshape is the 
		//		//Topology::Ptr = pClosestSimplestSubshape->Merge(pFaceAperture);
		//	}
		//}

		Context::Ptr pContext = Context::ByTopologyParameters(pClosestSimplestSubshape, kDefaultParameter, kDefaultParameter, kDefaultParameter);

		std::shared_ptr<Aperture> pAperture = std::make_shared<Aperture>(kpTopology, pContext, kDefaultStatus);

		//kpContextTopology->AddSubContent(pAperture, Face::Type());
		
		return pAperture;
	}

	std::shared_ptr<Aperture> Aperture::ByTopologyContextStatus(const Topology::Ptr& kpTopology, const std::shared_ptr<Context>& kpContext, const bool kOpenStatus)
	{
		std::shared_ptr<Aperture> pAperture = std::make_shared<Aperture>(kpTopology, kpContext, kOpenStatus);
		// HACK
		kpContext->Topology()->AddContent(pAperture, Face::Type());
		pAperture->AddContext(kpContext);
		return pAperture;
	}

	//bool Aperture::IsOpen() const
	//{
	//	return !m_occtAperturePaths.empty();
	//}

	//bool Aperture::IsOpen(const std::array<Topology::Ptr, 2>& rkTopologies) const
	//{
	//	AperturePath aperturePath(
	//		rkTopologies[0] == nullptr? TopoDS_Shape() : rkTopologies[0]->GetOcctShape(), 
	//		rkTopologies[1] == nullptr? TopoDS_Shape() : rkTopologies[1]->GetOcctShape()
	//	);
	//	std::list<AperturePath>::const_iterator kTopologyPairIterator = 
	//		std::find(m_occtAperturePaths.begin(), m_occtAperturePaths.end(), aperturePath);
	//	return kTopologyPairIterator != m_occtAperturePaths.end();
	//}

	//void Aperture::Open()
	//{
	//	// 1. Get the immediate parent topology
	//	int occtContextTopologyType = (int) GetMainContext()->Topology()->GetOcctShape().ShapeType();
	//	if (occtContextTopologyType >= (int) TopAbs_COMPOUND)
	//	{
	//		return;
	//	}
	//	int occtParentTopologyTypeInt = occtContextTopologyType + 1; // always 1 level up

	//	m_occtAperturePaths.clear();
	//	TopTools_IndexedDataMapOfShapeListOfShape apertureToTopologyMap;
	//	TopExp::MapShapesAndUniqueAncestors(, occtContextTopologyType, occtParentTopologyType, apertureToTopologyMap);

	//	const TopTools_ListOfShape& rkOcctParentTopologies = apertureToTopologyMap.FindFromKey(m_pMainContext->Topology()->GetOcctShape());

	//	if (rkOcctParentTopologies.IsEmpty())
	//	{
	//		m_occtAperturePaths.push_back(AperturePath(TopoDS_Shape(), TopoDS_Shape()));
	//	}
	//	else if (rkOcctParentTopologies.Size() == 1)
	//	{
	//		TopTools_ListOfShape::const_iterator kOcctParentIterator = rkOcctParentTopologies.begin();
	//		m_occtAperturePaths.push_back(AperturePath(TopoDS_Shape(), *kOcctParentIterator));
	//		m_occtAperturePaths.push_back(AperturePath(*kOcctParentIterator, TopoDS_Shape()));
	//	}else
	//	{
	//		for(TopTools_ListIteratorOfListOfShape kOcctParentIterator1(rkOcctParentTopologies);
	//			kOcctParentIterator1.More();
	//			kOcctParentIterator1.Next())
	//		{
	//			for (TopTools_ListIteratorOfListOfShape kOcctParentIterator2(rkOcctParentTopologies);
	//				kOcctParentIterator2.More();
	//				kOcctParentIterator2.Next())
	//			{
	//				if (kOcctParentIterator1 == kOcctParentIterator2)
	//				{
	//					continue;
	//				}

	//				m_occtAperturePaths.push_back(AperturePath(kOcctParentIterator1.Value(), kOcctParentIterator2.Value()));
	//			}
	//		}
	//	}
	//}

	//TopoDS_Shape FindSubentityAdjacentAndHigherDimensionalTo(const TopoDS_Shape& rkShape1, const TopoDS_Shape& rkShape2)
	//{
	//	TopAbs_ShapeEnum occtShape2Type = rkShape2.ShapeType();
	//	TopAbs_ShapeEnum occtTypeToSearch = TopAbs_SHAPE;
	//	if (occtShape2Type == TopAbs_VERTEX)
	//	{
	//		occtTypeToSearch = TopAbs_EDGE;
	//	}
	//	else if (occtShape2Type == TopAbs_EDGE)
	//	{
	//		occtTypeToSearch = TopAbs_WIRE;
	//	}
	//	else if (occtShape2Type == TopAbs_WIRE)
	//	{
	//		occtTypeToSearch = TopAbs_FACE;
	//	}
	//	else if (occtShape2Type == TopAbs_FACE)
	//	{
	//		occtTypeToSearch = TopAbs_SHELL;
	//	}
	//	else if (occtShape2Type == TopAbs_SHELL)
	//	{
	//		occtTypeToSearch = TopAbs_SOLID;
	//	}
	//	else if (occtShape2Type == TopAbs_SOLID)
	//	{
	//		occtTypeToSearch = TopAbs_COMPSOLID;
	//	}
	//	else
	//	{
	//		throw std::exception("Invalid topology");
	//	}

	//	TopExp_Explorer occtExplorer;
	//	for (occtExplorer.Init(rkShape1, occtTypeToSearch); occtExplorer.More(); occtExplorer.Next())
	//	{
	//		const TopoDS_Shape& occtCurrent = occtExplorer.Current();
	//		TopExp_Explorer occtCurrentExplorer;
	//		for (occtCurrentExplorer.Init(occtCurrent, occtShape2Type); occtCurrentExplorer.More(); occtCurrentExplorer.Next())
	//		{
	//			const TopoDS_Shape& occtCurrentChild = occtCurrentExplorer.Current();
	//			if (!occtCurrentChild.IsSame(rkShape2))
	//			{
	//				return occtCurrent;
	//			}
	//		}
	//	}

	//	return TopoDS_Shape(); // empty
	//}

	//void Aperture::Open(const std::array<Topology::Ptr, 2>& rkTopologies)
	//{
	//	AperturePath aperturePath(
	//		rkTopologies[0] == nullptr ? 
	//			TopoDS_Shape() : 
	//			FindSubentityAdjacentAndHigherDimensionalTo(
	//				rkTopologies[0]->GetOcctShape(), 
	//				m_pMainContext->Topology()->GetOcctShape()),
	//		rkTopologies[1] == nullptr ? 
	//			TopoDS_Shape() : 
	//			FindSubentityAdjacentAndHigherDimensionalTo(
	//				rkTopologies[1]->GetOcctShape(), 
	//				m_pMainContext->Topology()->GetOcctShape())
	//	);
	//	std::list<AperturePath>::const_iterator kTopologyPairIterator = std::find(m_occtAperturePaths.begin(), m_occtAperturePaths.end(), aperturePath);
	//	if (kTopologyPairIterator == m_occtAperturePaths.end())
	//	{
	//		m_occtAperturePaths.push_back(aperturePath);
	//	}
	//}

	//void Aperture::Close()
	//{
	//	m_occtAperturePaths.clear();
	//}

	//void Aperture::Close(const std::array<Topology::Ptr, 2>& rkTopologies)
	//{
	//	AperturePath aperturePath(
	//		rkTopologies[0] == nullptr ? TopoDS_Shape() : rkTopologies[0]->GetOcctShape(),
	//		rkTopologies[1] == nullptr ? TopoDS_Shape() : rkTopologies[1]->GetOcctShape()
	//	);

	//	for (std::list<AperturePath>::const_iterator kAperturePathIterator = m_occtAperturePaths.begin();
	//		kAperturePathIterator != m_occtAperturePaths.end();
	//		kAperturePathIterator++)
	//	{
	//		if (*kAperturePathIterator == aperturePath)
	//		{
	//			m_occtAperturePaths.erase(kAperturePathIterator);
	//			break;
	//		}
	//	}
	//}

	//void Aperture::Paths(std::list<std::list<Topology::Ptr>>& rPaths) const
	//{
	//	for(const AperturePath& rkAperturePath : m_occtAperturePaths)
	//	{
	//		std::list<Topology::Ptr> path;
	//		path.push_back(Topology::ByOcctShape(rkAperturePath.GetTopology1()));
	//		path.push_back(Topology::ByOcctShape(rkAperturePath.GetTopology2()));
	//		rPaths.push_back(path);
	//	}
	//}

	Vertex::Ptr Aperture::CenterOfMass() const
	{
		return Topology()->CenterOfMass();
	}

	bool Aperture::IsManifold() const
	{
		throw std::exception("Not implemented yet");
	}

	std::string Aperture::GetTypeAsString() const
	{
		return std::string("Aperture");
	}

	void Aperture::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		Topology()->Geometry(rOcctGeometries);
	}

	TopoDS_Shape& Aperture::GetOcctShape()
	{
		return Topology()->GetOcctShape();
	}

	void Aperture::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		m_pTopology->SetOcctShape(rkOcctShape);
	}

	const TopoDS_Shape& Aperture::GetOcctShape() const
	{
		return Topology()->GetOcctShape();
	}

	TopologyType Aperture::GetType() const
	{
		return TOPOLOGY_APERTURE;
	}

	bool Aperture::IsContainerType()
	{
		return Topology()->IsContainerType();
	}

	Aperture::Ptr Aperture::ByBoundaryWithinHost(const Face::Ptr& kpHostFace, const Wire::Ptr& kpApertureBoundary, const bool kLink, const bool kOpenStatus)
	{
		/*Face::Ptr trimmedFace = kpHostFace->Trim(kpApertureBoundary);
		return std::make_shared<Aperture>(trimmedFace, nullptr, kOpenStatus);*/
		return nullptr;
	}

	Topology::Ptr Aperture::Topology() const
	{
		assert(m_pTopology != nullptr && "The underlying topology is null.");
		if (m_pTopology == nullptr)
		{
			throw std::exception("The underlying topology is null.");
		}
		return m_pTopology;
	}

	Aperture::Aperture(const Topology::Ptr& kpTopology, const std::shared_ptr<Context>& kpContext, const bool kOpenStatus, const std::string& rkGuid)
		: TopologicCore::Topology(kpTopology->Dimensionality(), kpTopology->GetOcctShape(), rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_pMainContext(kpContext)
		, m_pTopology(kpTopology)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<ApertureFactory>());
		if (kpTopology == nullptr)
		{
			throw std::exception("A null topology is passed.");
		}
		/*if (kpContext == nullptr)
		{
			throw std::exception("A null context is passed.");
		}*/

		if (kpContext != nullptr)
		{
			AddContext(kpContext);
		}

		/*if (kOpenStatus)
		{
			Open();
		}*/
	}

	Aperture::~Aperture()
	{
		// Does not delete the contents; if the aperture disappears, its contents continue to exist.
	}
}