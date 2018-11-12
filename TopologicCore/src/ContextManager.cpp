#include <ContextManager.h>
#include <Context.h>
#include <Topology.h>

namespace TopologicCore
{
	void ContextManager::Add(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Context>& kpContext)
	{
		if (m_occtShapeToContextsMap.find(rkOcctShape) == m_occtShapeToContextsMap.end())
		{
			std::list<Context::Ptr> contents;
			m_occtShapeToContextsMap.insert(std::pair<TopoDS_Shape, std::list<std::shared_ptr<Context>>>(rkOcctShape, contents));
		}

		m_occtShapeToContextsMap[rkOcctShape].push_back(kpContext);
	}

	void ContextManager::Remove(const TopoDS_Shape& rkOcctShape, const TopoDS_Shape& rkOcctContextShape)
	{
		if (m_occtShapeToContextsMap.find(rkOcctShape) != m_occtShapeToContextsMap.end())
		{
			m_occtShapeToContextsMap[rkOcctShape].remove_if(
				[&](const Context::Ptr& kpContext) { 
				return kpContext->Topology()->GetOcctShape().IsSame(rkOcctContextShape);
			});
		}
	}

	bool ContextManager::Find(const TopoDS_Shape& rkOcctShape, std::list<std::shared_ptr<Context>>& rContents)
	{
		if (m_occtShapeToContextsMap.find(rkOcctShape) != m_occtShapeToContextsMap.end())
		{
			std::list<Context::Ptr> contents = m_occtShapeToContextsMap[rkOcctShape];
			rContents.insert(rContents.end(), contents.begin(), contents.end());
			return true;
		}

		return false;
	}

	void ContextManager::ClearOne(const TopoDS_Shape & rkOcctShape)
	{
		if (m_occtShapeToContextsMap.find(rkOcctShape) != m_occtShapeToContextsMap.end())
		{
			// Remove from all contents.
			m_occtShapeToContextsMap[rkOcctShape].clear();
			m_occtShapeToContextsMap.erase(rkOcctShape);
		}
	}

	void ContextManager::ClearAll()
	{
		m_occtShapeToContextsMap.clear();
	}
}