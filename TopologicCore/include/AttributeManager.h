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

#pragma once

#include "Utilities.h"

#include <TopoDS_Shape.hxx>

#include <list>
#include <map>
#include <memory>

namespace TopologicCore
{
	class Topology;
	class Attribute;

	class AttributeManager
	{
	public:
		typedef std::shared_ptr<AttributeManager> Ptr;
		typedef std::map<std::string, std::shared_ptr<Attribute>> AttributeMap;

	public:
		TOPOLOGIC_API static AttributeManager& GetInstance();

		TOPOLOGIC_API void Add(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const std::string& kAttributeName, const std::shared_ptr<Attribute>& kpAttribute);

		TOPOLOGIC_API void Add(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName, const std::shared_ptr<Attribute>& kpAttribute);

		TOPOLOGIC_API void Remove(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const std::string& kAttributeName);

		TOPOLOGIC_API void Remove(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName);

		TOPOLOGIC_API std::shared_ptr<Attribute> Find(const TopoDS_Shape& rkOcctShape, const std::string& rkAttributeName);

		TOPOLOGIC_API bool FindAll(const TopoDS_Shape & rkOcctShape, std::map<std::string, std::shared_ptr<Attribute>>& rAttributes);

		TOPOLOGIC_API void ClearOne(const TopoDS_Shape& rkOcctShape);

		TOPOLOGIC_API void ClearAll();

		TOPOLOGIC_API void CopyAttributes(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2);

	protected:
		std::map<TopoDS_Shape, std::map<std::string, std::shared_ptr<Attribute>>, TopologicCore::OcctShapeComparator> m_occtShapeToAttributesMap;
	};
}