#include <ContentManager.h>
#include <ContextManager.h>
#include <Context.h>

#include <Topology.h>

namespace TopologicCore
{
	void ContentManager::Add(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Topology>& kpContentTopology)
	{
		if (m_occtShapeToContentsMap.find(rkOcctShape) == m_occtShapeToContentsMap.end())
		{
			std::list<Topology::Ptr> contents;
			m_occtShapeToContentsMap.insert(std::pair<TopoDS_Shape, std::list<std::shared_ptr<Topology>>>(rkOcctShape, contents));
		}

		m_occtShapeToContentsMap[rkOcctShape].push_back(kpContentTopology);

		// Also register to context NOT!
		//ContextManager::GetInstance().Add(kpContentTopology->GetOcctShape(), Context::ByTopologyParameters(Topology::ByOcctShape(rkOcctShape), 0.0, 0.0, 0.0));
		
		// Do not register context! Otherwise there is a cyclic recursion
	}

	//void ContentManager::Remove(const TopoDS_Shape & rkOcctShape, const std::shared_ptr<Topology>& kpContentTopology)
	void ContentManager::Remove(const TopoDS_Shape& rkOcctShape, const TopoDS_Shape& rkOcctContentTopology)
	{
		if (m_occtShapeToContentsMap.find(rkOcctShape) != m_occtShapeToContentsMap.end())
		{
			m_occtShapeToContentsMap[rkOcctShape].remove_if(
				[&](const Topology::Ptr& kpContent) {
				return kpContent->GetOcctShape().IsSame(rkOcctContentTopology);
			});
		}

		// Also remove from context. Removal is find: it will not do anything if the item is no longer there.
		ContextManager::GetInstance().Remove(rkOcctContentTopology, rkOcctShape);
	}

	bool ContentManager::Find(const TopoDS_Shape& rkOcctShape, std::list<std::shared_ptr<Topology>>& rContents)
	{
		if (m_occtShapeToContentsMap.find(rkOcctShape) != m_occtShapeToContentsMap.end())
		{
			std::list<Topology::Ptr> contents = m_occtShapeToContentsMap[rkOcctShape];
			rContents.insert(rContents.end(), contents.begin(), contents.end());
			return true;
		}

		return false;
	}

	void ContentManager::ClearOne(const TopoDS_Shape & rkOcctShape)
	{
		if (m_occtShapeToContentsMap.find(rkOcctShape) != m_occtShapeToContentsMap.end())
		{
			// Remove from all contexts.
			// Removal is find: it will not do anything if the item is no longer there.
			for(const Topology::Ptr& kpContentTopology : m_occtShapeToContentsMap[rkOcctShape])
			{
				ContextManager::GetInstance().Remove(kpContentTopology->GetOcctShape(), rkOcctShape);
			}
			m_occtShapeToContentsMap[rkOcctShape].clear();
			m_occtShapeToContentsMap.erase(rkOcctShape);
		}
	}

	void ContentManager::ClearAll()
	{
		m_occtShapeToContentsMap.clear();

		// Also remove contexts. Removal is find: it will not do anything if the item is no longer there.
		ContextManager::GetInstance().ClearAll();
	}
}