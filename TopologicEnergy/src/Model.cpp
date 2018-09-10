#include "Model.h"

namespace TopologicEnergy
{
	Model ^ Model::ByOsmPathOswPath(String ^ osmFilePath, String ^ oswFilePath)
	{
		return gcnew Model(osmFilePath, oswFilePath);
	}

	Model::Model(String ^ osmFilePath, String ^ oswFilePath)
		: m_osmFilePath(osmFilePath)
		, m_oswFilePath(oswFilePath)
	{

	}
}