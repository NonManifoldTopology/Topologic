#include <msclr/marshal_cppstd.h>

#include <Topology.h>
#include <Cluster.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Shell.h>
#include <Face.h>
#include <Wire.h>
#include <Edge.h>
#include <Vertex.h>
#include <TopoLogicCore/include/Topology.h>

#include <TopoDS_Shape.hxx>


namespace TopoLogic
{
	int Topology::Dimensionality::get()
	{
		return GetCoreTopology()->Dimensionality();
	}

	bool Topology::Locked::get()
	{
		return GetCoreTopology()->Locked();
	}

	bool Topology::SaveToBRep(Topology^ topology, String^ path)
	{
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		return topology->GetCoreTopology()->SaveToBrep(cppPath);
	}

	bool Topology::LoadFromBRep(Topology^ topology, String^ path)
	{
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		return topology->GetCoreTopology()->LoadFromBrep(cppPath);
	}

	String^ Topology::Analyze(Topology^ topology)
	{
		return gcnew String(TopoLogicCore::Topology::Analyze(topology->GetCoreTopology()).c_str());
	}

	Topology^ Topology::ByCoreTopology(TopoLogicCore::Topology * const kpCoreTopology)
	{
		TopAbs_ShapeEnum occtShapeType = kpCoreTopology->GetOcctShape()->ShapeType();
		switch (occtShapeType)
		{
		case TopAbs_COMPOUND: return gcnew Cluster(TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(kpCoreTopology));
		case TopAbs_COMPSOLID: return gcnew CellComplex(TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(kpCoreTopology));
		case TopAbs_SOLID: return gcnew Cell(TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(kpCoreTopology));
		case TopAbs_SHELL: return gcnew Shell(TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(kpCoreTopology));
		case TopAbs_FACE: return gcnew Face(TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(kpCoreTopology));
		case TopAbs_WIRE: return gcnew Wire(TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(kpCoreTopology));
		case TopAbs_EDGE: return gcnew Edge(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(kpCoreTopology));
		case TopAbs_VERTEX: return gcnew Vertex(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(kpCoreTopology));
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

	Dictionary<String^, Object^>^ Topology::MemberOf(Topology ^ topology)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Dictionary<String^, Object^>^ Topology::Members(Topology ^ topology)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Dictionary<String^, Object^>^ Topology::Difference(List<Topology^> topologyArguments, List<Topology^> topologyTools)
	{
		std::list<TopoLogicCore::Topology*> pCoreArguments;
		for each(Topology^ pTopology in topologyArguments)
		{
			pCoreArguments.push_back(pTopology->GetCoreTopology());
		}

		std::list<TopoLogicCore::Topology*> pCoreTools;
		for each(Topology^ pTopology in topologyTools)
		{
			pCoreTools.push_back(pTopology->GetCoreTopology());
		}

		TopoLogicCore::Topology* pDifferenceCoreTopology = TopoLogicCore::Topology::Difference(pCoreArguments, pCoreTools);
		Topology^ pTopology = Topology::ByCoreTopology(pDifferenceCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Impose(List<Topology^> topologyArguments, List<Topology^> topologyTools)
	{
		std::list<TopoLogicCore::Topology*> pCoreArguments;
		for each(Topology^ pTopology in topologyArguments)
		{
			pCoreArguments.push_back(pTopology->GetCoreTopology());
		}

		std::list<TopoLogicCore::Topology*> pCoreTools;
		for each(Topology^ pTopology in topologyTools)
		{
			pCoreTools.push_back(pTopology->GetCoreTopology());
		}

		TopoLogicCore::Topology* pImposeCoreTopology = TopoLogicCore::Topology::Impose(pCoreArguments, pCoreTools);
		Topology^ pTopology = Topology::ByCoreTopology(pImposeCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Imprint(List<Topology^> topologyArguments, List<Topology^> topologyTools)
	{
		std::list<TopoLogicCore::Topology*> pCoreArguments;
		for each(Topology^ pTopology in topologyArguments)
		{
			pCoreArguments.push_back(pTopology->GetCoreTopology());
		}

		std::list<TopoLogicCore::Topology*> pCoreTools;
		for each(Topology^ pTopology in topologyTools)
		{
			pCoreTools.push_back(pTopology->GetCoreTopology());
		}

		TopoLogicCore::Topology* pImprintCoreTopology = TopoLogicCore::Topology::Imprint(pCoreArguments, pCoreTools);
		Topology^ pTopology = Topology::ByCoreTopology(pImprintCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Intersection(List<Topology^> topologyArguments, List<Topology^> topologyTools)
	{
		std::list<TopoLogicCore::Topology*> pCoreArguments;
		for each(Topology^ pTopology in topologyArguments)
		{
			pCoreArguments.push_back(pTopology->GetCoreTopology());
		}

		std::list<TopoLogicCore::Topology*> pCoreTools;
		for each(Topology^ pTopology in topologyTools)
		{
			pCoreTools.push_back(pTopology->GetCoreTopology());
		}

		TopoLogicCore::Topology* pIntersectionCoreTopology = TopoLogicCore::Topology::Intersection(pCoreArguments, pCoreTools);
		Topology^ pTopology = Topology::ByCoreTopology(pIntersectionCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Union(List<Topology^> topologyArguments, List<Topology^> topologyTools)
	{
		std::list<TopoLogicCore::Topology*> pCoreArguments;
		for each(Topology^ pTopology in topologyArguments)
		{
			pCoreArguments.push_back(pTopology->GetCoreTopology());
		}

		std::list<TopoLogicCore::Topology*> pCoreTools;
		for each(Topology^ pTopology in topologyTools)
		{
			pCoreTools.push_back(pTopology->GetCoreTopology());
		}

		TopoLogicCore::Topology* pUnionCoreTopology = TopoLogicCore::Topology::Union(pCoreArguments, pCoreTools);
		Topology^ pTopology = Topology::ByCoreTopology(pUnionCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Merge(List<Topology^>^ topologyList)
	{
		std::list<TopoLogicCore::Topology*> pCoreTopologyList;
		for each(Topology^ pTopology in topologyList)
		{
			pCoreTopologyList.push_back(pTopology->GetCoreTopology());
		}

		TopoLogicCore::Topology* pMergeCoreTopology = TopoLogicCore::Topology::Merge(pCoreTopologyList);
		Topology^ pTopology = Topology::ByCoreTopology(pMergeCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Slice(List<Topology^> topologyArguments, List<Topology^> topologyTools)
	{
		std::list<TopoLogicCore::Topology*> pCoreArguments;
		for each(Topology^ pTopology in topologyArguments)
		{
			pCoreArguments.push_back(pTopology->GetCoreTopology());
		}

		std::list<TopoLogicCore::Topology*> pCoreTools;
		for each(Topology^ pTopology in topologyTools)
		{
			pCoreTools.push_back(pTopology->GetCoreTopology());
		}

		TopoLogicCore::Topology* pSliceCoreTopology = TopoLogicCore::Topology::Slice(pCoreArguments, pCoreTools);
		Topology^ pTopology = Topology::ByCoreTopology(pSliceCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}
}