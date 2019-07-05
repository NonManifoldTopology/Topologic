#include "NurbsSurface.h"
#include "Vertex.h"

namespace Topologic
{
	NurbsSurface::NurbsSurface(const std::shared_ptr<TopologicCore::NurbsSurface>& kpCoreNurbsSurface)
		: m_pCoreNurbsSurface(kpCoreNurbsSurface != nullptr ? new TopologicCore::NurbsSurface::Ptr(kpCoreNurbsSurface) : throw gcnew Exception("A null Nurbs Curve was created."))
	{

	}

	NurbsSurface::~NurbsSurface()
	{

	}

	int NurbsSurface::UDegree::get()
	{
		return (*m_pCoreNurbsSurface)->UDegree();
	}

	int NurbsSurface::VDegree::get()
	{
		return (*m_pCoreNurbsSurface)->VDegree();
	}

	bool NurbsSurface::IsURational::get()
	{
		return (*m_pCoreNurbsSurface)->IsURational();
	}

	bool NurbsSurface::IsVRational::get()
	{
		return (*m_pCoreNurbsSurface)->IsVRational();
	}

	int NurbsSurface::NumOfUControlVertices::get()
	{
		return (*m_pCoreNurbsSurface)->NumOfUControlVertices();
	}

	int NurbsSurface::NumOfVControlVertices::get()
	{
		return (*m_pCoreNurbsSurface)->NumOfVControlVertices();
	}

	Vertex^ NurbsSurface::ControlVertex(int u, int v)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = (*m_pCoreNurbsSurface)->ControlVertex(u, v);
		Vertex^ vertex = safe_cast<Vertex^>(Topology::ByCoreTopology(pCoreVertex));
		return vertex;
	}

	List<double>^ NurbsSurface::UKnots::get()
	{
		std::list<double> coreUKnots;
		(*m_pCoreNurbsSurface)->UKnots(coreUKnots);

		List<double>^ uKnots = gcnew List<double>();
		for (const double kUKnot : coreUKnots)
		{
			uKnots->Add(kUKnot);
		}
		return uKnots;
	}

	List<double>^ NurbsSurface::VKnots::get()
	{
		std::list<double> coreVKnots;
		(*m_pCoreNurbsSurface)->VKnots(coreVKnots);

		List<double>^ vKnots = gcnew List<double>();
		for (const double kVKnot : coreVKnots)
		{
			vKnots->Add(kVKnot);
		}
		return vKnots;
	}
	/*bool NurbsSurface::IsPeriodic::get()
	{
		return (*m_pCoreNurbsSurface)->IsPeriodic();
	}*/
}