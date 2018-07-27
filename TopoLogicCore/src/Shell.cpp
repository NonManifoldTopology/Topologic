#include <Shell.h>
#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Aperture.h>

//#include <BOPAlgo_Splitter.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepCheck_Shell.hxx>
#include <BRepGProp.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>
#include <GProp_GProps.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <ShapeFix_ShapeTolerance.hxx>
#include <ShapeFix_Shell.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>
#include <TopTools_MapOfShape.hxx>

#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <Geom_Circle.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepCheck_Wire.hxx>
#include <Geom2d_Line.hxx>
#include <Geom2d_CartesianPoint.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

//#include <TDataStd_Integer.hxx>
//#include <TDF_ChildIterator.hxx>
//#include <TNaming_NamedShape.hxx>
// ShapeOp
#include <Constraint.h>
#include <Solver.h>

// Earcut
#include <mapbox/earcut.hpp>

// GPC
extern "C" {
#include <gpc.h>
}

#include <array>
#include <assert.h>

namespace TopologicCore
{
	void Shell::Cells(const Topology::Ptr& kpParentTopology, std::list<Cell::Ptr>& rCells) const
	{
		UpwardNavigation(kpParentTopology, rCells);
	}

	void Shell::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Shell::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	void Shell::Faces(std::list<Face::Ptr>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	bool Shell::IsClosed() const
	{
		BRepCheck_Shell occtBrepCheckShell(TopoDS::Shell(GetOcctShape()));
		return (occtBrepCheckShell.Closed() == BRepCheck_NoError);
	}

	void Shell::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	Shell::Ptr Shell::ByFaces(const std::list<Face::Ptr>& rkFaces)
	{
		if (rkFaces.empty())
		{
			throw std::exception("No face is passed.");
		}

		BRepBuilderAPI_Sewing occtSewing;
		for(const Face::Ptr& kpFace : rkFaces)
		{
			occtSewing.Add(kpFace->GetOcctShape());
		}

		occtSewing.Perform();
		Shell::Ptr pShell = std::make_shared<Shell>(TopoDS::Shell(occtSewing.SewedShape()));

		// HACK: add the v1 contents to the current shell faces.
		Topology::Ptr pUpcastShell = TopologicalQuery::Upcast<Topology>(pShell);
		//GlobalCluster::GetInstance().GetCluster()->AddChildLabel(pUpcastShell, REL_CONSTITUENT);

		for (const Face::Ptr& kShellFace : rkFaces)
		{
			const TopoDS_Shape& rkModifiedShape = occtSewing.Modified(kShellFace->GetOcctShape());
			Topology::Ptr pChildTopology = Topology::ByOcctShape(rkModifiedShape);
			//pShell->AddChildLabel(pChildTopology, REL_CONSTITUENT);

			// Map the aperture to the modifed shell faces.
			std::list<Topology::Ptr>& rContents = kShellFace->Contents();
			for (const Topology::Ptr& rkContent : rContents)
			{
				if (rkContent->GetType() != TOPOLOGY_APERTURE)
				{
					continue;
				}

				std::shared_ptr<Aperture> aperture = TopologicalQuery::Downcast<Aperture>(rkContent);

				if (aperture->Topology()->GetType() != TOPOLOGY_FACE)
				{
					continue;
				}

				Face::Ptr pApertureFace = TopologicalQuery::Downcast<Face>(aperture->Topology());
				Topology::Ptr pUpcastApertureFace = TopologicalQuery::Upcast<Topology>(pApertureFace);
				//pChildTopology->AddChildLabel(pUpcastApertureFace, REL_APERTURE);
			}
		}

		return pShell;
	}

	Shell::Ptr Shell::ByVerticesFaceIndices(const std::vector<Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkFaceIndices)
	{
		std::list<Face::Ptr> faces;
		for(const std::list<int>& rkVertexIndices : rkFaceIndices)
		{
			BRepBuilderAPI_MakeWire occtMakeWire;

			std::list<int>::const_iterator kSecondFromLastVertexIterator = --rkVertexIndices.end();
			for(std::list<int>::const_iterator kVertexIterator = rkVertexIndices.begin();
				kVertexIterator != kSecondFromLastVertexIterator;
				kVertexIterator++)
			{
				int vertexIndex = *kVertexIterator;

				std::list<int>::const_iterator kNextVertexIterator = kVertexIterator;
				kNextVertexIterator++;
				int nextVertexIndex = *kNextVertexIterator;

				occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
					TopoDS::Vertex(rkVertices[vertexIndex]->GetOcctShape()),
					TopoDS::Vertex(rkVertices[nextVertexIndex]->GetOcctShape())
				));
			}
			occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
				rkVertices[*--rkVertexIndices.end()]->GetOcctVertex(),
				rkVertices[*rkVertexIndices.begin()]->GetOcctVertex()
			));

			TopoDS_Wire pOcctWire(occtMakeWire);
			BRepBuilderAPI_MakeFace occtMakeFace(pOcctWire);
			faces.push_back(std::make_shared<Face>(occtMakeFace));
		}
		Shell::Ptr pShell = ByFaces(faces);

		// Only add the vertices; the faces are dealt with in ByFaces()
		/*for (std::vector<Vertex::Ptr>::const_iterator kVertexIterator = rkVertices.begin();
			kVertexIterator != rkVertices.end();
			kVertexIterator++)
		{
			const Vertex::Ptr& kpVertex = *kVertexIterator;
			kpVertex->AddIngredientTo(pShell);
		}*/
		return pShell;
	}

	Shell::Ptr Shell::ByFacePlanarization(
		const Face::Ptr& kpFace,
		const int kIteration,
		const int kEdgeSamples,
		const int kNumUPanels,
		const int kNumVPanels,
		const double kTolerance,
		const bool kCapBottom,
		const bool kCapTop,
		std::list<Vertex::Ptr>& vertices,
		std::list<Edge::Ptr>& edges,
		std::list<Wire::Ptr>& wires,
		std::list<Face::Ptr>& faces)
	{
		if (kNumUPanels < 1)
		{
			std::string errorMessage = std::string("The number of u-panels must be larger than 0.");
			throw std::exception(errorMessage.c_str());
		}
		if (kNumVPanels < 1)
		{
			std::string errorMessage = std::string("The number of v-panels must be larger than 0.");
			throw std::exception(errorMessage.c_str());
		}

		double dU = 1.0 / (double)kNumUPanels;
		double dV = 1.0 / (double)kNumVPanels;
		
		std::list<double> uValues;
		for (int i = 0; i < kNumUPanels + 1; ++i) // kNumUPanels + 1 because it is the vertices being processed
		{
			double u = (double)i * dU;
			if (u < 0.0) u = 0.0;
			else if (u > 1.0) u = 1.0;
			uValues.push_back(u);
		}

		std::list<double> vValues;
		for (int i = 0; i < kNumVPanels + 1; ++i)  // kNumVPanels + 1 because it is the vertices being processed
		{
			double v = (double)i * dV;
			if (v < 0.0) v = 0.0;
			else if (v > 1.0) v = 1.0;
			vValues.push_back(v);
		}

		return ByFacePlanarization(kpFace, kIteration, kEdgeSamples, uValues, vValues, kTolerance, kCapBottom, kCapTop, vertices, edges, wires, faces);
	}

	bool SutherlandHodgmanIsInside(const Handle(Geom2d_CartesianPoint)& kpPoint, const Handle(Geom2d_CartesianPoint)& kpLinePoint1, const Handle(Geom2d_CartesianPoint)& kpLinePoint2)
	{
		return (kpLinePoint2->X() - kpLinePoint1->X())*(kpPoint->Y() - kpLinePoint1->Y()) >
			   (kpLinePoint2->Y() - kpLinePoint1->Y())*(kpPoint->X() - kpLinePoint1->X());
	}

	Handle(Geom2d_CartesianPoint) ComputeIntersection(const Handle(Geom2d_CartesianPoint)& S, const Handle(Geom2d_CartesianPoint)& E, const Handle(Geom2d_CartesianPoint)& kpLinePoint1, const Handle(Geom2d_CartesianPoint)& kpLinePoint2)
	{
		Handle(Geom2d_CartesianPoint) dc = new Geom2d_CartesianPoint(kpLinePoint1->X() - kpLinePoint2->X(), kpLinePoint1->Y() - kpLinePoint2->Y());
		Handle(Geom2d_CartesianPoint) dp = new Geom2d_CartesianPoint(S->X() - E->X(), S->Y() - E->Y());
		double n1 = kpLinePoint1->X() * kpLinePoint2->Y() - kpLinePoint1->Y() * kpLinePoint2->X();
		double n2 = S->X() * E->Y() - S->Y() * E->X();
		double n3 = 1.0 / (dc->X() * dp->Y() - dc->Y() * dp->X());
		return new Geom2d_CartesianPoint((n1*dp->X() - n2 * dc->X()) * n3, (n1*dp->Y() - n2 * dc->Y()) * n3);
	}

	void SutherlandHodgmanClipping(
		const std::list<std::pair<Handle(Geom2d_CartesianPoint), Handle(Geom2d_CartesianPoint)>>& rkClipPolygons,
		const std::list<Handle(Geom2d_CartesianPoint)>& rkSubjectPoints,
		std::list<Handle(Geom2d_CartesianPoint)>& rOutputList)
	{
		rOutputList = rkSubjectPoints;
		for (const std::pair<Handle(Geom2d_CartesianPoint), Handle(Geom2d_CartesianPoint)>& kpClipEdge : rkClipPolygons)
		{
			if (!rOutputList.empty())
			{
				std::list<Handle(Geom2d_CartesianPoint)> inputList = rOutputList;
				rOutputList.clear();
				Handle(Geom2d_CartesianPoint) S = inputList.back();
				for (const Handle(Geom2d_CartesianPoint)& E : inputList)
				{
					if (SutherlandHodgmanIsInside(E, kpClipEdge.first, kpClipEdge.second))
					{
						if (!SutherlandHodgmanIsInside(S, kpClipEdge.first, kpClipEdge.second))
						{
							rOutputList.push_back(ComputeIntersection(S, E, kpClipEdge.first, kpClipEdge.second));
						}
						rOutputList.push_back(E);
					}
					else if (SutherlandHodgmanIsInside(S, kpClipEdge.first, kpClipEdge.second))
					{
						rOutputList.push_back(ComputeIntersection(S, E, kpClipEdge.first, kpClipEdge.second));
					}
					
					S = E;
				}
			}
		}
	}


	void VattiClipping(
		const std::list<Handle(Geom2d_CartesianPoint)>& rkClipPoints,
		const std::list<Handle(Geom2d_CartesianPoint)>& rkSubjectPoints,
		std::list<std::list<Handle(Geom2d_CartesianPoint)>>& rOutputPoints)
	{
		gpc_polygon subjectPolygon = { 0, nullptr, nullptr };;
		gpc_polygon clipPolygon = { 0, nullptr, nullptr };;
		gpc_polygon resultPolygon = { 0, nullptr, nullptr };;

		// transfer from Topologic
		gpc_vertex* pSubjectVertices = new gpc_vertex[rkSubjectPoints.size()];
		int i = 0;
		for (const Handle(Geom2d_CartesianPoint)& kpSubjectPoints : rkSubjectPoints)
		{
			pSubjectVertices[i] = { kpSubjectPoints->X(), kpSubjectPoints ->Y() };
			++i;
		}
		gpc_vertex_list subjectContour = { (int) rkSubjectPoints.size(), pSubjectVertices };
		gpc_add_contour(&subjectPolygon, &subjectContour, 0);

		gpc_vertex* pClipVertices = new gpc_vertex[rkClipPoints.size()];
		i = 0;
		for (const Handle(Geom2d_CartesianPoint)& kpClipVertices : rkClipPoints)
		{
			pClipVertices[i] = { kpClipVertices->X(), kpClipVertices->Y() };
			++i;
		}
		gpc_vertex_list clipContour = { (int) rkClipPoints.size(), pClipVertices };
		gpc_add_contour(&clipPolygon, &clipContour, 0);

		gpc_polygon_clip(GPC_INT, &subjectPolygon, &clipPolygon, &resultPolygon);

		// transfer back to Topologic
		for (int i = 0; i < resultPolygon.num_contours; ++i)
		{
			std::list<Handle(Geom2d_CartesianPoint)> outputContours;
			for (int j = 0; j < resultPolygon.contour[i].num_vertices; ++j)
			{
				outputContours.push_back(new Geom2d_CartesianPoint(
					resultPolygon.contour[i].vertex[j].x,
					resultPolygon.contour[i].vertex[j].y
				));
			}
			rOutputPoints.push_back(outputContours);
		}

		// clean up
		delete[] pClipVertices;
		delete[] pSubjectVertices;

		gpc_free_polygon(&resultPolygon);
		gpc_free_polygon(&clipPolygon);
		gpc_free_polygon(&subjectPolygon);
	}

	Shell::Ptr Shell::ByFacePlanarization(
		const Face::Ptr& kpFace, 
		const int kIteration, 
		const int kEdgeSamples,
		const std::list<double>& rkUValues, 
		const std::list<double>& rkVValues, 
		const double kTolerance,
		const bool kCapBottom,
		const bool kCapTop,
		std::list<Vertex::Ptr>& vertices,
		std::list<Edge::Ptr>& edges,
		std::list<Wire::Ptr>& wires,
		std::list<Face::Ptr>& faces)
	{
		// Check that kIteration, kNumUPanels, and kNumVPanels are 1 or more.
		if (kIteration < 1)
		{
			std::string errorMessage = std::string("The number of iteration must be larger than 0.");
			throw std::exception(errorMessage.c_str());
		}
		if (kEdgeSamples < 2)
		{
			std::string errorMessage = std::string("The number of sample points on edges must be larger than 1.");
			throw std::exception(errorMessage.c_str());
		}
		if (rkUValues.empty())
		{
			std::string errorMessage = std::string("The number of u-panels must be larger than 0.");
			throw std::exception(errorMessage.c_str());
		}
		if (rkVValues.empty())
		{
			std::string errorMessage = std::string("The number of v-panels must be larger than 0.");
			throw std::exception(errorMessage.c_str());
		}
		if (kTolerance <= 0.0)
		{
			std::string errorMessage = std::string("The tolerance must have a positive value");
			throw std::exception(errorMessage.c_str());
		}

		// Subdivide the face in the UV space and find the points
		Handle(Geom_Surface) pOcctWallSurface = kpFace->Surface();
		double occtUMin = 0.0, occtUMax = 0.0, occtVMin = 0.0, occtVMax = 0.0;
		ShapeAnalysis::GetFaceUVBounds(kpFace->GetOcctFace(), occtUMin, occtUMax, occtVMin, occtVMax);
		double occtURange = occtUMax - occtUMin;
		double occtVRange = occtVMax - occtVMin;
		int numUPanels = (int) rkUValues.size() - 1;
		int numVPanels = (int) rkVValues.size() - 1;
		bool isUClosed = pOcctWallSurface->IsUClosed();
		int numUPoints = numUPanels;
		if (!isUClosed)
		{
			numUPoints += 1;
		}
		bool isVClosed = pOcctWallSurface->IsVClosed();
		int numVPoints = numVPanels;
		if (!isVClosed)
		{
			numVPoints += 1;
		}

		// Compute OCCT's non-normalized UV values
		// At the same time, get the isolines
		std::list<double> occtUValues;
		BOPCol_ListOfShape occtIsolines;
		for(double u : rkUValues)
		{
			double occtU = occtUMin + occtURange * u;
			if (occtU < occtUMin) {
				occtU = occtUMin;
			}
			else if (occtU > occtUMax)
			{
				occtU = occtUMax;
			}
			occtUValues.push_back(occtU);
		};
		std::list<double> occtVValues;
		for(double v : rkVValues)
		{
			double occtV = occtVMin + occtVRange * v;
			if (occtV < occtVMin) {
				occtV = occtVMin;
			}
			else if (occtV > occtVMax)
			{
				occtV = occtVMax;
			}
			occtVValues.push_back(occtV);
		};

		// Initialise ShapeOp matrix
		int numOfPoints = numUPoints * numVPoints;
		ShapeOp::Matrix3X shapeOpMatrix(3, numOfPoints); //column major
		int i = 0;
		std::list<double>::const_iterator endUIterator = occtUValues.end();
		if (isUClosed)
		{
			endUIterator--;
		}
		std::list<double>::const_iterator endVIterator = occtVValues.end();
		if (isVClosed)
		{
			endVIterator--;
		}

		for (const double kU : occtUValues)
		{
			for (const double kV : occtVValues)
			{
				gp_Pnt occtPoint = pOcctWallSurface->Value(kU, kV);
				shapeOpMatrix(0, i) = occtPoint.X();
				shapeOpMatrix(1, i) = occtPoint.Y();
				shapeOpMatrix(2, i) = occtPoint.Z();

				++i;
			}
		}

		std::list<Topology::Ptr>& rContents = kpFace->Contents();
		std::list<std::list<Handle(Geom2d_CartesianPoint)>> allApertureSampleVerticesUV;
		for (const Topology::Ptr& rkContent : rContents)
		{
			if (rkContent->GetType() != TOPOLOGY_APERTURE)
			{
				continue;
			}

			std::shared_ptr<Aperture> aperture = TopologicalQuery::Downcast<Aperture>(rkContent);

			if (aperture->Topology()->GetType() != TOPOLOGY_FACE)
			{
				continue;
			}

			Face::Ptr apertureFace = TopologicalQuery::Downcast<Face>(aperture->Topology());
			Wire::Ptr apertureWire = apertureFace->OuterBoundary();
			//wires.push_back(apertureWire);
			std::list<Edge::Ptr> apertureEdges;
			apertureWire->Edges(apertureEdges, true);
			
			for (Edge::Ptr e : apertureEdges)
			{
				edges.push_back(e);
			}
			std::list<Handle(Geom2d_CartesianPoint)> apertureSampleVerticesUV;
			for (const Edge::Ptr& kpEdge : apertureEdges)
			{
				// HACK: go backward
				//for (int i = 0; i < numberOfSamples - 1; ++i)
				for (int i = kEdgeSamples - 2; i >= 0; --i)
				{
					double parameter = (double)i / (double)(kEdgeSamples - 1);
					Vertex::Ptr apertureSampleVertex = kpEdge->PointAtParameter(parameter);
					//edges.push_back(kpEdge);
					//vertices.push_back(apertureSampleVertex);

					// Get the UV coordinate of the aperture sample vertex
					double apertureSampleVertexU = 0.0, apertureSampleVertexV = 0.0;
					kpFace->UVParameterAtPoint(apertureSampleVertex, apertureSampleVertexU, apertureSampleVertexV);
					apertureSampleVerticesUV.push_back(new Geom2d_CartesianPoint(apertureSampleVertexU, apertureSampleVertexV));
					//vertices.push_back(kpFace->PointAtParameter(apertureSampleVertexU, apertureSampleVertexV));
				}
			}
			// CHECK draw the subject polygons
			//{
			//	if (apertureSampleVerticesUV.size() > 2)
			//	{
			//		std::list<TopoDS_Vertex> testVertices;
			//		for (const Handle(Geom2d_CartesianPoint)& point : apertureSampleVerticesUV)
			//		{
			//			//DEBUG: draw the polygon
			//			BRepBuilderAPI_MakeVertex occtMakeVertex(pOcctWallSurface->Value(point->X(), point->Y()));
			//			//BRepBuilderAPI_MakeVertex occtMakeVertex(pOcctWallSurface->Value(point[0], point[1]));

			//			const TopoDS_Vertex& rkOcctVertex = occtMakeVertex.Vertex();
			//			testVertices.push_back(rkOcctVertex);
			//			vertices.push_back(TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex)));
			//		}

			//		std::list<TopoDS_Vertex>::const_iterator endIterator = testVertices.end();
			//		endIterator--;

			//		TopTools_ListOfShape edges;
			//		for (std::list<TopoDS_Vertex>::const_iterator vertexIter = testVertices.begin();
			//			vertexIter != endIterator;
			//			vertexIter++)
			//		{
			//			std::list<TopoDS_Vertex>::const_iterator nextIter = vertexIter;
			//			nextIter++;
			//			BRepBuilderAPI_MakeEdge occtMakeEdge(*vertexIter, *nextIter);
			//			if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
			//			{
			//				continue;
			//			}
			//			edges.Append(occtMakeEdge);
			//		}
			//		BRepBuilderAPI_MakeEdge occtMakeEdge(testVertices.back(), testVertices.front());
			//		if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
			//		{
			//		continue;
			//		}
			//		edges.Append(occtMakeEdge);

			//		BRepBuilderAPI_MakeWire occtMakeWire;
			//		occtMakeWire.Add(edges);

			//		// subject polygon
			//		const TopoDS_Wire& rkMappedApertureWire = occtMakeWire.Wire();
			//		//wires.push_back(TopologicalQuery::Downcast<Wire>(Topology::ByOcctShape(rkMappedApertureWire)));
			//	}
			//}
			allApertureSampleVerticesUV.push_back(apertureSampleVerticesUV);
		}

		// Solve ShapeOp, including applying constraints.
		ShapeOp::Solver shapeOpSolver;
		shapeOpSolver.setPoints(shapeOpMatrix);
		ShapeOp::Scalar weight = 1.0;

		// Planarity constraint to the panels
		// i and j are just used to iterate, the actual indices are the u and v variables.
		for (int i = 0; i < numUPanels; ++i)
		{
			for (int j = 0; j < numVPanels; ++j)
			{
				// Default values
				int minU = i;		int minV = j;
				int maxU = i + 1;	int maxV = j + 1;

				// Border values
				if (isUClosed && i == numUPanels - 1)
				{
					maxU = 0;
				}
				if (isVClosed && j == numVPanels - 1)
				{
					maxV = 0;
				}

				std::vector<int> vertexIDs;
				vertexIDs.push_back(minU * numVPanels + minV);
				vertexIDs.push_back(minU * numVPanels + maxV);
				vertexIDs.push_back(maxU * numVPanels + maxV);
				vertexIDs.push_back(maxU * numVPanels + maxV);
				auto shapeOpConstraint = std::make_shared<ShapeOp::PlaneConstraint>(vertexIDs, weight, shapeOpSolver.getPoints());
				shapeOpSolver.addConstraint(shapeOpConstraint);
			}
		}

		// Closeness constraints to the vertices on the edges along the u-axis
		for (int i = 0; i < numUPoints; ++i)
		{
			{
				std::vector<int> vertexIDs;
				vertexIDs.push_back(i * numVPoints);
				auto shapeOpConstraint = std::make_shared<ShapeOp::ClosenessConstraint>(vertexIDs, weight, shapeOpSolver.getPoints());
				shapeOpSolver.addConstraint(shapeOpConstraint);
			}

			if (!isVClosed)
			{
				std::vector<int> vertexIDs;
				vertexIDs.push_back((i + 1) * numVPoints - 1);
				auto shapeOpConstraint = std::make_shared<ShapeOp::ClosenessConstraint>(vertexIDs, weight, shapeOpSolver.getPoints());
				shapeOpSolver.addConstraint(shapeOpConstraint);
			}
		}

		// Closeness constraints to the vertices on the edge along the v-axis
		for (int j = 0; j < numVPoints; ++j)
		{
			{
				std::vector<int> vertexIDs;
				vertexIDs.push_back(j);
				auto shapeOpConstraint = std::make_shared<ShapeOp::ClosenessConstraint>(vertexIDs, weight, shapeOpSolver.getPoints());
				shapeOpSolver.addConstraint(shapeOpConstraint);
			}

			if (!isUClosed)
			{
				std::vector<int> vertexIDs;
				vertexIDs.push_back((numUPoints - 1) * numVPoints + j);
				auto shapeOpConstraint = std::make_shared<ShapeOp::ClosenessConstraint>(vertexIDs, weight, shapeOpSolver.getPoints());
				shapeOpSolver.addConstraint(shapeOpConstraint);
			}
		}

		// Closeness constraints to the vertices not on the surface's edges
		for (int i = 1; i < numUPanels; ++i)
		{
			for (int j = 1; j < numVPanels; ++j)
			{
				std::vector<int> vertexIDs;
				vertexIDs.push_back(i * numVPoints + j);
				auto shapeOpConstraint = std::make_shared<ShapeOp::ClosenessConstraint>(vertexIDs, 0.1*weight, shapeOpSolver.getPoints());
				shapeOpSolver.addConstraint(shapeOpConstraint);
			}
		}

		bool initialisationResult = shapeOpSolver.initialize();
		if (!initialisationResult)
			throw std::exception("Failed to initialize solver.");
		bool solveResult = shapeOpSolver.solve(kIteration);
		if (!solveResult)
			throw std::exception("Failed to solve.");
		const ShapeOp::Matrix3X& rkShapeOpResult = shapeOpSolver.getPoints();

		// Iterate against the wall panels
		// Create faces from rkShapeOpResult
		TopTools_ListOfShape borderEdges[4]; // 0 = uMin, 1 = uMax, 2 = vMin, 3 = vMax
											 // Use array for iteration

		// The u and v values are normalised. These are from the wall.
		TopTools_DataMapOfShapeListOfShape occtMapFacePanelToApertures;
		BRepBuilderAPI_Sewing occtSewing;
		endUIterator = occtUValues.end();
		endUIterator--;
		endVIterator = occtVValues.end();
		endVIterator--;
		i = 0;
		BOPCol_ListOfShape panelFaces;
		for (std::list<double>::const_iterator uIterator = occtUValues.begin();
			uIterator != endUIterator;
			uIterator++, ++i)
		{
			double wallU = *uIterator;
			//double offsetU = wallU + 0.001; // shrink
			double offsetU = wallU; // not shrink
			std::list<double>::const_iterator nextUIterator = uIterator;
			nextUIterator++;
			double nextWallU = *nextUIterator;
			//double nextOffsetU = nextWallU - 0.001; // shrink
			double nextOffsetU = nextWallU; // not shrink

			int j = 0;
			for (std::list<double>::const_iterator vIterator = occtVValues.begin();
				vIterator != endVIterator;
				vIterator++, ++j)
			{
				double wallV = *vIterator;
				//double offsetV = wallV + 0.001;
				double offsetV = wallV;
				std::list<double>::const_iterator nextVIterator = vIterator;
				nextVIterator++;
				double nextWallV = *nextVIterator;
				//double nextOffsetV = nextWallV - 0.001;
				double nextOffsetV = nextWallV;

				// Default values
				int minU = i;		int minV = j;
				int maxU = i + 1;	int maxV = j + 1;

				// Border values for ShapeOp result
				if (isUClosed && i == numUPanels - 1)
				{
					maxU = 0;
				}
				if (isVClosed && j == numVPanels - 1)
				{
					maxV = 0;
				}

				{
					//DEBUG: draw the shrunk faces
					BRepBuilderAPI_MakeVertex occtMakeVertex1(kpFace->PointAtParameter(offsetU, offsetV)->Point()->Pnt());
					BRepBuilderAPI_MakeVertex occtMakeVertex2(kpFace->PointAtParameter(offsetU, nextOffsetV)->Point()->Pnt());
					BRepBuilderAPI_MakeVertex occtMakeVertex3(kpFace->PointAtParameter(nextOffsetU, nextOffsetV)->Point()->Pnt());
					BRepBuilderAPI_MakeVertex occtMakeVertex4(kpFace->PointAtParameter(nextOffsetU, offsetV)->Point()->Pnt());

					const TopoDS_Vertex& rkOcctVertex1 = occtMakeVertex1.Vertex();
					const TopoDS_Vertex& rkOcctVertex2 = occtMakeVertex2.Vertex();
					const TopoDS_Vertex& rkOcctVertex3 = occtMakeVertex3.Vertex();
					const TopoDS_Vertex& rkOcctVertex4 = occtMakeVertex4.Vertex();

					Vertex::Ptr pVertex1 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex1));
					Vertex::Ptr pVertex2 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex2));
					Vertex::Ptr pVertex3 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex3));
					Vertex::Ptr pVertex4 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex4));

					BRepBuilderAPI_MakeEdge occtMakeEdge12(rkOcctVertex1, rkOcctVertex2);
					BRepBuilderAPI_MakeEdge occtMakeEdge23(rkOcctVertex2, rkOcctVertex3);
					BRepBuilderAPI_MakeEdge occtMakeEdge34(rkOcctVertex3, rkOcctVertex4);
					BRepBuilderAPI_MakeEdge occtMakeEdge41(rkOcctVertex4, rkOcctVertex1);

					const TopoDS_Shape& rkEdge12 = occtMakeEdge12.Shape();
					const TopoDS_Shape& rkEdge23 = occtMakeEdge23.Shape();
					const TopoDS_Shape& rkEdge34 = occtMakeEdge34.Shape();
					const TopoDS_Shape& rkEdge41 = occtMakeEdge41.Shape();

					TopTools_ListOfShape occtEdges;
					occtEdges.Append(rkEdge34);
					occtEdges.Append(rkEdge41);
					occtEdges.Append(rkEdge12);
					occtEdges.Append(rkEdge23);

					BRepBuilderAPI_MakeWire occtMakeWire;
					occtMakeWire.Add(occtEdges);

					const TopoDS_Wire& rkPanelWire = occtMakeWire.Wire();
					//wires.push_back(TopologicalQuery::Downcast<Wire>(Topology::ByOcctShape(rkPanelWire)));
				}

				// Create the panels to be sewn into a shell.
				// (0,0)
				int index1 = minU * numVPoints + minV;
				Handle(Geom_Point) pOcctPoint1 = new Geom_CartesianPoint(rkShapeOpResult(0, index1), rkShapeOpResult(1, index1), rkShapeOpResult(2, index1));

				// (0,1)
				int index2 = minU * numVPoints + maxV;
				Handle(Geom_Point) pOcctPoint2 = new Geom_CartesianPoint(rkShapeOpResult(0, index2), rkShapeOpResult(1, index2), rkShapeOpResult(2, index2));

				// (1,1)
				int index3 = maxU * numVPoints + maxV;
				Handle(Geom_Point) pOcctPoint3 = new Geom_CartesianPoint(rkShapeOpResult(0, index3), rkShapeOpResult(1, index3), rkShapeOpResult(2, index3));

				// (1,0)
				int index4 = maxU * numVPoints + minV;
				Handle(Geom_Point) pOcctPoint4 = new Geom_CartesianPoint(rkShapeOpResult(0, index4), rkShapeOpResult(1, index4), rkShapeOpResult(2, index4));

				BRepBuilderAPI_MakeVertex occtMakeVertex1(pOcctPoint1->Pnt());
				BRepBuilderAPI_MakeVertex occtMakeVertex2(pOcctPoint2->Pnt());
				BRepBuilderAPI_MakeVertex occtMakeVertex3(pOcctPoint3->Pnt());
				BRepBuilderAPI_MakeVertex occtMakeVertex4(pOcctPoint4->Pnt());

				const TopoDS_Vertex& rkOcctVertex1 = occtMakeVertex1.Vertex();
				const TopoDS_Vertex& rkOcctVertex2 = occtMakeVertex2.Vertex();
				const TopoDS_Vertex& rkOcctVertex3 = occtMakeVertex3.Vertex();
				const TopoDS_Vertex& rkOcctVertex4 = occtMakeVertex4.Vertex();

				Vertex::Ptr pWallVertex1 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex1));
				Vertex::Ptr pWallVertex2 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex2));
				Vertex::Ptr pWallVertex3 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex3));
				Vertex::Ptr pWallVertex4 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex4));

				BRepBuilderAPI_MakeEdge occtMakeEdge12(rkOcctVertex1, rkOcctVertex2);
				BRepBuilderAPI_MakeEdge occtMakeEdge23(rkOcctVertex2, rkOcctVertex3);
				BRepBuilderAPI_MakeEdge occtMakeEdge34(rkOcctVertex3, rkOcctVertex4);
				BRepBuilderAPI_MakeEdge occtMakeEdge41(rkOcctVertex4, rkOcctVertex1);

				const TopoDS_Shape& rkEdge12 = occtMakeEdge12.Shape();
				const TopoDS_Shape& rkEdge23 = occtMakeEdge23.Shape();
				const TopoDS_Shape& rkEdge34 = occtMakeEdge34.Shape();
				const TopoDS_Shape& rkEdge41 = occtMakeEdge41.Shape();

				TopTools_ListOfShape occtEdges;
				occtEdges.Append(rkEdge34);
				occtEdges.Append(rkEdge41);
				occtEdges.Append(rkEdge12);
				occtEdges.Append(rkEdge23);

				BRepBuilderAPI_MakeWire occtMakeWire;
				occtMakeWire.Add(occtEdges);

				const TopoDS_Wire& rkPanelWire = occtMakeWire.Wire();
				ShapeFix_ShapeTolerance occtShapeTolerance;
				occtShapeTolerance.SetTolerance(rkPanelWire, kTolerance, TopAbs_WIRE);
				BRepBuilderAPI_MakeFace occtMakeFace(rkPanelWire);
				const TopoDS_Face& rkOcctPanelFace = occtMakeFace.Face();

				//====================
				// HERE
				occtSewing.Add(rkOcctPanelFace);
				panelFaces.Append(rkOcctPanelFace);

				Handle(Geom_Surface) pOcctPanelSurface = BRep_Tool::Surface(rkOcctPanelFace);

				//========================
				// Map the apertures.
				ShapeAnalysis_Surface occtSurfaceAnalysis(pOcctPanelSurface);
				Face::Ptr pPanelFace = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(rkOcctPanelFace));
				
				// These are all the vertices of (and the UV on) the panel (the shell of flat faces).
				// The UV are NON-NORMALISED
				// (0,0)
				gp_Pnt2d occtUVWallVertex1OnPanel = occtSurfaceAnalysis.ValueOfUV(pWallVertex1->Point()->Pnt(), Precision::Confusion());
				double panelCorner1U = occtUVWallVertex1OnPanel.X(), panelCorner1V = occtUVWallVertex1OnPanel.Y();

				// (0,1)
				gp_Pnt2d occtUVWallVertex2OnPanel = occtSurfaceAnalysis.ValueOfUV(pWallVertex2->Point()->Pnt(), Precision::Confusion());
				double panelCorner2U = occtUVWallVertex2OnPanel.X(), panelCorner2V = occtUVWallVertex2OnPanel.Y();

				// (1,1)
				gp_Pnt2d occtUVWallVertex3OnPanel = occtSurfaceAnalysis.ValueOfUV(pWallVertex3->Point()->Pnt(), Precision::Confusion());
				double panelCorner3U = occtUVWallVertex3OnPanel.X(), panelCorner3V = occtUVWallVertex3OnPanel.Y();

				// (1,0)
				gp_Pnt2d occtUVWallVertex4OnPanel = occtSurfaceAnalysis.ValueOfUV(pWallVertex4->Point()->Pnt(), Precision::Confusion());
				double panelCorner4U = occtUVWallVertex4OnPanel.X(), panelCorner4V = occtUVWallVertex4OnPanel.Y();
				
				//Compute deltas
				double deltaPanelBottomU = panelCorner4U - panelCorner1U;
				double deltaPanelBottomV = panelCorner4V - panelCorner1V;

				double deltaPanelTopU = panelCorner3U - panelCorner2U;
				double deltaPanelTopV = panelCorner3V - panelCorner2V;

				if (kCapBottom || kCapTop)
				{
					if (!isUClosed && j == 0) // Use j == 0 to do this only once
					{
						if (kCapBottom && i == 0) // Store the edge rkEdge12 to uMin edges 
						{
							borderEdges[0].Append(rkEdge12);
						}
						else if (kCapTop && i == numUPanels - 1) // Store the edge rkEdge34 to uMax edges 
						{
							borderEdges[1].Append(rkEdge34);
						}
					}

					if (!isVClosed)
					{
						if (kCapBottom && j == 0) // Store the edge rkEdge41 to vMin edges 
						{
							borderEdges[2].Append(rkEdge41);
						}
						else if (kCapTop && j == numVPanels - 1) // Store the edge rkEdge23 to vMax edges 
						{
							borderEdges[3].Append(rkEdge23);
						}
					}
				} // if (kCapBottom || kCapTop)


				// Clip the apertures sample points UV against the wall panel's UV using Sutherland-Hodgman.
				// The clip polygons are offseted.
				// For Sutherland-Hodgman
				std::list<std::pair<Handle(Geom2d_CartesianPoint), Handle(Geom2d_CartesianPoint)>> clipPolygons;
				clipPolygons.push_back(std::make_pair<Handle(Geom2d_CartesianPoint), Handle(Geom2d_CartesianPoint)>(
					new Geom2d_CartesianPoint(offsetU, offsetV), 
					new Geom2d_CartesianPoint(nextOffsetU, offsetV)));
				clipPolygons.push_back(std::make_pair<Handle(Geom2d_CartesianPoint), Handle(Geom2d_CartesianPoint)>(
					new Geom2d_CartesianPoint(nextOffsetU, offsetV), 
					new Geom2d_CartesianPoint(nextOffsetU, nextOffsetV)));
				clipPolygons.push_back(std::make_pair<Handle(Geom2d_CartesianPoint), Handle(Geom2d_CartesianPoint)>(
					new Geom2d_CartesianPoint(nextOffsetU, nextOffsetV), 
					new Geom2d_CartesianPoint(offsetU, nextOffsetV)));
				clipPolygons.push_back(std::make_pair<Handle(Geom2d_CartesianPoint), Handle(Geom2d_CartesianPoint)>(
					new Geom2d_CartesianPoint(offsetU, nextOffsetV),
					new Geom2d_CartesianPoint(offsetU, offsetV)));

				// For Vatti
				std::list<Handle(Geom2d_CartesianPoint)> clipPoints;
				clipPoints.push_back(new Geom2d_CartesianPoint(offsetU, offsetV));
				clipPoints.push_back(new Geom2d_CartesianPoint(nextOffsetU, offsetV));
				clipPoints.push_back(new Geom2d_CartesianPoint(nextOffsetU, nextOffsetV));
				clipPoints.push_back(new Geom2d_CartesianPoint(offsetU, nextOffsetV));

				/*vertices.push_back(kpFace->PointAtParameter(u, v));
				vertices.push_back(kpFace->PointAtParameter(u, nextV));
				vertices.push_back(kpFace->PointAtParameter(nextU, nextV));
				vertices.push_back(kpFace->PointAtParameter(nextU, v));*/

				// List of apertures mapped to this panel
				BOPCol_ListOfShape aperturePanels;

				using Coord = double;

				// The index type. Defaults to uint32_t, but you can also pass uint16_t if you know that your
				// data won't have more than 65536 vertices.
				using N = uint32_t;

				// Create array
				using Point = std::array<Coord, 2>;

				// Fill polygon structure with actual data. Any winding order works.
				// The first polyline defines the main polygon.

				for (const std::list<Handle(Geom2d_CartesianPoint)>& rkApertureSampleVerticesUV : allApertureSampleVerticesUV)
				{

					// The result is in the original surface's UV
					std::list<std::list<Handle(Geom2d_CartesianPoint)>> clippedApertureSampleVerticesUVOriginalWall;
					//SutherlandHodgmanClipping(clipPolygons, rkApertureSampleVerticesUV, clippedApertureSampleVerticesUVOriginalWall);
					VattiClipping(clipPoints, rkApertureSampleVerticesUV, clippedApertureSampleVerticesUVOriginalWall);

					//std::list<TopoDS_Vertex> occtMappedApertureVertices;
					for (const std::list<Handle(Geom2d_CartesianPoint)>& kpClippedApertureSampleVertexUVOriginalWallList : clippedApertureSampleVerticesUVOriginalWall)
					{
						std::vector<Point> polygon;
						for (const Handle(Geom2d_CartesianPoint)& kpClippedApertureSampleVertexUVOriginalWall : kpClippedApertureSampleVertexUVOriginalWallList)
						{
							/*vertices.push_back(kpFace->PointAtParameter(
								kpClippedApertureSampleVertexUVOriginalWall->X(),
								kpClippedApertureSampleVertexUVOriginalWall->Y())
								);*/

							// normalised to the panel, use U and V					
							// However, the panel currently has arbitrary UV, with the min and max values located outside the actual panel (the wire boundary).
							Handle(Geom2d_CartesianPoint) clippedApertureSampleVerticesUVPanel = new Geom2d_CartesianPoint(
								(kpClippedApertureSampleVertexUVOriginalWall->X() - wallU) / (nextWallU - wallU),
								(kpClippedApertureSampleVertexUVOriginalWall->Y() - wallV) / (nextWallV - wallV)
							);

							// Translate the clipped aperture sample vertices UV to the (NON-NORMALISED) UV coordinate of pOcctPanelSurface,
							// which now has its own UV coordinate.
							// Use bilinear mapping.

							// The first step uses X
							double mappedAperturePointUBottom = panelCorner1U + clippedApertureSampleVerticesUVPanel->X() * deltaPanelBottomU;
							double mappedAperturePointVBottom = panelCorner1V + clippedApertureSampleVerticesUVPanel->X() * deltaPanelBottomV;
							double mappedAperturePointUTop = panelCorner2U + clippedApertureSampleVerticesUVPanel->X() * deltaPanelTopU;
							double mappedAperturePointVTop = panelCorner2V + clippedApertureSampleVerticesUVPanel->X() * deltaPanelTopV;

							double deltaMappedAperturePointU = mappedAperturePointUTop - mappedAperturePointUBottom;
							double deltaMappedAperturePointV = mappedAperturePointVTop - mappedAperturePointVBottom;

							// 2nd step uses Y
							double mappedAperturePointU = mappedAperturePointUBottom + clippedApertureSampleVerticesUVPanel->Y() * deltaMappedAperturePointU;
							double mappedAperturePointV = mappedAperturePointVBottom + clippedApertureSampleVerticesUVPanel->Y() * deltaMappedAperturePointV;
							polygon.push_back({ mappedAperturePointU, mappedAperturePointV });
							/*vertices.push_back(Vertex::ByPoint(new Geom_CartesianPoint(pOcctPanelSurface->Value(
								mappedAperturePointU,
								mappedAperturePointV)))
							);*/
						}

						std::vector<std::vector<Point>> polygons;
						polygons.push_back(polygon);
						{
							if (polygon.size() > 2)
							{
								std::list<TopoDS_Vertex> testVertices;
								for (const Point& point : polygon)
								{
									//DEBUG: draw the polygon
									BRepBuilderAPI_MakeVertex occtMakeVertex(pOcctPanelSurface->Value(point[0], point[1]));
									//BRepBuilderAPI_MakeVertex occtMakeVertex(pOcctWallSurface->Value(point[0], point[1]));

									const TopoDS_Vertex& rkOcctVertex = occtMakeVertex.Vertex();
									testVertices.push_back(rkOcctVertex);
									//vertices.push_back(TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex)));
								}

								std::list<TopoDS_Vertex>::const_iterator endIterator = testVertices.end();
								endIterator--;

								TopTools_ListOfShape edges;
								for (std::list<TopoDS_Vertex>::const_iterator vertexIter = testVertices.begin();
									vertexIter != endIterator;
									vertexIter++)
								{
									std::list<TopoDS_Vertex>::const_iterator nextIter = vertexIter;
									nextIter++;
									BRepBuilderAPI_MakeEdge occtMakeEdge(*vertexIter, *nextIter);
									if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
									{
										continue;
									}
									edges.Append(occtMakeEdge);
								}
								BRepBuilderAPI_MakeEdge occtMakeEdge(testVertices.back(), testVertices.front());
								if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
								{
									continue;
								}
								edges.Append(occtMakeEdge);

								BRepBuilderAPI_MakeWire occtMakeWire;
								occtMakeWire.Add(edges);

								// apertures before triangulation
								const TopoDS_Wire& rkMappedApertureWire = occtMakeWire.Wire();
								wires.push_back(TopologicalQuery::Downcast<Wire>(Topology::ByOcctShape(rkMappedApertureWire)));
								BRepBuilderAPI_MakeFace occtMakeFace(pOcctPanelSurface, rkMappedApertureWire);
								faces.push_back(TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(occtMakeFace)));
							}
						}

						std::vector<N> indices = mapbox::earcut<N>(polygons);
						int numberOfTriangles = (int)indices.size() / 3;
						for (int i = 0; i < numberOfTriangles; ++i)
						{
							int index1 = i * 3;
							int index2 = i * 3 + 1;
							int index3 = i * 3 + 2;

							Point p1 = polygon[indices[index1]];
							gp_Pnt occtMappedAperturePoint1 = pOcctPanelSurface->Value(p1[0], p1[1]);
							Point p2 = polygon[indices[index2]];
							gp_Pnt occtMappedAperturePoint2 = pOcctPanelSurface->Value(p2[0], p2[1]);
							Point p3 = polygon[indices[index3]];
							gp_Pnt occtMappedAperturePoint3 = pOcctPanelSurface->Value(p3[0], p3[1]);


							//==============
							//SCALE DOWN HERE
							double sumX = occtMappedAperturePoint1.X() + occtMappedAperturePoint2.X() + occtMappedAperturePoint3.X();
							double sumY = occtMappedAperturePoint1.Y() + occtMappedAperturePoint2.Y() + occtMappedAperturePoint3.Y();
							double sumZ = occtMappedAperturePoint1.Z() + occtMappedAperturePoint2.Z() + occtMappedAperturePoint3.Z();

							gp_Pnt faceCentroid(sumX / 3.0, sumY / 3.0, sumZ / 3.0);

							double sqrtScaleFactor = 0.99;
							
							gp_Vec vector1(faceCentroid, occtMappedAperturePoint1);
							gp_Vec vector2(faceCentroid, occtMappedAperturePoint2);
							gp_Vec vector3(faceCentroid, occtMappedAperturePoint3);

							vector1.Multiply(sqrtScaleFactor);
							vector2.Multiply(sqrtScaleFactor);
							vector3.Multiply(sqrtScaleFactor);

							occtMappedAperturePoint1 = gp_Pnt(
								faceCentroid.X() + vector1.X(),
								faceCentroid.Y() + vector1.Y(), 
								faceCentroid.Z() + vector1.Z());
							occtMappedAperturePoint2 = gp_Pnt(
								faceCentroid.X() + vector2.X(),
								faceCentroid.Y() + vector2.Y(),
								faceCentroid.Z() + vector2.Z());
							occtMappedAperturePoint3 = gp_Pnt(
								faceCentroid.X() + vector3.X(),
								faceCentroid.Y() + vector3.Y(),
								faceCentroid.Z() + vector3.Z());
							//==========================

							BRepBuilderAPI_MakeVertex occtMakeVertex1(occtMappedAperturePoint1);
							BRepBuilderAPI_MakeVertex occtMakeVertex2(occtMappedAperturePoint2);
							BRepBuilderAPI_MakeVertex occtMakeVertex3(occtMappedAperturePoint3);

							BRepBuilderAPI_MakeEdge occtMakeEdge12(occtMakeVertex1.Vertex(), occtMakeVertex2.Vertex());
							BRepBuilderAPI_MakeEdge occtMakeEdge23(occtMakeVertex2.Vertex(), occtMakeVertex3.Vertex());
							BRepBuilderAPI_MakeEdge occtMakeEdge31(occtMakeVertex3.Vertex(), occtMakeVertex1.Vertex());

							try {
								TopTools_ListOfShape occtMappedApertureEdges;
								occtMappedApertureEdges.Append(occtMakeEdge12.Edge());
								occtMappedApertureEdges.Append(occtMakeEdge23.Edge());
								occtMappedApertureEdges.Append(occtMakeEdge31.Edge());

								BRepBuilderAPI_MakeWire occtMakeMappedApertureWire;
								occtMakeMappedApertureWire.Add(occtMappedApertureEdges);
								BRepBuilderAPI_MakeFace occtMakeMappedApertureFace(pOcctPanelSurface, occtMakeMappedApertureWire.Wire());
								Face::Ptr mappedApertureFace = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(occtMakeMappedApertureFace.Face()));
								aperturePanels.Append(occtMakeMappedApertureFace.Shape());

								//faces.push_back(mappedApertureFace);
							}
							catch (...)
							{

							}
						}
					}

				}

				occtMapFacePanelToApertures.Bind(rkOcctPanelFace, aperturePanels);
			}
		}

		// These are the caps, no need to introduce apertures.
		if (kCapBottom || kCapTop)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (!borderEdges[i].IsEmpty())
				{
					// Make wire from the edges.
					BRepBuilderAPI_MakeWire occtMakeWire;
					occtMakeWire.Add(borderEdges[i]);

					const TopoDS_Wire& rkOuterWire = occtMakeWire.Wire();
					ShapeFix_ShapeTolerance occtShapeTolerance;
					occtShapeTolerance.SetTolerance(rkOuterWire, kTolerance, TopAbs_WIRE);
					BRepBuilderAPI_MakeFace occtMakeFace(TopoDS::Wire(occtMakeWire.Shape()));
					const TopoDS_Face& rkOcctFace = occtMakeFace.Face();
					occtSewing.Add(rkOcctFace);
				}
			}
		}

		occtSewing.Perform();

		// Reconstruct the shape
		Shell::Ptr pOutputShell = std::make_shared<Shell>(TopoDS::Shell(occtSewing.SewedShape()));

		// Register the shapes to the OCCT documents.
		Topology::Ptr pUpcastTopology = TopologicalQuery::Upcast<Topology>(pOutputShell);
		//GlobalCluster::GetInstance().GetCluster()->AddChildLabel(pUpcastTopology, REL_CONSTITUENT);

		// Iterate through the faces of the shell and attach them as labels of the shell's label.
		std::list<Face::Ptr> outputFaces;
		pOutputShell->Faces(outputFaces);

		TopTools_ListOfShape occtOutputFaces;
		for (TopExp_Explorer occtExplorer(pOutputShell->GetOcctShape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtOutputFaces.Contains(occtCurrent))
			{
				occtOutputFaces.Append(occtCurrent);
			}
		}

		int counter = 0;
		for (TopTools_DataMapIteratorOfDataMapOfShapeListOfShape iterator(occtMapFacePanelToApertures);
			iterator.More();
			iterator.Next())
		{
			bool isModified = occtSewing.IsModifiedSubShape(iterator.Key());
			if (isModified)
			{
				// This is a panel face, which is added to the sewer object.
				const TopoDS_Shape& rkKeyShape = iterator.Key();
				bool isInList = panelFaces.Contains(rkKeyShape);

				// This should be a face in the output shell
				TopoDS_Shape occtModifiedShape = occtSewing.ModifiedSubShape(rkKeyShape);
				Topology::Ptr modifiedShape = Topology::ByOcctShape(occtModifiedShape);

				//pOutputShell->AddChildLabel(modifiedShape, REL_CONSTITUENT);

				// The apertures of the modifiedShape are transferred to 
				const BOPCol_ListOfShape& rkOcctApertures = iterator.Value();
				for (BOPCol_ListIteratorOfListOfShape occtApertureIterator(rkOcctApertures);
					occtApertureIterator.More();
					occtApertureIterator.Next())
				{
					const TopoDS_Shape& rkOcctAperture = occtApertureIterator.Value();
					Topology::Ptr aperture = Topology::ByOcctShape(rkOcctAperture);
					//modifiedShape->AddChildLabel(aperture, REL_APERTURE);
				}
			}
		}

		return pOutputShell;
	}

	Shell::Ptr Shell::ByLoft(const std::list<Wire::Ptr>& rkWires)
	{
		BRepOffsetAPI_ThruSections occtLoft;
		for(const Wire::Ptr& kpWire : rkWires)
		{
			occtLoft.AddWire(kpWire->GetOcctWire());
		};
		try {
			occtLoft.Build();
		}
		catch (...)
		{
			throw std::exception("Loft error");
		}
		return std::make_shared<Shell>(TopoDS::Shell(occtLoft.Shape()));
	}

	TopoDS_Shape& Shell::GetOcctShape()
	{
		return GetOcctShell();
	}

	const TopoDS_Shape& Shell::GetOcctShape() const
	{
		return GetOcctShell();
	}

	TopoDS_Shell& Shell::GetOcctShell()
	{
		assert(!m_occtShell.IsNull() && "Shell::m_occtShell is null.");
		if (m_occtShell.IsNull())
		{
			throw std::exception("Shell::m_occtShell is null.");
		}

		return m_occtShell;
	}

	const TopoDS_Shell& Shell::GetOcctShell() const
	{
		assert(!m_occtShell.IsNull() && "Shell::m_occtShell is null.");
		if (m_occtShell.IsNull())
		{
			throw std::exception("Shell::m_occtShell is null.");
		}

		return m_occtShell;
	}

	Vertex::Ptr Shell::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::SurfaceProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	void Shell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of faces
		std::list<Face::Ptr> faces;
		Faces(faces);
		for (const Face::Ptr& kpFace : faces)
		{
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	Shell::Shell(const TopoDS_Shell& rkOcctShell)
		: Topology(2)
		, m_occtShell(rkOcctShell)
	{
		//GlobalCluster::GetInstance().Add(this);
		/*SetOcctLabel(rkOcctLabel);
		OcctCounterAttribute::IncreaseCounter(GetOcctLabel());*/
	}

	Shell::~Shell()
	{
		//GlobalCluster::GetInstance().Remove(this);
		//DecreaseCounter();
	}
}
