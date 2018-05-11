#pragma once

#include "Utilities.h"
#include "TopologicalQuery.h"

#include <memory>

namespace TopologicCore
{
	class Topology;

	/// <summary>
	/// A Context defines a Topological relationship between two otherwise independent Topologies.
	/// </summary>
	class Context : public TopologicalQuery
	{
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
		static Topologic_API std::shared_ptr<Context> ByTopologyParameters(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const double kU, const double kV, const double kW);

		/// <summary>
		/// 
		/// </summary>
		/// <returns>The associated topology</returns>
		Topologic_API std::shared_ptr<Topology> Topology() const { return m_pTopology; }

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The U parameter</returns>
		Topologic_API double U() const { return m_u; }

		/// <summary>
		/// Returns the V parameter.
		/// </summary>
		/// <returns>The V parameter</returns>
		Topologic_API double V() const { return m_v; }

		/// <summary>
		/// Returns the U parameter.
		/// </summary>
		/// <returns>The W parameter</returns>
		Topologic_API double W() const { return m_w; }

	protected:
		std::shared_ptr<TopologicCore::Topology> m_pTopology;
		double m_u;
		double m_v;
		double m_w;
	};
}