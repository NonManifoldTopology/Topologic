#include <Topology.h>

namespace TopoLogic
{
	int Topology::Dimensionality::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here

	}

	bool Topology::Locked::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here

	}

	Dictionary<String^, Object^>^ Topology::Union(List<Topology^>^ topologyList)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
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
	Dictionary<String^, Object^>^ Topology::Difference(Topology ^ topology1, Topology ^ topology2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Topology::Impose(Topology ^ topology1, Topology ^ topology2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Topology::Imprint(Topology ^ topology1, Topology ^ topology2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Topology::Intersection(Topology ^ topology1, Topology ^ topology2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Topology::Merge(List<Topology^>^ topologyList)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Dictionary<String^, Object^>^ Topology::Slice(Topology ^ topology, List<Topology^>^ topologyList)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}