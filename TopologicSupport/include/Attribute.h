#pragma once

#include <memory>
#include <map>

namespace TopologicSupport
{
	class Attribute
	{
	public:
		typedef std::shared_ptr<TopologicSupport::Attribute> Ptr;

	public:
		virtual ~Attribute() {};

		virtual std::string GUID() const = 0;
	};
}