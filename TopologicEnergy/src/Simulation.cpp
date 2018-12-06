#include "Simulation.h"
#include "EnergyModel.h"

namespace TopologicEnergy
{
	Simulation::Simulation(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ osSpaces, String^ timestamp)
		: m_cells(cells)
		, m_osModel(osModel)
		, m_osSpaces(osSpaces)
	{
		System::String^ directory = System::IO::Path::GetDirectoryName(oswPath);
		System::String^ sqlPath = directory + "\\run_" + timestamp + "\\eplusout.sql";
		m_osSqlFile = gcnew OpenStudio::SqlFile(gcnew OpenStudio::Path(sqlPath));
		m_osModel->setSqlFile(m_osSqlFile);
	}

	Simulation::~Simulation()
	{
		m_osSqlFile->close();
		delete m_osSqlFile;
	}

	Dictionary<String^, Dictionary<String^, Object^>^>^ Simulation::Query(String ^ EPReportName, String ^ EPReportForString, String ^ EPTableName, String ^ EPColumnName, String ^ EPUnits)
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
			maxValue = EnergyModel::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
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
				aValue = EnergyModel::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
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
		Dictionary<String^, Dictionary<String^, Object^>^>^ data = gcnew Dictionary<String^, Dictionary<String^, Object^>^>();

		//STEP 2: Find the cell that matches the space and set its colour.
		int i = 0;
		for each(OpenStudio::Space^ space in m_osSpaces)
		{
			++i;
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double outputVariable = 0.0;
			try {
				outputVariable = EnergyModel::DoubleValueFromQuery(m_osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
			}

			// Map the outputVariable to a ratio between 0 and 1
			double ratio = (outputVariable - minValue) / (maxValue - minValue);
			DSCore::Color^ color = EnergyModel::GetColor(ratio);

			Dictionary<String^, Object^>^ attributes = gcnew Dictionary<String^, Object^>();
			attributes->Add("Value", outputVariable);
			attributes->Add("r", (long long int)color->Red);
			attributes->Add("g", (long long int)color->Green);
			attributes->Add("b", (long long int)color->Blue);
			attributes->Add("a", (long long int)50);
			delete color;
			data->Add(spaceName, attributes);
		}

		return data;
	}

	List<Modifiers::GeometryColor^>^ Simulation::Display(EnergyModel ^ energyModel, Dictionary<String^, Dictionary<String^, Object^>^>^ data)
	{
		List<Modifiers::GeometryColor^>^ dynamoGeometryColors = gcnew List<Modifiers::GeometryColor^>();
		List<Modifiers::GeometryColor^>^ dynamoApertures = gcnew List<Modifiers::GeometryColor^>();
		for each(Cell^ cell in energyModel->BuildingCells)
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
			catch (...)
			{
				throw gcnew Exception("A cell's name is not a string.");
			}

			DSCore::Color^ color = nullptr;// DSCore::Color::ByARGB(alpha, (int)r, (int)g, (int)b);
			bool hasKey = data->ContainsKey(strName);
			if (hasKey)
			{
				Dictionary<String^, Object^>^ attributes = data[strName];
				long long int a = (long long int)attributes["a"];
				long long int r = (long long int)attributes["r"];
				long long int g = (long long int)attributes["g"];
				long long int b = (long long int)attributes["b"];
				color = DSCore::Color::ByARGB((int)a, (int)r, (int)g, (int)b);
			}else
			{
				color = DSCore::Color::ByARGB(50, 128, 128, 128);
			}

			{
				bool hasGeometry = false;
				System::Object^ cellGeometry = cell->Geometry; Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(cellGeometry);
				if (dynamoGeometry != nullptr)
				{
					Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, color);
					dynamoGeometryColors->Add(dynamoGeometryColor);
					hasGeometry = true;
				}

				if (!hasGeometry)
				{
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
					}
				}
			}

			DSCore::Color^ contentColor = DSCore::Color::ByARGB(255, 128, 128, 128);
			List<Topologic::Topology^>^ subcontents = cell->SubContents;
			for each(Topologic::Topology^ subcontent in subcontents)
			{
				bool hasContentGeometry = false;
				System::Object^ contentGeometry = subcontent->Geometry;
				Autodesk::DesignScript::Geometry::Geometry^ dynamoContentGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(contentGeometry);
				if (dynamoContentGeometry!= nullptr)
				{
					Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoContentGeometry, contentColor);
					dynamoApertures->Add(dynamoGeometryColor);
					hasContentGeometry = true;
				}

				if (!hasContentGeometry)
				{
					// 2. Try a list of Dynamo geometries
					List<Object^>^ listOfObjects = dynamic_cast<List<Object^>^>(contentGeometry);
					if (listOfObjects != nullptr)
					{
						for each(Object^ object in listOfObjects)
						{
							Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(object);
							if (dynamoContentGeometry != nullptr)
							{
								Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoContentGeometry, contentColor);
								delete object;
								dynamoApertures->Add(dynamoGeometryColor);
							}
						}
					}
				}
			}

		}
		dynamoGeometryColors->AddRange(dynamoApertures);
		return dynamoGeometryColors;
	}

	List<Topologic::Cell^>^ Simulation::Topology::get()
	{
		return m_cells;
	}
}