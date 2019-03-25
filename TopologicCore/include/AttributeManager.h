#pragma once

#include "Utilities.h"

#include <TopoDS_Shape.hxx>

#include <list>
#include <map>
#include <memory>

namespace TopologicCore
{
	class Topology;
	class Attribute;

	class AttributeManager
	{
	public:
		typedef std::shared_ptr<AttributeManager> Ptr;

	public:
		TOPOLOGIC_API static AttributeManager& GetInstance();

		TOPOLOGIC_API void Add(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const std::string& kAttributeName, const std::shared_ptr<Attribute>& kpAttribute);

		TOPOLOGIC_API void Add(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName, const std::shared_ptr<Attribute>& kpAttribute);

		TOPOLOGIC_API void Remove(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const std::string& kAttributeName);

		TOPOLOGIC_API void Remove(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName);

		TOPOLOGIC_API std::shared_ptr<Attribute> Find(const TopoDS_Shape& rkOcctShape, const std::string& rkAttributeName);

		TOPOLOGIC_API bool FindAll(const TopoDS_Shape & rkOcctShape, std::map<std::string, std::shared_ptr<Attribute>>& rAttributes);

		TOPOLOGIC_API void ClearOne(const TopoDS_Shape& rkOcctShape);

		TOPOLOGIC_API void ClearAll();

		TOPOLOGIC_API void CopyAttributes(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2);

	protected:
		std::map<TopoDS_Shape, std::map<std::string, std::shared_ptr<Attribute>>, TopologicCore::OcctShapeComparator> m_occtShapeToAttributesMap;
	};
}