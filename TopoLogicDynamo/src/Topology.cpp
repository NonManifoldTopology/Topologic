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
#include <DualGraph.h>
#include <Aperture.h>
#include <Context.h>
#include <TopologyFactoryManager.h>
#include <TopologyFactory.h>
#include <ApertureFactory.h>
#include <VertexFactory.h>
#include <EdgeFactory.h>
#include <WireFactory.h>
#include <FaceFactory.h>
#include <ShellFactory.h>
#include <CellFactory.h>
#include <CellComplexFactory.h>
#include <ClusterFactory.h>
#include <DualGraphFactory.h>
#include <ApertureFactory.h>
#include <AttributeManager.h>
#include <AttributeFactory.h>

#include <TopologicSupport/include/AttributeManager.h>
#include <TopologicSupport/include/IntAttribute.h>
#include <TopologicSupport/include/DoubleAttribute.h>
#include <TopologicSupport/include/StringAttribute.h>

namespace Topologic
{
	int Topology::Dimensionality::get()
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		return pCoreTopology->Dimensionality();
	}

	Topology^ Topology::ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry)
	{
		Autodesk::DesignScript::Geometry::Point^ dynamoPoint = dynamic_cast<Autodesk::DesignScript::Geometry::Point^>(geometry);
		if (dynamoPoint != nullptr)
		{
			return Vertex::ByPoint_(dynamoPoint);
		}

		// Do this first so that a polycurve (which is a curve) is not handled by Edge.
		Autodesk::DesignScript::Geometry::PolyCurve^ dynamoPolyCurve = dynamic_cast<Autodesk::DesignScript::Geometry::PolyCurve^>(geometry);
		if (dynamoPolyCurve != nullptr)
		{
			return Wire::ByPolyCurve_(dynamoPolyCurve);
		}

		Autodesk::DesignScript::Geometry::Curve^ dynamoCurve = dynamic_cast<Autodesk::DesignScript::Geometry::Curve^>(geometry);
		if (dynamoCurve != nullptr)
		{
			return Edge::ByCurve_(dynamoCurve);
		}

		// Do this first so that a polysurface (which is a surface) is not handled by Face.
		Autodesk::DesignScript::Geometry::PolySurface^ dynamoPolySurface = dynamic_cast<Autodesk::DesignScript::Geometry::PolySurface^>(geometry);
		if (dynamoPolySurface != nullptr)
		{
			return Shell::ByPolySurface_(dynamoPolySurface);
		}

		Autodesk::DesignScript::Geometry::Surface^ dynamoSurface = dynamic_cast<Autodesk::DesignScript::Geometry::Surface^>(geometry);
		if (dynamoSurface != nullptr)
		{
			return Face::BySurface_(dynamoSurface);
		}

		Autodesk::DesignScript::Geometry::Solid^ dynamoSolid = dynamic_cast<Autodesk::DesignScript::Geometry::Solid^>(geometry);
		if (dynamoSolid != nullptr)
		{
			return Cell::BySolid_(dynamoSolid);
		}

		throw gcnew NotImplementedException("This geometry is not currently handled.");
	}

	Topology ^ Topology::ByGeometryA(Autodesk::DesignScript::Geometry::DesignScriptEntity ^ geometry)
	{
		Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(geometry);
		if (dynamoGeometry != nullptr)
		{
			return ByGeometry(dynamoGeometry);
		}

		Autodesk::DesignScript::Geometry::Vertex^ dynamoVertex = dynamic_cast<Autodesk::DesignScript::Geometry::Vertex^>(geometry);
		if (dynamoVertex != nullptr)
		{
			return Vertex::ByPoint_(dynamoVertex->PointGeometry);
		}

		Autodesk::DesignScript::Geometry::Edge^ dynamoEdge = dynamic_cast<Autodesk::DesignScript::Geometry::Edge^>(geometry);
		if (dynamoEdge != nullptr)
		{
			return Edge::ByCurve_(dynamoEdge->CurveGeometry);
		}

		Autodesk::DesignScript::Geometry::Face^ dynamoFace = dynamic_cast<Autodesk::DesignScript::Geometry::Face^>(geometry);
		if (dynamoFace != nullptr)
		{
			return Face::BySurface_(dynamoFace->SurfaceGeometry());
		}

		throw gcnew NotImplementedException("This geometry is not currently handled.");
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

	List<Topology^>^ Topology::ByVerticesIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::List<int>^>^ vertexIndices)
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

	bool Topology::ExportToBRep(String^ path)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		return pCoreTopology->SaveToBrep(cppPath);
	}

	Topology^ Topology::ImportFromBRep(String^ path)
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

	Topology ^ Topology::ClosestSimplestSubshape(Topology^ selector)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreQueryTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(selector->GetCoreTopologicalQuery());

		std::shared_ptr<TopologicCore::Topology> pClosestLowestSubshape = pCoreTopology->ClosestSimplestSubshape(pCoreQueryTopology);
		return ByCoreTopology(pClosestLowestSubshape);
	}

	double Topology::Distance(Topology ^ topology)
	{
		TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreOtherTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		return pCoreTopology->Distance(pCoreOtherTopology);
	}

	String^ Topology::Type::get()
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::string strType = pCoreTopology->GetTypeAsString();
		return gcnew String(strType.c_str());
	}

	Topology^ Topology::ByCoreTopology(const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology)
	{
		String^ guid = gcnew String(kpCoreTopology->GetInstanceGUID().c_str());
		return TopologyFactoryManager::Instance->Find(guid)->Create(TopologicCore::TopologyPtr(kpCoreTopology));
	}

	generic <class T>
	T Topology::Copy()
	{
		TopologicCore::Topology::Ptr pCoreTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyTopology = pCoreTopology->Copy();

		TopologicSupport::AttributeManager::GetInstance().CopyAttributes(pCoreTopology->GetOcctShape(), pCoreCopyTopology->GetOcctShape());
		
		Topology^ topology = ByCoreTopology(pCoreCopyTopology);
		return safe_cast<T>(topology);
	}

	void Topology::RegisterFactory(const TopologicCore::Topology::Ptr & kpCoreTopology, TopologyFactory^ topologyFactory)
	{
		TopologyFactoryManager::Instance->Add(kpCoreTopology, topologyFactory);
	}

	void Topology::RegisterFactory(String^ guid, TopologyFactory^ topologyFactory)
	{
		TopologyFactoryManager::Instance->Add(guid, topologyFactory);
	}
	
	List<Topology^>^ Topology::HostTopology__::get()
	{
		throw gcnew NotImplementedException("Feature not yet implemented");
	}

	Topology::Topology()
	{
		// Register the factories
		static bool areFactoriesAdded = false;
		if(!areFactoriesAdded)
		{
			RegisterFactory(gcnew String(TopologicCore::VertexGUID::Get().c_str()), gcnew VertexFactory());
			RegisterFactory(gcnew String(TopologicCore::EdgeGUID::Get().c_str()), gcnew EdgeFactory());
			RegisterFactory(gcnew String(TopologicCore::WireGUID::Get().c_str()), gcnew WireFactory());
			RegisterFactory(gcnew String(TopologicCore::FaceGUID::Get().c_str()), gcnew FaceFactory());
			RegisterFactory(gcnew String(TopologicCore::ShellGUID::Get().c_str()), gcnew ShellFactory());
			RegisterFactory(gcnew String(TopologicCore::CellGUID::Get().c_str()), gcnew CellFactory());
			RegisterFactory(gcnew String(TopologicCore::CellComplexGUID::Get().c_str()), gcnew CellComplexFactory());
			RegisterFactory(gcnew String(TopologicCore::ClusterGUID::Get().c_str()), gcnew ClusterFactory());
			RegisterFactory(gcnew String(TopologicCore::DualGraphGUID::Get().c_str()), gcnew DualGraphFactory());
			RegisterFactory(gcnew String(TopologicCore::ApertureGUID::Get().c_str()), gcnew ApertureFactory());
			areFactoriesAdded = true;
		}
	}

	Topology::~Topology()
	{

	}

	Topology^ Topology::SetKeysValues(List<String^>^ keys, List<Object^>^ values)
	{
		if (keys->Count == 0)
		{
			throw gcnew Exception("An empty list of keys is given.");
		}

		if (values->Count == 0)
		{
			throw gcnew Exception("An empty list of values is given.");
		}

		if (values->Count != keys->Count)
		{
			throw gcnew Exception("The keys and values lists do not have the same length.");
		}

		Dictionary<String^, Object^>^ dictionary = gcnew Dictionary<String^, Object^>();
		for (int i = 0; i < keys->Count; i++)
		{
			dictionary->Add(keys[i], values[i]);
		}

		// 1. Copy the core topology
		TopologicCore::Topology::Ptr pCoreTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyTopology = pCoreTopology->Copy();
		Topology^ copyTopology = Topology::ByCoreTopology(pCoreCopyTopology);
		copyTopology->AddAttributesNoCopy(dictionary);

		return copyTopology;
	}


	Object ^ Topology::ValueAtKey(String ^ key)
	{
		TopologicCore::Topology::Ptr pCoreTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::string cppName = msclr::interop::marshal_as<std::string>(key);

		TopologicSupport::Attribute::Ptr pSupportAttribute = TopologicSupport::AttributeManager::GetInstance().Find(pCoreTopology->GetOcctShape(), cppName);
		if (pSupportAttribute == nullptr)
		{
			return nullptr;
		}

		AttributeFactory^ attributeFactory = AttributeManager::Instance->GetFactory(pSupportAttribute);
		return attributeFactory->CreateValue(pSupportAttribute);
	}

	Topology ^ Topology::AddAttributesNoCopy(Dictionary<String^, Object^>^ attributes)
	{
		TopologicCore::Topology::Ptr pCoreTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());

		for each(KeyValuePair<String^, Object^>^ entry in attributes)
		{
			System::Type^ entryValueType = entry->Value->GetType();
			AttributeManager::Instance->SetAttribute(this, entry->Key, entry->Value); 
		}
		return this;
	}

	Object ^ Topology::AttributeValue(String ^ name)
	{
		TopologicCore::Topology::Ptr pCoreTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		std::string cppName = msclr::interop::marshal_as<std::string>(name);

		TopologicSupport::Attribute::Ptr pSupportAttribute = TopologicSupport::AttributeManager::GetInstance().Find(pCoreTopology->GetOcctShape(), cppName);
		AttributeFactory^ attributeFactory = AttributeManager::Instance->GetFactory(pSupportAttribute);
		return attributeFactory->CreateValue(pSupportAttribute);
	}

	List<List<Object^>^>^ Topology::KeysValues::get()
	{
		TopologicCore::Topology::Ptr pCoreTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());

		std::map<std::string, std::shared_ptr<TopologicSupport::Attribute>> coreAttributes;
		bool isFound = TopologicSupport::AttributeManager::GetInstance().FindAll(pCoreTopology->GetOcctShape(), coreAttributes);
		if (!isFound)
		{
			return nullptr;
		}

		List<Object^>^ keys = gcnew List<Object^>();
		List<Object^>^ values = gcnew List<Object^>();
		for (const std::pair<std::string, TopologicSupport::Attribute::Ptr>& rkAttributePair : coreAttributes)
		{
			String^ key = gcnew String(rkAttributePair.first.c_str());
			keys->Add(key);

			AttributeFactory^ attributeFactory = AttributeManager::Instance->GetFactory(rkAttributePair.second);
			values->Add(attributeFactory->CreateValue(rkAttributePair.second));
		}

		List<List<Object^>^>^ keysValues = gcnew List<List<Object^>^>();
		keysValues->Add(keys);
		keysValues->Add(values);
		return keysValues;
	}

	Topology^ Topology::RemoveKeys(List<String^>^ keys)
	{
		Topology^ copyTopology = Copy<Topology^>();
		std::shared_ptr<TopologicCore::Topology> pCoreCopyTopology = 
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(copyTopology->GetCoreTopologicalQuery());
		for each(String^ key in keys)
		{
			std::string cppKey = msclr::interop::marshal_as<std::string>(key);
			TopologicSupport::AttributeManager::GetInstance().Remove(pCoreCopyTopology, cppKey);
		}

		return copyTopology;
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

	Topology^ Topology::AddContent(Topology^ topology)
	{
		// 1. Copy this topology
		TopologicCore::Topology::Ptr pCoreParentTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyParentTopology = pCoreParentTopology->Copy();
		
		// 2. Get the center of mass of the contentTopology
		// MOVED to Core
		/*TopologicCore::Vertex::Ptr pCoreContentCenterOfMass = pCoreContentTopology->CenterOfMass();*/

		// 3. Find the closest simplest topology of the copy topology
		// MOVED to Core
		//TopologicCore::Topology::Ptr closestSimplestSubshape = pCoreCopyParentTopology->ClosestSimplestSubshape(pCoreContentCenterOfMass);

		// 4. Copy the content topology
		TopologicCore::Topology::Ptr pCoreContentTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyContentTopology = pCoreContentTopology->Copy();

		pCoreCopyParentTopology->AddContent(pCoreCopyContentTopology, true);
		// 5. Add contentTopology as the content of the closest simplest topology
		//closestSimplestSubshape->AddContent(pCoreCopyContentTopology);

		// 6. Add closestSimplestSubshape as the context of pCoreCopyContentTopology
		const double kDefaultParameter = 0.0; // TODO: calculate the parameters
		pCoreCopyContentTopology->AddContext(
			TopologicCore::Context::ByTopologyParameters(
				pCoreCopyParentTopology,
				kDefaultParameter, kDefaultParameter, kDefaultParameter
			));

		// 7. Return the copy topology
		return Topology::ByCoreTopology(pCoreCopyParentTopology);
	}

	Topology^ Topology::RemoveContent(Topology^ topology)
	{
		/*std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->RemoveContent(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()));
		return this;*/

		throw gcnew NotImplementedException();
	}

	Topology^ Topology::AddAperture(Topology ^ apertureTopology)
	{
		// 1. Copy this topology
		TopologicCore::Topology::Ptr pCoreParentTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyParentTopology = pCoreParentTopology->Copy();

		// 2. Copy the aperture topology
		TopologicCore::Topology::Ptr pCoreApertureTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(apertureTopology->GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyApertureTopology = pCoreApertureTopology->Copy();

		// 3. Add the aperture
		TopologicCore::Aperture::Ptr pCoreAperture = TopologicCore::Aperture::ByTopologyContext(
			pCoreCopyApertureTopology, 
			pCoreCopyParentTopology);

		// 4. Return the copy parent topology
		return Topology::ByCoreTopology(pCoreCopyParentTopology);
	}

	Topology ^ Topology::AddApertures(System::Collections::Generic::IEnumerable<Topology^>^ apertureTopologies)
	{
		// 1. Copy this topology
		TopologicCore::Topology::Ptr pCoreParentTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyParentTopology = pCoreParentTopology->Copy();

		// 2. Copy the aperture topology
		for each(Topology^ apertureTopology in apertureTopologies)
		{
			TopologicCore::Topology::Ptr pCoreApertureTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(apertureTopology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreCopyApertureTopology = pCoreApertureTopology->Copy();
			TopologicCore::Aperture::Ptr pCoreAperture = TopologicCore::Aperture::ByTopologyContext(
				pCoreCopyApertureTopology,
				pCoreCopyParentTopology);
		}
		
		// 3. Add the aperture

		// 4. Return the copy parent topology
		return Topology::ByCoreTopology(pCoreCopyParentTopology);
	}

	Topology^ Topology::AddContext(Context^ context)
	{
		// 1. Copy this topology
		TopologicCore::Topology::Ptr pCoreInstanceTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyInstanceTopology = pCoreInstanceTopology->Copy();

		// 2. Get the center of mass of the instanceTopology
		//TopologicCore::Vertex::Ptr pCoreInstanceCenterOfMass = pCoreInstanceTopology->CenterOfMass();

		// 3. Copy the context topology
		TopologicCore::Topology::Ptr pCoreContextTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(context->Topology->GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyContextTopology = pCoreContextTopology->Copy();

		// 4. Find the closest simplest topology of the copy topology
		//TopologicCore::Topology::Ptr closestSimplestSubshape = pCoreCopyContextTopology->ClosestSimplestSubshape(pCoreInstanceCenterOfMass);

		// 5. Add closestSimplestSubshape as the contex of the copyInstanceTopology
		//TopologicCore::Context::Ptr pCoreContext = TopologicCore::Context::ByTopologyParameters(closestSimplestSubshape, context->U(), context->V(), context->W());
		TopologicCore::Context::Ptr pCoreContext = TopologicCore::Context::ByTopologyParameters(pCoreCopyContextTopology, context->U(), context->V(), context->W());
		pCoreCopyInstanceTopology->AddContext(pCoreContext);

		pCoreCopyContextTopology->AddContent(pCoreCopyInstanceTopology, true);

		// 7. Return the copy topology
		return Topology::ByCoreTopology(pCoreInstanceTopology);

		/*std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		pCoreTopology->AddContext(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(context->GetCoreTopologicalQuery()));
		return this;*/
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

	Topology^ Topology::Intersect(Topology^ topology)
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

	Topology^ Topology::Translate(double x, double y, double z)
	{
		// 1. Copy this topology
		TopologicCore::Topology::Ptr pCoreTopology =
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery());
		TopologicCore::Topology::Ptr pCoreCopyTopology = pCoreTopology->Copy();
		pCoreCopyTopology->Translate(x, y, z);

		return Topology::ByCoreTopology(pCoreCopyTopology);
	}

	List<Topology^>^ Topology::SubTopologies::get()
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