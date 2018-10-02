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

	List<Cell^>^ SimulationResult::Query(String ^ EPReportName, String ^ EPReportForString, String ^ EPTableName, String ^ EPColumnName, String ^ EPUnits)
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

		// Create a map: space name -> cell
		Dictionary<String^, Cell^>^ cellNames = gcnew Dictionary<String^, Cell^>();
		for each(Cell^ cell in m_cells)
		{
			Object^ name = cell->AttributeValue("Name");
			if (name == nullptr)
			{
				throw gcnew Exception("A cell does not have a name.");
			}

			String^ strName = nullptr;
			try
			{
				strName = safe_cast<String^>(name);
			}
			catch(...)
			{
				throw gcnew Exception("A cell's name is not a string.");
			}

			cellNames->Add(strName, cell);
		}

		//STEP 2: Find the cell that matches the space and set its colour.
		int i = 0;
		List<Cell^>^ outputCells = gcnew List<Cell^>();
		for each(OpenStudio::Space^ space in m_osSpaces)
		{
			bool hasKey = cellNames->ContainsKey(space->nameString());
			if (!hasKey)
			{
				throw gcnew Exception("Does not find a cell which has the same name as the space.");
			}
			Topologic::Cell^ selectedCell = cellNames[space->nameString()];
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
			DSCore::Color^ color = TopologicEnergy::GetColor(ratio);

			Dictionary<String^, Object^>^ attributes = gcnew Dictionary<String^, Object^>();
			attributes->Add("Value", outputVariable);
			attributes->Add("r", (long long int)color->Red);
			attributes->Add("g", (long long int)color->Green);
			attributes->Add("b", (long long int)color->Blue);
			selectedCell->AddAttributesNoCopy(attributes);
			outputCells->Add(selectedCell);
		}

		return outputCells;
	}

	List<Modifiers::GeometryColor^>^ SimulationResult::Display(List<Topologic::Cell^> buildingCells, int alpha)
	{
		List<Modifiers::GeometryColor^>^ dynamoGeometryColors = gcnew List<Modifiers::GeometryColor^>();
		for each(Cell^ buildingCell in buildingCells)
		{
			// Get the colors
			Object^ objR = buildingCell->AttributeValue("r");
			if (objR == nullptr)
			{
				throw gcnew Exception("A cell does not have a red color.");
			}

			long long int r = 0;
			try
			{
				r = safe_cast<long long int>(objR);
			}
			catch (...)
			{
				throw gcnew Exception("A cell's red color is not a int value.");
			}

			// Get the colors
			Object^ objG = buildingCell->AttributeValue("g");
			if (objG == nullptr)
			{
				throw gcnew Exception("A cell does not have a green color.");
			}

			long long int g = 0;
			try
			{
				g = safe_cast<long long int>(objG);
			}
			catch (...)
			{
				throw gcnew Exception("A cell's green color is not a int value.");
			}

			// Get the colors
			Object^ objB = buildingCell->AttributeValue("b");
			if (objB == nullptr)
			{
				throw gcnew Exception("A cell does not have a blue color.");
			}

			long long int b = 0;
			try
			{
				b = safe_cast<long long int>(objB);
			}
			catch (...)
			{
				throw gcnew Exception("A cell's blue color is not a int value.");
			}

			DSCore::Color^ color = DSCore::Color::ByARGB(alpha, (int) r, (int) g, (int) b);
			System::Object^ cellGeometry = buildingCell->Geometry;

			// 1. Try a Dynamo geometry
			Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(cellGeometry);
			if (dynamoGeometry != nullptr)
			{
				Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, color);
				dynamoGeometryColors->Add(dynamoGeometryColor);
				continue;
			}

			// 2. Try a list of Dynamo geometries
			List<Object^>^ listOfObjects = dynamic_cast<List<Object^>^>(cellGeometry);
			if (listOfObjects != nullptr)
			{
				for each(Object^ object in listOfObjects)
				{
					Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(object);
					if (dynamoGeometry != nullptr)
					{
						Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, color);
						delete object;
						dynamoGeometryColors->Add(dynamoGeometryColor);
					}
				}
				continue;
			}
		}

		return dynamoGeometryColors;
	}

	List<Topologic::Cell^>^ SimulationResult::Topology::get()
	{
		return m_cells;
	}
}