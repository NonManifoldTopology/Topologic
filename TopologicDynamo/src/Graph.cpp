#include <Graph.h>
#include <CellComplex.h>
#include <Vertex.h>

#include <assert.h>

namespace Topologic
{
	namespace Extensions {
		DualGraph_^ DualGraph_::ByCellComplex(
			CellComplex^ cellComplex,
			bool useCells,
			bool useNonManifoldFaces,
			bool useManifoldFaces,
			bool useApertures)
		{
			TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(cellComplex->GetCoreTopologicalQuery());
			try {
				TopologicExtensions::DualGraph_::Ptr pCoreGraph = TopologicExtensions::DualGraph_::ByCellComplex(
					pCoreCellComplex,
					useCells,
					useNonManifoldFaces,
					useManifoldFaces,
					useApertures);

				return gcnew DualGraph_(pCoreGraph);
			}
			catch (std::exception& e)
			{
				throw gcnew Exception(gcnew String(e.what()));
			}
		}

		/*Object^ Graph::Geometry_::get()
		{
			List<Object^>^ graphGeometry = gcnew List<Object^>();
			graphGeometry->Add(Cluster::Geometry_);

			return graphGeometry;
		}*/

		std::shared_ptr<TopologicCore::TopologicalQuery> DualGraph_::GetCoreTopologicalQuery()
		{
			return Cluster::GetCoreTopologicalQuery();
		}

		DualGraph_::DualGraph_(const TopologicCore::Cluster::Ptr& kpCoreCluster)
			: Cluster(kpCoreCluster)
		{

		}

		DualGraph_::~DualGraph_()
		{
			//delete m_pCoreWire;
		}
	}
}