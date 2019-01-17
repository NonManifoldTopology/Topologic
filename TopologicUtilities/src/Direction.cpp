#include <Direction.h>

namespace TopologicUtilities
{
	Direction::Ptr Direction::ByCoordinates(const double kX, const double kY, const double kZ)
	{
		return std::make_shared<Direction>(new Geom_Direction(kX, kY, kZ));
	}

	Direction::Ptr Direction::ByVector(const Vector::Ptr& kpVector)
	{
		Vector::Ptr normalizedVector = Vector::ByNormalizedVector(kpVector);
		return std::make_shared<Direction>(new Geom_Direction(normalizedVector->X(), normalizedVector->Y(), normalizedVector->Z()));
	}
	Direction::~Direction()
	{
	}
	Direction::Direction(const Handle(Geom_Direction) kpOcctDirection)
		: m_pOcctDirection(kpOcctDirection)
	{
	}
}