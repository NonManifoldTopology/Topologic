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

#include "Utilities.h"
#include "TopologicalQuery.h"

#include <memory>

namespace TopologicCore
{
	class Topology;

	/// <summary>
	/// A Context defines a topological relationship between two otherwise independent Topologies.
	/// </summary>
	class Context : public TopologicalQuery
	{
	public:
		typedef std::shared_ptr<Context> Ptr;

	public:
		Context(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const double kU, const double kV, const double kW);
		virtual ~Context();

		/// <summary>
		/// Creates a context by a topology and three parameters
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kU">The u parameter</param>
		/// <param name="kV">The v parameter</param>
		/// <param name="kW">The w parameter</param>
		/// <returns>A context</returns>
		static TOPOLOGIC_API std::shared_ptr<Context> ByTopologyParameters(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const double kU, const double kV, const double kW);

		/// <summary>
		/// 
		/// </summary>
		/// <returns>The associated topology</returns>
		TOPOLOGIC_API std::shared_ptr<Topology> Topology() const;

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The U parameter</returns>
		TOPOLOGIC_API double U() const { return m_u; }

		/// <summary>
		/// Returns the V parameter.
		/// </summary>
		/// <returns>The V parameter</returns>
		TOPOLOGIC_API double V() const { return m_v; }

		/// <summary>
		/// Returns the W parameter.
		/// </summary>
		/// <returns>The W parameter</returns>
		TOPOLOGIC_API double W() const { return m_w; }

	protected:
		TopoDS_Shape m_occtShape;
		double m_u;
		double m_v;
		double m_w;
	};
}