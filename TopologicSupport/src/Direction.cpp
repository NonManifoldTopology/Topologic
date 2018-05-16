#include <Direction.h>

namespace TopologicCore
{
	Direction::Ptr Direction::ByCoordinates(const double kX, const double kY, const double kZ)
	{
		return std::make_shared<Direction>(new Geom_Direction(kX, kY, kZ));
	}
	Direction::~Direction()
	{
	}
	Direction::Direction(const Handle(Geom_Direction) kpOcctDirection)
		: m_pOcctDirection(kpOcctDirection)
	{
	}
}