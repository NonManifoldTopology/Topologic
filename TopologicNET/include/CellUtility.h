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

#pragma once

#include "Cell.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	

		/// <summary>
		/// CellUtility includes geometric methods relevant to a Cell.
		/// </summary>
		public ref class CellUtility
		{
		public:
			/*/// <summary>
			/// Creates a Cell by a list of Vertices and a list of Face indices.
			/// </summary>
			/// <param name="vertices">A list of Vertices</param>
			/// <param name="faceIndices">A list of Face indices</param>
			/// <returns name="Cell">The created Cell</returns>
			static Cell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);*/

			/// <summary>
			/// Creates a Cell by lofting through a set of Wires.
			/// </summary>
			/// <param name="wires">A set of Wires</param>
			/// <returns name="Cell">The created Cell</returns>
			static Cell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

			/// <summary>
			/// Creates a Cell by a Cuboid.
			/// </summary>
			/// <param name="xCentroid">The X coordinate of the centroid</param>
			/// <param name="yCentroid">The Y coordinate of the centroid</param>
			/// <param name="zCentroid">The Z coordinate of the centroid</param>
			/// <param name="xDimension">The size along the X axis</param>
			/// <param name="yDimension">The size along the Y axis</param>
			/// <param name="zDimension">The size along the Z axis</param>
			/// <param name="xNormal">The X component of the normal</param>
			/// <param name="yNormal">The Y component of the normal</param>
			/// <param name="zNormal">The Z component of the normal</param>
			/// <param name="xAxisX">The X component of the X-axis</param>
			/// <param name="yAxisX">The X component of the Y-axis</param>
			/// <param name="ZAxisX">The X component of the Z-axis</param>
			/// <returns name="Cell">The created Cell</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static Cell^ ByCuboid(double xCentroid, double yCentroid, double zCentroid,
				double xDimension, double yDimension, double zDimension,
				double xNormal, double yNormal, double zNormal,
				double xAxisX, double yAxisX, double ZAxisX,
				double xAxisY, double yAxisY, double ZAxisY);

			/// <summary>
			/// Creates a Cell by a Sphere.
			/// </summary>
			/// <param name="xCentroid">The X coordinate of the centroid</param>
			/// <param name="yCentroid">The Y coordinate of the centroid</param>
			/// <param name="zCentroid">The Z coordinate of the centroid</param>
			/// <param name="radius">The radius of the Sphere</param>
			/// <returns name="Cell">The created Cell</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static Cell^ BySphere(double xCentroid, double yCentroid, double zCentroid, double radius);

			/// <summary>
			/// Creates an Axis-Aligned Bounding Box Cell by two Vertices at the minimum and maximum corners.
			/// </summary>
			/// <param name="minVertex">The minimum Vertex</param>
			/// <param name="maxVertex">The maximum Vertex</param>
			/// <returns name="Cell">The created Cell</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static Cell ^ ByTwoCorners(Vertex ^ minVertex, Vertex ^ maxVertex);

			/// <summary>
			/// Returns the volume of a Cell.
			/// </summary>
			/// <param name="cell">The Cell</param>
			/// <returns>The volume of the Cell</returns>
			static double Volume(Cell^ cell);

			/// <summary>
			/// Checks if a Vertex is contained in a Cell or not.
			/// </summary>
			/// <param name="cell">The Cell</param>
			/// <param name="vertex">The Vertex</param>
			/// <param name="allowOnBoundary">Is the Vertex allowed on the Cell's boundary?</param>
			/// <param name="tolerance">A positive tolerance value</param>
			/// <returns>True or false</returns>
			static bool Contains(
				Cell^ cell, 
				Vertex^ vertex, 
				bool allowOnBoundary,
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
				double tolerance
			);

			/// <summary>
			/// Returns six values associated to the x, y, z coordinates of the bounding box of the Cell.
			/// </summary>
			/// <param name="cell">The Cell</param>
			/// <returns>Six values associated to the x, y, z coordinates of the bounding box of the topological entity</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static List<double>^ GetMinMax(Cell^ cell);

			/// <summary>
			/// Returns a Vertex inside the Cell.
			/// </summary>
			/// <param name="cell">A Cell</param>
			/// <param name="tolerance">A positive tolerance value</param>
			/// <returns name="Vertex">A Vertex inside the Cell</returns>
			static Vertex^ InternalVertex(
				Cell^ cell,
#ifdef TOPOLOGIC_DYNAMO
				[DefaultArgument("0.0001")]
#endif
			double tolerance);

		public protected:
			CellUtility() {}
		};
	}
}