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

#include "EnergySimulation.h"
#include "EnergyModel.h"

using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Linq;

namespace TopologicEnergy
{
	EnergySimulation^ EnergySimulation::ByEnergyModel(EnergyModel ^ energyModel, String ^ openStudioExePath, String ^ openStudioOutputDirectory, bool run)
	{
		if (!run)
		{
			return nullptr;
		}

		String^ oswPath = nullptr;

		String^ timestamp = DateTime::Now.ToString("yyyy-MM-dd_HH-mm-ss-fff");
		String^ openStudioTimestampOutputDirectory = openStudioOutputDirectory + "\\TopologicEnergy_" + timestamp;
		// Create the TopologicEnergy_timestamp folder
		energyModel->Export(energyModel, openStudioTimestampOutputDirectory, oswPath);

		// https://stackoverflow.com/questions/5168612/launch-program-with-parameters
		String^ args = "run -w \"" + oswPath + "\"";
		System::Diagnostics::ProcessStartInfo^ startInfo = gcnew ProcessStartInfo(openStudioExePath, args);
		startInfo->WorkingDirectory = Path::GetDirectoryName(oswPath);

		Process^ process = Process::Start(startInfo);
		process->WaitForExit();

		EnergySimulation^ simulation = gcnew EnergySimulation(
			energyModel->Topology,
			oswPath,
			energyModel->OsModel,
			energyModel->OsSpaces);

		return simulation;
	}

	EnergySimulation::EnergySimulation(IList<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, OpenStudio::SpaceVector^ osSpaces)
		: m_osModel(gcnew OpenStudio::Model(osModel))
		, m_osSpaces(osSpaces)
	{
		if (oswPath == nullptr)
		{
			throw gcnew Exception("The input oswPath must not be null.");
		}

		OpenStudio::Space^ osSpace = osSpaces[0];
		System::String^ directory = System::IO::Path::GetDirectoryName(oswPath);
		System::String^ sqlPath = directory + "\\run\\eplusout.sql";
		m_osSqlFile = gcnew OpenStudio::SqlFile(OpenStudio::OpenStudioUtilitiesCore::toPath(sqlPath));
		m_osModel->setSqlFile(m_osSqlFile);
	}

	EnergySimulation::~EnergySimulation()
	{
		m_osSqlFile->close();
		delete m_osSqlFile;
	}
}