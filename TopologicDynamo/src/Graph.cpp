#include <Graph.h>
#include <CellComplex.h>
#include <Vertex.h>

#include <assert.h>

namespace Topologic
{
	namespace Extension {
		Graph^ Graph::ByCellComplex(
			CellComplex^ cellComplex,
			bool useCells,
			bool useNonManifoldFaces,
			bool useManifoldFaces,
			bool useApertures)
		{
			TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(cellComplex->GetCoreTopologicalQuery());
			TopologicExtension::Graph::Ptr pCoreGraph = TopologicExtension::Graph::ByCellComplex(
				pCoreCellComplex,
				useCells,
				useNonManifoldFaces,
				useManifoldFaces,
				useApertures);

			return gcnew Graph(pCoreGraph);
		}

		Object^ Graph::Geometry::get()
		{
			List<Object^>^ GraphGeometry = gcnew List<Object^>();
			GraphGeometry->Add(Wire::Geometry);
			List<Vertex^>^ vertices = Vertices();
			for each (Vertex^ vertex in vertices)
			{
				GraphGeometry->Add(Autodesk::DesignScript::Geometry::Sphere::ByCenterPointRadius(vertex->Point(), 0.2));
			}

			return GraphGeometry;
		}

		std::shared_ptr<TopologicCore::TopologicalQuery> Graph::GetCoreTopologicalQuery()
		{
			return Wire::GetCoreTopologicalQuery();
		}

		Graph::Graph(const TopologicCore::Wire::Ptr& kpCoreWire)
			: Wire(kpCoreWire)
		{

		}

		Graph::~Graph()
		{
			//delete m_pCoreWire;
		}
	}
}