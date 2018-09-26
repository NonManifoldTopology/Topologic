#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace Topologic
{
	ref class Cell;
}

namespace TopologicEnergy
{
	public ref class Model
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		static Model^ ByOsmPathOswPath(String^ osmFilePath, String^ oswFilePath, OpenStudio::Model^ osModel, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces);

	public protected:

		property String^ OSWFilePath {
			String^ get() { return m_oswFilePath; }
		}

		property String^ OSMFilePath {
			String^ get() { return m_osmFilePath; }
		}

		property OpenStudio::Model^ OsModel {
			OpenStudio::Model^ get() { return m_osModel; }
		}

		property List<Topologic::Cell^>^ BuildingCells {
			List<Topologic::Cell^>^ get() { return m_buildingCells; }
		}

		property List<OpenStudio::Space^>^ OsSpaces {
			List<OpenStudio::Space^>^ get() { return m_osSpaces; }
		}

	protected:
		Model(String^ osmFilePath, String^ oswFilePath, OpenStudio::Model^ osModel, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces);
		~Model() {}
		
		String^ m_osmFilePath;
		String^ m_oswFilePath;
		List<Topologic::Cell^>^ m_buildingCells;
		List<OpenStudio::Space^>^ m_osSpaces;
		OpenStudio::Model^ m_osModel;
	};
}