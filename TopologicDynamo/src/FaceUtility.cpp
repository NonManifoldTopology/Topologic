#include "FaceUtility.h"
#include "Vertex.h"
#include "Wire.h"

namespace Topologic {
	namespace Support {
		double FaceUtility::Area(Face^ face)
		{
			TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery());
			return pCoreFace->Area();
		}

		Face ^ FaceUtility::ByVertices(Face^ face, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<Vertex^>^>^ vertices)
		{
			std::list<std::list<TopologicCore::Vertex::Ptr>> coreVertices;
			for each(System::Collections::Generic::IEnumerable<Vertex^>^ verticesList in vertices)
			{
				std::list<TopologicCore::Vertex::Ptr> coreVerticesList;
				for each(Vertex^ vertex in verticesList)
				{
					coreVerticesList.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery()));
				}
				coreVertices.push_back(coreVerticesList);
			}
			try {
				TopologicCore::Face::Ptr pCoreFace = TopologicCore::Face::ByVertices(coreVertices);
				return gcnew Face(pCoreFace);
			}
			catch (const std::exception& e)
			{
				throw gcnew Exception(gcnew String(e.what()));
			}
		}


		Autodesk::DesignScript::Geometry::UV^ FaceUtility::UVParameterAtVertex(Face^ face, Vertex^ vertex)
		{
			TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery());
			double u = 0.0, v = 0.0;
			pCoreFace->UVParameterAtPoint(
				TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery()),
				u, v
			);

			return Autodesk::DesignScript::Geometry::UV::ByCoordinates(u, v);
		}

		Vertex^ FaceUtility::VertexAtParameter(Face^ face, Autodesk::DesignScript::Geometry::UV^ uv)
		{
			TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery());
			try {
				TopologicCore::Vertex::Ptr pCoreVertex = pCoreFace->PointAtParameter(uv->U, uv->V);
				return safe_cast<Vertex^>(Topology::ByCoreTopology(pCoreVertex));
			}
			catch (std::exception& e)
			{
				throw gcnew Exception(gcnew String(e.what()));
			}
			return nullptr;
		}

		Autodesk::DesignScript::Geometry::Vector^ FaceUtility::NormalAtParameter(Face^ face, Autodesk::DesignScript::Geometry::UV^ uv)
		{
			TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery());
			gp_Dir normal = pCoreFace->NormalAtParameter(uv->U, uv->V);

			return Autodesk::DesignScript::Geometry::Vector::ByCoordinates(normal.X(), normal.Y(), normal.Z());
		}

		Face^ FaceUtility::TrimByWire(Face^ face, Wire^ wire)
		{
			TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery());
			TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(wire->GetCoreTopologicalQuery());

			TopologicCore::Face::Ptr pTrimmedFace = pCoreFace->Trim(pCoreWire);
			return safe_cast<Face^>(Topology::ByCoreTopology(pTrimmedFace));
		}

	}
}