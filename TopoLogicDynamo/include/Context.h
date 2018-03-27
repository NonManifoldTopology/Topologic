#pragma once

#include <TopologicalQuery.h>
#include <Topology.h>

#include <TopoLogicCore/include/Context.h>

#include <memory>

namespace TopoLogic
{
	ref class Topology;

	/// <summary>
	/// A Context defines a topological relationship between two otherwise independent Topologies.
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
		Topology^ Topology();

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The U parameter</returns>
		double U();

		/// <summary>
		/// Returns the V parameter.
		/// </summary>
		/// <returns>The V parameter</returns>
		double V();

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The W parameter</returns>
		double W();

	public protected:
		Context(const std::shared_ptr<TopoLogicCore::Context>& kpCoreContext);

		virtual std::shared_ptr<TopoLogicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Context();

		std::shared_ptr<TopoLogicCore::Context>* m_pCoreContext;
	};
}