#include <TransformationMatrix2D.h>

namespace TopologicUtility
{
	TransformationMatrix2D::~TransformationMatrix2D()
	{
	}
	TransformationMatrix2D::TransformationMatrix2D(const Handle(Geom2d_Transformation) kpOcctTransformationMatrix2D)
		: m_pOcctTransformationMatrix2D(kpOcctTransformationMatrix2D)
	{
	}
}