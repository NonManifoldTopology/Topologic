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

		// Also register to context
		ContextManager::GetInstance().Add(kpContentTopology->GetOcctShape(), Context::ByTopologyParameters(Topology::ByOcctShape(rkOcctShape), 0.0, 0.0, 0.0));
	}

	void ContentManager::Remove(const TopoDS_Shape & rkOcctShape, const std::shared_ptr<Topology>& kpContentTopology)
	{
		if (m_occtShapeToContentsMap.find(rkOcctShape) != m_occtShapeToContentsMap.end())
		{
			m_occtShapeToContentsMap[rkOcctShape].remove(kpContentTopology);
		}

		// Also remove from context
		ContextManager::GetInstance().Remove(kpContentTopology->GetOcctShape(), rkOcctShape);
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
			// Remove from all contexts
			for(const Topology::Ptr& kpTopology : m_occtShapeToContentsMap[rkOcctShape])
			{
				ContextManager::GetInstance().Remove(kpTopology->GetOcctShape(), rkOcctShape);
			}
			m_occtShapeToContentsMap[rkOcctShape].clear();
			m_occtShapeToContentsMap.erase(rkOcctShape);
		}
	}

	void ContentManager::ClearAll()
	{
		m_occtShapeToContentsMap.clear();

		// Also remove contexts
		ContextManager::GetInstance().ClearAll();
	}
}