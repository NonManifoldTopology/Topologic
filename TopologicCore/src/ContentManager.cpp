#include <ContentManager.h>
#include <Context.h>

#include <Topology.h>

#include <algorithm>

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
	}

	void ContentManager::Remove(const TopoDS_Shape& rkOcctShape, const TopoDS_Shape& rkOcctContentTopology)
	{
		if (m_occtShapeToContentsMap.find(rkOcctShape) != m_occtShapeToContentsMap.end())
		{
			m_occtShapeToContentsMap[rkOcctShape].remove_if(
				[&](const Topology::Ptr& kpContent) {
				return kpContent->GetOcctShape().IsSame(rkOcctContentTopology);
			});
		}
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

	bool ContentManager::HasContent(const TopoDS_Shape & rkOcctShape, const TopoDS_Shape& rkOcctContentTopology)
	{
		std::list<Topology::Ptr> contents;
		bool hasContents = Find(rkOcctShape, contents);
		if (!hasContents)
			return false;

		std::list<Topology::Ptr>::iterator contentIterator = std::find_if(contents.begin(), contents.end(), 
			[&](const Topology::Ptr& kpContent) { 
			return kpContent->GetOcctShape().IsSame(rkOcctContentTopology);
		});

		return contentIterator != contents.end();
	}

	void ContentManager::ClearOne(const TopoDS_Shape & rkOcctShape)
	{
		if (m_occtShapeToContentsMap.find(rkOcctShape) != m_occtShapeToContentsMap.end())
		{
			// Remove from all contexts.
			// Removal is find: it will not do anything if the item is no longer there.
			m_occtShapeToContentsMap[rkOcctShape].clear();
			m_occtShapeToContentsMap.erase(rkOcctShape);
		}
	}

	void ContentManager::ClearAll()
	{
		m_occtShapeToContentsMap.clear();
	}
}