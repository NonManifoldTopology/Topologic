#include "ShellUtility.h"
#include "Face.h"
#include "Wire.h"
#include "Edge.h"
#include "Vertex.h"

#include <TopologicUtility/include/ShellUtility.h>

namespace Topologic {
	namespace Utility {

		Shell^ ShellUtility::ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires)
		{
			std::list<TopologicCore::Wire::Ptr> coreWires;
			for each(Wire^ pWire in wires)
			{
				coreWires.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(pWire->GetCoreTopologicalQuery()));
			}

			try {
				TopologicCore::Shell::Ptr pCoreShell = TopologicUtility::ShellUtility::ByLoft(coreWires);
				return gcnew Shell(pCoreShell);
			}
			catch (std::exception&)
			{
				throw gcnew Exception("Loft error");
			}
		}

		Shell^ ShellUtility::ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices)
		{
			std::vector<TopologicCore::Vertex::Ptr> coreVertices;
			for each(Vertex^ pVertex in vertices)
			{
				coreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
			}

			std::list<std::list<int>> coreFaceIndices;
			for each(List<int>^ pFaceIndex in faceIndices)
			{
				std::list<int> coreFaceIndex;
				for each(int vertexIndex in pFaceIndex)
				{
					coreFaceIndex.push_back(vertexIndex);
				}
				coreFaceIndices.push_back(coreFaceIndex);
			}

			return gcnew Shell(TopologicUtility::ShellUtility::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
		}

		List<Face^>^ ShellUtility::ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, int numUPanels, int numVPanels, double tolerance, bool capBottom, bool capTop)
		{
			std::list<TopologicCore::Vertex::Ptr> coreVertices;
			std::list<TopologicCore::Edge::Ptr> coreIsocurves;
			std::list<TopologicCore::Wire::Ptr> coreWires;
			std::list<TopologicCore::Face::Ptr> coreFaces;
			TopologicCore::Shell::Ptr pCoreTopology = TopologicUtility::ShellUtility::ByFacePlanarization(
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery()),
				iteration,
				numEdgeSamples,
				numUPanels,
				numVPanels,
				tolerance,
				capBottom,
				capTop,
				coreVertices,
				coreIsocurves,
				coreWires,
				coreFaces
			);
			//return gcnew Shell(pCoreTopology);

			List<Edge^>^ isoedges = gcnew List<Edge^>();
			for (const TopologicCore::Edge::Ptr& rkCoreEdge : coreIsocurves)
			{
				isoedges->Add(safe_cast<Edge^>(Topology::ByCoreTopology(rkCoreEdge)));
			}
			List<Face^>^ faces = gcnew List<Face^>();
			for (const TopologicCore::Face::Ptr& rkCoreFace : coreFaces)
			{
				faces->Add(safe_cast<Face^>(Topology::ByCoreTopology(rkCoreFace)));
			}
			return faces;
		}

		//Dictionary<String^, Object^>^ ShellUtility::ByFacePlanarization_(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop)
		Shell^ ShellUtility::ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop)
		{
			std::list<TopologicCore::Vertex::Ptr> coreVertices;
			std::list<TopologicCore::Edge::Ptr> coreIsocurves;
			std::list<TopologicCore::Wire::Ptr> coreWires;
			std::list<TopologicCore::Face::Ptr> coreFaces;
			std::list<double> coreUValues, coreVValues;
			for each(double u in uValues)
			{
				coreUValues.push_back(u);
			}

			for each(double v in vValues)
			{
				coreVValues.push_back(v);
			}

			TopologicCore::Shell::Ptr pCoreTopology = TopologicUtility::ShellUtility::ByFacePlanarizationV2(
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery()),
				iteration,
				numEdgeSamples,
				coreUValues,
				coreVValues//,
				//tolerance,
				/*capBottom,
				capTop,*/
				/*coreVertices,
				coreIsocurves,
				coreWires,
				coreFaces*/
			);
			Shell^ pShell = gcnew Shell(pCoreTopology);
			//return gcnew Shell(pCoreTopology);

			List<Vertex^>^ vertices = gcnew List<Vertex^>();
			for (const TopologicCore::Vertex::Ptr& rkCoreVertex : coreVertices)
			{
				vertices->Add(safe_cast<Vertex^>(Topology::ByCoreTopology(rkCoreVertex)));
			}
			List<Edge^>^ edges = gcnew List<Edge^>();
			for (const TopologicCore::Edge::Ptr& rkCoreEdge : coreIsocurves)
			{
				edges->Add(safe_cast<Edge^>(Topology::ByCoreTopology(rkCoreEdge)));
			}
			List<Wire^>^ wires = gcnew List<Wire^>();
			for (const TopologicCore::Wire::Ptr& rkCoreWire : coreWires)
			{
				wires->Add(safe_cast<Wire^>(Topology::ByCoreTopology(rkCoreWire)));
			}
			List<Face^>^ faces = gcnew List<Face^>();
			for (const TopologicCore::Face::Ptr& rkCoreFace : coreFaces)
			{
				faces->Add(safe_cast<Face^>(Topology::ByCoreTopology(rkCoreFace)));
			}
			Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
			pDictionary->Add("Vertices", vertices);
			pDictionary->Add("Edges", edges);
			pDictionary->Add("Wires", wires);
			pDictionary->Add("Faces", faces);
			pDictionary->Add("Shell", pShell);
			//return pDictionary;
			return pShell;
		}
	}
}