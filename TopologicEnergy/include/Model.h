#pragma once

using namespace System;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	public ref class Model
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		static Model^ ByOsmPathOswPath(String^ osmFilePath, String^ oswFilePath);

	public protected:

		property String^ OSWFilePath {
			String^ get() { return m_oswFilePath; }
		}

		property String^ OSMFilePath {
			String^ get() { return m_osmFilePath; }
		}

	protected:
		Model(String^ osmFilePath, String^ oswFilePath);
		~Model() {}
		
		String^ m_osmFilePath;
		String^ m_oswFilePath;
	};
}