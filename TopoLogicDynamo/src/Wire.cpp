#include <Wire.h>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Wire::Edges(Wire ^ topoLogicWire)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Wire::Faces(Wire ^ topoLogicWire)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Wire::IsClosed()
	{
		return false;
	}
	Dictionary<String^, Object^>^ Wire::Vertices(Wire ^ topoLogicWire)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Wire::ByEdges(List<Edge^>^ edges)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Wire::ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve ^ polycurve)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Object^ Wire::Geometry::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Wire::Wire()
	{
		throw gcnew System::NotImplementedException();
	}
	Wire::~Wire()
	{
		throw gcnew System::NotImplementedException();
	}
}