#include "TopologicEnergy.h"
#include <Model.h>

using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Linq;

namespace TopologicEnergy
{
	bool TopologicEnergy::CreateIdfFile(OpenStudio::Model^ osModel, String^ idfPathName)
	{
		OpenStudio::EnergyPlusForwardTranslator^ osForwardTranslator = gcnew OpenStudio::EnergyPlusForwardTranslator();
		OpenStudio::Workspace^ osWorkspace = osForwardTranslator->translateModel(osModel);
		OpenStudio::IdfFile^ osIdfFile = osWorkspace->toIdfFile();
		OpenStudio::Path^ osIdfPath = gcnew OpenStudio::Path(idfPathName);
		bool idfSaveCondition = osIdfFile->save(osIdfPath);
		return idfSaveCondition;
	}

	Model^ TopologicEnergy::CreateEnergyModel(
		CellComplex^ building,
		String ^ buildingType, 
		String ^ buildingName, 
		String ^ spaceType, 
		List<double>^ floorLevels, 
		double glazingRatio,
		String^ weatherFilePath,
		String ^ designDayFilePath,
		String ^ openStudioTemplatePath,
		String^ openStudioOutputPath,
		double coolingTemp, 
		double heatingTemp,
		Cluster^ shadingCluster)
	{
		numOfApertures = 0;
		numOfAppliedApertures = 0;
		subsurfaceCounter = 1;

		// Create an OpenStudio model from the template, EPW, and DDY
		OpenStudio::Model^ osModel = GetModelFromTemplate(openStudioTemplatePath, weatherFilePath, designDayFilePath);

		double buildingHeight = Enumerable::Max(floorLevels);
		int numFloors = floorLevels->Count - 1;
		OpenStudio::Building^ osBuilding = ComputeBuilding(osModel, buildingName, buildingType, buildingHeight, numFloors, spaceType);
		List<Cell^>^ pBuildingCells = building->Cells;

		// Create OpenStudio spaces
		List<OpenStudio::Space^>^ osSpaces = gcnew List<OpenStudio::Space^>();
		for each(Cell^ buildingCell in pBuildingCells)
		{
			int spaceNumber = 1;
			OpenStudio::Space^ osSpace = AddSpace(
				spaceNumber,
				buildingCell,
				building,
				osModel,
				Autodesk::DesignScript::Geometry::Vector::ZAxis(),
				buildingHeight,
				floorLevels,
				glazingRatio,
				heatingTemp,
				coolingTemp
			);

			for each(OpenStudio::Space^ osExistingSpace in osSpaces)
			{
				osSpace->matchSurfaces(osExistingSpace);
			}

			osSpaces->Add(osSpace);
		}

		// Create shading surfaces
		if (shadingCluster != nullptr)
		{
			OpenStudio::ShadingSurfaceGroup^ osShadingGroup = gcnew OpenStudio::ShadingSurfaceGroup(osModel);
			List<Face^>^ contextFaces = shadingCluster->Faces;
			int faceIndex = 1;
			for each(Face^ contextFace in contextFaces)
			{
				AddShadingSurfaces(contextFace, osModel, osShadingGroup, faceIndex++);
			}
		}

		osModel->purgeUnusedResourceObjects();

		// Save model to an OSM file
		bool saveCondition = SaveModel(osModel, openStudioOutputPath);

		if (!saveCondition)
		{
			throw gcnew Exception("Failed to save the OSM file.");
		}

		OpenStudio::SqlFile^ osSqlFile = CreateSqlFile(osModel, "C:\\Users\\Nicholas Wardhana\\Documents\\Visual Studio 2017\\test.sql");

		String^ EPReportName = gcnew String("HVACSizingSummary");
		String^ EPReportForString = gcnew String("Entire Facility");
		String^ EPTableName = gcnew String("Zone Sensible Cooling");
		String^ EPColumnName = gcnew String("Calculated Design Load");
		String^ EPUnits = gcnew String("W");

		OpenStudio::WorkflowJSON^ workflow = gcnew OpenStudio::WorkflowJSON();
		try{
			String^ osmFilename = Path::GetFileNameWithoutExtension(openStudioOutputPath);
			String^ osmDirectory = Path::GetDirectoryName(openStudioOutputPath);
			String^ oswPath = osmDirectory + "\\" + osmFilename + ".osw";
			OpenStudio::Path^ osOswPath = gcnew OpenStudio::Path(oswPath);
			workflow->setSeedFile(gcnew OpenStudio::Path(openStudioOutputPath));
			workflow->setWeatherFile(gcnew OpenStudio::Path());
			workflow->saveAs(osOswPath);

			// Create a TopologicEnergy model
			return Model::ByOsmPathOswPath(openStudioOutputPath, oswPath, osModel, pBuildingCells, osSpaces,
				EPReportName, EPReportForString, EPTableName, EPColumnName, EPUnits);
		}
		catch (...)
		{
			throw gcnew Exception("Failed to create the OSW file.");
		}
	}

	List<Modifiers::GeometryColor^>^ TopologicEnergy::PerformEnergyAnalysis(Model ^ model, String ^ openStudioExePath)
	{
		// https://stackoverflow.com/questions/5168612/launch-program-with-parameters
		String^ args = "run -w \"" + model->OSWFilePath + "\"";
		System::Diagnostics::ProcessStartInfo^ startInfo = gcnew ProcessStartInfo(openStudioExePath, args);
		startInfo->WorkingDirectory = Path::GetDirectoryName(model->OSWFilePath);

		Process^ process = Process::Start(startInfo);
		process->WaitForExit();

		List<Modifiers::GeometryColor^>^ dynamoGeometryColors = AnalyzeSqlFile(
			gcnew OpenStudio::SqlFile(gcnew OpenStudio::Path("C:\\Users\\Nicholas Wardhana\\Documents\\NMT\\2016-NMT-architecture\\files\\OSM\\run\\eplusout.sql")),
			model->OsModel,
			model->OsSpaces, 
			model->BuildingCells,
			model->EPReportName,
			model->EPReportForString,
			model->EPTableName,
			model->EPColumnName,
			model->EPUnits);

		return dynamoGeometryColors;
	}

	bool TopologicEnergy::SaveModel(OpenStudio::Model^ osModel, String^ osmPathName)
	{
		// Purge unused resources
		osModel->purgeUnusedResourceObjects();

		// Create a path string
		OpenStudio::Path^ osPath = gcnew OpenStudio::Path(osmPathName);
		bool osCondition = osModel->save(osPath, true);
		return osCondition;
	}

	OpenStudio::Model^ TopologicEnergy::GetModelFromTemplate(String^ osmTemplatePath, String^ epwWeatherPath, String^ ddyPath)
	{
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

	OpenStudio::ThermalZone^ TopologicEnergy::CreateThermalZone(OpenStudio::Model^ model, OpenStudio::Space^ space, double ceilingHeight, double heatingTemp, double coolingTemp)
	{
		//TopologicEnergy dsos = new TopologicEnergy();

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

	OpenStudio::BuildingStory^ TopologicEnergy::AddBuildingStory(OpenStudio::Model^ model, int floorNumber)
	{
		OpenStudio::BuildingStory^ osBuildingStory = gcnew OpenStudio::BuildingStory(model);
		osBuildingStory->setName("STORY_" + floorNumber);
		osBuildingStory->setDefaultConstructionSet(getDefaultConstructionSet(model));
		osBuildingStory->setDefaultScheduleSet(getDefaultScheduleSet(model));
		return osBuildingStory;
	}

	OpenStudio::Building^ TopologicEnergy::ComputeBuilding(
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

	List<OpenStudio::BuildingStory^>^ TopologicEnergy::CreateBuildingStories(OpenStudio::Model^ osModel, int numFloors)
	{
		List<OpenStudio::BuildingStory^>^ osBuildingStories = gcnew List<OpenStudio::BuildingStory^>();
		for (int i = 0; i < numFloors; i++)
		{
			osBuildingStories->Add(AddBuildingStory(osModel, (i + 1)));
		}
		return osBuildingStories;
	}

	OpenStudio::SqlFile^ TopologicEnergy::CreateSqlFile(OpenStudio::Model ^ osModel, String^ sqlFilePath)
	{
		// TODO: Add timestamp to this file path
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

	List<Modifiers::GeometryColor^>^ TopologicEnergy::AnalyzeSqlFile(OpenStudio::SqlFile ^ osSqlFile, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ spaces, List<Cell^>^ buildingCells,
		String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits)
		//double minValue, double maxValue)
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
		/*OpenStudio::OptionalDouble^ minValue = nullptr;
		OpenStudio::OptionalDouble^ maxValue = nullptr;
		if (minValue == nullptr || maxValue == nullptr)
		{*/
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
		// selectedCellSolid = null;
		//Print(EPReportName + "-" + EPTableName + " for " + EPReportForString + " " + EPColumnName)

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

			//selectedCellSolid.wirecolor = aColor;
			//selectedCellSolid.material.diff_color = aColor;
			////-- selectedCellSolid.material.ambient = aColor
			//selectedCellSolid.gr = glazingRatio;
			//selectedCellSolid.zccdl = (outputVariable as float);
			//farea = (s.floorArea as float);
			//selectedCellSolid.floorarea = farea;
			//selectedCellSolid.clpfa = (outputVariable / farea);
			/*else
			{
				throw gcnew Exception("Warning: Failed to obtain result from EnergyPlus Output File for Space: " + spaceName + ". To correct: Quit the software, delete all folders inside the 'Output' folder, then restart DesignScript.");
			}*/
		}
			
		//this is the important step otherwise the sqlfile stays locked
		osSqlFile->close();
		delete osSqlFile;

		return dynamoGeometryColors;
	}


	double TopologicEnergy::DoubleValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits)
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

	String^ TopologicEnergy::StringValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits)
	{
		String^ query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='" + EPReportName + "' AND ReportForString='" + EPReportForString + "' AND TableName = '" + EPTableName + "' AND RowName = '" + EPRowName + "' AND ColumnName= '" + EPColumnName + "' AND Units='" + EPUnits + "'";
		return sqlFile->execAndReturnFirstString(query)->get();
	}

	int TopologicEnergy::IntValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits)
	{
		String^ query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='" + EPReportName + "' AND ReportForString='" + EPReportForString + "' AND TableName = '" + EPTableName + "' AND RowName = '" + EPRowName + "' AND ColumnName= '" + EPColumnName + "' AND Units='" + EPUnits + "'";
		return sqlFile->execAndReturnFirstInt(query)->get();
	}


	OpenStudio::DefaultScheduleSet^ TopologicEnergy::getDefaultScheduleSet(OpenStudio::Model^ model)
	{
		// Get list of default schedule sets
		OpenStudio::DefaultScheduleSetVector^ defaultScheduleSets = model->getDefaultScheduleSets();
		OpenStudio::DefaultScheduleSetVector::DefaultScheduleSetVectorEnumerator^ defSchedEnum = defaultScheduleSets->GetEnumerator();
		defSchedEnum->MoveNext();
		defaultScheduleSet = defSchedEnum->Current;
		return defaultScheduleSet;
	}

	OpenStudio::DefaultConstructionSet^ TopologicEnergy::getDefaultConstructionSet(OpenStudio::Model ^ model)
	{
		// Get list of default construction sets
		OpenStudio::DefaultConstructionSetVector^ defaultConstructionSets = model->getDefaultConstructionSets();
		// Get the first item and use as the default construction set
		OpenStudio::DefaultConstructionSetVector::DefaultConstructionSetVectorEnumerator^ defConEnum = defaultConstructionSets->GetEnumerator();
		defConEnum->MoveNext();
		defaultConstructionSet = defConEnum->Current;
		return defaultConstructionSet;
	}

	Face^ TopologicEnergy::ApplyAperture(Face^ face, Face^ apertureDesign, int numEdgeSamples)
	{
		if (numEdgeSamples <= 0)
		{
			throw gcnew Exception("numEdgeSamples must be positive.");
		}
		// 1. Convert the apertures and boundary as faces.
		Wire^ pOuterApertureWire = apertureDesign->ExternalBoundary;
		List<Wire^>^ pApertureWires = apertureDesign->InternalBoundaries;

		List<Topology^>^ pFaces = gcnew List<Topology^>();

		// 2. For each wires, iterate through the edges, sample points, and map them to the 
		for each(Wire^ pApertureWire in pApertureWires)
		{
			List<Edge^>^ pApertureEdges = pApertureWire->Edges_(true);
			List<Edge^>^ pMappedApertureEdges = gcnew List<Edge^>();

			for each(Edge^ pApertureEdge in pApertureEdges)
			{
				List<Vertex^>^ pMappedSampleVertices = gcnew List<Vertex^>();
				for (int i = 0; i < numEdgeSamples; ++i)
				{
					double t = (double)i / (double)(numEdgeSamples - 1);
					if (t < 0.0)
					{
						t = 0.0;
					}
					else if (t > 1.0)
					{
						t = 1.0;
					}

					// Find the actual point on the edge
					Vertex^ pSampleVertex = pApertureEdge->VertexAtParameter(t);

					// Find the UV-coordinate of the point on the aperture design
					Autodesk::DesignScript::Geometry::UV^ pUV = apertureDesign->UVParameterAtVertex(pSampleVertex);
					double checkedU = pUV->U, checkedV = pUV->V;
					if (checkedU < 0.0)
					{
						checkedU = 0.0;
					}
					else if (checkedU > 1.0)
					{
						checkedU = 1.0;
					}

					if (checkedV < 0.0)
					{
						checkedV = 0.0;
					}
					else if (checkedV > 1.0)
					{
						checkedV = 1.0;
					}

					pUV = Autodesk::DesignScript::Geometry::UV::ByCoordinates(checkedU, checkedV);

					// Find the point with the same UV-coordinate on the surface, add it to the list
					Vertex^ pMappedSampleVertex = face->VertexAtParameter(pUV);
					pMappedSampleVertices->Add(pMappedSampleVertex);
				}

				// Interpolate the mapped vertices to an edge.
				Edge^ pMappedApertureEdge = Edge::ByVertices(pMappedSampleVertices);
				pMappedApertureEdges->Add(pMappedApertureEdge);
			}

			// Connect the mapped edges to a wire
			Wire^ pMappedApertureWire = Wire::ByEdges(pMappedApertureEdges);

			//// Use the wire to make a face on the same supporting surface as the input face's
			Face^ pMappedApertureFace = face->TrimByWire_(pMappedApertureWire);
			pFaces->Add(pMappedApertureFace);

			// and attach it as an aperture to the face.
			/*Context^ pFaceContext = Context::ByTopologyParameters(face, 0.0, 0.0, 0.0);
			Aperture^ pMappedAperture = Aperture::ByTopologyContext(pMappedApertureFace, pFaceContext);*/
		}
		Face^ pCopyFace = safe_cast<Face^>(face->AddApertures(pFaces));

		// TODO: should return a copy
		return pCopyFace;
	}

	OpenStudio::Space^ TopologicEnergy::AddSpace(
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

		Autodesk::DesignScript::Geometry::Solid^ solid = safe_cast<Autodesk::DesignScript::Geometry::Solid^>(cell->Geometry);
		List<Autodesk::DesignScript::Geometry::Geometry^>^ solids = gcnew List<Autodesk::DesignScript::Geometry::Geometry^>();
		solids->Add(solid);
		Autodesk::DesignScript::Geometry::BoundingBox^ boundingBox =
			Autodesk::DesignScript::Geometry::BoundingBox::ByGeometry(solids);
		double ceilingHeight = Math::Abs(boundingBox->MaxPoint->Z - boundingBox->MinPoint->Z);

		OpenStudio::ThermalZone^ thermalZone = CreateThermalZone(osModel, osSpace, ceilingHeight, heatingTemp, coolingTemp);

		return osSpace;
	}


	void TopologicEnergy::AddShadingSurfaces(Cell^ buildingCell, OpenStudio::Model^ osModel)
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

	void TopologicEnergy::AddShadingSurfaces(Face ^ buildingFace, OpenStudio::Model ^ osModel, OpenStudio::ShadingSurfaceGroup^ osShadingGroup, int faceIndex)
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

	OpenStudio::Surface^ TopologicEnergy::AddSurface(
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

		bool isUnderground = IsUnderground(buildingFace);
		FaceType faceType = CalculateFaceType(buildingFace, osFacePoints, buildingSpace, upVector);

		OpenStudio::Surface^ osSurface = gcnew OpenStudio::Surface(osFacePoints, osModel);
		osSurface->setSpace(osSpace);
		OpenStudio::OptionalString^ osSpaceOptionalString = osSpace->name();
		String^ spaceName = osSpace->name()->get();
		String^ surfaceName = osSpace->name()->get() + "_SURFACE_" + surfaceNumber.ToString();
		osSurface->setName(surfaceName);
		/*OpenStudio::Vector3d^ pSurfaceNormal = osSurface->outwardNormal();

		double x = pSurfaceNormal->x();
		double y = pSurfaceNormal->y();
		double z = pSurfaceNormal->z();*/

		int adjCount = AdjacentCellCount(buildingFace, cellComplex);

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
			//OpenStudio::Vector3d^ downwardVector = gcnew OpenStudio::Vector3d(0, 0, -1.0);
			//// If the normal does not point downward, flip it. Use dot product.
			//double dotProduct = pSurfaceNormal->dot(downwardVector);
			//if (dotProduct < 0.98)
			//{
			//	OpenStudio::Point3dVector^ surfaceVertices = osSurface->vertices();
			//	surfaceVertices->Reverse();
			//	osSurface->setVertices(surfaceVertices);
			//}
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

			if (glazingRatio > 0)
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
			else
			{
				//osSurface->setWindowToWallRatio(glazingRatio, 900.0, true);

				// Use the surface apertures
				List<Topology^>^ pContents = buildingFace->Contents(true);
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
					double area = pFaceAperture->Area();
					if (area <= 0.1)
					{
						continue;
					}
					Wire^ pApertureWire = pFaceAperture->ExternalBoundary;
					List<Vertex^>^ pApertureVertices = pApertureWire->Vertices_(true);
					//pApertureVertices->Reverse();
					OpenStudio::Point3dVector^ osWindowFacePoints = gcnew OpenStudio::Point3dVector();
					for each(Vertex^ pApertureVertex in pApertureVertices)
					{
						Object^ pVertexGeometry = pApertureVertex->Geometry;
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

					numOfApertures++;

					OpenStudio::SubSurface^ osWindowSubSurface = gcnew OpenStudio::SubSurface(osWindowFacePoints, osModel);
					double grossSubsurfaceArea = osWindowSubSurface->grossArea();
					double netSubsurfaceArea = osWindowSubSurface->netArea();
					double grossSurfaceArea = osSurface->grossArea();
					double netSurfaceArea = osSurface->netArea();
					if (grossSubsurfaceArea > 0.1)
					{
						osWindowSubSurface->setSubSurfaceType("FixedWindow");
						bool result = osWindowSubSurface->setSurface(osSurface);
						//osWindowSubSurface->setSurface(osSurface);
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

	List<Vertex^>^ TopologicEnergy::ScaleFaceVertices(Face^ buildingFace, double scaleFactor)
	{
		List<Vertex^>^ scaledVertices = gcnew List<Vertex^>();
		Vertex^ faceCentre = GetFaceCentre(buildingFace);
		Autodesk::DesignScript::Geometry::Point^ faceCenterPoint =
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(faceCentre->Geometry);

		Wire^ pApertureWire = buildingFace->ExternalBoundary;
		List<Vertex^>^ vertices = pApertureWire->Vertices_(true);
		vertices->Reverse();

		double sqrtScaleFactor = Math::Sqrt(scaleFactor);
		for each(Vertex^ aVertex in vertices)
		{
			Autodesk::DesignScript::Geometry::Point^ originalPoint =
				safe_cast<Autodesk::DesignScript::Geometry::Point^>(aVertex->Geometry);
			Autodesk::DesignScript::Geometry::Point^ scaledVertex = originalPoint->Subtract(faceCenterPoint->AsVector());
			scaledVertex = safe_cast<Autodesk::DesignScript::Geometry::Point^>(scaledVertex->Scale(sqrtScaleFactor));
			scaledVertex = scaledVertex->Add(faceCenterPoint->AsVector());
			scaledVertices->Add(Vertex::ByPoint_(scaledVertex));
		}
		return scaledVertices;
	}

	Vertex^ TopologicEnergy::GetFaceCentre(Face^ buildingFace)
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
		return Vertex::ByPoint_(safe_cast<Autodesk::DesignScript::Geometry::Point^>(sumPoint->Scale(1.0 / (double)vertices->Count)));
	}

	OpenStudio::Point3dVector^ TopologicEnergy::GetFacePoints(Face^ buildingFace)
	{
		Wire^ buildingOuterWire = buildingFace->ExternalBoundary;
		List<Vertex^>^ vertices = buildingOuterWire->Vertices_(true);
		// HACK
		vertices->Reverse();
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

	bool TopologicEnergy::IsUnderground(Face^ buildingFace)
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

	FaceType TopologicEnergy::CalculateFaceType(Face^ buildingFace, OpenStudio::Point3dVector^% facePoints, Cell^ buildingSpace, Autodesk::DesignScript::Geometry::Vector^ upVector)
	{
		FaceType faceType = FACE_WALL;
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

		Autodesk::DesignScript::Geometry::Point^ centerPoint =
			Autodesk::DesignScript::Geometry::Point::ByCoordinates(
			(p1->X + p2->X + p3->X) / 3.0,
				(p1->Y + p2->Y + p3->Y) / 3.0,
				(p1->Z + p2->Z + p3->Z) / 3.0
			);

		Autodesk::DesignScript::Geometry::Point^ pDynamoOffsetPoint =
			dynamic_cast<Autodesk::DesignScript::Geometry::Point^>(centerPoint->Translate(faceNormal->Scale(0.001)));

		Vertex^ pOffsetVertex = Vertex::ByPoint_(pDynamoOffsetPoint);

		if (faceAngle < 5.0 || faceAngle > 175.0)
		{
			bool isInside = buildingSpace->Contains(pOffsetVertex);
			// The offset vertex has to be false, so if isInside is true, reverse the face.

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

	int TopologicEnergy::AdjacentCellCount(Face^ buildingFace, CellComplex^ cellComplex)
	{
		return buildingFace->Cells_(cellComplex)->Count;
	}

	int TopologicEnergy::StoryNumber(Cell^ buildingCell, double buildingHeight, List<double>^ floorLevels)
	{
		double volume = buildingCell->Volume();
		Vertex^ centreOfMass = buildingCell->CenterOfMass();
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

	DSCore::Color^ TopologicEnergy::GetColor(double ratio)
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
}