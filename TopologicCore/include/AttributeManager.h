#pragma once

#include "Utilities.h"

#include <TopoDS_Shape.hxx>

#include <list>
#include <map>
#include <memory>

namespace TopologicCore
{
	class Attribute;

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

		void Add(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Attribute>& kpAttribute);

		void Remove(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Attribute>& kpAttribute);

		bool Find(const TopoDS_Shape& rkOcctShape, std::list<std::shared_ptr<Attribute>>& rAttributes);

		void ClearOne(const TopoDS_Shape& rkOcctShape);

		void ClearAll();

	protected:
		std::map<TopoDS_Shape, std::list<std::shared_ptr<Attribute>>, OcctShapeComparator> m_occtShapeToAttributesMap;
	};
}