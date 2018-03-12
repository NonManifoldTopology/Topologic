#pragma once

#include <TopologicalQuery.h>

#include <TopoLogicCore/include/Aperture.h>

using namespace System::Collections::Generic;

namespace TopoLogic
{
	ref class Topology;
	ref class Context;

	/// <summary>
	/// <para>
	/// An aperture controls connection paths between two topologies that share a common topology or between a
	/// topology and the outside space.It can be uni - directional or bi - directional.For each path direction, 
	/// it may allow a varying normalised degree of weights(i.e.between 0 and 1). 
	/// A closed aperture has a path weight of 0 for both paths.A fully open aperture has a weight of 1.
	/// </para>
	/// </summary>
	public ref class Aperture : public TopologicalQuery
	{
		/// <summary>
		/// Creates an aperture by a topology and a context.
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpContext"></param>
		/// <returns></returns>
		static Aperture^ ByTopologyContext(Topology^ topology, Context^ context);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		double FirstPathWeight();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		double SecondPathWeight();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Topology^ Topology();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsOpen();

		/// <summary>
		/// 
		/// </summary>
		List<TopoLogic::Topology^>^ FirstPath();

		/// <summary>
		/// 
		/// </summary>
		List<TopoLogic::Topology^>^ SecondPath();

	public protected:
		Aperture(TopoLogicCore::Aperture* const kpCoreAperture);

		virtual TopoLogicCore::TopologicalQuery* GetCoreTopologicalQuery() override;

	protected:
		virtual ~Aperture();

		TopoLogicCore::Aperture* m_pCoreAperture;
	};
}