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

#include <TopologicCore/include/Topology.h>

using namespace System;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicStructure
{
	ref class LoadFactory : Topologic::TopologyFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Topologic::Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;

		static String^ GetGUID() { return gcnew String("52bb61cc-1453-46fd-9bc0-fe7d5e7be0e7"); }
	};
}