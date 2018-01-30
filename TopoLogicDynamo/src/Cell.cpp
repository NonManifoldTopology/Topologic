#include <Cell.h>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Cell::ByFaces(List<Object^>^ faces)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::BySolid(Autodesk::DesignScript::Geometry::Solid ^ solid)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::ByShell(Object ^ shell)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::ByVerticesFaceIndices(List<Object^>^ vertices, List<List<int>^>^ faceIndices)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::CellComplex(Cell ^ topoLogicCell)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::Faces(Cell ^ topoLogicCell)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::Wires(Cell ^ topoLogicCell)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::Edges(Cell ^ topoLogicCell)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::Vertices(Cell ^ topoLogicCell)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::AdjacentCells(Cell ^ cell)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::SharedFaces(Cell ^ cell1, Cell ^ cell2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::SharedEdges(Cell ^ cell1, Cell ^ cell2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Cell::SharedVertices(Cell ^ cell1, Cell ^ cell2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Object^ Cell::Geometry::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Cell::Cell()
	{

	}
	Cell::~Cell()
	{

	}
}