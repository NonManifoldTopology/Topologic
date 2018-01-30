#pragma once

#include <TopoLogicCore/include/Attribute.h>

namespace TopoLogic
{
	public ref class Attribute
	{
	public:
		
	protected:
		Attribute();
		~Attribute();

		TopoLogicCore::Attribute* m_pCoreAttribute;
	};
}