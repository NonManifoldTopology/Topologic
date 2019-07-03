#include "NurbsCurve.h"
#include "Vertex.h"

namespace Topologic
{
	NurbsCurve::NurbsCurve(const std::shared_ptr<TopologicCore::NurbsCurve>& kpCoreNurbsCurve)
		: m_pCoreNurbsCurve(kpCoreNurbsCurve != nullptr ? new TopologicCore::NurbsCurve::Ptr(kpCoreNurbsCurve) : throw gcnew Exception("A null Nurbs Curve was created."))
	{

	}

	NurbsCurve::~NurbsCurve()
	{

	}

	bool NurbsCurve::IsPeriodic::get()
	{
		return (*m_pCoreNurbsCurve)->IsPeriodic();
	}

	int NurbsCurve::Degree::get()
	{
		return (*m_pCoreNurbsCurve)->Degree();
	}

	List<Vertex^>^ NurbsCurve::ControlVertices::get()
	{
		std::list<TopologicCore::Vertex::Ptr> coreControlVertices;
		(*m_pCoreNurbsCurve)->ControlVertices(coreControlVertices);

		List<Vertex^>^ controlVertices = gcnew List<Vertex^>();
		for (const TopologicCore::Vertex::Ptr kpCoreControlVertex : coreControlVertices)
		{
			Vertex^ vertex = safe_cast<Vertex^>(Topology::ByCoreTopology(kpCoreControlVertex));
			controlVertices->Add(vertex);
		}
		return controlVertices;
	}
}