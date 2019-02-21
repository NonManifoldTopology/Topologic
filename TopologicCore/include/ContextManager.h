#pragma once

#include "Utilities.h"

#include <TopoDS_Shape.hxx>

#include <list>
#include <map>
#include <memory>

namespace TopologicCore
{
	class Context;

	class ContextManager
	{
	public:
		typedef std::shared_ptr<ContextManager> Ptr;

	public:
		static ContextManager& GetInstance()
		{
			static ContextManager instance;
			return instance;
		}

		void Add(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Context>& kpContext);

		//void Remove(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Context>& kpContext);
		void Remove(const TopoDS_Shape& rkOcctShape, const TopoDS_Shape& rkOcctContextShape);

		bool Find(const TopoDS_Shape& rkOcctShape, std::list<std::shared_ptr<Context>>& rContexts);

		void ClearOne(const TopoDS_Shape& rkOcctShape);

		void ClearAll();

	protected:
		std::map<TopoDS_Shape, std::list<std::shared_ptr<Context>>, OcctShapeComparator> m_occtShapeToContextsMap;
	};
}