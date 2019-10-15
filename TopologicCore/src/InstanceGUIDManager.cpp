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

#include <InstanceGUIDManager.h>

#include <Topology.h>

namespace TopologicCore
{
	void InstanceGUIDManager::Add(const TopoDS_Shape& rkOcctShape, const std::string& rkGUID)
	{
		m_occtShapeToGUIDMap[rkOcctShape] = rkGUID;
	}

	void InstanceGUIDManager::Remove(const TopoDS_Shape & rkOcctShape)
	{
		m_occtShapeToGUIDMap.erase(rkOcctShape);
	}

	bool InstanceGUIDManager::Find(const TopoDS_Shape& rkOcctShape, std::string& rkGUID)
	{
		if (m_occtShapeToGUIDMap.find(rkOcctShape) != m_occtShapeToGUIDMap.end())
		{
			rkGUID = m_occtShapeToGUIDMap[rkOcctShape];
			return true;
		}

		return false;
	}

	void InstanceGUIDManager::ClearAll()
	{
		m_occtShapeToGUIDMap.clear();
	}
}