#pragma once

#include <memory>

namespace TopologicCore {
	class Attribute
	{
	public:
		typedef std::shared_ptr<Attribute> Ptr;

		virtual ~Attribute() {}

		virtual void* Value() = 0;
	};
}