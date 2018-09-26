#include "SimulationResult.h"
#include <TopologicEnergy.h>

namespace TopologicEnergy
{
	SimulationResult::SimulationResult(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ osSpaces, String^ timestamp)
		: m_cells(cells)
		, m_osModel(osModel)
		, m_osSpaces(osSpaces)
	{
		System::String^ directory = System::IO::Path::GetDirectoryName(oswPath);
		System::String^ sqlPath = directory + "\\run_" + timestamp + "\\eplusout.sql";
		m_osSqlFile = gcnew OpenStudio::SqlFile(gcnew OpenStudio::Path(sqlPath));
		m_osModel->setSqlFile(m_osSqlFile);
	}

	SimulationResult::~SimulationResult()
	{
		m_osSqlFile->close();
		delete m_osSqlFile;
	}

	List<Modifiers::GeometryColor^>^ SimulationResult::GeometryQuery(String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits)
	{
		OpenStudio::OptionalDouble^ totalSE = m_osSqlFile->totalSiteEnergy();
		double totalSEGJ = totalSE->__float__();
		double totalSEkwh = totalSEGJ * 277.8;
		OpenStudio::OptionalDouble^ totalEU = m_osSqlFile->electricityTotalEndUses();
		double totalEUGJ = totalEU->__float__();
		double totalEUkwh = totalEUGJ * 277.8;
		OpenStudio::OptionalString^ spaceNameTemp = m_osSpaces[0]->name();
		String^ spaceName = spaceNameTemp->get();
		String^ EPRowName = spaceName + "_THERMAL_ZONE";

		//If min and max values are not specified then calculate them from the data
		double maxValue = 0.0;
		try {
			maxValue = TopologicEnergy::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
		}
		catch (...)
		{
			throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
		}
		double minValue = maxValue;
		for each(OpenStudio::Space^ space in m_osSpaces)
		{
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double aValue = 10000.0;
			try {
				aValue = TopologicEnergy::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
			}
			if (aValue > maxValue)
			{
				maxValue = aValue;
			}
			if (aValue < minValue)
			{
				minValue = aValue;
			}
		}

		if (minValue <= maxValue && maxValue - minValue < 0.0001)
		{
			minValue = maxValue - (maxValue * 0.0001);
		}

		//STEP 2: Find the cell that matches the space and set its colour.
		int i = 0;
		List<Modifiers::GeometryColor^>^ dynamoGeometryColors = gcnew List<Modifiers::GeometryColor^>();
		for each(OpenStudio::Space^ space in m_osSpaces)
		{
			Topologic::Cell^ selectedCell = m_cells[i];
			++i;
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double outputVariable = 0.0;
			try {
				outputVariable = TopologicEnergy::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
			}

			// Map the outputVariable to a ratio between 0 and 1
			double ratio = (outputVariable - minValue) / (maxValue - minValue);
			DSCore::Color^ aColor = TopologicEnergy::GetColor(ratio);
			System::Object^ cellGeometry = selectedCell->Geometry;

			// 1. Try a Dynamo geometry
			Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(cellGeometry);
			if (dynamoGeometry != nullptr)
			{
				Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, aColor);
				dynamoGeometryColors->Add(dynamoGeometryColor);
				continue;
			}

			// 2. Try a Dynamo geometry
			List<Object^>^ listOfObjects = dynamic_cast<List<Object^>^>(cellGeometry);
			if (listOfObjects != nullptr)
			{
				for each(Object^ object in listOfObjects)
				{
					Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(object);
					if (dynamoGeometry != nullptr)
					{
						Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, aColor);
						delete object;
						dynamoGeometryColors->Add(dynamoGeometryColor);
					}
				}
				continue;
			}
		}

		return dynamoGeometryColors;
	}

	List<double>^ SimulationResult::ValueQuery(String ^ EPReportName, String ^ EPReportForString, String ^ EPTableName, String ^ EPColumnName, String ^ EPUnits)
	{
		OpenStudio::OptionalDouble^ totalSE = m_osSqlFile->totalSiteEnergy();
		double totalSEGJ = totalSE->__float__();
		double totalSEkwh = totalSEGJ * 277.8;
		OpenStudio::OptionalDouble^ totalEU = m_osSqlFile->electricityTotalEndUses();
		double totalEUGJ = totalEU->__float__();
		double totalEUkwh = totalEUGJ * 277.8;
		OpenStudio::OptionalString^ spaceNameTemp = m_osSpaces[0]->name();
		String^ spaceName = spaceNameTemp->get();
		String^ EPRowName = spaceName + "_THERMAL_ZONE";

		//If min and max values are not specified then calculate them from the data
		double maxValue = 0.0;
		try {
			maxValue = TopologicEnergy::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
		}
		catch (...)
		{
			throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
		}
		double minValue = maxValue;
		for each(OpenStudio::Space^ space in m_osSpaces)
		{
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double aValue = 0.0;
			try {
				aValue = TopologicEnergy::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
			}
			if (aValue > maxValue)
			{
				maxValue = aValue;
			}
			if (aValue < minValue)
			{
				minValue = aValue;
			}
		}

		if (minValue <= maxValue && maxValue - minValue < 0.0001)
		{
			minValue = maxValue - (maxValue * 0.0001);
		}

		//STEP 2: Find the cell that matches the space and set its colour.
		int i = 0;
		List<double>^ values = gcnew List<double>();
		for each(OpenStudio::Space^ space in m_osSpaces)
		{
			Topologic::Cell^ selectedCell = m_cells[i];
			++i;
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double outputVariable = 0.0;
			try {
				outputVariable = TopologicEnergy::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
			}

			values->Add(outputVariable);
		}

		return values;
	}

	List<Topologic::Cell^>^ SimulationResult::Topology::get()
	{
		return m_cells;
	}
}