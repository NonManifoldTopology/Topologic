#include <Vector.h>

namespace TopologicUtilities
{
	Vector::Ptr Vector::ByCoordinates(const double kX, const double kY, const double kZ)
	{
		return std::make_shared<Vector>(new Geom_VectorWithMagnitude(kX, kY, kZ));
	}

	Vector::Ptr Vector::ByReverseVector(const Vector::Ptr& kpAnotherVector)
	{
		Handle(Geom_Vector) pOcctReversedVector = kpAnotherVector->m_pOcctVector->Reversed();
		return std::make_shared<Vector>(Handle(Geom_VectorWithMagnitude)::DownCast(pOcctReversedVector));
	}

	Vector::Ptr Vector::ByScaledVector(const Vector::Ptr& kpAnotherVector, const double kScalingFactor)
	{
		return std::make_shared<Vector>(kpAnotherVector->m_pOcctVector->Multiplied(kScalingFactor));
	}

	Vector::Ptr Vector::ByNormalizedVector(const Vector::Ptr& kpAnotherVector)
	{
		Handle(Geom_VectorWithMagnitude) pOcctNormalizedVector = kpAnotherVector->m_pOcctVector->Normalized();
		return std::make_shared<Vector>(pOcctNormalizedVector);
	}

	double Vector::Magnitude() const
	{
		return m_pOcctVector->Magnitude();
	}

	double Vector::X() const
	{
		return m_pOcctVector->X();
	}

	double Vector::Y() const
	{
		return m_pOcctVector->Y();
	}

	double Vector::Z() const
	{
		return m_pOcctVector->Z();
	}

	Vector::~Vector()
	{
	}

	Vector::Vector(const Handle(Geom_VectorWithMagnitude) kpOcctVector)
		: m_pOcctVector(kpOcctVector)
	{
	}
}