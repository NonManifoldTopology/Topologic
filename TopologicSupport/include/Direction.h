#pragma once

#include <Geom_Direction.hxx>

#include <Utilities.h>

#include <memory>

namespace TopologicCore
{
	class Direction
	{
	public:
		typedef std::shared_ptr<Direction> Ptr;

	public:
		TOPOLOGIC_SUPPORT_API Direction(const Handle(Geom_Direction) kpOcctVector);
		~Direction();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kX"></param>
		/// <param name="kY"></param>
		/// <param name="kZ"></param>
		/// <returns></returns>
		Direction::Ptr ByCoordinates(const double kX, const double kY, const double kZ);

	protected:

		Handle(Geom_Direction) m_pOcctDirection;
	};
}