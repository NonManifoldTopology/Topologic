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