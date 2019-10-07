#pragma once

#include "Utilities.h"
#include "Surface.h"

#include <Geom_BSplineSurface.hxx>

#include <vector>
#include <list>

namespace TopologicCore
{
	class Vertex;
	class Face;

	class NurbsSurface : Surface
	{
	public:
		typedef std::shared_ptr<NurbsSurface> Ptr;

	public:
		TOPOLOGIC_API NurbsSurface(Handle(Geom_BSplineSurface) pOcctBSplineSurface, const std::shared_ptr<Face> kpFace);
		virtual ~NurbsSurface();

		TOPOLOGIC_API int UDegree() const;

		TOPOLOGIC_API int VDegree() const;

		TOPOLOGIC_API void ControlVertices(std::vector<std::shared_ptr<Vertex>>& rControlVertices, int& rUCount, int& rVCount) const;

		TOPOLOGIC_API bool IsURational() const;

		TOPOLOGIC_API bool IsVRational() const;

		TOPOLOGIC_API int NumOfUControlVertices() const;

		TOPOLOGIC_API int NumOfVControlVertices() const;

		TOPOLOGIC_API std::shared_ptr<Vertex> ControlVertex(const int u, const int v) const;

		TOPOLOGIC_API void UKnots(std::list<double>& rUKnots) const;

		TOPOLOGIC_API void VKnots(std::list<double>& rVKnots) const;

		/*TOPOLOGIC_API bool IsPeriodic() const;*/

	protected:
		Handle(Geom_BSplineSurface) m_pOcctBSplineSurface;
		std::shared_ptr<Face> m_pFace;	// This is needed to get the wires information
	};
}