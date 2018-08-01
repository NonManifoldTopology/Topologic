#pragma once

#include "Utilities.h"

#include <TopoDS_Shape.hxx>

#include <list>
#include <map>
#include <memory>

namespace TopologicCore
{
	class Topology;

	class InstanceGUIDManager
	{
	public:
		typedef std::shared_ptr<InstanceGUIDManager> Ptr;

	public:
		static InstanceGUIDManager& GetInstance()
		{
			static InstanceGUIDManager instance;
			return instance;
		}

		void Add(const TopoDS_Shape& rkOcctShape, const std::string& rkGUID);

		void Remove(const TopoDS_Shape& rkOcctShape);

		bool Find(const TopoDS_Shape& rkOcctShape, std::string& rkGUID);

		void ClearAll();

	protected:
		std::map<TopoDS_Shape, std::string, OcctShapeComparator> m_occtShapeToGUIDMap;
	};
}