#include <msclr/marshal_cppstd.h>

#include "Topology.h"

#include <Cluster.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Shell.h>
#include <Face.h>
#include <Wire.h>
#include <Edge.h>
#include <Vertex.h>
#include <Aperture.h>
#include <Attribute.h>
#include <Context.h>

#include <TopoDS_Shape.hxx>
#include <TDF_AttributeIterator.hxx>

namespace TopoLogic
{
	int Topology::Dimensionality::get()
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		return pCoreTopology->Dimensionality();
	}

	Topology^ Topology::ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Topology^ Topology::ByContext()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Topology^ Topology::ByVertexIndex(List<array<double, 3>^>^ vertexCoordinates, List<List<int>^>^ vertexIndices)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Topology^ Topology::ByVertexIndex(List<Vertex^>^ vertices, List<List<int>^>^ vertexIndices)
	{
		std::vector<std::shared_ptr<TopoLogicCore::Vertex>> coreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			std::shared_ptr<TopoLogicCore::Vertex> pCoreVertex = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Vertex>(pVertex->GetCoreTopologicalQuery());
			coreVertices.push_back(pCoreVertex);
		}

		std::list<std::list<int>> coreIndices;
		for each(List<int>^ vertex1DIndices in vertexIndices)
		{
			std::list<int> coreVertex1DIndices;
			for each(int vertexIndex in vertex1DIndices)
			{
				if (vertexIndex < 0)
				{
					throw gcnew Exception("The index list contains a negative index.");
				}
				coreVertex1DIndices.push_back(vertexIndex);
			}
			coreIndices.push_back(coreVertex1DIndices);
		}
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::Topology::ByVertexIndex(coreVertices, coreIndices);
		return Topology::ByCoreTopology(pCoreTopology);
	}

	/*void ExploreChild(const TDF_Label& rkOcctLabel)
	{
		List<Object^>^ pAttributeList = gcnew List<Object^>();
		for (TDF_ChildIterator occtChildIterator(rkOcctLabel); occtChildIterator.More(); occtChildIterator.Next())
		{
			const TDF_Label kOcctChildLabel = occtChildIterator.Value();
			for (TDF_AttributeIterator occtAttributeIterator(kOcctChildLabel); occtAttributeIterator.More(); occtAttributeIterator.Next())
			{
				String^ pString = gcnew String(typeid(*occtAttributeIterator.Value()).name());
				pAttributeList->Add(pString);
			}
			Handle(TNaming_NamedShape) occtShapeAttribute;
			bool result = kOcctChildLabel.FindAttribute(TNaming_NamedShape::GetID(), occtShapeAttribute);
			if(result)
			{
				TopoDS_Shape occtShape = occtShapeAttribute->Get();
				int a = 0;
			}
			int nbChildren = kOcctChildLabel.NbChildren();
			ExploreChild(kOcctChildLabel);
		}
	}*/
	List<Object^>^ Topology::Attributes::get()
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());

		// TODO: have a list of predefined attributes, and do this in TopologicCore.
		List<Object^>^ pAttributeList = gcnew List<Object^>();
		const TDF_Label& rkOcctLabel = pCoreTopology->GetOcctLabel();
		for (TDF_AttributeIterator occtAttributeIterator(rkOcctLabel); occtAttributeIterator.More(); occtAttributeIterator.Next())
		{
			String^ pString = gcnew String(typeid(*occtAttributeIterator.Value()).name());
			pAttributeList->Add(pString);
		}

		/*int nbChildren = rkOcctLabel.NbChildren();
		ExploreChild(rkOcctLabel);*/

		return pAttributeList;
	}

	bool Topology::SaveToBRep(String^ path)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		return pCoreTopology->SaveToBrep(cppPath);
	}

	Topology^ Topology::LoadFromBRep(String^ path)
	{
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::Topology::LoadFromBrep(cppPath);
		Topology^ pTopology = Topology::ByCoreTopology(pCoreTopology);
		return pTopology;
	}

	String^ Topology::Analyze()
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		return gcnew String(pCoreTopology->Analyze().c_str());
	}

	bool Topology::IsSame(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pOtherCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());
		return pCoreTopology->IsSame(pOtherCoreTopology);
	}

	Topology ^ Topology::ClosestLowestSubshapeTo(Topology^ queryTopology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreQueryTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(queryTopology->GetCoreTopologicalQuery());

		std::shared_ptr<TopoLogicCore::Topology> pClosestLowestSubshape = pCoreTopology->ClosestLowestSubshapeTo(pCoreQueryTopology);
		return ByCoreTopology(pClosestLowestSubshape);
	}

	Topology^ Topology::ByCoreTopology(const std::shared_ptr<TopoLogicCore::Topology>& kpCoreTopology)
	{
		switch (kpCoreTopology->GetType())
		{
		case TopoLogicCore::TOPOLOGY_CLUSTER: return gcnew Cluster(TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_CELLCOMPLEX: return gcnew CellComplex(TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_CELL: return gcnew Cell(TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_SHELL: return gcnew Shell(TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_FACE: return gcnew Face(TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_WIRE: return gcnew Wire(TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_EDGE: return gcnew Edge(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_VERTEX: return gcnew Vertex(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(kpCoreTopology));
		case TopoLogicCore::TOPOLOGY_APERTURE: return gcnew Aperture(TopoLogicCore::Topology::Downcast<TopoLogicCore::Aperture>(kpCoreTopology));
		default:
			throw std::exception("Topology::ByCoreTopology: unknown topology.");
		}
	}

	Topology::Topology()
	{

	}

	Topology::~Topology()
	{

	}

	List<Topology^>^ Topology::Contents()
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreContents;
		pCoreTopology->Contents(coreContents);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();

		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator rkCoreContentIterator = coreContents.cbegin();
			rkCoreContentIterator != coreContents.cend();
			rkCoreContentIterator++)
		{
			pTopologies->Add(Topology::ByCoreTopology(*rkCoreContentIterator));
		}

		return pTopologies;
	}

	List<Topology^>^ Topology::ContentsV2(bool allLevel)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Topology>> rkCoreContents;
		pCoreTopology->ContentsV2(allLevel, rkCoreContents);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();

		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator rkCoreContentIterator = rkCoreContents.cbegin();
			rkCoreContentIterator != rkCoreContents.cend();
			rkCoreContentIterator++)
		{
			pTopologies->Add(Topology::ByCoreTopology(*rkCoreContentIterator));
		}

		return pTopologies;
	}

	List<Context^>^ Topology::Contexts()
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		const std::list<std::shared_ptr<TopoLogicCore::Context>>& rkCoreContexts = pCoreTopology->Contexts();

		List<Context^>^ pContexts = gcnew List<Context^>();

		for (std::list<std::shared_ptr<TopoLogicCore::Context>>::const_iterator rkCoreContextIterator = rkCoreContexts.cbegin();
			rkCoreContextIterator != rkCoreContexts.cend();
			rkCoreContextIterator++)
		{
			pContexts->Add(gcnew Context(*rkCoreContextIterator));
		}

		return pContexts;
	}

	Topology^ Topology::AddContent(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->AddContent(TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery()));
		return this;
	}

	Topology^ Topology::RemoveContent(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->RemoveContent(TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery()));
		return this;
	}

	Topology^ Topology::AddContext(Context^ context)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->AddContext(TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(context->GetCoreTopologicalQuery()));
		return this;
	}

	Topology^ Topology::RemoveContext(Context^ context)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->RemoveContext(TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(context->GetCoreTopologicalQuery()));
		return this;
	}

	List<Topology^>^ Topology::SharedTopologies(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreSharedTopologies;
		pCoreTopology->SharedTopologies(TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery()), coreSharedTopologies);
		List<Topology^>^ pSharedTopologies = gcnew List<Topology^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreSharedTopologyIterator = coreSharedTopologies.begin();
			kCoreSharedTopologyIterator != coreSharedTopologies.end();
			kCoreSharedTopologyIterator++)
		{
			pSharedTopologies->Add(Topology::ByCoreTopology(*kCoreSharedTopologyIterator));
		}
		return pSharedTopologies;
	}

	List<List<Topology^>^>^ Topology::PathsTo(Topology^ topology, int maxLevel, int maxPaths)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::list<std::list<std::shared_ptr<TopoLogicCore::TopologicalQuery>>> corePaths;
		pCoreTopology->PathsTo(
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery()), 
			maxLevel,
			maxPaths,
			corePaths);

		throw gcnew NotImplementedException();
	}

	/*Dictionary<String^, Object^>^ Topology::BooleanImages(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		if (pCoreTopology == nullptr)
		{
			throw gcnew Exception("Not a topology");
		}
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreArgumentsInArguments;
		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreArgumentsInTools;
		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreToolsInArguments;
		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreToolsInTools;
		pCoreTopologyA->BooleanImages(pCoreTopologyB, coreArgumentsInArguments, coreArgumentsInTools, coreToolsInArguments, coreToolsInTools);

		List<Topology^>^ pArgumentsInArguments = gcnew List<Topology^>();
		List<Object^>^ pGeometryArgumentsInArguments = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreTopology = coreArgumentsInArguments.begin();
			kCoreTopology != coreArgumentsInArguments.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pArgumentsInArguments->Add(pTopology);
			pGeometryArgumentsInArguments->Add(pTopology->Geometry);
		}

		List<Topology^>^ pArgumentsInTools = gcnew List<Topology^>();
		List<Object^>^ pGeometryArgumentsInTools = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreTopology = coreArgumentsInTools.begin();
			kCoreTopology != coreArgumentsInTools.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pArgumentsInTools->Add(pTopology);
			pGeometryArgumentsInTools->Add(pTopology->Geometry);
		}

		List<Topology^>^ pToolsInArguments = gcnew List<Topology^>();
		List<Object^>^ pGeometryToolsInArguments = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreTopology = coreToolsInArguments.begin();
			kCoreTopology != coreToolsInArguments.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pToolsInArguments->Add(pTopology);
			pGeometryToolsInArguments->Add(pTopology->Geometry);
		}

		List<Topology^>^ pToolsInTools = gcnew List<Topology^>();
		List<Object^>^ pGeometryToolsInTools = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreTopology = coreToolsInTools.begin();
			kCoreTopology != coreToolsInTools.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pToolsInTools->Add(pTopology);
			pGeometryToolsInTools->Add(pTopology->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("A_A", pArgumentsInArguments);
		pDictionary->Add("A_B", pArgumentsInTools);
		pDictionary->Add("B_A", pToolsInArguments);
		pDictionary->Add("B_B", pToolsInTools);
		pDictionary->Add("Geometry A_A", pGeometryArgumentsInArguments);
		pDictionary->Add("Geometry A_B", pGeometryArgumentsInTools);
		pDictionary->Add("Geometry B_A", pGeometryToolsInArguments);
		pDictionary->Add("Geometry B_B", pGeometryToolsInTools);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::BooleanParts(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		if (pCoreTopology == nullptr)
		{
			throw gcnew Exception("Not a topology");
		}
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreParts_A_A_and_B_A;
		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreParts_B_A_and_A_B;
		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreParts_A_B_and_B_B;
		pCoreTopologyA->BooleanParts(pCoreTopologyB, coreParts_A_A_and_B_A, coreParts_B_A_and_A_B, coreParts_A_B_and_B_B);

		List<Topology^>^ pParts_A_A_and_B_A = gcnew List<Topology^>();
		List<Object^>^ pGeometryParts_A_A_and_B_A = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreTopology = coreParts_A_A_and_B_A.begin();
			kCoreTopology != coreParts_A_A_and_B_A.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pParts_A_A_and_B_A->Add(pTopology);
			pGeometryParts_A_A_and_B_A->Add(pTopology->Geometry);
		}

		List<Topology^>^ pParts_B_A_and_A_B = gcnew List<Topology^>();
		List<Object^>^ pGeometryParts_B_A_and_A_B = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreTopology = coreParts_B_A_and_A_B.begin();
			kCoreTopology != coreParts_B_A_and_A_B.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pParts_B_A_and_A_B->Add(pTopology);
			pGeometryParts_B_A_and_A_B->Add(pTopology->Geometry);
		}

		List<Topology^>^ pParts_A_B_and_B_B = gcnew List<Topology^>();
		List<Object^>^ pGeometryParts_A_B_and_B_B = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kCoreTopology = coreParts_A_B_and_B_B.begin();
			kCoreTopology != coreParts_A_B_and_B_B.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pParts_A_B_and_B_B->Add(pTopology);
			pGeometryParts_A_B_and_B_B->Add(pTopology->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Parts between A_A and B_A", pParts_A_A_and_B_A);
		pDictionary->Add("Parts between B_A and A_B", pParts_B_A_and_A_B);
		pDictionary->Add("Parts between A_B and B_B", pParts_A_B_and_B_B);
		pDictionary->Add("Parts geometry between A_A and B_A", pGeometryParts_A_A_and_B_A);
		pDictionary->Add("Parts geometry between B_A and A_B", pGeometryParts_B_A_and_A_B);
		pDictionary->Add("Parts geometry between A_B and B_B", pGeometryParts_A_B_and_B_B);
		return pDictionary;
	}*/

	Topology^ Topology::Difference(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopoLogicCore::Topology> pDifferenceCoreTopology = pCoreTopologyA->Difference(pCoreTopologyB);
			return Topology::ByCoreTopology(pDifferenceCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Impose(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopoLogicCore::Topology> pImposeCoreTopology = pCoreTopologyA->Impose(pCoreTopologyB); 
			return Topology::ByCoreTopology(pImposeCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Imprint(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopoLogicCore::Topology> pImprintCoreTopology = pCoreTopologyA->Imprint(pCoreTopologyB);
			return Topology::ByCoreTopology(pImprintCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Intersection(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try {
			std::shared_ptr<TopoLogicCore::Topology> pIntersectionCoreTopology = pCoreTopologyA->Intersection(pCoreTopologyB);
			return Topology::ByCoreTopology(pIntersectionCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}
	
	Topology^ Topology::Union(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopoLogicCore::Topology> pUnionCoreTopology = pCoreTopologyA->Union(pCoreTopologyB);
			return Topology::ByCoreTopology(pUnionCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Merge(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopoLogicCore::Topology> pMergeCoreTopology = pCoreTopologyA->Merge(pCoreTopologyB);
			return Topology::ByCoreTopology(pMergeCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Slice(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopoLogicCore::Topology> pSliceCoreTopology = pCoreTopologyA->Slice(pCoreTopologyB);
			return Topology::ByCoreTopology(pSliceCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::XOR(Topology^ topology)
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyA = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopologyB = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopoLogicCore::Topology> pSliceCoreTopology = pCoreTopologyA->XOR(pCoreTopologyB);
			return Topology::ByCoreTopology(pSliceCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	List<Topology^>^ Topology::ImmediateMembers()
	{
		std::shared_ptr<TopoLogicCore::Topology> pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopoLogicCore::Topology>> coreTopologies;
		pCoreTopology->ImmediateMembers(coreTopologies);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Topology>>::const_iterator kTopologyIterator = coreTopologies.begin();
			kTopologyIterator != coreTopologies.end();
			kTopologyIterator++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kTopologyIterator);
			pTopologies->Add(pTopology);
		}
		return pTopologies;
	}
}