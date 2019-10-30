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

#include "ShellUtility.h"
#include "Face.h"
#include "Wire.h"
#include "Edge.h"
#include "Vertex.h"

#include <TopologicCore/include/Utilities/ShellUtility.h>

namespace Topologic {
	namespace Utilities {

		Shell^ ShellUtility::ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires)
		{
			std::list<TopologicCore::Wire::Ptr> coreWires;
			for each(Wire^ pWire in wires)
			{
				coreWires.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(pWire->GetCoreTopologicalQuery()));
			}

			try {
				TopologicCore::Shell::Ptr pCoreShell = TopologicUtilities::ShellUtility::ByLoft(coreWires);
				return gcnew Shell(pCoreShell);
			}
			catch (std::exception&)
			{
				throw gcnew Exception("Loft error");
			}
		}
	}
}