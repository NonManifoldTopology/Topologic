#pragma once

#include <TopoLogicCore/include/Attribute.h>

#include <memory>

namespace TopoLogic
{
	public ref class Attribute
	{
	public:
		
	public protected:
		Attribute(const std::shared_ptr<TopoLogicCore::Attribute>& kpCoreAttribute);

	protected:
		~Attribute();

		std::shared_ptr<TopoLogicCore::Attribute>* m_pCoreAttribute;
	};
}