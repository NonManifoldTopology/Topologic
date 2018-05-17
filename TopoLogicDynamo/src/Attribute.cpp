#include <Attribute.h>

namespace Topologic
{
	Attribute::Attribute(Handle(TDF_Attribute) pCoreAttribute)
		: m_pCoreAttribute(new Handle(TDF_Attribute)(pCoreAttribute))
	{

	}

	Attribute::~Attribute()
	{
		delete m_pCoreAttribute;
	}
}