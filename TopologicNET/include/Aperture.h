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

#pragma once

#include <Topology.h>

#include <TopologicCore/include/Aperture.h>

using namespace System::Collections::Generic;

namespace Topologic
{
	ref class Face;
	ref class Wire;
	ref class Context;

	/// <summary>
	/// An Aperture is any designed opening in a building including but not limited to any door, gate, window, skylight, or hatch.
	/// </summary>
	public ref class Aperture : public Topology
	{
	public:
		/// <summary>
		/// Creates an Aperture by a Topology and a Context.
		/// </summary>
		/// <param name="topology">A Topology</param>
		/// <param name="context">A Context</param>
		/// <returns name="Aperture">An Aperture</returns>
		static Aperture^ ByTopologyContext(Topology^ topology, Context^ context);

		/// <summary>
		/// Returns the underlying Topology.
		/// </summary>
		/// <returns name="Topology">The underlying Topology</returns>
		property Topologic::Topology^ Topology
		{
			Topologic::Topology^ get();
		}

		/// <summary>
		/// Creates a geometry from Aperture.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ BasicGeometry
		{
			virtual Object^ get() override;
		}

	public protected:
		Aperture();
		Aperture(const std::shared_ptr<TopologicCore::Aperture>& kpCoreAperture);

		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Aperture();

		std::shared_ptr<TopologicCore::Aperture>* m_pCoreAperture;
	};
}