#include "Aperture.h"
#include <Topology.h>
#include <Context.h>
#include <ApertureContentFactory.h>

#include <assert.h>
#include <array>

using namespace System;

namespace Topologic
{
	Aperture^ Aperture::ByTopologyContext(Topologic::Topology^ topology, Context^ context)
	{
		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::Aperture::ByTopologyContext(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()),
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(context->GetCoreTopologicalQuery())
			);
		return gcnew Aperture(pCoreAperture);
	}

	Aperture^ Aperture::ByTopologyContextStatus(Topologic::Topology^ topology, Context^ context, bool openStatus)
	{
		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::Aperture::ByTopologyContextStatus(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()),
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(context->GetCoreTopologicalQuery()),
			openStatus
		);
		return gcnew Aperture(pCoreAperture);
	}

	Topologic::Topology^ Aperture::Topology()
	{
		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = pCoreAperture->Topology();
		return Topology::ByCoreTopology(pCoreTopology);
	}

	bool Aperture::IsOpen()
	{
		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		return pCoreAperture->IsOpen();
	}

	bool Aperture::IsOpen(List<Topologic::Topology^>^ topologies)
	{
		if (topologies->Count != 2)
		{
			throw gcnew Exception("The input list must contain exactly 2 elements.");
		}

		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		std::array<std::shared_ptr<TopologicCore::Topology>, 2> coreTopologies
		{
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topologies[0]->GetCoreTopologicalQuery()),
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topologies[1]->GetCoreTopologicalQuery()),
		};

		return pCoreAperture->IsOpen(coreTopologies);
	}

	List<List<Topologic::Topology^>^>^ Aperture::Paths()
	{
		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		std::list<std::list<std::shared_ptr<TopologicCore::Topology>>> corePaths;
		pCoreAperture->Paths(corePaths);
		List<List<Topologic::Topology^>^>^ pPaths = gcnew List<List<Topologic::Topology^>^>();
		for (std::list<std::list<std::shared_ptr<TopologicCore::Topology>>>::const_iterator kCorePathIterator = corePaths.begin();
			kCorePathIterator != corePaths.end();
			kCorePathIterator++)
		{
			List<Topologic::Topology^>^ pPath = gcnew List<Topologic::Topology^>();
			for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopologyIterator = kCorePathIterator->begin();
				kCoreTopologyIterator != kCorePathIterator->end();
				kCoreTopologyIterator++)
			{
				const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology = *kCoreTopologyIterator;
				if (kpCoreTopology == nullptr)
				{
					pPath->Add(nullptr);
				}else
				{
					pPath->Add(Topology::ByCoreTopology(kpCoreTopology));
				}
			}
			pPaths->Add(pPath);
		}
		return pPaths;
	}

	Aperture^ Topologic::Aperture::Open()
	{
		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		pCoreAperture->Open();
		return this;
	}

	Aperture^ Topologic::Aperture::Open(List<Topologic::Topology^>^ topologies)
	{
		if (topologies->Count != 2)
		{
			throw gcnew Exception("The input list must contain exactly 2 elements.");
		}

		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		std::array<std::shared_ptr<TopologicCore::Topology>, 2> coreTopologies
		{
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topologies[0]->GetCoreTopologicalQuery()),
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topologies[1]->GetCoreTopologicalQuery()),
		};
		pCoreAperture->Open(coreTopologies);
		return this;
	}

	Aperture^ Topologic::Aperture::Close()
	{
		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		pCoreAperture->Close();
		return this;
	}

	Aperture^ Topologic::Aperture::Close(List<Topologic::Topology^>^ topologies)
	{
		if (topologies->Count != 2)
		{
			throw gcnew Exception("The input list must contain exactly 2 elements.");
		}

		std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		std::array<std::shared_ptr<TopologicCore::Topology>, 2> coreTopologies
		{
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topologies[0]->GetCoreTopologicalQuery()),
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topologies[1]->GetCoreTopologicalQuery()),
		};
		pCoreAperture->Close(coreTopologies);
		return this;
	}

	Aperture::Aperture(const std::shared_ptr<TopologicCore::Aperture>& kpCoreAperture)
		: m_pCoreAperture(new std::shared_ptr<TopologicCore::Aperture>(kpCoreAperture))
	{
		// Register the factory
		RegisterFactory(kpCoreAperture, gcnew ApertureContentFactory());
	}

	Aperture::~Aperture()
	{
		//delete m_pCoreAperture;
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Aperture::GetCoreTopologicalQuery()
	{
		assert(m_pCoreAperture != nullptr && "Context::GetCoreTopologicalQuery() returns null.");
		if (m_pCoreAperture == nullptr)
		{
			throw gcnew Exception("Context::GetCoreTopologicalQuery() returns null.");
		}

		return *m_pCoreAperture;
	}

	Object^ Aperture::Geometry::get()
	{
		return Topology()->Geometry;
	}
}