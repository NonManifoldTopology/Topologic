#include <Shell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>
#include <Cell.h>

#include <assert.h>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Shell::Cell(Shell^ topoLogicShell)
	{
		TopoLogicCore::Shell* pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(topoLogicShell->GetCoreTopology());
		TopoLogic::Cell^ pCell = gcnew TopoLogic::Cell(pCoreShell->Cell());

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cell", pCell);
		pDictionary->Add("Solid", pCell->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Shell::Faces(Shell^ topoLogicShell)
	{
		List<Face^>^ pFaces = topoLogicShell->Faces();
		List<System::Object^>^ pDynamoEntities = gcnew List<System::Object^>();
		for each(Face^ pFace in pFaces)
		{
			pDynamoEntities->Add(pFace->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Faces", pFaces);
		pDictionary->Add("Surfaces", pDynamoEntities);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Shell::Edges(Shell ^ topoLogicShell)
	{
		TopoLogicCore::Shell* pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(topoLogicShell->GetCoreTopology());
		std::list<TopoLogicCore::Edge*> coreEdges;
		pCoreShell->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		List<System::Object^>^ pDynamoCurves = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Edge*>::const_iterator kCoreEdgeIterator = coreEdges.begin();
			kCoreEdgeIterator != coreEdges.end();
			kCoreEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kCoreEdgeIterator);
			pEdges->Add(pEdge);
			pDynamoCurves->Add(pEdge->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Edges", pEdges);
		pDictionary->Add("Curves", pDynamoCurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Shell::Vertices(Shell^ topoLogicShell)
	{
		TopoLogicCore::Shell* pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(topoLogicShell->GetCoreTopology());
		std::list<TopoLogicCore::Vertex*> coreVertices;
		pCoreShell->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		List<System::Object^>^ pDynamoPoints = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Vertex*>::const_iterator kCoreVertexIterator = coreVertices.begin();
			kCoreVertexIterator != coreVertices.end();
			kCoreVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kCoreVertexIterator);
			pVertices->Add(pVertex);
			pDynamoPoints->Add(pVertex->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertices", pVertices);
		pDictionary->Add("Points", pDynamoPoints);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Shell::ByFaces(List<Face^>^ faces)
	{
		std::list<TopoLogicCore::Face*> coreFaces;
		for each(Face^ pFace in faces)
		{
			coreFaces.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(pFace->GetCoreTopology()));
		}

		Shell^ pShell = gcnew Shell(TopoLogicCore::Shell::ByFaces(coreFaces));
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Shell", pShell);
		pDictionary->Add("PolySurface", pShell->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Shell::ByPolysurface(Autodesk::DesignScript::Geometry::PolySurface^ polysurface)
	{
		List<Face^>^ pFaces = gcnew List<Face^>();
		for each(Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface in polysurface->Surfaces())
		{
			pFaces->Add(gcnew Face(pDynamoSurface));
		}
		return ByFaces(pFaces);
	}

	Dictionary<String^, Object^>^ Shell::ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices)
	{
		std::vector<TopoLogicCore::Vertex*> coreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			coreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(pVertex->GetCoreTopology()));
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
		
		Shell^ pShell = gcnew Shell(TopoLogicCore::Shell::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Shell", pShell);
		pDictionary->Add("PolySurface", pShell->Geometry);
		return pDictionary;
	}

	bool Shell::IsClosed::get()
	{
		TopoLogicCore::Shell* pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopology());
		return pCoreShell->IsClosed();
	}

	Object^ Shell::Geometry::get()
	{
		List<Autodesk::DesignScript::Geometry::Surface^>^ pDynamoSurfaces = gcnew List<Autodesk::DesignScript::Geometry::Surface^>();
		List<Face^>^ pFaces = Faces();
		for each(Face^ pFace in pFaces)
		{
			pDynamoSurfaces->Add(pFace->Surface());
		}

		try {
			return Autodesk::DesignScript::Geometry::PolySurface::ByJoinedSurfaces(pDynamoSurfaces);
		}
		catch (ApplicationException^)
		{
			return pDynamoSurfaces;
		}
	}

	Shell::Shell(TopoLogicCore::Shell* const kpCoreShell)
		: Topology()
		, m_pCoreShell(kpCoreShell)
	{

	}

	TopoLogicCore::Topology* Shell::GetCoreTopology()
	{
		assert(m_pCoreShell != nullptr && "Shell::m_pCoreShell is null.");
		if (m_pCoreShell == nullptr)
		{
			throw gcnew Exception("Shell::m_pCoreShell is null.");
		}

		return m_pCoreShell;
	}

	List<Face^>^ Shell::Faces()
	{
		TopoLogicCore::Shell* pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopology());
		std::list<TopoLogicCore::Face*> coreFaces;
		pCoreShell->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<TopoLogicCore::Face*>::const_iterator kCoreFaceIterator = coreFaces.begin();
			kCoreFaceIterator != coreFaces.end();
			kCoreFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kCoreFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	Shell::~Shell()
	{
		delete m_pCoreShell;
	}
}