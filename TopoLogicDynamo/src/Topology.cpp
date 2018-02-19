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

	Topology^ Topology::SetLocked(bool value)
	{
		GetCoreTopology()->Locked(value);
		return this;
	}

	bool Topology::SaveToBRep(String^ path)
	{
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		return GetCoreTopology()->SaveToBrep(cppPath);
	}

	bool Topology::LoadFromBRep(String^ path)
	{
		std::string cppPath = msclr::interop::marshal_as<std::string>(path);
		return GetCoreTopology()->LoadFromBrep(cppPath);
	}

	String^ Topology::Analyze()
	{
		return gcnew String(TopoLogicCore::Topology::Analyze(GetCoreTopology()).c_str());
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

	Dictionary<String^, Object^>^ Topology::MemberOf()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Dictionary<String^, Object^>^ Topology::Members()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Dictionary<String^, Object^>^ Topology::BooleanImages(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		std::list<TopoLogicCore::Topology*> coreArgumentsInArguments;
		std::list<TopoLogicCore::Topology*> coreArgumentsInTools;
		std::list<TopoLogicCore::Topology*> coreToolsInArguments;
		std::list<TopoLogicCore::Topology*> coreToolsInTools;
		pCoreTopologyA->BooleanImages(pCoreTopologyB, coreArgumentsInArguments, coreArgumentsInTools, coreToolsInArguments, coreToolsInTools);

		List<Topology^>^ pArgumentsInArguments = gcnew List<Topology^>();
		List<Object^>^ pGeometryArgumentsInArguments = gcnew List<Object^>();
		for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreTopology = coreArgumentsInArguments.begin();
			kCoreTopology != coreArgumentsInArguments.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pArgumentsInArguments->Add(pTopology);
			pGeometryArgumentsInArguments->Add(pTopology->Geometry);
		}

		List<Topology^>^ pArgumentsInTools = gcnew List<Topology^>();
		List<Object^>^ pGeometryArgumentsInTools = gcnew List<Object^>();
		for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreTopology = coreArgumentsInTools.begin();
			kCoreTopology != coreArgumentsInTools.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pArgumentsInTools->Add(pTopology);
			pGeometryArgumentsInTools->Add(pTopology->Geometry);
		}

		List<Topology^>^ pToolsInArguments = gcnew List<Topology^>();
		List<Object^>^ pGeometryToolsInArguments = gcnew List<Object^>();
		for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreTopology = coreToolsInArguments.begin();
			kCoreTopology != coreToolsInArguments.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pToolsInArguments->Add(pTopology);
			pGeometryToolsInArguments->Add(pTopology->Geometry);
		}

		List<Topology^>^ pToolsInTools = gcnew List<Topology^>();
		List<Object^>^ pGeometryToolsInTools = gcnew List<Object^>();
		for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreTopology = coreToolsInTools.begin();
			kCoreTopology != coreToolsInTools.end();
			kCoreTopology++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kCoreTopology);
			pToolsInTools->Add(pTopology);
			pGeometryToolsInTools->Add(pTopology->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("ArgumentImagesInArguments", pArgumentsInArguments);
		pDictionary->Add("ArgumentImagesInTools", pArgumentsInTools);
		pDictionary->Add("ToolImagesInArguments", pToolsInArguments);
		pDictionary->Add("ToolImagesInTools", pToolsInTools);
		pDictionary->Add("GeometryArgumentImagesInArguments", pGeometryArgumentsInArguments);
		pDictionary->Add("GeometryArgumentImagesInTools", pGeometryArgumentsInTools);
		pDictionary->Add("GeometryToolImagesInArguments", pGeometryToolsInArguments);
		pDictionary->Add("GeometryToolImagesInTools", pGeometryToolsInTools);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Difference(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pDifferenceCoreTopology = pCoreTopologyA->Difference(pCoreTopologyB);
		Topology^ pTopology = Topology::ByCoreTopology(pDifferenceCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Impose(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pImposeCoreTopology = pCoreTopologyA->Impose(pCoreTopologyB); 
		Topology^ pTopology = Topology::ByCoreTopology(pImposeCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Imprint(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pImprintCoreTopology = pCoreTopologyA->Imprint(pCoreTopologyB);
		Topology^ pTopology = Topology::ByCoreTopology(pImprintCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Intersection(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pIntersectionCoreTopology = pCoreTopologyA->Intersection(pCoreTopologyB);
		Topology^ pTopology = Topology::ByCoreTopology(pIntersectionCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}
	
	Dictionary<String^, Object^>^ Topology::Union(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pUnionCoreTopology = pCoreTopologyA->Union(pCoreTopologyB);
		Topology^ pTopology = Topology::ByCoreTopology(pUnionCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Merge(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pMergeCoreTopology = pCoreTopologyA->Merge(pCoreTopologyB);
		Topology^ pTopology = Topology::ByCoreTopology(pMergeCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::Slice(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pSliceCoreTopology = pCoreTopologyA->Slice(pCoreTopologyB);
		Topology^ pTopology = Topology::ByCoreTopology(pSliceCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Topology::XOR(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopologyA = GetCoreTopology();
		TopoLogicCore::Topology* pCoreTopologyB = topology->GetCoreTopology();

		TopoLogicCore::Topology* pSliceCoreTopology = pCoreTopologyA->XOR(pCoreTopologyB);
		Topology^ pTopology = Topology::ByCoreTopology(pSliceCoreTopology);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Topology", pTopology);
		pDictionary->Add("Geometry", pTopology->Geometry);
		return pDictionary;
	}
}