#pragma once

#include "Utilities.h"

#include <Geom_VectorWithMagnitude.hxx>

#include <memory>

namespace TopologicUtilities
{
	class Vector
	{
	public:
		typedef std::shared_ptr<Vector> Ptr;

	public:
		Vector(const Handle(Geom_VectorWithMagnitude) kpOcctVector);
		~Vector();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kX"></param>
		/// <param name="kY"></param>
		/// <param name="kZ"></param>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API static Vector::Ptr ByCoordinates(const double kX, const double kY, const double kZ);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherVector"></param>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API static Vector::Ptr ByReverseVector(const Vector::Ptr& kpAnotherVector);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherVector"></param>
		/// <param name="kScalingFactor"></param>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API static Vector::Ptr ByScaledVector(const Vector::Ptr& kpAnotherVector, const double kScalingFactor);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherVector"></param>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API static Vector::Ptr ByNormalizedVector(const Vector::Ptr& kpAnotherVector);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API double Magnitude() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API double X() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API double Y() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_UTILITIES_API double Z() const;

	protected:
		Handle(Geom_VectorWithMagnitude) m_pOcctVector;
	};
}