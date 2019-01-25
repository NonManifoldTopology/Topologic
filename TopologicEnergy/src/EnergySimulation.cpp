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
		energyModel->Export(energyModel, openStudioOutputDirectory, oswPath);

		// https://stackoverflow.com/questions/5168612/launch-program-with-parameters
		String^ args = "run -w \"" + oswPath + "\"";
		System::Diagnostics::ProcessStartInfo^ startInfo = gcnew ProcessStartInfo(openStudioExePath, args);
		startInfo->WorkingDirectory = Path::GetDirectoryName(oswPath);

		Process^ process = Process::Start(startInfo);
		process->WaitForExit();

		// Rename run and reports directory, add timestamp
		String^ timestamp = DateTime::Now.ToString("yyyy-MM-dd_HH-mm-ss-fff");
		Directory::Move(startInfo->WorkingDirectory + "\\run", startInfo->WorkingDirectory + "\\run_" + timestamp);
		Directory::Move(startInfo->WorkingDirectory + "\\reports", startInfo->WorkingDirectory + "\\reports_" + timestamp);

		EnergySimulation^ simulation = gcnew EnergySimulation(
			energyModel->BuildingCells,
			oswPath,
			energyModel->OsModel,
			energyModel->OsSpaces,
			timestamp);

		return simulation;
	}

	EnergySimulation::EnergySimulation(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ osSpaces, String^ timestamp)
		: m_cells(cells)
		, m_osModel(osModel)
		, m_osSpaces(osSpaces)
	{
		System::String^ directory = System::IO::Path::GetDirectoryName(oswPath);
		System::String^ sqlPath = directory + "\\run_" + timestamp + "\\eplusout.sql";
		m_osSqlFile = gcnew OpenStudio::SqlFile(gcnew OpenStudio::Path(sqlPath));
		m_osModel->setSqlFile(m_osSqlFile);
	}

	EnergySimulation::~EnergySimulation()
	{
		m_osSqlFile->close();
		delete m_osSqlFile;
	}


	List<Topologic::Cell^>^ EnergySimulation::Topology::get()
	{
		return m_cells;
	}
}