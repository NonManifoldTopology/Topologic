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

#include <LoadAnalysis.h>

#include <OcctLoadAttribute.h>

#include <Standard_GUID.hxx>

namespace TopologicStructure
{
	List<Load^>^ LoadAnalysis::Loads(Topologic::Topology ^ topology)
	{
		List<Topologic::Topology^>^ contentTopologies = topology->Contents(true);
		List<Load^>^ loads = gcnew List<Load^>();
		//for each (Topologic::Topology^ contentTopology in contentTopologies)
		//{
		//	char strID[36] = "";
		//	OcctLoadAttribute::GetID().ToCString(strID);
		//	TDF_Attribute* pAttribute = contentTopology->FindAttribute(gcnew String(strID));
		//	if (pAttribute == nullptr)
		//	{
		//		continue;
		//	}

		//	// Do NOT delete these; they are from the smart pointers in OCAF.
		//	OcctLoadAttribute* pLoadAttribute = dynamic_cast<OcctLoadAttribute*>(pAttribute);
		//	if(pLoadAttribute != nullptr)
		//	{
		//		Topologic::Vertex^ vertex = safe_cast<Topologic::Vertex^>(contentTopology);
		//		TopologicUtility::Vector::Ptr vector = TopologicUtility::Vector::ByCoordinates(pLoadAttribute->X(), pLoadAttribute->Y(), pLoadAttribute->Z());
		//		Load^ load = gcnew Load(vertex, vector, false);
		//		loads->Add(load);
		//	}
		//}
		return loads;
	}
}