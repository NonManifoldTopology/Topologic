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

#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>

#include <list>

namespace TopologicCore
{
	class Topology;

	class GlobalCluster
	{
	public:
		typedef std::shared_ptr<GlobalCluster> Ptr;

	public:
		TOPOLOGIC_API static GlobalCluster& GetInstance()
		{
			static GlobalCluster instance;
			return instance;
		}

		TOPOLOGIC_API GlobalCluster();
		TOPOLOGIC_API ~GlobalCluster();

		TOPOLOGIC_API void AddTopology(const std::shared_ptr<Topology>& rkTopology);

		TOPOLOGIC_API void AddTopology(const TopoDS_Shape& rkOcctShape);

		void RemoveTopology(const std::shared_ptr<Topology>& rkTopology);

		void RemoveTopology(const TopoDS_Shape& rkOcctShape);

		void Clear();

		const TopoDS_Compound& GetOcctCompound() const;

		TopoDS_Compound& GetOcctCompound();

		TOPOLOGIC_API void SubTopologies(std::list<std::shared_ptr<Topology>>& rSubTopologies) const;

	protected:
		TopoDS_Compound m_occtCompound;
		TopoDS_Builder m_occtBuilder;
	};
}