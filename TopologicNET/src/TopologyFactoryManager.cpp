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

#include "TopologyFactoryManager.h"
#include "CellComplexFactory.h"
#include "FaceFactory.h"
#include "VertexFactory.h"
#include "EdgeFactory.h"
#include "WireFactory.h"
#include "ShellFactory.h"
#include "CellFactory.h"
#include "ClusterFactory.h"
#include "ApertureFactory.h"

namespace Topologic
{
	namespace Factories
	{
		void TopologyFactoryManager::Add(const TopologicCore::Topology::Ptr& content, TopologyFactory^ value)
		{
			Add(gcnew String(content->GetClassGUID().c_str()), value);
		}

		void TopologyFactoryManager::Add(String^ key, TopologyFactory^ value)
		{
			try {
				m_TopologyFactoryDict->Add(key, value);
			}
			catch (Exception^)
			{

			}
		}

		TopologyFactory^ TopologyFactoryManager::Find(String^ key)
		{
			if (m_TopologyFactoryDict->ContainsKey(key))
			{
				return m_TopologyFactoryDict[key];
			}
			else
			{
				throw gcnew Exception("Not found");
			}
		}

		TopologyFactory^ TopologyFactoryManager::GetDefaultFactory(const TopologicCore::Topology::Ptr & topology)
		{
			switch (topology->GetType())
			{
			case TopologicCore::TOPOLOGY_CLUSTER: return gcnew ClusterFactory();
			case TopologicCore::TOPOLOGY_CELLCOMPLEX: return gcnew CellComplexFactory();
			case TopologicCore::TOPOLOGY_CELL: return gcnew CellFactory();
			case TopologicCore::TOPOLOGY_SHELL: return gcnew ShellFactory();
			case TopologicCore::TOPOLOGY_FACE: return gcnew FaceFactory();
			case TopologicCore::TOPOLOGY_WIRE: return gcnew WireFactory();
			case TopologicCore::TOPOLOGY_EDGE: return gcnew EdgeFactory();
			case TopologicCore::TOPOLOGY_VERTEX: return gcnew VertexFactory();
			case TopologicCore::TOPOLOGY_APERTURE: return gcnew ApertureFactory();
			default:
				throw gcnew Exception("Topology::ByOcctShape: unknown topology.");
			}
		}
	}
}