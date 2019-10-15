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
//
//#include "Utilities.h"
//
#include <TopAbs_ShapeEnum.hxx>
//
//#include <list>
#include <map>
#include <memory>
#include <string>

namespace TopologicCore
{
	class TopologyFactory;

	class TopologyFactoryManager
	{
	public:
		typedef std::shared_ptr<TopologyFactoryManager> Ptr;

	public:
		static TopologyFactoryManager& GetInstance()
		{
			static TopologyFactoryManager instance;
			return instance;
		}

		void Add(const std::string& rkGuid, const std::shared_ptr<TopologyFactory>& kpTopologyFactory);

		bool Find(const std::string& rkGuid, std::shared_ptr<TopologyFactory>& rTopologyFactory);

		static std::shared_ptr<TopologyFactory> GetDefaultFactory(const TopAbs_ShapeEnum kOcctType);

	protected:
		std::map<std::string, std::shared_ptr<TopologyFactory>> m_topologyFactoryMap;
	};
}