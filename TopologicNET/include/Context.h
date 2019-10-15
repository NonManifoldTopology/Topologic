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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <TopologicalQuery.h>
#include <Topology.h>

#include <TopologicCore/include/Context.h>

#include <memory>

namespace Topologic
{
	ref class Topology;

	/// <summary>
	/// A Context defines a topological relationship between two otherwise independent Topologies.
	/// </summary>

	public ref class Context : public TopologicalQuery
	{
	public:
		/*/// <summary>
		/// Creates a context by a Topology and three parameters.
		/// </summary>
		/// <param name="topology">A Topology</param>
		/// <param name="U">The U parameter</param>
		/// <param name="V">The V parameter</param>
		/// <param name="W">The W parameter</param>
		/// <returns>A context</returns>
		[IsVisibleInDynamoLibrary(false)]
		static Context^ ByTopologyParameters__(Topology^ topology, double U, double V, double W);*/

		/// <summary>
		/// Returns the associated Topology to the Context.
		/// </summary>
		/// <returns name="Topology">The associated Topology</returns>
		property Topology^ Topology
		{
			Topologic::Topology^ get();
		}

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The U parameter</returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		double U();

		/// <summary>
		/// Returns the V parameter.
		/// </summary>
		/// <returns>The V parameter</returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		double V();

		/// <summary>
		/// Returns the W parameter.
		/// </summary>
		/// <returns>The W parameter</returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		double W();

	public protected:
		Context();

		Context(const std::shared_ptr<TopologicCore::Context>& kpCoreContext);

		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Context();

		std::shared_ptr<TopologicCore::Context>* m_pCoreContext;
	};
}