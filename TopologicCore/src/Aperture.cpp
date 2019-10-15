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
#include <ApertureFactory.h>
#include <Context.h>
#include <Face.h>
#include <Vertex.h>

#include <assert.h>
#include <array>

namespace TopologicCore
{
	Aperture::Ptr Aperture::ByTopologyContext(const Topology::Ptr& kpTopology, const Context::Ptr& kpContext)
	{
		return std::make_shared<Aperture>(kpTopology, kpContext);
	}

	Aperture::Ptr Aperture::ByTopologyContext(const Topology::Ptr& kpTopology, const Topology::Ptr& kpContextTopology)
	{
		const double kDefaultParameter = 0.0;

		// Identify the closest simplest subshape
		Topology::Ptr pClosestSimplestSubshape = kpContextTopology->ClosestSimplestSubshape(kpTopology->CenterOfMass());

		// Create a Context from kpContextTopology
		Context::Ptr pContext = Context::ByTopologyParameters(pClosestSimplestSubshape, kDefaultParameter, kDefaultParameter, kDefaultParameter);

		// Create the Aperture
		Aperture::Ptr pAperture = ByTopologyContext(kpTopology, pContext);
		
		return pAperture;
	}

	Vertex::Ptr Aperture::CenterOfMass() const
	{
		return Topology()->CenterOfMass();
	}

	bool Aperture::IsManifold() const
	{
		return Topology()->IsManifold();
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

	Topology::Ptr Aperture::Topology() const
	{
		assert(m_pTopology != nullptr && "The underlying topology is null.");
		if (m_pTopology == nullptr)
		{
			throw std::exception("The underlying topology is null.");
		}
		return m_pTopology;
	}

	Aperture::Aperture(const Topology::Ptr& kpTopology, const std::shared_ptr<Context>& kpContext, const std::string& rkGuid)
		: TopologicCore::Topology(kpTopology->Dimensionality(), kpTopology->GetOcctShape(), rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_pMainContext(kpContext)
		, m_pTopology(kpTopology)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<ApertureFactory>());
		if (kpTopology == nullptr)
		{
			throw std::exception("A null topology is passed.");
		}
		
		if (kpContext != nullptr)
		{
			AddContext(kpContext);
		}
	}

	Aperture::~Aperture()
	{
		// Does not delete the contents; if the aperture disappears, its contents continue to exist.
	}
}