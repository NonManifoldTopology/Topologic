#include <Shell.h>
#include <GlobalCluster.h>
#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Aperture.h>

#include <BOPAlgo_Splitter.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepCheck_Shell.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>
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
#include <TopExp_Explorer.hxx>

// ShapeOp
#include <Constraint.h>
#include <Solver.h>

// Earcut
#include <mapbox/earcut.hpp>

#include <array>
#include <assert.h>

namespace TopoLogicCore
{
	void Shell::Cells(std::list<std::shared_ptr<Cell>>& rCells) const
	{
		UpwardNavigation(rCells);
	}

	void Shell::Edges(std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Shell::Wires(std::list<std::shared_ptr<Wire>>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	void Shell::Faces(std::list<std::shared_ptr<Face>>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	bool Shell::IsClosed() const
	{
		BRepCheck_Shell occtBrepCheckShell(TopoDS::Shell(*GetOcctShape()));
		return (occtBrepCheckShell.Closed() == BRepCheck_NoError);
	}

	void Shell::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	std::shared_ptr<Shell> Shell::ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces)
	{
		if (rkFaces.empty())
		{
			throw std::exception("No face is passed.");
		}

		BRepBuilderAPI_Sewing occtSewing;
		for(std::list<std::shared_ptr<Face>>::const_iterator kFaceIterator = rkFaces.begin();
			kFaceIterator != rkFaces.end();
			kFaceIterator++)
		{
			const std::shared_ptr<Face>& kpFace = *kFaceIterator;
			occtSewing.Add(*kpFace->GetOcctShape());
		}

		occtSewing.Perform();
		std::shared_ptr<Shell> pShell = std::make_shared<Shell>(TopoDS::Shell(occtSewing.SewedShape()));
		for (std::list<std::shared_ptr<Face>>::const_iterator kFaceIterator = rkFaces.begin();
			kFaceIterator != rkFaces.end();
			kFaceIterator++)
		{
			const std::shared_ptr<Face>& kpFace = *kFaceIterator;
			kpFace->AddIngredientTo(pShell);
		}

		return pShell;
	}

	std::shared_ptr<Shell> Shell::ByVerticesFaceIndices(const std::vector<std::shared_ptr<Vertex>>& rkVertices, const std::list<std::list<int>>& rkFaceIndices)
	{
		std::list<std::shared_ptr<Face>> faces;
		for(std::list<std::list<int>>::const_iterator kFaceIndexIterator = rkFaceIndices.begin();
			kFaceIndexIterator != rkFaceIndices.end();
			kFaceIndexIterator++)
		{
			const std::list<int>& rkVertexIndices = *kFaceIndexIterator;

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
					TopoDS::Vertex(*rkVertices[vertexIndex]->GetOcctShape()),
					TopoDS::Vertex(*rkVertices[nextVertexIndex]->GetOcctShape())
				));
			}
			occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
				TopoDS::Vertex(*rkVertices[*--rkVertexIndices.end()]->GetOcctShape()),
				TopoDS::Vertex(*rkVertices[*rkVertexIndices.begin()]->GetOcctShape())
			));

			TopoDS_Wire pOcctWire(occtMakeWire);
			BRepBuilderAPI_MakeFace occtMakeFace(pOcctWire);
			faces.push_back(std::make_shared<Face>(occtMakeFace));
		}
		std::shared_ptr<Shell> pShell = ByFaces(faces);

		// Only add the vertices; the faces are dealt with in ByFaces()
		for (std::vector<std::shared_ptr<Vertex>>::const_iterator kVertexIterator = rkVertices.begin();
			kVertexIterator != rkVertices.end();
			kVertexIterator++)
		{
			const std::shared_ptr<Vertex>& kpVertex = *kVertexIterator;
			kpVertex->AddIngredientTo(pShell);
		}
		return pShell;
	}

	std::shared_ptr<Shell> Shell::ByFacePlanarization(
		const std::shared_ptr<Face>& kpFace,
		const int kIteration,
		const int kNumUPanels,
		const int kNumVPanels,
		const double kTolerance,
		const bool kCapBottom,
		const bool kCapTop,
		std::list<std::shared_ptr<Vertex>>& vertices,
		std::list<std::shared_ptr<Edge>>& edges,
		std::list<std::shared_ptr<Wire>>& wires,
		std::list<std::shared_ptr<Face>>& faces)
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

		return ByFacePlanarization(kpFace, kIteration, uValues, vValues, kTolerance, kCapBottom, kCapTop, vertices, edges, wires, faces);
	}

	bool SutherlandHodgmanIsInside(const Handle(Geom2d_CartesianPoint)& kpPoint, const Handle(Geom2d_Line)& kpLine)
	{
		gp_Pnt2d linePoint1 = kpLine->Value(kpLine->FirstParameter());
		gp_Pnt2d linePoint2 = kpLine->Value(kpLine->LastParameter());

		return (linePoint2.X() - linePoint1.X())*(kpPoint->Y() - linePoint1.Y()) >
			   (linePoint2.Y() - linePoint1.Y())*(kpPoint->X() - linePoint1.X());
	}

	Handle(Geom2d_CartesianPoint) ComputeIntersection(const Handle(Geom2d_CartesianPoint)& S, const Handle(Geom2d_CartesianPoint)& E, const Handle(Geom2d_Line)& kpClipEdge)
	{
		gp_Pnt2d linePoint1 = kpClipEdge->Value(kpClipEdge->FirstParameter());
		gp_Pnt2d linePoint2 = kpClipEdge->Value(kpClipEdge->LastParameter());

		Handle(Geom2d_CartesianPoint) dc = new Geom2d_CartesianPoint(linePoint1.X() - linePoint2.X(), linePoint1.Y() - linePoint2.Y());
		Handle(Geom2d_CartesianPoint) dp = new Geom2d_CartesianPoint(S->X() - E->X(), S->Y() - E->Y());
		double n1 = linePoint1.X() * linePoint2.Y() - linePoint1.Y() * linePoint2.X();
		double n2 = S->X() * E->Y() - S->Y() * E->X();
		double n3 = 1.0 / (dc->X() * dp->Y() - dc->Y() * dp->X());
		return new Geom2d_CartesianPoint((n1*dp->X() - n2 * dc->X()) * n3, (n1*dp->Y() - n2 * dc->Y()) * n3);
	}

	void SutherlandHodgmanClipping(
		const std::list<Handle(Geom2d_Line)>& rkClipPolygons, 
		const std::list<Handle(Geom2d_CartesianPoint)>& rkSubjectPoints,
		std::list<Handle(Geom2d_CartesianPoint)>& rOutputList)
	{
		rOutputList = rkSubjectPoints;
		for (const Handle(Geom2d_Line)& kpClipEdge : rkClipPolygons)
		{
			if (!rOutputList.empty())
			{
				std::list<Handle(Geom2d_CartesianPoint)> inputList = rOutputList;
				rOutputList.clear();
				Handle(Geom2d_CartesianPoint) S = inputList.back();
				for (const Handle(Geom2d_CartesianPoint)& E : inputList)
				{
					if (SutherlandHodgmanIsInside(E, kpClipEdge))
					{
						if (!SutherlandHodgmanIsInside(S, kpClipEdge))
						{
							rOutputList.push_back(ComputeIntersection(S, E, kpClipEdge));
						}
						rOutputList.push_back(E);
					}
					else if (SutherlandHodgmanIsInside(S, kpClipEdge))
					{
						rOutputList.push_back(ComputeIntersection(S, E, kpClipEdge));
					}
					
					S = E;
				}
			}
		}
	}

	std::shared_ptr<Shell> Shell::ByFacePlanarization(
		const std::shared_ptr<Face>& kpFace, 
		const int kIteration, 
		const std::list<double>& rkUValues, 
		const std::list<double>& rkVValues, 
		const double kTolerance,
		const bool kCapBottom,
		const bool kCapTop,
		std::list<std::shared_ptr<Vertex>>& vertices,
		std::list<std::shared_ptr<Edge>>& edges,
		std::list<std::shared_ptr<Wire>>& wires,
		std::list<std::shared_ptr<Face>>& faces)
	{
		// Check that kIteration, kNumUPanels, and kNumVPanels are 1 or more.
		if (kIteration < 1)
		{
			std::string errorMessage = std::string("The number of iteration must be larger than 0.");
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
		ShapeAnalysis::GetFaceUVBounds(TopoDS::Face(*kpFace->GetOcctShape()), occtUMin, occtUMax, occtVMin, occtVMax);
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

		for (std::list<double>::const_iterator uIterator = occtUValues.begin();
			uIterator != endUIterator;
			uIterator++)
		{
			const double& rkU = *uIterator;
			for (std::list<double>::const_iterator vIterator = occtVValues.begin();
				vIterator != endVIterator;
				vIterator++)
			{
				const double& rkV = *vIterator;
				gp_Pnt occtPoint = pOcctWallSurface->Value(rkU, rkV);
				shapeOpMatrix(0, i) = occtPoint.X();
				shapeOpMatrix(1, i) = occtPoint.Y();
				shapeOpMatrix(2, i) = occtPoint.Z();

				++i;
			}
		}

		std::list<std::shared_ptr<Topology>>& rContents = kpFace->Contents();
		std::list<std::list<Handle(Geom2d_CartesianPoint)>> allApertureSampleVerticesUV;
		for (const std::shared_ptr<Topology>& rkContent : rContents)
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

			std::shared_ptr<Face> apertureFace = TopologicalQuery::Downcast<Face>(aperture->Topology());
			std::shared_ptr<Wire> apertureWire = apertureFace->OuterBoundary();
			std::list<std::shared_ptr<Edge>> apertureEdges;
			apertureWire->Edges(apertureEdges);
			std::list<Handle(Geom2d_CartesianPoint)> apertureSampleVerticesUV;
			for (const std::shared_ptr<Edge>& kpEdge : apertureEdges)
			{
				int numberOfSamples = 100;
				for (int i = 0; i < numberOfSamples; ++i)
				{
					double parameter = (double)i / (double)(numberOfSamples - 1);
					std::shared_ptr<Vertex> apertureSampleVertex = kpEdge->PointAtParameter(parameter);

					// Get the UV coordinate of the aperture sample vertex
					double apertureSampleVertexU = 0.0, apertureSampleVertexV = 0.0;
					kpFace->UVParameterAtPoint(apertureSampleVertex, apertureSampleVertexU, apertureSampleVertexV);
					apertureSampleVerticesUV.push_back(new Geom2d_CartesianPoint(apertureSampleVertexU, apertureSampleVertexV));
				}
			}
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


		TopTools_DataMapOfShapeListOfShape occtMapFacePanelToApertures;
		BRepBuilderAPI_Sewing occtSewing;
		endUIterator = occtUValues.end();
		endUIterator--;
		endVIterator = occtVValues.end();
		endVIterator--;
		i = 0;
		for (std::list<double>::const_iterator uIterator = occtUValues.begin();
			uIterator != endUIterator;
			uIterator++, ++i)
		{
			double u = *uIterator;
			std::list<double>::const_iterator nextUIterator = uIterator;
			nextUIterator++;
			double nextU = *nextUIterator;

			int j = 0;
			for (std::list<double>::const_iterator vIterator = occtVValues.begin();
				vIterator != endVIterator;
				vIterator++, ++j)
			{
				double v = *vIterator;
				std::list<double>::const_iterator nextVIterator = vIterator;
				nextVIterator++;
				double nextV = *nextVIterator;

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

				std::shared_ptr<Vertex> pVertex1 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex1));
				std::shared_ptr<Vertex> pVertex2 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex2));
				std::shared_ptr<Vertex> pVertex3 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex3));
				std::shared_ptr<Vertex> pVertex4 = TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex4));

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
				occtSewing.Add(rkOcctPanelFace);
				Handle(Geom_Surface) pOcctPanelSurface = BRep_Tool::Surface(rkOcctPanelFace);

				//========================
				// Map the apertures.
				ShapeAnalysis_Surface occtSurfaceAnalysis(pOcctPanelSurface);
				std::shared_ptr<Face> pPanelFace = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(rkOcctPanelFace));
				
				// (0,0)
				gp_Pnt2d occtUV1 = occtSurfaceAnalysis.ValueOfUV(pVertex1->Point()->Pnt(), Precision::Confusion());
				double panelPoint1U = occtUV1.X(), panelPoint1V = occtUV1.Y();

				// (0,1)
				gp_Pnt2d occtUV2 = occtSurfaceAnalysis.ValueOfUV(pVertex2->Point()->Pnt(), Precision::Confusion());
				double panelPoint2U = occtUV2.X(), panelPoint2V = occtUV2.Y();

				// (1,1)
				gp_Pnt2d occtUV3 = occtSurfaceAnalysis.ValueOfUV(pVertex3->Point()->Pnt(), Precision::Confusion());
				double panelPoint3U = occtUV3.X(), panelPoint3V = occtUV3.Y();

				// (1,0)
				gp_Pnt2d occtUV4 = occtSurfaceAnalysis.ValueOfUV(pVertex4->Point()->Pnt(), Precision::Confusion());
				double panelPoint4U = occtUV4.X(), panelPoint4V = occtUV4.Y();
				
				//Compute deltas
				double deltaPanelBottomU = panelPoint4U - panelPoint1U;
				double deltaPanelBottomV = panelPoint4V - panelPoint1V;

				double deltaPanelTopU = panelPoint3U - panelPoint2U;
				double deltaPanelTopV = panelPoint3V - panelPoint2V;

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


				// Clip the apertures sample points UV against the wall panel's UV using Sutherland-Hodgman
				std::list<Handle(Geom2d_Line)> clipPolygons;
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(u, v), gp_Dir2d(nextU - u, 0)));
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(nextU, v), gp_Dir2d(0, nextV - v)));
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(nextU, nextV), gp_Dir2d(u - nextU, 0)));
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(u, nextV), gp_Dir2d(0, v - nextV)));

				BOPCol_ListOfShape aperturePanels;

				using Coord = double;

				// The index type. Defaults to uint32_t, but you can also pass uint16_t if you know that your
				// data won't have more than 65536 vertices.
				using N = uint32_t;

				// Create array
				using Point = std::array<Coord, 2>;

				// Fill polygon structure with actual data. Any winding order works.
				// The first polyline defines the main polygon.
				std::vector<Point> polygon;

				for (const std::list<Handle(Geom2d_CartesianPoint)>& rkApertureSampleVerticesUV : allApertureSampleVerticesUV)
				{
					// The result is in the original surface's UV
					std::list<Handle(Geom2d_CartesianPoint)> clippedApertureSampleVerticesUVOriginalWall;
					SutherlandHodgmanClipping(clipPolygons, rkApertureSampleVerticesUV, clippedApertureSampleVerticesUVOriginalWall);

					//std::list<TopoDS_Vertex> occtMappedApertureVertices;
					for (const Handle(Geom2d_CartesianPoint)& kpClippedApertureSampleVertexUVOriginalWall : clippedApertureSampleVerticesUVOriginalWall)
					{
						// normalised to the panel, use U and V
						Handle(Geom2d_CartesianPoint) clippedApertureSampleVerticesUVPanel = new Geom2d_CartesianPoint(
							(kpClippedApertureSampleVertexUVOriginalWall->X() - u)/(nextU - u),
							(kpClippedApertureSampleVertexUVOriginalWall->Y() - v)/(nextV - v)
						);
						// Translate the clipped aperture sample vertices UV to the UV coordinate of pOcctPanelSurface,
						// which now has its own UV coordinate.
						// Use bilinear mapping.

						// The first step uses X
						double mappedAperturePointUBottom = panelPoint1U + clippedApertureSampleVerticesUVPanel->X() * deltaPanelBottomU;
						double mappedAperturePointVBottom = panelPoint1V + clippedApertureSampleVerticesUVPanel->X() * deltaPanelBottomV;
						double mappedAperturePointUTop = panelPoint2U + clippedApertureSampleVerticesUVPanel->X() * deltaPanelTopU;
						double mappedAperturePointVTop = panelPoint2V + clippedApertureSampleVerticesUVPanel->X() * deltaPanelTopV;

						double deltaMappedAperturePointU = mappedAperturePointUTop - mappedAperturePointUBottom;
						double deltaMappedAperturePointV = mappedAperturePointVTop - mappedAperturePointVBottom;

						// 2nd step uses Y
						double mappedAperturePointU = mappedAperturePointUBottom + clippedApertureSampleVerticesUVPanel->Y() * deltaMappedAperturePointU;
						double mappedAperturePointV = mappedAperturePointVBottom + clippedApertureSampleVerticesUVPanel->Y() * deltaMappedAperturePointV;
						polygon.push_back({ mappedAperturePointU, mappedAperturePointV});

					}

					std::vector<std::vector<Point>> polygons;
					polygons.push_back(polygon);

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

						BRepBuilderAPI_MakeVertex occtMakeVertex1(occtMappedAperturePoint1);
						BRepBuilderAPI_MakeVertex occtMakeVertex2(occtMappedAperturePoint2);
						BRepBuilderAPI_MakeVertex occtMakeVertex3(occtMappedAperturePoint3);

						BRepBuilderAPI_MakeEdge occtMakeEdge12(occtMakeVertex1.Vertex(), occtMakeVertex2.Vertex());
						BRepBuilderAPI_MakeEdge occtMakeEdge23(occtMakeVertex2.Vertex(), occtMakeVertex3.Vertex());
						BRepBuilderAPI_MakeEdge occtMakeEdge31(occtMakeVertex3.Vertex(), occtMakeVertex1.Vertex());

						try{
							TopTools_ListOfShape occtMappedApertureEdges;
							occtMappedApertureEdges.Append(occtMakeEdge12.Edge());
							occtMappedApertureEdges.Append(occtMakeEdge23.Edge());
							occtMappedApertureEdges.Append(occtMakeEdge31.Edge());

							BRepBuilderAPI_MakeWire occtMakeMappedApertureWire;
							occtMakeMappedApertureWire.Add(occtMappedApertureEdges);
							BRepBuilderAPI_MakeFace occtMakeMappedApertureFace(pOcctPanelSurface, occtMakeMappedApertureWire.Wire());
							std::shared_ptr<Face> mappedApertureFace = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(occtMakeMappedApertureFace.Face()));
							faces.push_back(mappedApertureFace);
							aperturePanels.Append(occtMakeMappedApertureFace.Shape());
						}
						catch (...)
						{

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
		std::shared_ptr<Shell> pOutputShell = std::make_shared<Shell>(TopoDS::Shell(occtSewing.SewedShape()));

		// Register the shapes to the OCCT documents.
		std::shared_ptr<Topology> pUpcastTopology = TopologicalQuery::Upcast<Topology>(pOutputShell);
		GlobalCluster::GetInstance().GetCluster()->AddChildLabel(pUpcastTopology, REL_CONSTITUENT);

		// Iterate through the faces of the shell and attach them as labels of the shell's label.
		std::list<std::shared_ptr<Face>> outputFaces;
		pOutputShell->Faces(outputFaces);
		int counter = 0;
		for (TopTools_DataMapIteratorOfDataMapOfShapeListOfShape iterator(occtMapFacePanelToApertures);
			iterator.More();
			iterator.Next())
		{
			bool isModified = occtSewing.IsModifiedSubShape(iterator.Key());
			if (isModified)
			{
				TopoDS_Shape occtModifiedShape = occtSewing.ModifiedSubShape(iterator.Key());
				// Register the shapes to the shell.
				std::shared_ptr<Topology> modifiedShape = Topology::ByOcctShape(occtModifiedShape);
				pOutputShell->AddChildLabel(modifiedShape, REL_CONSTITUENT);

				const BOPCol_ListOfShape& rkOcctApertures = iterator.Value();
				for (BOPCol_ListIteratorOfListOfShape occtApertureIterator(rkOcctApertures);
					occtApertureIterator.More();
					occtApertureIterator.Next())
				{
					const TopoDS_Shape& rkOcctAperture = occtApertureIterator.Value();
					std::shared_ptr<Topology> aperture = Topology::ByOcctShape(rkOcctAperture);
					modifiedShape->AddChildLabel(aperture, REL_APERTURE);
				}
			}
		}

		return pOutputShell;
	}

	std::shared_ptr<Shell> Shell::ByLoft(const std::list<std::shared_ptr<Wire>>& rkWires)
	{
		BRepOffsetAPI_ThruSections occtLoft;
		for(const std::shared_ptr<Wire>& kpWire : rkWires)
		{
			occtLoft.AddWire(TopoDS::Wire(*kpWire->GetOcctShape()));
		};
		occtLoft.Build();
		return std::make_shared<Shell>(TopoDS::Shell(occtLoft.Shape()));
	}

	std::shared_ptr<TopoDS_Shape> Shell::GetOcctShape() const
	{
		assert(m_pOcctShell != nullptr && "Shell::m_pOcctShell is null.");
		if (m_pOcctShell == nullptr)
		{
			throw std::exception("Shell::m_pOcctShell is null.");
		}

		return m_pOcctShell;
	}

	void Shell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of faces
		std::list<std::shared_ptr<Face>> faces;
		Faces(faces);
		for (std::list<std::shared_ptr<Face>>::const_iterator kFaceIterator = faces.begin();
			kFaceIterator != faces.end();
			kFaceIterator++)
		{
			const std::shared_ptr<Face>& kpFace = *kFaceIterator;
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	Shell::Shell(const TopoDS_Shell& rkOcctShell)
		: Topology(2)
		, m_pOcctShell(nullptr)
	{
		ShapeFix_Shell occtShapeFixShell(rkOcctShell);
		occtShapeFixShell.Perform();
		m_pOcctShell = std::make_shared<TopoDS_Shell>(occtShapeFixShell.Shell());
		GlobalCluster::GetInstance().Add(this);
	}

	Shell::~Shell()
	{
		GlobalCluster::GetInstance().Remove(this);
	}
}
