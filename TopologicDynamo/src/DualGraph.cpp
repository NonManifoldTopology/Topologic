#include <DualGraph.h>
#include <CellComplex.h>
#include <Vertex.h>

#include <assert.h>

namespace Topologic
{
	DualGraph^ DualGraph::ByCellComplex(
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

		return gcnew DualGraph(pCoreDualGraph);
	}

	Object^ DualGraph::Geometry::get()
	{
		List<Object^>^ dualGraphGeometry = gcnew List<Object^>(); 
		dualGraphGeometry->Add(Wire::Geometry);
		List<Vertex^>^ vertices = Vertices(false);
		for each (Vertex^ vertex in vertices)
		{
			dualGraphGeometry->Add(Autodesk::DesignScript::Geometry::Sphere::ByCenterPointRadius(vertex->Point(), 0.2));
		}

		return dualGraphGeometry;
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> DualGraph::GetCoreTopologicalQuery()
	{
		return Wire::GetCoreTopologicalQuery();
	}

	DualGraph::DualGraph(const TopologicCore::Wire::Ptr& kpCoreWire)
		: Wire(kpCoreWire)
	{

	}

	DualGraph::DualGraph(System::Collections::Generic::IEnumerable<Edge^>^ pEdges)
		: Wire(pEdges)
	{

	}

	DualGraph::DualGraph(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve)
		: Wire(pDynamoPolycurve)
	{
	}

	DualGraph::~DualGraph()
	{
		//delete m_pCoreWire;
	}
}