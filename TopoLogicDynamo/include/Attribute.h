#pragma once

#include <TopologicCore/include/Attribute.h>

#include <memory>

namespace Topologic
{
	public ref class Attribute
	{
	public:
		
	public protected:
		Attribute(const std::shared_ptr<TopologicCore::Attribute>& kpCoreAttribute);

	protected:
		~Attribute();

		std::shared_ptr<TopologicCore::Attribute>* m_pCoreAttribute;
	};
}