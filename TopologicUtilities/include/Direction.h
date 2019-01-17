#pragma once

#include "Utilities.h"
#include "Vector.h"

#include <Geom_Direction.hxx>

#include <memory>

namespace TopologicUtilities
{
	class Direction
	{
	public:
		typedef std::shared_ptr<Direction> Ptr;

	public:
		Direction(const Handle(Geom_Direction) kpOcctVector);
		~Direction();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kX"></param>
		/// <param name="kY"></param>
		/// <param name="kZ"></param>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API static Direction::Ptr ByCoordinates(const double kX, const double kY, const double kZ);

		TOPOLOGIC_UTILITIES_API static Direction::Ptr ByVector(const Vector::Ptr& kpVector);

	protected:

		Handle(Geom_Direction) m_pOcctDirection;
	};
}