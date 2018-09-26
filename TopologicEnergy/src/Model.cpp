#include <Model.h>

namespace TopologicEnergy
{
	Model ^ Model::ByOsmPathOswPath(String ^ osmFilePath, String ^ oswFilePath, OpenStudio::Model^ osModel, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces)
	{
		return gcnew Model(osmFilePath, oswFilePath, osModel, pBuildingCells, osSpaces);
	}

	Model::Model(String ^ osmFilePath, String ^ oswFilePath, OpenStudio::Model^ osModel, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces)
		: m_osmFilePath(osmFilePath)
		, m_oswFilePath(oswFilePath)
		, m_osModel(osModel)
		, m_buildingCells(pBuildingCells)
		, m_osSpaces(osSpaces)
	{

	}
}