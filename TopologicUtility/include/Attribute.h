#pragma once

#include <memory>

namespace TopologicUtility {
	class Attribute
	{
	public:
		typedef std::shared_ptr<Attribute> Ptr;

		virtual ~Attribute() {}

		virtual void* Value() = 0;
	};
}