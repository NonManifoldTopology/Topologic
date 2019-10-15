// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

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