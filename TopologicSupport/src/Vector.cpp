#include <Vector.h>

namespace TopologicCore
{
	Vector::Ptr Vector::ByCoordinates(const double kX, const double kY, const double kZ)
	{
		return std::make_shared<Vector>(new Geom_VectorWithMagnitude(kX, kY, kZ));
	}

	Vector::~Vector()
	{
	}

	Vector::Vector(const Handle(Geom_VectorWithMagnitude) kpOcctVector)
		: m_pOcctVector(kpOcctVector)
	{
	}
}