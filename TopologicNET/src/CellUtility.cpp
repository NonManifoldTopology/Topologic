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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "CellUtility.h"
#include "Wire.h"
#include "Vertex.h"

#include <TopologicCore/include/Utilities/CellUtility.h>

namespace Topologic {
	namespace Utilities {
		/*Cell^ CellUtility::ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices)
		{
			std::vector<TopologicCore::Vertex::Ptr> coreVertices;
			for each(Vertex^ pVertex in vertices)
			{
				coreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
			}

			std::list<std::list<int>> coreFaceIndices;
			for each(System::Collections::Generic::IEnumerable<int>^ pFaceIndex in faceIndices)
			{
				std::list<int> coreFaceIndex;
				for each(int vertexIndex in pFaceIndex)
				{
					coreFaceIndex.push_back(vertexIndex);
				}
				coreFaceIndices.push_back(coreFaceIndex);
			}

			Cell^ pCell = gcnew Cell(TopologicUtilities::CellUtility::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
			return pCell;
		}*/

		Cell^ CellUtility::ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires)
		{

			std::list<TopologicCore::Wire::Ptr> coreWires;
			for each(Wire^ pWire in wires)
			{
				coreWires.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(pWire->GetCoreTopologicalQuery()));
			}

			try {
				TopologicCore::Cell::Ptr pCoreCell = TopologicUtilities::CellUtility::ByLoft(coreWires);
				return gcnew Cell(pCoreCell);
			}
			catch (std::exception&)
			{
				throw gcnew Exception("Loft error");
			}
		}

		Cell^ CellUtility::ByCuboid(double xCentroid, double yCentroid, double zCentroid,
			double xDimension, double yDimension, double zDimension,
			double xNormal, double yNormal, double zNormal,
			double xAxisX, double yAxisX, double ZAxisX,
			double xAxisY, double yAxisY, double ZAxisY)
		{
			try {
				TopologicCore::Cell::Ptr pCoreCell = TopologicUtilities::CellUtility::ByCuboid(
					xCentroid, yCentroid, zCentroid, 
					xDimension, yDimension, zDimension,
					xNormal, yNormal, zNormal,
					xAxisX, yAxisX, ZAxisX,
					xAxisY, yAxisY, ZAxisY);
				return gcnew Cell(pCoreCell);
			}
			catch (std::exception&)
			{
				throw gcnew Exception("Error creating a Cell from a Cuboid.");
			}
		}

		Cell^ CellUtility::BySphere(double xCentroid, double yCentroid, double zCentroid, double radius)
		{
			try {
				TopologicCore::Cell::Ptr pCoreCell = TopologicUtilities::CellUtility::BySphere(
					xCentroid, yCentroid, zCentroid, radius);
				return gcnew Cell(pCoreCell);
			}
			catch (std::exception&)
			{
				throw gcnew Exception("Error creating a Cell from a Spher.");
			}
		}

		Cell^ CellUtility::ByTwoCorners(Vertex^ minVertex, Vertex^ maxVertex)
		{
			TopologicCore::Vertex::Ptr pCoreMinVertex= TopologicCore::Topology::Downcast<TopologicCore::Vertex>(minVertex->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreMaxVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(maxVertex->GetCoreTopologicalQuery());
			try {
				TopologicCore::Cell::Ptr pCoreCell = TopologicUtilities::CellUtility::ByTwoCorners(
					pCoreMinVertex, pCoreMaxVertex);
				return gcnew Cell(pCoreCell);
			}
			catch (std::exception&)
			{
				throw gcnew Exception("Error creating a Cell from a Spher.");
			}
		}

		double CellUtility::Volume(Cell^ cell)
		{
			TopologicCore::Cell::Ptr pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());
			return TopologicUtilities::CellUtility::Volume(pCoreCell);
		}

		bool CellUtility::Contains(Cell^ cell, Vertex ^ vertex, bool allowOnBoundary, double tolerance)
		{
			TopologicCore::Cell::Ptr pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery());
			TopologicUtilities::CellContainmentState isContained = TopologicUtilities::CellUtility::Contains(pCoreCell, pCoreVertex, tolerance);
			if (allowOnBoundary)
			{
				if (isContained == TopologicUtilities::INSIDE || isContained == TopologicUtilities::ON_BOUNDARY)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (isContained == TopologicUtilities::INSIDE)
				{
					return true;
				}
				else
				{
					return false;
				}

			}
		}

		List<double>^ CellUtility::GetMinMax(Cell ^ cell)
		{
			TopologicCore::Cell::Ptr pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());
			double minX = 0.0, maxX = 0.0, minY = 0.0, maxY = 0.0, minZ = 0.0, maxZ = 0.0;
			TopologicUtilities::CellUtility::GetMinMax(pCoreCell, minX, maxX, minY, maxY, minZ, maxZ);
			List<double>^ minMaxes = gcnew List<double>();
			minMaxes->Add(minX);
			minMaxes->Add(maxX);
			minMaxes->Add(minY);
			minMaxes->Add(maxY);
			minMaxes->Add(minZ);
			minMaxes->Add(maxZ);
			return minMaxes;
		}

		Vertex^ CellUtility::InternalVertex(Cell^ cell, double tolerance)
		{
			TopologicCore::Cell::Ptr pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());
			try
			{
				TopologicCore::Vertex::Ptr pCoreVertex = TopologicUtilities::CellUtility::InternalVertex(pCoreCell, tolerance);
				return gcnew Vertex(pCoreVertex);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}
		}

	}
}