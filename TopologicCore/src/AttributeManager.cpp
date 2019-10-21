// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "AttributeManager.h"
#include "Attribute.h"
#include "Topology.h"

#include <TopExp_Explorer.hxx>
#include <TopoDS_Vertex.hxx>

namespace TopologicCore
{
	AttributeManager & AttributeManager::GetInstance()
	{
		static AttributeManager instance;
		return instance;
	}

	void AttributeManager::Add(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const std::string & kAttributeName, const std::shared_ptr<Attribute>& kpAttribute)
	{
		Add(kpTopology->GetOcctShape(), kAttributeName, kpAttribute);
	}

	void AttributeManager::Add(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName, const std::shared_ptr<Attribute>& kpAttribute)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) == m_occtShapeToAttributesMap.end())
		{
			std::map<std::string, Attribute::Ptr> attributeMap;
			m_occtShapeToAttributesMap.insert(std::pair<TopoDS_Shape, std::map<std::string, std::shared_ptr<Attribute>>>(rkOcctShape, attributeMap));
		}

		m_occtShapeToAttributesMap[rkOcctShape][kAttributeName] = kpAttribute;
	}

	void AttributeManager::Remove(const TopologicCore::Topology::Ptr& kpTopology, const std::string& kAttributeName)
	{
		Remove(kpTopology->GetOcctShape(), kAttributeName);
	}

	void AttributeManager::Remove(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap[rkOcctShape].erase(kAttributeName);
		}
	}

	Attribute::Ptr AttributeManager::Find(const TopoDS_Shape& rkOcctShape, const std::string& rkAttributeName)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			std::map<std::string, Attribute::Ptr>& rkAttributeMap = m_occtShapeToAttributesMap[rkOcctShape];
			if (rkAttributeMap.find(rkAttributeName) != rkAttributeMap.end())
			{
				return rkAttributeMap[rkAttributeName];
			}
			return nullptr;
		}

		return nullptr;
	}

	bool AttributeManager::FindAll(const TopoDS_Shape & rkOcctShape, std::map<std::string, std::shared_ptr<Attribute>>& rAttributes)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			rAttributes = m_occtShapeToAttributesMap[rkOcctShape];
			return true;
		}

		return false;
	}

	void AttributeManager::ClearOne(const TopoDS_Shape & rkOcctShape)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap[rkOcctShape].clear();
			m_occtShapeToAttributesMap.erase(rkOcctShape);
		}
	}

	void AttributeManager::ClearAll()
	{
		m_occtShapeToAttributesMap.clear();
	}

	void AttributeManager::CopyAttributes(const TopoDS_Shape & rkOcctShape1, const TopoDS_Shape & rkOcctShape2)
	{
		std::map<std::string, Attribute::Ptr> attributes;
		bool isFound1 = FindAll(rkOcctShape1, attributes);
		if (isFound1)
		{
			std::map<std::string, Attribute::Ptr> attributes2;
			bool isFound2 = FindAll(rkOcctShape2, attributes2);
			if (isFound2)
			{
				for(auto keyValuePair : attributes)
				{
					attributes2[keyValuePair.first] = keyValuePair.second;
				}
				m_occtShapeToAttributesMap[rkOcctShape2] = attributes2;
			}else
			{
				m_occtShapeToAttributesMap[rkOcctShape2] = std::map<std::string, Attribute::Ptr>(attributes);
			}
		}
	}


	void AttributeManager::DeepCopyAttributes(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2)
	{
		// For parent topology
		TopoDS_Shape occtSelectedSubtopology = Topology::SelectSubtopology(
			rkOcctShape2, Topology::CenterOfMass(rkOcctShape1), Topology::GetTopologyType(rkOcctShape1.ShapeType()));
		if (!occtSelectedSubtopology.IsNull())
		{
			CopyAttributes(rkOcctShape1, occtSelectedSubtopology);
		}

		// Get all subtopologies
		for (int occtShapeTypeInt = (int)rkOcctShape1.ShapeType() + 1; occtShapeTypeInt < (int)TopAbs_SHAPE; ++occtShapeTypeInt)
		{
			TopAbs_ShapeEnum occtShapeType = (TopAbs_ShapeEnum)occtShapeTypeInt;
			for (TopExp_Explorer occtExplorer(rkOcctShape1, occtShapeType); occtExplorer.More(); occtExplorer.Next())
			{
				TopoDS_Shape occtSubshape1 = occtExplorer.Current();
				std::map<std::string, Attribute::Ptr> attributes;
				bool isFound1 = FindAll(occtSubshape1, attributes);
				if (!isFound1)
				{
					continue;
				}

				// WARNING: very costly. Only do this if necessary.
				TopoDS_Shape occtSelectedSubtopology = Topology::SelectSubtopology(
					rkOcctShape2, Topology::CenterOfMass(occtSubshape1), Topology::GetTopologyType(occtSubshape1.ShapeType()));
				if (!occtSelectedSubtopology.IsNull())
				{
					CopyAttributes(occtSubshape1, occtSelectedSubtopology);
				}
			}
		}
	}
}