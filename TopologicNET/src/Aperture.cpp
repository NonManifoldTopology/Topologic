// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

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
		//TopologicCore::Topology::Ptr pCoreCopyTopology = pCoreTopology->DeepCopy();

		// 2. Copy context
		TopologicCore::Topology::Ptr pCoreContextTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(context->Topology->GetCoreTopologicalQuery());
		//TopologicCore::Topology::Ptr pCoreCopyContextTopology = pCoreContextTopology->DeepCopy();
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

	Topologic::Topology^ Aperture::Topology::get()
	{
		TopologicCore::Aperture::Ptr pCoreAperture = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Aperture>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreTopology = pCoreAperture->Topology();
		return Factories::TopologyFactoryManager::GetDefaultFactory(pCoreTopology)->Create(pCoreTopology);
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