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
using namespace Topologic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	enum FaceType
	{
		FACE_WALL,
		FACE_FLOOR,
		FACE_ROOFCEILING
	};

	ref class Model;
	ref class EnergySimulation;

	public ref class EnergyModel
	{
	public:

		/// <summary>
		/// Create an EnergyModel from a Topologic shape.
		/// </summary>
		/// <param name="building">The building as a CellComplex</param>
		/// <param name="shadingSurfaces">The shading surfaces as a Cluster of Faces</param>
		/// <param name="floorLevels">The number of floor levels</param>
		/// <param name="buildingName">The building's name</param>
		/// <param name="buildingType">The building's type</param>
		/// <param name="defaultSpaceType">The default space type</param>
		/// <param name="northAxis">Specify the angle at the north axis in degrees, with 0 signifying positive Y-axis. Positive values indivate a counter-clockwise rotation.</param>
		/// <param name="glazingRatio">The optional glazing ratio. If not provided, the Apertures on the Faces will be used.</param>
		/// <param name="coolingTemp">The cooling temperature</param>
		/// <param name="heatingTemp">The heating temperature</param>
		/// <param name="weatherFilePath">Path to a .epw file</param>
		/// <param name="designDayFilePath">Path to a .ddy file</param>
		/// <param name="openStudioTemplatePath">Path to a template .osm file</param>
		/// <returns name="energyModel">An EnergyModel</returns>
		static EnergyModel^ ByCellComplex(
			CellComplex^ building,
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Cluster^ shadingSurfaces,
			List<double>^ floorLevels,
			[Autodesk::DesignScript::Runtime::DefaultArgument("Building")] String^ buildingName,
			[Autodesk::DesignScript::Runtime::DefaultArgument("Commercial")] String^ buildingType,
			[Autodesk::DesignScript::Runtime::DefaultArgument("ASHRAE 189.1-2009 ClimateZone 4-8 MediumOffice")] String^ defaultSpaceType,
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")] double northAxis,
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> glazingRatio,
			double coolingTemp,
			double heatingTemp,
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\GBR_London.Gatwick.037760_IWEC.epw")] String^ weatherFilePath,
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\GBR_London.Gatwick.037760_IWEC.ddy")] String^ designDayFilePath,
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\MinimalTemplate120.osm")] String^ openStudioTemplatePath
			);

		/// <summary>
		/// Exports an EnergyModel to a .osm file.
		/// </summary>
		/// <param name="energyModel">An EnergyModel</param>
		/// <param name="filePath">The file to export the EnergyModel to</param>
		/// <returns name="bool">True if a .osm file can be created, otherwise False</returns>
		static bool ExportToOSM(EnergyModel^ energyModel, String^ filePath);
		
		/// <summary>
		/// Imports an .osm file into an EnergyModel
		/// </summary>
		/// <param name="filePath">The file to import the EnergyModel from</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="EnergyModel">An EnergyModel</returns>
		static EnergyModel^ ByImportedOSM(String^ filePath, [Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")] double tolerance);

		/// <summary>
		/// Returns the Topology of this EnergyModel.
		/// </summary>
		property List<Topologic::Cell^>^ Topology
		{
			List<Topologic::Cell^>^ get();
		}

		/// <summary>
		/// Exports an EnergyModel to a gbXML file.
		/// </summary>
		/// <param name="energyModel">An EnergyModel</param>
		/// <param name="filePath">The file to export the EnergyModel to</param>
		/// <returns name="bool">True if a .gbXML file can be created, otherwise False</returns>
		static bool ExportTogbXML(EnergyModel^ energyModel, String^ filePath);

		/// <summary>
		/// Imports a .gbXML file into an EnergyModel
		/// </summary>
		/// <param name="filePath">The file to import the EnergyModel from</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="EnergyModel">An EnergyModel</returns>
		static EnergyModel^ ByImportedgbXML(String^ filePath, [Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")] double tolerance);

	public protected:
		static List<int>^ GetColor(double ratio);

		static double DoubleValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static String^ StringValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static int IntValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static bool Export(EnergyModel^ energyModel, String^ openStudioOutputDirectory, String^% oswPath);

		property OpenStudio::Model^ OsModel {
			OpenStudio::Model^ get() { return m_osModel; }
		}

		property OpenStudio::SpaceVector^ OsSpaces {
			OpenStudio::SpaceVector^ get() { return m_osSpaceVector; }
		}

		property String^ BuildingName {
			String^ get();
		}


	private:
		EnergyModel(OpenStudio::Model^ osModel, OpenStudio::Building^ osBuilding, List<Topologic::Cell^>^ pBuildingCells, Cluster^ shadingSurfaces, OpenStudio::SpaceVector^ osSpaceVector);
		~EnergyModel() {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="osModel"></param>
		/// <param name="idfPathName"></param>
		/// <returns name="bool"></returns>
		static bool CreateIdfFile(OpenStudio::Model^ osModel, String^ idfPathName);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="osModel"></param>
		/// <param name="osmPathName"></param>
		/// <returns name="bool"></returns>
		static bool SaveModel(OpenStudio::Model^ osModel, String^ osmPathName);

		static OpenStudio::Space^ AddSpace(
			int spaceNumber,
			Cell^ cell,
			CellComplex^ cellComplex,
			OpenStudio::Model^ osModel,
			Autodesk::DesignScript::Geometry::Vector^ upVector,
			double buildingHeight,
			List<double>^ floorLevels,
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> glazingRatio,
			double heatingTemp,
			double coolingTemp
		);

		static void AddShadingSurfaces(Cell^ buildingCell, OpenStudio::Model^ osModel);

		static void AddShadingSurfaces(Face^ buildingFace, OpenStudio::Model^ osModel, OpenStudio::ShadingSurfaceGroup^ osShadingGroup, int faceIndex);

		static OpenStudio::Surface^ AddSurface(
			int surfaceNumber,
			Face^ buildingFace,
			Cell^ buildingSpace,
			CellComplex^ cellComplex,
			OpenStudio::Point3dVector^ osFacePoints,
			OpenStudio::Space^ osSpace,
			OpenStudio::Model^ osModel,
			Autodesk::DesignScript::Geometry::Vector^ upVector,
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> glazingRatio);

		static OpenStudio::Model^ GetModelFromTemplate(String^ osmTemplatePath, String^ epwWeatherPath, String^ ddyPath);

		static List<Vertex^>^ ScaleFaceVertices(Face^ buildingFace, double scaleFactor);

		static List<Vertex^>^ ScaleVertices(List<Vertex^>^ vertices, double scaleFactor);

		static Vertex^ GetCentreVertex(List<Vertex^>^ vertices);

		static OpenStudio::Point3dVector^ GetFacePoints(Face^ buildingFace);

		static bool IsUnderground(Face^ buildingFace);

		static FaceType CalculateFaceType(Face^ buildingFace, OpenStudio::Point3dVector^% facePoints, Cell^ buildingSpace, Autodesk::DesignScript::Geometry::Vector^ upVector);

		static int AdjacentCellCount(Face^ buildingFace);

		static int StoryNumber(
			Cell^ buildingCell,
			double buildingHeight,
			List<double>^ floorLevels
		);

		static OpenStudio::ThermalZone^ CreateThermalZone(OpenStudio::Model^ model, OpenStudio::Space^ space, double ceilingHeight, double heatingTemp, double coolingTemp);

		static OpenStudio::BuildingStory^ AddBuildingStory(OpenStudio::Model^ model, int floorNumber);

		static OpenStudio::SubSurface^ CreateSubSurface(List<Topologic::Vertex^>^ vertices, OpenStudio::Model^ osModel);

		static OpenStudio::Building^ ComputeBuilding(
			OpenStudio::Model^ osModel,
			String^ buildingName,
			String^ buildingType,
			double buildingHeight,
			int numFloors,
			double northAxis,
			String^ spaceType);


		static OpenStudio::DefaultScheduleSet^ getDefaultScheduleSet(OpenStudio::Model^ model);

		static OpenStudio::DefaultConstructionSet^ getDefaultConstructionSet(OpenStudio::Model^ model);

		static List<OpenStudio::BuildingStory^>^ CreateBuildingStories(OpenStudio::Model^ osModel, int numFloors);

		static OpenStudio::SqlFile^ CreateSqlFile(OpenStudio::Model^ osModel, String^ sqlFilePath);

		static Topologic::Face^ FaceByOsSurface(OpenStudio::PlanarSurface^ osPlanarSurface);

		static void EnergyModel::ProcessOsModel(
			OpenStudio::Model^ osModel, 
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")] double tolerance,
			OpenStudio::Building^% osBuilding,
			List<Cell^>^% buildingCells,
			Topologic::Cluster^% shadingFaces,
			OpenStudio::SpaceVector^% osSpaceVector);

		static List<OpenStudio::BuildingStory^>^ buildingStories;
		static OpenStudio::DefaultConstructionSet^ defaultConstructionSet;
		static OpenStudio::DefaultScheduleSet^ defaultScheduleSet;

		static int numOfApertures;
		static int numOfAppliedApertures;

		List<Topologic::Cell^>^ m_buildingCells;
		Cluster^ m_shadingSurfaces;
		OpenStudio::SpaceVector^ m_osSpaceVector;
		OpenStudio::Model^ m_osModel;
		OpenStudio::Building^ m_osBuilding;
	};
}