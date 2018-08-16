#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Utilities.h>
#include <TopologicCore/include/Topology.h>

#include <TopoDS_Shape.hxx>

#include <list>
#include <map>
#include <memory>

namespace TopologicSupport
{
	class Attribute;
	class AttributeMap;

	class AttributeManager
	{
	public:
		typedef std::shared_ptr<AttributeManager> Ptr;

	public:
		static AttributeManager& GetInstance()
		{
			static AttributeManager instance;
			return instance;
		}

		TOPOLOGIC_SUPPORT_API void AddAttribute(const TopoDS_Shape& rkOcctShape, const std::string& rkKey, const std::shared_ptr<Attribute>& kpAttribute);

		TOPOLOGIC_SUPPORT_API void AddAttributeMap(const TopologicCore::Topology::Ptr& kpTopology, const std::shared_ptr<AttributeMap>& rkAttributeMap);
		
		TOPOLOGIC_SUPPORT_API void AddAttributeMap(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<AttributeMap>& rkAttributeMap);

		TOPOLOGIC_SUPPORT_API void GetAttributeMap(const TopologicCore::Topology::Ptr& kpTopology, std::shared_ptr<AttributeMap>& rAttributeMap);

		TOPOLOGIC_SUPPORT_API void GetAttributeMap(const TopoDS_Shape& rkOcctShape, std::shared_ptr<AttributeMap>& rAttributeMap);

		TOPOLOGIC_SUPPORT_API void Remove(const TopoDS_Shape& rkOcctShape, const std::string& rkKey);

		TOPOLOGIC_SUPPORT_API bool Find(const TopoDS_Shape& rkOcctShape, const std::string& rkKey, std::shared_ptr<Attribute>& rAttributes);

		TOPOLOGIC_SUPPORT_API void ClearOne(const TopoDS_Shape& rkOcctShape);

		TOPOLOGIC_SUPPORT_API void ClearAll();

	protected:
		std::map<TopoDS_Shape, std::shared_ptr<AttributeMap>, OcctShapeComparator> m_occtShapeToAttributesMap;
	};
}