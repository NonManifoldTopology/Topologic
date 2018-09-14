#pragma once

#include <TopologicalQuery.h>
#include <Topology.h>

#include <TopologicCore/include/Context.h>

#include <memory>

namespace Topologic
{
	ref class Topology;

	/// <summary>
	/// A Context defines a Topological relationship between two otherwise independent Topologies.
	/// </summary>
	public ref class Context : public TopologicalQuery
	{
	public:
		/// <summary>
		/// Creates a context by a topology and three parameters
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="U">The u parameter</param>
		/// <param name="V">The v parameter</param>
		/// <param name="W">The w parameter</param>
		/// <returns>A context</returns>
		static Context^ ByTopologyParameters(Topology^ topology, double U, double V, double W);

		/// <summary>
		/// 
		/// </summary>
		/// <returns>The associated topology</returns>
		property Topology^ Topology
		{
			Topologic::Topology^ get();
		}

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The U parameter</returns>
		[IsVisibleInDynamoLibrary(false)]
		double U();

		/// <summary>
		/// Returns the V parameter.
		/// </summary>
		/// <returns>The V parameter</returns>
		[IsVisibleInDynamoLibrary(false)]
		double V();

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The W parameter</returns>
		[IsVisibleInDynamoLibrary(false)]
		double W();

	public protected:
		Context(const std::shared_ptr<TopologicCore::Context>& kpCoreContext);

		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Context();

		std::shared_ptr<TopologicCore::Context>* m_pCoreContext;
	};
}