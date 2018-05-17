#pragma once

#include <TopologicCore/include/Attribute.h>

#include <memory>

#include <TDF_Attribute.hxx>

namespace Topologic
{
	public ref class Attribute
	{
	public:
		
	public protected:
		Attribute(Handle(TDF_Attribute) pCoreAttribute);

	protected:
		~Attribute();

		Handle(TDF_Attribute)* m_pCoreAttribute;
	};
}