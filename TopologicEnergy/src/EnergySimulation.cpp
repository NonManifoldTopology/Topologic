#include "EnergySimulation.h"
#include "EnergyModel.h"

using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Linq;

namespace TopologicEnergy
{
	EnergySimulation^ EnergySimulation::ByEnergyModel(EnergyModel ^ energyModel, String ^ openStudioExePath, String ^ openStudioOutputDirectory, bool run)
	{
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

	EnergySimulation::EnergySimulation(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, OpenStudio::SpaceVector^ osSpaces)
		: m_osModel(osModel)
		, m_osSpaces(osSpaces)
	{
		OpenStudio::Space^ osSpace = osSpaces[0];
		System::String^ directory = System::IO::Path::GetDirectoryName(oswPath);
		System::String^ sqlPath = directory + "\\run\\eplusout.sql";
		m_osSqlFile = gcnew OpenStudio::SqlFile(gcnew OpenStudio::Path(sqlPath));
		m_osModel->setSqlFile(m_osSqlFile);
		OpenStudio::Space^ osSpace2 = OsSpaces[0];
	}

	EnergySimulation::~EnergySimulation()
	{
		m_osSqlFile->close();
		delete m_osSqlFile;
	}
}