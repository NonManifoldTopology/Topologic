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
		[IsVisibleInDynamoLibrary(false)]
		static Aperture^ ByTopologyContextStatus(Topology^ topology, Context^ context, bool openStatus);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="hostFace"></param>
		/// <param name="apertureBoundary"></param>
		/// <param name="link"></param>
		/// <param name="open"></param>
		/// <returns name="Aperture"></returns>
		[IsVisibleInDynamoLibrary(false)]
		static Aperture^ ByBoundaryWithinHost(Face^ hostFace, Wire^ apertureBoundary, bool link, bool open);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="contextTopology"></param>
		/// <returns name="Aperture"></returns>
		static Aperture^ ByTopologyContext(Topology^ topology, Topology^ contextTopology);

		/// <summary>
		/// Returns the underlying topology.
		/// </summary>
		/// <returns name="Topology">The underlying topology</returns>
		Topologic::Topology^ Topology();

		/*/// <summary>
		/// Checks if any aperture path is open.
		/// </summary>
		/// <returns name="bool">True if the aperture is open</returns>
		bool IsOpen();

		/// <summary>
		/// Checks if the aperture path from the first to the second topologies is open.
		/// </summary>
		/// <param name="topologies">A list of exactly two topologies</param>
		/// <returns name="bool">True if the aperture between the two paths is open</returns>
		bool IsOpen(List<Topologic::Topology^>^ topologies);

		/// <summary>
		/// Returns all open paths.
		/// </summary>
		/// <returns name="Topology[][]">The open paths</returns>
		List<List<Topologic::Topology^>^>^ Paths();

		/// <summary>
		/// Open the aperture paths connecting all adjacent topologies.
		/// </summary>
		/// <returns name="Aperture">An aperture</returns>
		Aperture^ Open();

		/// <summary>
		/// Open the aperture path connecting the input topologies.
		/// </summary>
		/// <param name="topologies">Topologies</param>
		/// <returns name="Aperture">An aperture</returns>
		Aperture^ Open(List<Topologic::Topology^>^ topologies);

		/// <summary>
		/// Close all aperture paths.
		/// </summary>
		/// <returns name="Aperture"></returns>
		Aperture^ Close();

		/// <summary>
		/// Close the aperture path between the input topologies.
		/// </summary>
		/// <param name="topologies">Topologies</param>
		/// <returns name="Aperture">An aperture</returns>
		Aperture^ Close(List<Topologic::Topology^>^ topologies);*/

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Geometry"></returns>
			virtual Object^ get() override;
		}

	public protected:
		Aperture(const std::shared_ptr<TopologicCore::Aperture>& kpCoreAperture);

		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Aperture();

		std::shared_ptr<TopologicCore::Aperture>* m_pCoreAperture;
	};
}