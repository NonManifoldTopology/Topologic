#include <Attribute.h>

namespace TopoLogic
{
	Attribute::Attribute(const std::shared_ptr<TopoLogicCore::Attribute>& kpCoreAttribute)
		: m_pCoreAttribute(new std::shared_ptr<TopoLogicCore::Attribute>(kpCoreAttribute))
	{

	}

	Attribute::~Attribute()
	{
		delete m_pCoreAttribute;
	}
}