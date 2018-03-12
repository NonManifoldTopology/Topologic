#include <Attribute.h>

namespace TopoLogic
{
	Attribute::Attribute(TopoLogicCore::Attribute * const kpCoreAttribute)
		: m_pCoreAttribute(kpCoreAttribute)
	{

	}

	Attribute::~Attribute()
	{
		// Core attribute is owned by the topology, not this class, so it is not deleted.
	}
}