// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <Aperture.h>
#include <Topology.h>
#include <Context.h>
#include <ApertureFactory.h>
#include <Face.h>
#include <Wire.h>
#include <TopologyFactoryManager.h>
#include <TopologyFactory.h>

#include <assert.h>
#include <array>

using namespace System;

namespace Topologic
{
	Aperture^ Aperture::ByTopologyContext(Topologic::Topology^ topology, Context^ context)
	{
		// 1. Copy topology
		TopologicCore::Topology::Ptr pCoreTopology = 
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		// 2. Copy context
		TopologicCore::Topology::Ptr pCoreContextTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(context->Topology->GetCoreTopologicalQuery());
		try {
			TopologicCore::Context::Ptr pCoreContext = TopologicCore::Context::ByTopologyParameters(
				pCoreContextTopology,
				context->U(), context->V(), context->W());

			// 3. Copy topology becomes the content of copy context's topology
			std::shared_ptr<TopologicCore::Aperture> pCoreAperture = TopologicCore::Aperture::ByTopologyContext(
				pCoreTopology,
				pCoreContext
			);

			return gcnew Aperture(pCoreAperture);
		}
		catch (const std::exception& rkException)
		{
			throw gcnew Exception(gcnew String(rkException.what()));
		}
	}

	Topologic::Topology^ Aperture::Topology::get()
	{
		TopologicCore::Aperture::Ptr pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		try {
			TopologicCore::Topology::Ptr pCoreTopology = pCoreAperture->Topology();
			return Factories::TopologyFactoryManager::GetDefaultFactory(pCoreTopology)->Create(pCoreTopology);
		}
		catch (const std::exception& rkException)
		{
			throw gcnew Exception(gcnew String(rkException.what()));
		}
	}

	Aperture::Aperture()
		: Topologic::Topology()
		, m_pCoreAperture(nullptr)
	{

	}

	Aperture::Aperture(const std::shared_ptr<TopologicCore::Aperture>& kpCoreAperture)
		: m_pCoreAperture(kpCoreAperture != nullptr ? new TopologicCore::Aperture::Ptr(kpCoreAperture) : throw gcnew Exception("A null aperture was created."))
	{

	}

	Aperture::~Aperture()
	{
		delete m_pCoreAperture;
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

	Object^ Aperture::BasicGeometry::get()
	{
		return Topology->BasicGeometry;
	}
}