#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Aperture.h>

using namespace System::Collections::Generic;

namespace TopoLogic
{
	ref class Context;

	/// <summary>
	/// <para>
	/// An Aperture defines a connection path between two Topologies that share a common Topology or between a
	/// Topology and the outside space. A connection path can be uni-directional or bi-directional.
	/// </para>
	/// </summary>
	public ref class Aperture : public Topology
	{
	public:
		/// <summary>
		/// Creates an aperture by a topology and a context.
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="context"></param>
		/// <returns></returns>
		static Aperture^ ByTopologyContext(Topology^ topology, Context^ context);

		/// <summary>
		/// Creates an aperture by a topology, a context, and an open status.
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="context"></param>
		/// <param name="openStatus"></param>
		/// <returns></returns>
		static Aperture^ ByTopologyContextStatus(Topology^ topology, Context^ context, bool openStatus);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsOpen();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologies"></param>
		/// <returns></returns>
		bool IsOpen(List<Topology^>^ topologies);

		/// <summary>
		/// 
		/// </summary>
		List<List<TopoLogic::Topology^>^>^ Paths();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Aperture^ Open();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologies"></param>
		/// <returns></returns>
		Aperture^ Open(List<Topology^>^ topologies);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Aperture^ Close();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologies"></param>
		/// <returns></returns>
		Aperture^ Close(List<Topology^>^ topologies);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	public protected:
		Aperture(TopoLogicCore::Aperture* const kpCoreAperture);

		virtual TopoLogicCore::TopologicalQuery* GetCoreTopologicalQuery() override;

	protected:
		virtual ~Aperture();

		TopoLogicCore::Aperture* m_pCoreAperture;
	};
}