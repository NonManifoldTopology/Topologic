#include <DualGraph.h>
#include <CellComplex.h>
#include <Vertex.h>

#include <assert.h>

namespace Topologic
{
	DualGraph_^ DualGraph_::ByCellComplex(
		CellComplex^ cellComplex,
		bool useCells,
		bool useNonManifoldFaces,
		bool useManifoldFaces,
		bool useApertures)
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(cellComplex->GetCoreTopologicalQuery());
		TopologicCore::DualGraph::Ptr pCoreDualGraph = TopologicCore::DualGraph::ByCellComplex(
			pCoreCellComplex, 
			useCells,
			useNonManifoldFaces,
			useManifoldFaces,
			useApertures);

		return gcnew DualGraph_(pCoreDualGraph);
	}

	Object^ DualGraph_::Geometry::get()
	{
		List<Object^>^ dualGraphGeometry = gcnew List<Object^>(); 
		dualGraphGeometry->Add(Wire::Geometry);
		List<Vertex^>^ vertices = Vertices_(false);
		for each (Vertex^ vertex in vertices)
		{
			dualGraphGeometry->Add(Autodesk::DesignScript::Geometry::Sphere::ByCenterPointRadius(vertex->Point(), 0.2));
		}

		return dualGraphGeometry;
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> DualGraph_::GetCoreTopologicalQuery()
	{
		return Wire::GetCoreTopologicalQuery();
	}

	DualGraph_::DualGraph_(const TopologicCore::Wire::Ptr& kpCoreWire)
		: Wire(kpCoreWire)
	{

	}

	DualGraph_::~DualGraph_()
	{
		//delete m_pCoreWire;
	}
}