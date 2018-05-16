#pragma once

#include <Utilities.h>

#include <Geom_VectorWithMagnitude.hxx>

#include <memory>

namespace TopologicCore
{
	class Vector
	{
	public:
		typedef std::shared_ptr<Vector> Ptr;

	public:
		TOPOLOGIC_SUPPORT_API Vector(const Handle(Geom_VectorWithMagnitude) kpOcctVector);
		~Vector();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kX"></param>
		/// <param name="kY"></param>
		/// <param name="kZ"></param>
		/// <returns></returns>
		TOPOLOGIC_SUPPORT_API Vector::Ptr ByCoordinates(const double kX, const double kY, const double kZ);

	protected:
		Handle(Geom_VectorWithMagnitude) m_pOcctVector;
	};
}