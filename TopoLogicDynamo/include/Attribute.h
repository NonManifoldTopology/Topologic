#pragma once

#include <TopoLogicCore/include/Attribute.h>

namespace TopoLogic
{
	public ref class Attribute
	{
	public:
		
	public protected:
		Attribute(TopoLogicCore::Attribute * const kpCoreAttribute);

	protected:
		~Attribute();

		TopoLogicCore::Attribute* m_pCoreAttribute;
	};
}