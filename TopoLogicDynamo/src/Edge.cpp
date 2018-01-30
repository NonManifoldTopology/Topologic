#include <Edge.h>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Edge::StartVertex(Edge ^ topoLogicEdge)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Edge::EndVertex(Edge ^ topoLogicEdge)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Edge::Wires(Edge ^ topoLogicEdge)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Edge::ByCurve(Autodesk::DesignScript::Geometry::Curve ^ curve)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Edge::ByVertices(List<Vertex^>^ vertices)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Edge::SharedVertex(Edge ^ topoLogicEdge1, Edge ^ topoLogicEdge2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Object^ Edge::Geometry::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Edge::Edge(TopoLogicCore::Edge * const kpCoreEdge)
		: Topology()
		, m_pCoreEdge(kpCoreEdge)
	{

	}
	Edge::Edge()
	{
		throw gcnew System::NotImplementedException();
	}
	Edge::~Edge()
	{
		throw gcnew System::NotImplementedException();
	}
}