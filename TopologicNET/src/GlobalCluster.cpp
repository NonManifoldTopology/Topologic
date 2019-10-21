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

#include "GlobalCluster.h"

#ifdef _DEBUG

namespace Topologic
{
	List<Topology^>^ GlobalCluster::SubTopologies(bool refresh)
	{
		if (!refresh)
			return nullptr;

		std::list<std::shared_ptr<TopologicCore::Topology>> coreTopologies;
		TopologicCore::GlobalCluster::GetInstance().SubTopologies(coreTopologies);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kTopologyIterator = coreTopologies.begin();
			kTopologyIterator != coreTopologies.end();
			kTopologyIterator++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kTopologyIterator);
			pTopologies->Add(pTopology);
		}

		return pTopologies;
	}
}

#endif