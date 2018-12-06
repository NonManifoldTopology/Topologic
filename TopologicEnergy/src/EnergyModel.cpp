#include "EnergyModel.h"
#include "Simulation.h"

using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Linq;

namespace TopologicEnergy
{
	bool EnergyModel::CreateIdfFile(OpenStudio::Model^ osModel, String^ idfPathName)
	{
		OpenStudio::EnergyPlusForwardTranslator^ osForwardTranslator = gcnew OpenStudio::EnergyPlusForwardTranslator();
		OpenStudio::Workspace^ osWorkspace = osForwardTranslator->translateModel(osModel);
		OpenStudio::IdfFile^ osIdfFile = osWorkspace->toIdfFile();
		OpenStudio::Path^ osIdfPath = gcnew OpenStudio::Path(idfPathName);
		bool idfSaveCondition = osIdfFile->save(osIdfPath);
		return idfSaveCondition;
	}

	EnergyModel^ EnergyModel::Create(
		CellComplex^ building,
		Cluster^ shadingSurfaces,
		List<double>^ floorLevels,
		String^ buildingName,
		String^ buildingType,
		String^ defaultSpaceType,
		double glazingRatio,
		double coolingTemp,
		double heatingTemp,
		String^ weatherFilePath,
		String^ designDayFilePath,
		String^ openStudioTemplatePath//,
	)
	{
		numOfApertures = 0;
		numOfAppliedApertures = 0;
		CellComplex^ buildingCopy = building->Copy<CellComplex^>();

		// Create an OpenStudio model from the template, EPW, and DDY
		OpenStudio::Model^ osModel = GetModelFromTemplate(openStudioTemplatePath, weatherFilePath, designDayFilePath);

		double buildingHeight = Enumerable::Max(floorLevels);
		int numFloors = floorLevels->Count - 1;
		OpenStudio::Building^ osBuilding = ComputeBuilding(osModel, buildingName, buildingType, buildingHeight, numFloors, defaultSpaceType);
		List<Cell^>^ pBuildingCells = buildingCopy->Cells;

		// Create OpenStudio spaces
		List<OpenStudio::Space^>^ osSpaces = gcnew List<OpenStudio::Space^>();
		for each(Cell^ buildingCell in pBuildingCells)
		{
			int spaceNumber = 1;
			OpenStudio::Space^ osSpace = AddSpace(
				spaceNumber,
				buildingCell,
				buildingCopy,
				osModel,
				Autodesk::DesignScript::Geometry::Vector::ZAxis(),
				buildingHeight,
				floorLevels,
				glazingRatio,
				heatingTemp,
				coolingTemp
			);

			Dictionary<String^, Object^>^ attributes = gcnew Dictionary<String^, Object^>();
			attributes->Add("Name", osSpace->nameString());
			buildingCell->AddAttributesNoCopy(attributes);

			for each(OpenStudio::Space^ osExistingSpace in osSpaces)
			{
				osSpace->matchSurfaces(osExistingSpace);
			}

			osSpaces->Add(osSpace);
		}

		// Create shading surfaces
		if (shadingSurfaces != nullptr)
		{
			OpenStudio::ShadingSurfaceGroup^ osShadingGroup = gcnew OpenStudio::ShadingSurfaceGroup(osModel);
			List<Face^>^ contextFaces = shadingSurfaces->Faces;
			int faceIndex = 1;
			for each(Face^ contextFace in contextFaces)
			{
				AddShadingSurfaces(contextFace, osModel, osShadingGroup, faceIndex++);
			}
		}

		osModel->purgeUnusedResourceObjects();

		return gcnew EnergyModel(osModel, osBuilding, pBuildingCells, osSpaces);
	}

	Simulation^ EnergyModel::Analyze(EnergyModel ^ energyModel, String ^ openStudioExePath, String ^ openStudioOutputDirectory, bool run)
	{
		String^ oswPath = nullptr;
		Export(energyModel, openStudioOutputDirectory, oswPath);

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

		Simulation^ simulation = gcnew Simulation(
			energyModel->BuildingCells,
			oswPath,
			energyModel->OsModel,
			energyModel->OsSpaces,
			timestamp);

		return simulation;
	}

	void EnergyModel::Export(EnergyModel ^ energyModel, String ^ openStudioOutputDirectory)
	{
		String^ oswPath = nullptr;
		Export(energyModel, openStudioOutputDirectory, oswPath);
	}

	bool EnergyModel::SaveModel(OpenStudio::Model^ osModel, String^ osmPathName)
	{
		// Purge unused resources
		osModel->purgeUnusedResourceObjects();

		// Create a path string
		OpenStudio::Path^ osPath = gcnew OpenStudio::Path(osmPathName);
		bool osCondition = osModel->save(osPath, true);
		return osCondition;
	}

	OpenStudio::Model^ EnergyModel::GetModelFromTemplate(String^ osmTemplatePath, String^ epwWeatherPath, String^ ddyPath)
	{
		if (!File::Exists(osmTemplatePath))
		{
			throw gcnew FileNotFoundException("OSM file not found.");
		}
		if (!File::Exists(epwWeatherPath))
		{
			throw gcnew FileNotFoundException("EPW file not found.");
		}
		if (!File::Exists(ddyPath))
		{
			throw gcnew FileNotFoundException("DDY file not found.");
		}
		OpenStudio::Path^ osTemplatePath = gcnew OpenStudio::Path(osmTemplatePath);

		// Create an abstract model
		OpenStudio::OptionalModel^ osOptionalModel = OpenStudio::Model::load(osTemplatePath);
		OpenStudio::Model^ osModel = osOptionalModel->__ref__();

		// Read an EPW weather file
		OpenStudio::Path^ osEPWPath = gcnew OpenStudio::Path(epwWeatherPath);
		OpenStudio::EpwFile^ osEPWFile = gcnew OpenStudio::EpwFile(osEPWPath);
		OpenStudio::WeatherFile^ osWeatherFile = osModel->getWeatherFile();
		OpenStudio::WeatherFile::setWeatherFile(osModel, osEPWFile);

		// Read an DDY design days files
		OpenStudio::Path^ osDDYPath = gcnew OpenStudio::Path(ddyPath);
		OpenStudio::EnergyPlusReverseTranslator^ osTranslator = gcnew OpenStudio::EnergyPlusReverseTranslator();
		OpenStudio::OptionalModel^ tempModel01 = osTranslator->loadModel(osDDYPath);
		OpenStudio::Model^ tempModel02 = tempModel01->__ref__();
		OpenStudio::DesignDayVector^ designDays = tempModel02->getDesignDays();
		OpenStudio::DesignDayVector::DesignDayVectorEnumerator^ designDaysEnumerator = designDays->GetEnumerator();

		while (designDaysEnumerator->MoveNext())
		{
			OpenStudio::DesignDay^ aDesignDay = designDaysEnumerator->Current;
			OpenStudio::IdfObject^ anIdfObject = aDesignDay->idfObject();
			osModel->addObject(anIdfObject);
		}
		return osModel;
	}

	OpenStudio::ThermalZone^ EnergyModel::CreateThermalZone(OpenStudio::Model^ model, OpenStudio::Space^ space, double ceilingHeight, double heatingTemp, double coolingTemp)
	{
		// Create a thermal zone for the space
		OpenStudio::ThermalZone^ osThermalZone = gcnew OpenStudio::ThermalZone(model);
		osThermalZone->setName(space->name()->get() + "_THERMAL_ZONE");
		String^ name = osThermalZone->name()->get();
		osThermalZone->setUseIdealAirLoads(true);
		osThermalZone->setCeilingHeight(ceilingHeight);
		osThermalZone->setVolume(space->volume());

		// Assign Thermal Zone to space
		// aSpace.setThermalZone(osThermalZone);//Not available in C#
		OpenStudio::UUID^ tzHandle = osThermalZone->handle();
		int location = 10;
		space->setPointer(location, tzHandle);

		OpenStudio::ScheduleConstant^ heatingScheduleConstant = gcnew OpenStudio::ScheduleConstant(model);
		heatingScheduleConstant->setValue(heatingTemp);
		OpenStudio::ScheduleConstant^ coolingScheduleConstant = gcnew OpenStudio::ScheduleConstant(model);
		coolingScheduleConstant->setValue(coolingTemp);

		// Create a Thermostat
		OpenStudio::ThermostatSetpointDualSetpoint^ osThermostat = gcnew OpenStudio::ThermostatSetpointDualSetpoint(model);

		// Set Heating and Cooling Schedules on the Thermostat
		osThermostat->setHeatingSetpointTemperatureSchedule(heatingScheduleConstant);
		osThermostat->setCoolingSetpointTemperatureSchedule(coolingScheduleConstant);

		// Assign Thermostat to the Thermal Zone
		osThermalZone->setThermostatSetpointDualSetpoint(osThermostat);
		return osThermalZone;
	}

	OpenStudio::BuildingStory^ EnergyModel::AddBuildingStory(OpenStudio::Model^ model, int floorNumber)
	{
		OpenStudio::BuildingStory^ osBuildingStory = gcnew OpenStudio::BuildingStory(model);
		osBuildingStory->setName("STORY_" + floorNumber);
		osBuildingStory->setDefaultConstructionSet(getDefaultConstructionSet(model));
		osBuildingStory->setDefaultScheduleSet(getDefaultScheduleSet(model));
		return osBuildingStory;
	}

	OpenStudio::SubSurface ^ EnergyModel::CreateSubSurface(List<Topologic::Vertex^>^ vertices, OpenStudio::Model^ osModel)
	{
		OpenStudio::Point3dVector^ osWindowFacePoints = gcnew OpenStudio::Point3dVector();
		for each(Vertex^ vertex in vertices)
		{
			Object^ pVertexGeometry = vertex->Geometry;
			Autodesk::DesignScript::Geometry::Point^ pAperturePoint =
				dynamic_cast<Autodesk::DesignScript::Geometry::Point^>(pVertexGeometry);
			if (pAperturePoint == nullptr)
			{
				continue;
			}
			OpenStudio::Point3d^ osPoint = gcnew OpenStudio::Point3d(
				pAperturePoint->X,
				pAperturePoint->Y,
				pAperturePoint->Z);
			osWindowFacePoints->Add(osPoint);
		}

		OpenStudio::SubSurface^ osWindowSubSurface = gcnew OpenStudio::SubSurface(osWindowFacePoints, osModel);
		return osWindowSubSurface;
	}

	OpenStudio::Building^ EnergyModel::ComputeBuilding(
		OpenStudio::Model^ osModel,
		String^ buildingName,
		String^ buildingType,
		double buildingHeight,
		int numFloors,
		String^ spaceType)
	{
		OpenStudio::Building^ osBuilding = osModel->getBuilding();
		//building.setNumberOfStories(stories);
		osBuilding->setStandardsNumberOfStories(numFloors);
		osBuilding->setDefaultConstructionSet(getDefaultConstructionSet(osModel));
		osBuilding->setDefaultScheduleSet(getDefaultScheduleSet(osModel));
		osBuilding->setName(buildingName);
		osBuilding->setStandardsBuildingType(buildingType);
		double floorToFloorHeight = (double)buildingHeight / (double)numFloors;
		osBuilding->setNominalFloortoFloorHeight(floorToFloorHeight);
		// Get all space types and find the one that matches
		OpenStudio::SpaceTypeVector^ spaceTypes = osModel->getSpaceTypes();
		OpenStudio::SpaceTypeVector::SpaceTypeVectorEnumerator^ spaceTypesEnumerator = spaceTypes->GetEnumerator();
		while (spaceTypesEnumerator->MoveNext())
		{
			OpenStudio::SpaceType^ aSpaceType = spaceTypesEnumerator->Current;
			String^ spaceTypeName = aSpaceType->name()->__str__();
			if (spaceTypeName == spaceType)
			{
				osBuilding->setSpaceType(aSpaceType);
			}
		}
		buildingStories = CreateBuildingStories(osModel, numFloors);
		return osBuilding;
	}

	List<OpenStudio::BuildingStory^>^ EnergyModel::CreateBuildingStories(OpenStudio::Model^ osModel, int numFloors)
	{
		List<OpenStudio::BuildingStory^>^ osBuildingStories = gcnew List<OpenStudio::BuildingStory^>();
		for (int i = 0; i < numFloors; i++)
		{
			osBuildingStories->Add(AddBuildingStory(osModel, (i + 1)));
		}
		return osBuildingStories;
	}

	OpenStudio::SqlFile^ EnergyModel::CreateSqlFile(OpenStudio::Model ^ osModel, String^ sqlFilePath)
	{
		OpenStudio::Path^ osSqlFilePath = gcnew OpenStudio::Path(sqlFilePath);
		OpenStudio::SqlFile^ osSqlFile = gcnew OpenStudio::SqlFile(osSqlFilePath);
		if (osSqlFile == nullptr)
		{
			throw gcnew Exception("Fails to create an SQL output file");
		}
		bool isSuccessful = osModel->setSqlFile(osSqlFile);
		if (!isSuccessful)
		{
			throw gcnew Exception("Fails to create an SQL output file");
		}
		return osSqlFile;
	}

	List<Modifiers::GeometryColor^>^ EnergyModel::AnalyzeSqlFile(OpenStudio::SqlFile ^ osSqlFile, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ spaces, List<Cell^>^ buildingCells,
		String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits)
	{
		osModel->setSqlFile(osSqlFile);
		OpenStudio::OptionalDouble^ totalSE = osSqlFile->totalSiteEnergy();
		double totalSEGJ = totalSE->__float__();
		double totalSEkwh = totalSEGJ * 277.8;
		//Print("Total Site Energy use: " + (totalSEGJ as string) + " Gigajoules (" + (totalSEkwh as string) + " kWh)")
		OpenStudio::OptionalDouble^ totalEU = osSqlFile->electricityTotalEndUses();
		double totalEUGJ = totalEU->__float__();
		double totalEUkwh = totalEUGJ * 277.8;
		//Print("Total Electricity End Uses: " + (totalEUGJ as string) + " Gigajoules (" + (totalEUkwh as string) + " kWh)")
		OpenStudio::OptionalString^ spaceNameTemp = spaces[0]->name();
		String^ spaceName = spaceNameTemp->get();
		String^ EPRowName = spaceName + "_THERMAL_ZONE";
		//String^ EPRowName = "test" + "_TZ";

		//If min and max values are not specified then calculate them from the data
		// TODO: Take custom values
		//Set an initial value for minimum and maximum values
		double maxValue = 0.0;
		try {
			maxValue = DoubleValueFromQuery(osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
		}
		catch (...)
		{
			maxValue = 10000.0;
		}
		double minValue = maxValue;
		for each(OpenStudio::Space^ space in spaces)
		{
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double aValue = 10000.0;
			try{
				aValue = DoubleValueFromQuery(osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				aValue = 10000.0;
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
		for each(OpenStudio::Space^ space in spaces)
		{
			Cell^ selectedCell = buildingCells[i];
			++i;
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double outputVariable = 0.0;
			try {
				outputVariable = DoubleValueFromQuery(osSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				outputVariable = 10000.0;
			}
			
			// Map the outputVariable to a ratio between 0 and 1
			double ratio = (outputVariable - minValue) / (maxValue - minValue);
			DSCore::Color^ aColor = GetColor(ratio);
			System::Object^ cellGeometry = selectedCell->Geometry;
			
			// 1. Try a Dynamo geometry
			Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(cellGeometry);
			if (dynamoGeometry != nullptr)
			{
				Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, aColor);
				dynamoGeometryColors->Add(dynamoGeometryColor);
				//delete cellGeometry;
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
			
		//this is the important step otherwise the sqlfile stays locked
		osSqlFile->close();
		delete osSqlFile;

		return dynamoGeometryColors;
	}

	double EnergyModel::DoubleValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits)
	{
		double doubleValue = 0.0;
		String^ query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='" + EPReportName + "' AND ReportForString='" + EPReportForString + "' AND TableName = '" + EPTableName + "' AND RowName = '" + EPRowName + "' AND ColumnName= '" + EPColumnName + "' AND Units='" + EPUnits + "'";
		OpenStudio::OptionalDouble^ osDoubleValue = sqlFile->execAndReturnFirstDouble(query);
		if (osDoubleValue->is_initialized())
		{
			doubleValue = osDoubleValue->get();
		}
		else
		{
			throw gcnew Exception("Fails to get a double value from the SQL file.");
		}
		return doubleValue;
	}

	String^ EnergyModel::StringValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits)
	{
		String^ query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='" + EPReportName + "' AND ReportForString='" + EPReportForString + "' AND TableName = '" + EPTableName + "' AND RowName = '" + EPRowName + "' AND ColumnName= '" + EPColumnName + "' AND Units='" + EPUnits + "'";
		return sqlFile->execAndReturnFirstString(query)->get();
	}

	int EnergyModel::IntValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits)
	{
		String^ query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='" + EPReportName + "' AND ReportForString='" + EPReportForString + "' AND TableName = '" + EPTableName + "' AND RowName = '" + EPRowName + "' AND ColumnName= '" + EPColumnName + "' AND Units='" + EPUnits + "'";
		return sqlFile->execAndReturnFirstInt(query)->get();
	}

	void EnergyModel::Export(EnergyModel ^ energyModel, String ^ openStudioOutputDirectory, String ^% oswPath)
	{
		// Add timestamp to the output file name
		String^ openStudioOutputTimeStampPath = Path::GetDirectoryName(openStudioOutputDirectory + "\\") + "\\" +
			Path::GetFileNameWithoutExtension(energyModel->BuildingName) + "_" +
			DateTime::Now.ToString("yyyy-MM-dd_HH-mm-ss-fff") +
			".osm";
		// Save model to an OSM file
		bool saveCondition = SaveModel(energyModel->m_osModel, openStudioOutputTimeStampPath);

		if (!saveCondition)
		{
			throw gcnew Exception("Failed to save the OSM file.");
		}

		OpenStudio::WorkflowJSON^ workflow = gcnew OpenStudio::WorkflowJSON();
		try {
			String^ osmFilename = Path::GetFileNameWithoutExtension(openStudioOutputTimeStampPath);
			String^ osmDirectory = Path::GetDirectoryName(openStudioOutputTimeStampPath);
			oswPath = osmDirectory + "\\" + osmFilename + ".osw";
			OpenStudio::Path^ osOswPath = gcnew OpenStudio::Path(oswPath);
			workflow->setSeedFile(gcnew OpenStudio::Path(openStudioOutputTimeStampPath));
			workflow->setWeatherFile(gcnew OpenStudio::Path());
			workflow->saveAs(osOswPath);
		}
		catch (...)
		{
			throw gcnew Exception("Failed to create the OSM and OSW files.");
		}
	}


	OpenStudio::DefaultScheduleSet^ EnergyModel::getDefaultScheduleSet(OpenStudio::Model^ model)
	{
		// Get list of default schedule sets
		OpenStudio::DefaultScheduleSetVector^ defaultScheduleSets = model->getDefaultScheduleSets();
		OpenStudio::DefaultScheduleSetVector::DefaultScheduleSetVectorEnumerator^ defSchedEnum = defaultScheduleSets->GetEnumerator();
		defSchedEnum->MoveNext();
		defaultScheduleSet = defSchedEnum->Current;
		return defaultScheduleSet;
	}

	OpenStudio::DefaultConstructionSet^ EnergyModel::getDefaultConstructionSet(OpenStudio::Model ^ model)
	{
		// Get list of default construction sets
		OpenStudio::DefaultConstructionSetVector^ defaultConstructionSets = model->getDefaultConstructionSets();
		// Get the first item and use as the default construction set
		OpenStudio::DefaultConstructionSetVector::DefaultConstructionSetVectorEnumerator^ defConEnum = defaultConstructionSets->GetEnumerator();
		defConEnum->MoveNext();
		defaultConstructionSet = defConEnum->Current;
		return defaultConstructionSet;
	}

	OpenStudio::Space^ EnergyModel::AddSpace(
		int spaceNumber,
		Cell^ cell,
		CellComplex^ cellComplex,
		OpenStudio::Model^ osModel,
		Autodesk::DesignScript::Geometry::Vector^ upVector,
		double buildingHeight,
		List<double>^ floorLevels,
		double glazingRatio,
		double heatingTemp,
		double coolingTemp)
	{
		OpenStudio::Space^ osSpace = gcnew OpenStudio::Space(osModel);

		int storyNumber = StoryNumber(cell, buildingHeight, floorLevels);
		OpenStudio::BuildingStory^ buildingStory = buildingStories[storyNumber];
		osSpace->setName(buildingStory->name()->get() + "_SPACE_" + spaceNumber.ToString());
		osSpace->setBuildingStory(buildingStory);
		osSpace->setDefaultConstructionSet(getDefaultConstructionSet(osModel));
		osSpace->setDefaultScheduleSet(getDefaultScheduleSet(osModel));

		List<Face^>^ faces = cell->Faces;
		List<OpenStudio::Point3dVector^>^ facePointsList = gcnew List<OpenStudio::Point3dVector^>();
		for each(Face^ face in faces)
		{
			OpenStudio::Point3dVector^ facePoints = GetFacePoints(face);
			facePointsList->Add(facePoints);
		}

		for (int i = 0; i < faces->Count; ++i)
		{
			AddSurface(i + 1, faces[i], cell, cellComplex, facePointsList[i], osSpace, osModel, upVector, glazingRatio);
		}

		// Get all space types
		OpenStudio::SpaceTypeVector^ osSpaceTypes = osModel->getSpaceTypes();
		OpenStudio::SpaceTypeVector::SpaceTypeVectorEnumerator^ osSpaceTypesEnumerator = osSpaceTypes->GetEnumerator();
		int spaceTypeCount = osSpaceTypes->Count;
		while (osSpaceTypesEnumerator->MoveNext())
		{
			OpenStudio::SpaceType^ osSpaceType = osSpaceTypesEnumerator->Current;
			OpenStudio::OptionalString^ osSpaceTypeOptionalString = osSpaceType->name();
			String^ spaceTypeName = osSpaceTypeOptionalString->__str__();
			if (spaceTypeName == "ASHRAE 189::1-2009 ClimateZone 4-8 MediumOffice")
			{
				osSpace->setSpaceType(osSpaceType);
			}
		}

		List<double>^ minMax = Topologic::Utility::CellUtility::GetMinMax(cell);
		double minZ = minMax[4];
		double maxZ = minMax[5];
		double ceilingHeight = Math::Abs(maxZ - minZ);

		OpenStudio::ThermalZone^ thermalZone = CreateThermalZone(osModel, osSpace, ceilingHeight, heatingTemp, coolingTemp);

		return osSpace;
	}


	void EnergyModel::AddShadingSurfaces(Cell^ buildingCell, OpenStudio::Model^ osModel)
	{
		OpenStudio::ShadingSurfaceGroup^ osShadingGroup = gcnew OpenStudio::ShadingSurfaceGroup(osModel);
		List<Face^>^ faceList = buildingCell->Faces;
		int faceIndex = 1;
		for each(Face^ face in faceList)
		{
			List<Vertex^>^ vertices = face->Vertices;
			OpenStudio::Point3dVector^ facePoints = gcnew OpenStudio::Point3dVector();

			for each(Vertex^ aVertex in vertices)
			{
				Autodesk::DesignScript::Geometry::Point^ p = safe_cast<Autodesk::DesignScript::Geometry::Point^>(aVertex->Geometry);
				OpenStudio::Point3d^ aPoint = gcnew OpenStudio::Point3d(p->X, p->Y, p->Z);
				facePoints->Add(aPoint);
			}

			OpenStudio::ShadingSurface^ aShadingSurface = gcnew OpenStudio::ShadingSurface(facePoints, osModel);

			String^ surfaceName = buildingCell->ToString() + "_SHADINGSURFACE_" + (faceIndex.ToString());
			aShadingSurface->setName(surfaceName);
			aShadingSurface->setShadingSurfaceGroup(osShadingGroup);

			++faceIndex;
		}
	}

	void EnergyModel::AddShadingSurfaces(Face ^ buildingFace, OpenStudio::Model ^ osModel, OpenStudio::ShadingSurfaceGroup^ osShadingGroup, int faceIndex)
	{
		List<Vertex^>^ vertices = buildingFace->Vertices;
		OpenStudio::Point3dVector^ facePoints = gcnew OpenStudio::Point3dVector();

		for each(Vertex^ aVertex in vertices)
		{
			Autodesk::DesignScript::Geometry::Point^ p = safe_cast<Autodesk::DesignScript::Geometry::Point^>(aVertex->Geometry);
			OpenStudio::Point3d^ aPoint = gcnew OpenStudio::Point3d(p->X, p->Y, p->Z);
			facePoints->Add(aPoint);
		}

		OpenStudio::ShadingSurface^ aShadingSurface = gcnew OpenStudio::ShadingSurface(facePoints, osModel);

		String^ surfaceName = "SHADINGSURFACE_" + (faceIndex.ToString());
		aShadingSurface->setName(surfaceName);
		aShadingSurface->setShadingSurfaceGroup(osShadingGroup);
	}

	OpenStudio::Surface^ EnergyModel::AddSurface(
		int surfaceNumber,
		Face^ buildingFace,
		Cell^ buildingSpace,
		CellComplex^ cellComplex,
		OpenStudio::Point3dVector^ osFacePoints,
		OpenStudio::Space^ osSpace,
		OpenStudio::Model^ osModel,
		Autodesk::DesignScript::Geometry::Vector^ upVector,
		double glazingRatio)
	{
		OpenStudio::Construction^ osInteriorCeilingType = nullptr;
		OpenStudio::Construction^ osExteriorRoofType = nullptr;
		OpenStudio::Construction^ osInteriorFloorType = nullptr;
		OpenStudio::Construction^ osInteriorWallType = nullptr;
		OpenStudio::Construction^ osExteriorDoorType = nullptr;
		OpenStudio::Construction^ osExteriorWallType = nullptr;
		OpenStudio::Construction^ osExteriorWindowType = nullptr;
		int subsurfaceCounter = 1;

		OpenStudio::ConstructionVector^ osConstructionTypes = osModel->getConstructions();
		OpenStudio::ConstructionVector::ConstructionVectorEnumerator^ osConstructionTypesEnumerator =
			osConstructionTypes->GetEnumerator();
		int constructionTypeCount = osConstructionTypes->Count;

		while (osConstructionTypesEnumerator->MoveNext())
		{
			OpenStudio::Construction^ osConstruction = osConstructionTypesEnumerator->Current;
			OpenStudio::OptionalString^ osConstructionTypeOptionalString = osConstruction->name();
			String^ constructionTypeName = osConstructionTypeOptionalString->__str__();
			if (constructionTypeName->Equals("000 Interior Ceiling"))
			{
				osInteriorCeilingType = osConstruction;
			}
			else if (constructionTypeName->Equals("000 Interior Floor"))
			{
				osInteriorFloorType = osConstruction;
			}
			else if (constructionTypeName->Equals("000 Interior Wall"))
			{
				osInteriorWallType = osConstruction;
			}
			else if (constructionTypeName->Equals("ASHRAE 189.1-2009 ExtWindow ClimateZone 4-5"))
			{
				osExteriorWindowType = osConstruction;
			}
			else if (constructionTypeName->Equals("000 Exterior Door"))
			{
				osExteriorDoorType = osConstruction;
			}
			else if (constructionTypeName->Equals("ASHRAE 189.1-2009 ExtRoof IEAD ClimateZone 2-5"))
			{
				osExteriorRoofType = osConstruction;
			}
			else if (constructionTypeName->Equals("ASHRAE 189.1-2009 ExtWall SteelFrame ClimateZone 4-8"))
			{
				osExteriorWallType = osConstruction;
			}
		} // while (osConstructionTypesEnumerator.MoveNext())

		int adjCount = AdjacentCellCount(buildingFace);
		//HACK
		/*if (adjCount > 1)
		{
			osFacePoints->Reverse();
		}*/

		OpenStudio::Surface^ osSurface = gcnew OpenStudio::Surface(osFacePoints, osModel);
		osSurface->setSpace(osSpace);
		OpenStudio::OptionalString^ osSpaceOptionalString = osSpace->name();
		String^ spaceName = osSpace->name()->get();
		String^ surfaceName = osSpace->name()->get() + "_SURFACE_" + surfaceNumber.ToString();
		bool isUnderground = IsUnderground(buildingFace);
		FaceType faceType = CalculateFaceType(buildingFace, osFacePoints, buildingSpace, upVector);
		osSurface->setName(surfaceName);

		if ((faceType == FACE_ROOFCEILING) && (adjCount > 1))
		{

			osSurface->setOutsideBoundaryCondition("Surface");
			osSurface->setSurfaceType("RoofCeiling");
			osSurface->setConstruction(osInteriorCeilingType);
			osSurface->setSunExposure("NoSun");
			osSurface->setWindExposure("NoWind");
		}
		else if ((faceType == FACE_ROOFCEILING) && (adjCount < 2) && (!isUnderground))
		{
			OpenStudio::Vector3d^ pSurfaceNormal = osSurface->outwardNormal();

			double x = pSurfaceNormal->x();
			double y = pSurfaceNormal->y();
			double z = pSurfaceNormal->z();
			pSurfaceNormal->normalize();
			OpenStudio::Vector3d^ upVector = gcnew OpenStudio::Vector3d(0, 0, 1.0);
			// If the normal does not point downward, flip it. Use dot product.
			double dotProduct = pSurfaceNormal->dot(upVector);
			if (dotProduct < 0.98)
			{
				OpenStudio::Point3dVector^ surfaceVertices = osSurface->vertices();
				surfaceVertices->Reverse();
				osSurface->setVertices(surfaceVertices);
			}
			osSurface->setOutsideBoundaryCondition("Outdoors");
			osSurface->setSurfaceType("RoofCeiling");
			osSurface->setConstruction(osExteriorRoofType);
			osSurface->setSunExposure("SunExposed");
			osSurface->setWindExposure("WindExposed");
		}
		else if ((faceType == FACE_ROOFCEILING) && (adjCount < 2) && isUnderground)
		{
			OpenStudio::Vector3d^ pSurfaceNormal = osSurface->outwardNormal();

			double x = pSurfaceNormal->x();
			double y = pSurfaceNormal->y();
			double z = pSurfaceNormal->z();
			pSurfaceNormal->normalize();
			OpenStudio::Vector3d^ upVector = gcnew OpenStudio::Vector3d(0, 0, 1.0);
			// If the normal does not point downward, flip it. Use dot product.
			double dotProduct = pSurfaceNormal->dot(upVector);
			if (dotProduct < 0.98)
			{
				OpenStudio::Point3dVector^ surfaceVertices = osSurface->vertices();
				surfaceVertices->Reverse();
				osSurface->setVertices(surfaceVertices);
			}
			osSurface->setOutsideBoundaryCondition("Ground");
			osSurface->setSurfaceType("RoofCeiling");
			osSurface->setConstruction(osExteriorRoofType);
			osSurface->setSunExposure("NoSun");
			osSurface->setWindExposure("NoWind");
		}
		else if ((faceType == FACE_FLOOR) && (adjCount > 1))
		{
			osSurface->setOutsideBoundaryCondition("Surface");
			osSurface->setSurfaceType("Floor");
			osSurface->setConstruction(osInteriorFloorType);
			osSurface->setSunExposure("NoSun");
			osSurface->setWindExposure("NoWind");
		}
		else if ((faceType == FACE_FLOOR) && (adjCount < 2))
		{
			OpenStudio::Vector3d^ pSurfaceNormal = osSurface->outwardNormal();

			double x = pSurfaceNormal->x();
			double y = pSurfaceNormal->y();
			double z = pSurfaceNormal->z();
			pSurfaceNormal->normalize();
			OpenStudio::Vector3d^ downwardVector = gcnew OpenStudio::Vector3d(0, 0, -1.0);
			// If the normal does not point downward, flip it. Use dot product.
			double dotProduct = pSurfaceNormal->dot(downwardVector);
			if (dotProduct < 0.98)
			{
				OpenStudio::Point3dVector^ surfaceVertices = osSurface->vertices();
				surfaceVertices->Reverse();
				osSurface->setVertices(surfaceVertices);
			}
			osSurface->setOutsideBoundaryCondition("Ground");
			osSurface->setSurfaceType("Floor");
			osSurface->setConstruction(osExteriorWallType);
			osSurface->setSunExposure("NoSun");
			osSurface->setWindExposure("NoWind");
		}
		else if ((faceType == FACE_WALL) && (adjCount > 1)) // internal wall
		{
			osSurface->setOutsideBoundaryCondition("Surface");
			osSurface->setSurfaceType("Wall");
			osSurface->setConstruction(osInteriorWallType);
			osSurface->setSunExposure("NoSun");
			osSurface->setWindExposure("NoWind");
		}
		else if ((faceType == FACE_WALL) && (adjCount < 2) && isUnderground) // external wall underground
		{
			osSurface->setOutsideBoundaryCondition("Ground");
			osSurface->setSurfaceType("Wall");
			osSurface->setConstruction(osExteriorWallType);
			osSurface->setSunExposure("NoSun");
			osSurface->setWindExposure("NoWind");
		}
		else if ((faceType == FACE_WALL) && (adjCount < 2) && (!isUnderground)) // external wall overground
		{
			osSurface->setOutsideBoundaryCondition("Outdoors");
			osSurface->setSurfaceType("Wall");
			osSurface->setConstruction(osExteriorWallType);
			osSurface->setSunExposure("SunExposed");
			osSurface->setWindExposure("WindExposed");

			if (glazingRatio >= 1.0)
			{
				throw gcnew Exception("Glazing ratio must have a value lower than 1.0 (negative values included).");
			}else if (glazingRatio > 0.0 && glazingRatio < 1.0)
			{
				// Triangulate the Windows
				List<Vertex^>^ scaledVertices = ScaleFaceVertices(buildingFace, glazingRatio);

				for (int i = 0; i < scaledVertices->Count - 2; ++i)
				{
					OpenStudio::Point3dVector^ osWindowFacePoints = gcnew OpenStudio::Point3dVector();

					Autodesk::DesignScript::Geometry::Point^ pDynamoPoint1 =
						safe_cast<Autodesk::DesignScript::Geometry::Point^>(scaledVertices[0]->Geometry);
					OpenStudio::Point3d^ p1 = gcnew OpenStudio::Point3d(
						pDynamoPoint1->X,
						pDynamoPoint1->Y,
						pDynamoPoint1->Z);

					Autodesk::DesignScript::Geometry::Point^ pDynamoPoint2 =
						safe_cast<Autodesk::DesignScript::Geometry::Point^>(scaledVertices[i + 1]->Geometry);
					OpenStudio::Point3d^ p2 = gcnew OpenStudio::Point3d(
						pDynamoPoint2->X,
						pDynamoPoint2->Y,
						pDynamoPoint2->Z);

					Autodesk::DesignScript::Geometry::Point^ pDynamoPoint3 =
						safe_cast<Autodesk::DesignScript::Geometry::Point^>(scaledVertices[i + 2]->Geometry);
					OpenStudio::Point3d^ p3 = gcnew OpenStudio::Point3d(
						pDynamoPoint3->X,
						pDynamoPoint3->Y,
						pDynamoPoint3->Z);

					osWindowFacePoints->Add(p1);
					osWindowFacePoints->Add(p2);
					osWindowFacePoints->Add(p3);

					OpenStudio::SubSurface^ osWindowSubSurface = gcnew OpenStudio::SubSurface(osWindowFacePoints, osModel);
					osWindowSubSurface->setSubSurfaceType("FixedWindow");
					osWindowSubSurface->setSurface(osSurface);
					osWindowSubSurface->setName(osSurface->name()->get() + "_SUBSURFACE_" + subsurfaceCounter.ToString());
					subsurfaceCounter++;

					double grossSubsurfaceArea = osWindowSubSurface->grossArea();
					double netSubsurfaceArea = osWindowSubSurface->netArea();
					double grossSurfaceArea = osSurface->grossArea();
					double netSurfaceArea = osSurface->netArea();
				} // for (int i = 0; i < scaledVertices->Count - 2; ++i)
			}
			else // glazingRatio <= 0.0
			{
				// Use the surface apertures
				List<Topology^>^ pContents = buildingFace->Contents;
				for each(Topology^ pContent in pContents)
				{
					Aperture^ pAperture = dynamic_cast<Aperture^>(pContent);
					if (pAperture == nullptr)
					{
						continue;
					}

					Face^ pFaceAperture = dynamic_cast<Face^>(pAperture->Topology());
					if (pAperture == nullptr)
					{
						continue;
					}
					// skip small triangles
					double area = Topologic::Utility::FaceUtility::Area(pFaceAperture);
					if (area <= 0.1)
					{
						continue;
					}
					Wire^ pApertureWire = pFaceAperture->ExternalBoundary;
					List<Vertex^>^ pApertureVertices = pApertureWire->Vertices;
					//OpenStudio::SubSurface^ osWindowSubSurface = gcnew OpenStudio::SubSurface(osWindowFacePoints, osModel);
					OpenStudio::SubSurface^ osWindowSubSurface = CreateSubSurface(pApertureVertices, osModel);
					double dotProduct = osWindowSubSurface->outwardNormal()->dot(osSurface->outwardNormal());
					if (dotProduct < -0.99) // flipped
					{
						pApertureVertices->Reverse();
						osWindowSubSurface->remove();
						osWindowSubSurface = CreateSubSurface(pApertureVertices, osModel);
					}
					else if (dotProduct > -0.99 && dotProduct < 0.99)
					{
						throw gcnew Exception("There is a non-coplanar subsurface.");
					}

					numOfApertures++;

					double grossSubsurfaceArea = osWindowSubSurface->grossArea();
					double netSubsurfaceArea = osWindowSubSurface->netArea();
					double grossSurfaceArea = osSurface->grossArea();
					double netSurfaceArea = osSurface->netArea();
					if (grossSubsurfaceArea > 0.1)
					{
						osWindowSubSurface->setSubSurfaceType("FixedWindow");
						bool result = osWindowSubSurface->setSurface(osSurface);
						if (result)
						{
							osWindowSubSurface->setName(osSurface->name()->get() + "_SUBSURFACE_" + subsurfaceCounter.ToString());
							subsurfaceCounter++;
							numOfAppliedApertures++;
						}
					}
					else
					{
						osWindowSubSurface->remove();
					}
				}
			}
		}

		return osSurface;
	}

	List<Vertex^>^ EnergyModel::ScaleFaceVertices(Face^ buildingFace, double scaleFactor)
	{
		List<Vertex^>^ scaledVertices = gcnew List<Vertex^>();
		Vertex^ faceCentre = GetFaceCentre(buildingFace);
		Autodesk::DesignScript::Geometry::Point^ faceCenterPoint =
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(faceCentre->Geometry);

		Wire^ pApertureWire = buildingFace->ExternalBoundary;
		List<Vertex^>^ vertices = pApertureWire->Vertices;
		vertices->Reverse();

		double sqrtScaleFactor = Math::Sqrt(scaleFactor);
		for each(Vertex^ aVertex in vertices)
		{
			Autodesk::DesignScript::Geometry::Point^ originalPoint =
				safe_cast<Autodesk::DesignScript::Geometry::Point^>(aVertex->Geometry);
			Autodesk::DesignScript::Geometry::Point^ scaledVertex = originalPoint->Subtract(faceCenterPoint->AsVector());
			scaledVertex = safe_cast<Autodesk::DesignScript::Geometry::Point^>(scaledVertex->Scale(sqrtScaleFactor));
			scaledVertex = scaledVertex->Add(faceCenterPoint->AsVector());
			scaledVertices->Add(safe_cast<Vertex^>(Topology::ByGeometry(scaledVertex)));
		}
		return scaledVertices;
	}

	Vertex^ EnergyModel::GetFaceCentre(Face^ buildingFace)
	{
		List<Vertex^>^ vertices = buildingFace->Vertices;
		Autodesk::DesignScript::Geometry::Point^ sumPoint = Autodesk::DesignScript::Geometry::Point::ByCoordinates(0, 0, 0);

		// assume vertices.count > 0
		if (vertices->Count < 3)
		{
			throw gcnew Exception("Invalid face");
		}

		for each(Vertex^ v in vertices)
		{
			Autodesk::DesignScript::Geometry::Point^ p =
				safe_cast<Autodesk::DesignScript::Geometry::Point^>(v->Geometry);
			sumPoint = sumPoint->Add(p->AsVector());
		}
		return safe_cast<Vertex^>(Topology::ByGeometry(safe_cast<Autodesk::DesignScript::Geometry::Point^>(sumPoint->Scale(1.0 / (double)vertices->Count))));
	}

	OpenStudio::Point3dVector^ EnergyModel::GetFacePoints(Face^ buildingFace)
	{
		Wire^ buildingOuterWire = buildingFace->ExternalBoundary;
		List<Vertex^>^ vertices = buildingOuterWire->Vertices;

		OpenStudio::Point3dVector^ osFacePoints = gcnew OpenStudio::Point3dVector();

		for each(Vertex^ v in vertices)
		{
			Autodesk::DesignScript::Geometry::Point^ point =
				safe_cast<Autodesk::DesignScript::Geometry::Point^>(v->Geometry);
			OpenStudio::Point3d^ osPoint = gcnew OpenStudio::Point3d(
				point->X,
				point->Y,
				point->Z);

			osFacePoints->Add(osPoint);
		}

		return osFacePoints;
	}

	bool EnergyModel::IsUnderground(Face^ buildingFace)
	{
		List<Vertex^>^ vertices = buildingFace->Vertices;

		for each(Vertex^ aVertex in vertices)
		{
			Autodesk::DesignScript::Geometry::Point^ point =
				safe_cast<Autodesk::DesignScript::Geometry::Point^>(aVertex->Geometry);

			if (point->Z > 0.0)
			{
				return false;
			}
		}

		return true;
	}

	FaceType EnergyModel::CalculateFaceType(Face^ buildingFace, OpenStudio::Point3dVector^% facePoints, Cell^ buildingSpace, Autodesk::DesignScript::Geometry::Vector^ upVector)
	{
		FaceType faceType = FACE_WALL;
		List<Face^>^ faces = Topologic::Utility::FaceUtility::Triangulate(buildingFace, 0.01);
		if (faces->Count == 0)
		{
			throw gcnew Exception("Failed to triangulate a face.");
		}

		Face^ firstFace = faces[0];
		Vertex^ centerPoint = Topologic::Utility::TopologyUtility::CenterOfMass(firstFace);
		Autodesk::DesignScript::Geometry::Point^ dynamoCenterPoint =
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(centerPoint->Geometry);

		List<Vertex^>^ vertices = buildingFace->Vertices;
		Autodesk::DesignScript::Geometry::Point^ p1 =
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(vertices[0]->Geometry);
		Autodesk::DesignScript::Geometry::Point^ p2 =
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(vertices[1]->Geometry);
		Autodesk::DesignScript::Geometry::Point^ p3 =
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(vertices[2]->Geometry);
		Autodesk::DesignScript::Geometry::Vector^ dir = (p2->Subtract(p1->AsVector()))->AsVector()->Cross((p3->Subtract(p1->AsVector()))->AsVector());
		Autodesk::DesignScript::Geometry::Vector^ faceNormal = dir->Normalized();
		double faceAngle = faceNormal->AngleWithVector(upVector);
		Autodesk::DesignScript::Geometry::Point^ pDynamoOffsetPoint =
			dynamic_cast<Autodesk::DesignScript::Geometry::Point^>(dynamoCenterPoint->Translate(faceNormal->Scale(0.001)));

		Vertex^ pOffsetVertex = safe_cast<Vertex^>(Topology::ByGeometry(pDynamoOffsetPoint));

		if (faceAngle < 5.0 || faceAngle > 175.0)
		{
			bool isInside = Topologic::Utility::CellUtility::Contains(buildingSpace, pOffsetVertex);
			// The offset vertex has to be false, so if isInside is true, reverse the face.

			//bool isFaceInsideOutWithRespectToTheCell = ;

			if (isInside)
			{
				facePoints->Reverse();
				faceNormal = faceNormal->Reverse();
				faceAngle = faceNormal->AngleWithVector(upVector);
			}

			if (faceAngle < 5.0)
			{
				faceType = FACE_ROOFCEILING;
			}
			else if (faceAngle > 175.0)
			{
				faceType = FACE_FLOOR;
			}
		}
		return faceType;
	}

	int EnergyModel::AdjacentCellCount(Face^ buildingFace)
	{
		return buildingFace->Cells->Count;
	}

	int EnergyModel::StoryNumber(Cell^ buildingCell, double buildingHeight, List<double>^ floorLevels)
	{
		double volume = Utility::CellUtility::Volume(buildingCell);
		Vertex^ centreOfMass = Utility::TopologyUtility::CenterOfMass(buildingCell);
		Autodesk::DesignScript::Geometry::Point^ centrePoint =
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(centreOfMass->Geometry);
		for (int i = 0; i < floorLevels->Count - 1; ++i)
		{
			if (centrePoint->Z > floorLevels[i] && centrePoint->Z < floorLevels[i + 1])
			{
				return i;
			}
		}

		return 0;
	}

	DSCore::Color^ EnergyModel::GetColor(double ratio)
	{
		double r = 0.0;
		double g = 0.0;
		double b = 0.0;

		if (ratio >= 0.0 && ratio <= 0.25)
		{
			r = 0.0;
			g = 4.0 * ratio;
			b = 1.0;
		}
		else if (ratio > 0.25 && ratio <= 0.5)
		{
			r = 0.0;
			g = 1.0;
			b = 1.0 - 4.0 * (ratio - 0.25);
		}
		else if (ratio > 0.5 && ratio <= 0.75)
		{
			r = 4.0*(ratio - 0.5);
			g = 1.0;
			b = 0.0;
		}
		else
		{
			r = 1.0;
			g = 1.0 - 4.0 * (ratio - 0.75);
			b = 0.0;
		}

		int rcom = (int) Math::Floor(Math::Max(Math::Min(Math::Floor(255.0 * r), 255.0), 0.0));
		int gcom = (int) Math::Floor(Math::Max(Math::Min(Math::Floor(255.0 * g), 255.0), 0.0));
		int bcom = (int) Math::Floor(Math::Max(Math::Min(Math::Floor(255.0 * b), 255.0), 0.0));
		return DSCore::Color::ByARGB(50, rcom, gcom, bcom);
	}

	String^ EnergyModel::BuildingName::get()
	{
		if (m_osBuilding == nullptr)
		{
			return "";
		}

		OpenStudio::OptionalString^ osName = m_osBuilding->name();
		return osName->get();
	}

	EnergyModel::EnergyModel(OpenStudio::Model^ osModel, OpenStudio::Building^ osBuilding, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces)
		: m_osModel(osModel)
		, m_osBuilding(osBuilding)
		, m_buildingCells(pBuildingCells)
		, m_osSpaces(osSpaces)
	{

	}
}