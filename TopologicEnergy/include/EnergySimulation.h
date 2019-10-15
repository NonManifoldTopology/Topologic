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

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	ref class EnergyModel;

	public ref class EnergySimulation
	{
	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="energyModel"></param>
		/// <param name="openStudioExePath"></param>
		/// <param name="openStudioOutputDirectory"></param>
		/// <param name="run"></param>
		/// <returns name="EnergySimulation"></returns>
		static EnergySimulation^ ByEnergyModel(EnergyModel^ energyModel, String^ openStudioExePath, String ^ openStudioOutputDirectory, bool run);

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="cells"></param>
		/// <param name="oswPath">Used to deduce the SQL path</param>
		EnergySimulation(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, OpenStudio::SpaceVector^ osSpaces);
		~EnergySimulation();

		property OpenStudio::SqlFile^ OsSqlFile
		{
			OpenStudio::SqlFile^ get()
			{
				return m_osSqlFile;
			}
		}

		property OpenStudio::SpaceVector^ OsSpaces
		{
			OpenStudio::SpaceVector^ get()
			{
				return m_osSpaces;
			}
		}

	protected:
		// The SqlFile
		OpenStudio::SqlFile^ m_osSqlFile;

		OpenStudio::Model^ m_osModel;
		
		OpenStudio::SpaceVector^ m_osSpaces;
	};
}