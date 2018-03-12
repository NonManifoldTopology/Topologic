#include <Aperture.h>
#include <Topology.h>
#include <Context.h>

#include <assert.h>

using namespace System;

namespace TopoLogic
{
	Aperture^ Aperture::ByTopologyContext(TopoLogic::Topology^ topology, Context ^ context)
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::Aperture::ByTopologyContext(
			TopoLogicCore::Topology::DowncastToTopology(topology->GetCoreTopologicalQuery()),
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(context->GetCoreTopologicalQuery())
			);
		return gcnew Aperture(pCoreAperture);
	}
	double Aperture::FirstPathWeight()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		return pCoreAperture->FirstPathWeight();
	}

	double Aperture::SecondPathWeight()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		return pCoreAperture->SecondPathWeight();
	}

	Topology^ Aperture::Topology()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		return Topology::ByCoreTopology(pCoreAperture->Topology());
	}

	bool Aperture::IsOpen()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		return pCoreAperture->IsOpen();
	}

	List<TopoLogic::Topology^>^ Aperture::FirstPath()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		std::list<TopoLogicCore::Topology*> coreFirstPath;
		pCoreAperture->FirstPath(coreFirstPath);
		List<TopoLogic::Topology^>^ pFirstPath = gcnew List<TopoLogic::Topology^>();
		for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreTopologyIterator = coreFirstPath.begin();
			kCoreTopologyIterator != coreFirstPath.end();
			kCoreTopologyIterator++)
		{
			pFirstPath->Add(Topology::ByCoreTopology(*kCoreTopologyIterator));
		}
		return pFirstPath;
	}

	List<TopoLogic::Topology^>^ Aperture::SecondPath()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		std::list<TopoLogicCore::Topology*> coreSecondPath;
		pCoreAperture->SecondPath(coreSecondPath);
		List<TopoLogic::Topology^>^ pSecondPath = gcnew List<TopoLogic::Topology^>();
		for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreTopologyIterator = coreSecondPath.begin();
			kCoreTopologyIterator != coreSecondPath.end();
			kCoreTopologyIterator++)
		{
			pSecondPath->Add(Topology::ByCoreTopology(*kCoreTopologyIterator));
		}
		return pSecondPath;
	}

	Aperture::Aperture(TopoLogicCore::Aperture * const kpCoreAperture)
		: m_pCoreAperture(kpCoreAperture)
	{

	}

	Aperture::~Aperture()
	{
		delete m_pCoreAperture;
	}

	TopoLogicCore::TopologicalQuery* Aperture::GetCoreTopologicalQuery()
	{
		assert(m_pCoreAperture != nullptr && "Context::GetCoreTopologicalQuery() returns null.");
		if (m_pCoreAperture == nullptr)
		{
			throw gcnew Exception("Context::GetCoreTopologicalQuery() returns null.");
		}

		return m_pCoreAperture;
	}
}