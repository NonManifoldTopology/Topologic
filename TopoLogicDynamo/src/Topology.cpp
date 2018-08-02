#include <msclr/marshal_cppstd.h>
#ifdef max
#undef max
#endif
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
#include <TopologyFactoryDictionary.h>
#include <TopologyFactory.h>

#include "TopologicCore/include/InstanceGUIDManager.h"

#include <TopoDS_Shape.hxx>
//#include <TDF_AttributeIterator.hxx>

namespace Topologic
{
	int Topology::Dimensionality::get()
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
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

	//Topology^ Topology::ByVertexIndex(List<array<double, 3>^>^ vertexCoordinates, List<List<int>^>^ vertexIndices)
	//{
	//	throw gcnew System::NotImplementedException();
	//	// TODO: insert return statement here
	//}

	List<Topology^>^ Topology::ByVertexIndex(IEnumerable<Vertex^>^ vertices, IEnumerable<IEnumerable<int>^>^ vertexIndices)
	{
		std::vector<TopologicCore::Vertex::Ptr> coreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(pVertex->GetCoreTopologicalQuery());
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
		std::list<TopologicCore::Topology::Ptr> pCoreTopologies;
		TopologicCore::Topology::ByVertexIndex(coreVertices, coreIndices, pCoreTopologies);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();
		for (const TopologicCore::Topology::Ptr& kpCoreTopology : pCoreTopologies)
		{
			pTopologies->Add(Topology::ByCoreTopology(kpCoreTopology));
		}
		return pTopologies;
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
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());

		// TODO: have a list of predefined attributes, and do this in TopologicCore.
		List<Object^>^ pAttributeList = gcnew List<Object^>();
		/*const TDF_Label& rkOcctLabel = pCoreTopology->GetOcctLabel();
		for (TDF_AttributeIterator occtAttributeIterator(rkOcctLabel); occtAttributeIterator.More(); occtAttributeIterator.Next())
		{
			String^ pString = gcnew String(typeid(*occtAttributeIterator.Value()).name());
			pAttributeList->Add(pString);
		}*/

		/*int nbChildren = rkOcctLabel.NbChildren();
		ExploreChild(rkOcctLabel);*/

		return pAttributeList;
	}

	bool Topology::SaveToBRep(String^ path)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		return pCoreTopology->SaveToBrep(cppPath);
	}

	Topology^ Topology::LoadFromBRep(String^ path)
	{
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::Topology::LoadFromBrep(cppPath);
		Topology^ pTopology = Topology::ByCoreTopology(pCoreTopology);
		return pTopology;
	}

	String^ Topology::Analyze()
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		return gcnew String(pCoreTopology->Analyze().c_str());
	}

	bool Topology::IsSame(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pOtherCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
		return pCoreTopology->IsSame(pOtherCoreTopology);
	}

	Topology ^ Topology::ClosestSimplestSubshape(Topology^ queryTopology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreQueryTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(queryTopology->GetCoreTopologicalQuery());

		std::shared_ptr<TopologicCore::Topology> pClosestLowestSubshape = pCoreTopology->ClosestSimplestSubshape(pCoreQueryTopology);
		return ByCoreTopology(pClosestLowestSubshape);
	}

	double Topology::Distance(Topology ^ topology)
	{
		TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreOtherTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		return pCoreTopology->Distance(pCoreOtherTopology);
	}

	String^ Topology::Type()
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::string strType = pCoreTopology->GetTypeAsString();
		return gcnew String(strType.c_str());
	}

	Topology^ Topology::ByCoreTopology(const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology)
	{
		String^ guid = gcnew String(kpCoreTopology->GetInstanceGUID().c_str());
		return TopologyFactoryDictionary::Instance->Find(guid)->Create(TopologicCore::TopologyPtr(kpCoreTopology));
	}

	void Topology::RegisterFactory(const TopologicCore::Topology::Ptr & kpCoreTopology, TopologyFactory^ topologyFactory)
	{
		TopologyFactoryDictionary::Instance->Add(kpCoreTopology, topologyFactory);
	}

	void Topology::RegisterFactory(String^ guid, TopologyFactory ^ topologyFactory)
	{
		TopologyFactoryDictionary::Instance->Add(guid, topologyFactory);
	}

	Topology::Topology()
	{

	}

	Topology::~Topology()
	{

	}

	List<Topology^>^ Topology::Contents(bool allLevels)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Topology>> coreContents;
		pCoreTopology->Contents(allLevels, coreContents);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();

		for (const TopologicCore::Topology::Ptr& kpCoreContent : coreContents)
		{
			Topology^ topology = Topology::ByCoreTopology(kpCoreContent);
			pTopologies->Add(topology);
		}
		return pTopologies;
	}

	List<Context^>^ Topology::Contexts()
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Context>> rkCoreContexts;
		pCoreTopology->Contexts(rkCoreContexts);

		List<Context^>^ pContexts = gcnew List<Context^>();

		for (std::list<std::shared_ptr<TopologicCore::Context>>::const_iterator rkCoreContextIterator = rkCoreContexts.cbegin();
			rkCoreContextIterator != rkCoreContexts.cend();
			rkCoreContextIterator++)
		{
			pContexts->Add(gcnew Context(*rkCoreContextIterator));
		}

		return pContexts;
	}

	Topology^ Topology::AddContent(Topology^ contentTopology)
		//Dictionary<String^, Object^>^ AddContent(Topology^ thisTopology, Topology^ contentTopology, Topology^ parentTopology)
	{
		// 1. Copy this topology
		TopologicCore::Topology::Ptr pCoreParentTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyParentTopology = pCoreParentTopology->Copy();
		
		// 2. Get the center of mass of the contentTopology
		TopologicCore::Topology::Ptr pCoreContentTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(contentTopology->GetCoreTopologicalQuery());
		TopologicCore::Vertex::Ptr pCoreContentCenterOfMass = pCoreContentTopology->CenterOfMass();

		// 3. Find the closest simplest topology of the copy topology
		TopologicCore::Topology::Ptr closestSimplestSubshape = pCoreCopyParentTopology->ClosestSimplestSubshape(pCoreContentCenterOfMass);

		// 4. Copy the content topology
		TopologicCore::Topology::Ptr pCoreCopyContentTopology = pCoreContentTopology->Copy();

		// 5. Add contentTopology as the content of the closest simplest topology
		closestSimplestSubshape->AddContent(pCoreCopyContentTopology);

		// 6. Return the copy topology
		return Topology::ByCoreTopology(pCoreCopyParentTopology);
	}

	Topology^ Topology::RemoveContent(Topology^ topology)
	{
		/*std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->RemoveContent(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()));
		return this;*/

		throw gcnew NotImplementedException();
	}

	Topology^ Topology::AddContext(Context^ context)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->AddContext(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(context->GetCoreTopologicalQuery()));
		return this;
	}

	Topology^ Topology::RemoveContext(Context^ context)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->RemoveContext(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(context->GetCoreTopologicalQuery()));
		return this;
	}

	List<Topology^>^ Topology::SharedTopologies(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Topology>> coreSharedTopologies;
		pCoreTopology->SharedTopologies(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()), coreSharedTopologies);
		List<Topology^>^ pSharedTopologies = gcnew List<Topology^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreSharedTopologyIterator = coreSharedTopologies.begin();
			kCoreSharedTopologyIterator != coreSharedTopologies.end();
			kCoreSharedTopologyIterator++)
		{
			pSharedTopologies->Add(Topology::ByCoreTopology(*kCoreSharedTopologyIterator));
		}
		return pSharedTopologies;
	}

	List<List<Topology^>^>^ Topology::PathsTo(Topology^ topology, Topology^ parentTopology, int maxLevel, int maxPaths)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreParentTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());
		std::list<std::list<std::shared_ptr<TopologicCore::TopologicalQuery>>> corePaths;
		pCoreTopology->PathsTo(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()),
			pCoreParentTopology,
			maxLevel,
			maxPaths,
			corePaths);

		throw gcnew NotImplementedException();
	}

	/*Dictionary<String^, Object^>^ Topology::BooleanImages(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		if (pCoreTopology == nullptr)
		{
			throw gcnew Exception("Not a topology");
		}
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Topology>> coreArgumentsInArguments;
		std::list<std::shared_ptr<TopologicCore::Topology>> coreArgumentsInTools;
		std::list<std::shared_ptr<TopologicCore::Topology>> coreToolsInArguments;
		std::list<std::shared_ptr<TopologicCore::Topology>> coreToolsInTools;
		pCoreTopologyA->BooleanImages(pCoreTopologyB, coreArgumentsInArguments, coreArgumentsInTools, coreToolsInArguments, coreToolsInTools);

		List<Topology^>^ pArgumentsInArguments = gcnew List<Topology^>();
		List<Object^>^ pGeometryArgumentsInArguments = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopology = coreArgumentsInArguments.begin();
			kCoreTopology != coreArgumentsInArguments.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pArgumentsInArguments->Add(pTopology);
			pGeometryArgumentsInArguments->Add(pTopology->Geometry);
		}

		List<Topology^>^ pArgumentsInTools = gcnew List<Topology^>();
		List<Object^>^ pGeometryArgumentsInTools = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopology = coreArgumentsInTools.begin();
			kCoreTopology != coreArgumentsInTools.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pArgumentsInTools->Add(pTopology);
			pGeometryArgumentsInTools->Add(pTopology->Geometry);
		}

		List<Topology^>^ pToolsInArguments = gcnew List<Topology^>();
		List<Object^>^ pGeometryToolsInArguments = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopology = coreToolsInArguments.begin();
			kCoreTopology != coreToolsInArguments.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pToolsInArguments->Add(pTopology);
			pGeometryToolsInArguments->Add(pTopology->Geometry);
		}

		List<Topology^>^ pToolsInTools = gcnew List<Topology^>();
		List<Object^>^ pGeometryToolsInTools = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopology = coreToolsInTools.begin();
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
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		if (pCoreTopology == nullptr)
		{
			throw gcnew Exception("Not a topology");
		}
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Topology>> coreParts_A_A_and_B_A;
		std::list<std::shared_ptr<TopologicCore::Topology>> coreParts_B_A_and_A_B;
		std::list<std::shared_ptr<TopologicCore::Topology>> coreParts_A_B_and_B_B;
		pCoreTopologyA->BooleanParts(pCoreTopologyB, coreParts_A_A_and_B_A, coreParts_B_A_and_A_B, coreParts_A_B_and_B_B);

		List<Topology^>^ pParts_A_A_and_B_A = gcnew List<Topology^>();
		List<Object^>^ pGeometryParts_A_A_and_B_A = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopology = coreParts_A_A_and_B_A.begin();
			kCoreTopology != coreParts_A_A_and_B_A.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pParts_A_A_and_B_A->Add(pTopology);
			pGeometryParts_A_A_and_B_A->Add(pTopology->Geometry);
		}

		List<Topology^>^ pParts_B_A_and_A_B = gcnew List<Topology^>();
		List<Object^>^ pGeometryParts_B_A_and_A_B = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopology = coreParts_B_A_and_A_B.begin();
			kCoreTopology != coreParts_B_A_and_A_B.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pParts_B_A_and_A_B->Add(pTopology);
			pGeometryParts_B_A_and_A_B->Add(pTopology->Geometry);
		}

		List<Topology^>^ pParts_A_B_and_B_B = gcnew List<Topology^>();
		List<Object^>^ pGeometryParts_A_B_and_B_B = gcnew List<Object^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreTopology = coreParts_A_B_and_B_B.begin();
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
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopologicCore::Topology> pDifferenceCoreTopology = pCoreTopologyA->Difference(pCoreTopologyB);
			return Topology::ByCoreTopology(pDifferenceCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Impose(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopologicCore::Topology> pImposeCoreTopology = pCoreTopologyA->Impose(pCoreTopologyB); 
			return Topology::ByCoreTopology(pImposeCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Imprint(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopologicCore::Topology> pImprintCoreTopology = pCoreTopologyA->Imprint(pCoreTopologyB);
			return Topology::ByCoreTopology(pImprintCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Intersection(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try {
			std::shared_ptr<TopologicCore::Topology> pIntersectionCoreTopology = pCoreTopologyA->Intersection(pCoreTopologyB);
			return Topology::ByCoreTopology(pIntersectionCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}
	
	Topology^ Topology::Union(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopologicCore::Topology> pUnionCoreTopology = pCoreTopologyA->Union(pCoreTopologyB);
			return Topology::ByCoreTopology(pUnionCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::SelfMerge()
	{
		TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());

		TopologicCore::Topology::Ptr pMergeCoreTopology = pCoreTopology->Merge();
		return Topology::ByCoreTopology(pMergeCoreTopology);
	}

	Topology^ Topology::Merge(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopologicCore::Topology> pMergeCoreTopology = pCoreTopologyA->Merge(pCoreTopologyB);
			return Topology::ByCoreTopology(pMergeCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::Slice(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopologicCore::Topology> pSliceCoreTopology = pCoreTopologyA->Slice(pCoreTopologyB);
			return Topology::ByCoreTopology(pSliceCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Topology^ Topology::XOR(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyA = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreTopologyB = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		try{
			std::shared_ptr<TopologicCore::Topology> pSliceCoreTopology = pCoreTopologyA->XOR(pCoreTopologyB);
			return Topology::ByCoreTopology(pSliceCoreTopology);
		}
		catch (std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	List<Topology^>^ Topology::ImmediateMembers()
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Topology>> coreTopologies;
		pCoreTopology->ImmediateMembers(coreTopologies);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kTopologyIterator = coreTopologies.begin();
			kTopologyIterator != coreTopologies.end();
			kTopologyIterator++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kTopologyIterator);
			pTopologies->Add(pTopology);
		}
		return pTopologies;
	}

	Vertex^ Topology::CenterOfMass()
	{
		TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreTopology->CenterOfMass();
		return gcnew Vertex(pCoreCenterOfMass);
	}
}