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
		/// <param name="topology">A topology</param>
		/// <param name="context">A context</param>
		/// <returns name="Aperture">An aperture</returns>
		static Aperture^ ByTopologyContext(Topology^ topology, Context^ context);

		/// <summary>
		/// Creates an aperture by a topology, a context, and an open status.
		/// </summary>
		/// <param name="topology">A topology</param>
		/// <param name="context">A context</param>
		/// <param name="openStatus">An open status</param>
		/// <returns name="Aperture">An aperture</returns>
		static Aperture^ ByTopologyContextStatus(Topology^ topology, Context^ context, bool openStatus);

		/// <summary>
		/// Checks if any aperture path is open.
		/// </summary>
		/// <returns>bool</returns>
		bool IsOpen();

		/// <summary>
		/// Checks if the aperture path from the first to the second topologies is open.
		/// </summary>
		/// <param name="Topology[]">A list of exactly two topologies</param>
		/// <returns>bool</returns>
		bool IsOpen(List<Topology^>^ topologies);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Topology[][]"></returns>
		List<List<TopoLogic::Topology^>^>^ Paths();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Aperture"></returns>
		Aperture^ Open();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologies"></param>
		/// <returns name="Aperture"></returns>
		Aperture^ Open(List<Topology^>^ topologies);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Aperture"></returns>
		Aperture^ Close();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologies"></param>
		/// <returns name="Aperture"></returns>
		Aperture^ Close(List<Topology^>^ topologies);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Geometry"></returns>
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