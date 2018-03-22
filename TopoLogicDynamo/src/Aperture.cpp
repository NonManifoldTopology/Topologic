#include "Aperture.h"
#include <Topology.h>
#include <Context.h>

#include <assert.h>
#include <array>

using namespace System;

namespace TopoLogic
{
	Aperture^ Aperture::ByTopologyContext(TopoLogic::Topology^ topology, Context^ context)
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::Aperture::ByTopologyContext(
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery()),
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(context->GetCoreTopologicalQuery())
			);
		return gcnew Aperture(pCoreAperture);
	}

	Aperture^ Aperture::ByTopologyContextStatus(TopoLogic::Topology^ topology, Context^ context, bool openStatus)
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::Aperture::ByTopologyContextStatus(
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery()),
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(context->GetCoreTopologicalQuery()),
			openStatus
		);
		return gcnew Aperture(pCoreAperture);
	}

	TopoLogic::Topology^ Aperture::Topology()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		TopoLogicCore::Topology* pCoreTopology = pCoreAperture->Topology();
		return Topology::ByCoreTopology(pCoreTopology);
	}

	bool Aperture::IsOpen()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		return pCoreAperture->IsOpen();
	}

	bool Aperture::IsOpen(List<TopoLogic::Topology^>^ topologies)
	{
		if (topologies->Count != 2)
		{
			throw gcnew Exception("The input list must contain exactly 2 elements.");
		}

		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		std::array<TopoLogicCore::Topology*, 2> coreTopologies
		{
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topologies[0]->GetCoreTopologicalQuery()),
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topologies[1]->GetCoreTopologicalQuery()),
		};

		return pCoreAperture->IsOpen(coreTopologies);
	}

	List<List<TopoLogic::Topology^>^>^ Aperture::Paths()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		std::list<std::list<TopoLogicCore::Topology*>> corePaths;
		pCoreAperture->Paths(corePaths);
		List<List<TopoLogic::Topology^>^>^ pPaths = gcnew List<List<TopoLogic::Topology^>^>();
		for (std::list<std::list<TopoLogicCore::Topology*>>::const_iterator kCorePathIterator = corePaths.begin();
			kCorePathIterator != corePaths.end();
			kCorePathIterator++)
		{
			List<TopoLogic::Topology^>^ pPath = gcnew List<TopoLogic::Topology^>();
			for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreTopologyIterator = kCorePathIterator->begin();
				kCoreTopologyIterator != kCorePathIterator->end();
				kCoreTopologyIterator++)
			{
				TopoLogicCore::Topology* pCoreTopology = *kCoreTopologyIterator;
				if (pCoreTopology == nullptr)
				{
					pPath->Add(nullptr);
				}else
				{
					pPath->Add(Topology::ByCoreTopology(pCoreTopology));
				}
			}
			pPaths->Add(pPath);
		}
		return pPaths;
	}

	Aperture^ TopoLogic::Aperture::Open()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		pCoreAperture->Open();
		return this;
	}

	Aperture^ TopoLogic::Aperture::Open(List<TopoLogic::Topology^>^ topologies)
	{
		if (topologies->Count != 2)
		{
			throw gcnew Exception("The input list must contain exactly 2 elements.");
		}

		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		std::array<TopoLogicCore::Topology*, 2> coreTopologies
		{
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topologies[0]->GetCoreTopologicalQuery()),
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topologies[1]->GetCoreTopologicalQuery()),
		};
		pCoreAperture->Open(coreTopologies);
		return this;
	}

	Aperture^ TopoLogic::Aperture::Close()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		pCoreAperture->Close();
		return this;
	}

	Aperture^ TopoLogic::Aperture::Close(List<TopoLogic::Topology^>^ topologies)
	{
		if (topologies->Count != 2)
		{
			throw gcnew Exception("The input list must contain exactly 2 elements.");
		}

		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		std::array<TopoLogicCore::Topology*, 2> coreTopologies
		{
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topologies[0]->GetCoreTopologicalQuery()),
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topologies[1]->GetCoreTopologicalQuery()),
		};
		pCoreAperture->Close(coreTopologies);
		return this;
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

	Object^ Aperture::Geometry::get()
	{
		TopoLogicCore::Aperture* pCoreAperture = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Aperture>(GetCoreTopologicalQuery());
		
		throw gcnew NotImplementedException();
	}
}