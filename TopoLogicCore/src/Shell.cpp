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

		// Create panels
		/*struct CurvedAperturePanel
		{
			std::shared_ptr<Face> face;
			double panelUMin;
			double panelUMax;
			double panelVMin;
			double panelVMax;

			double contextUMin;
			double contextUMax;
			double contextVMin;
			double contextVMax;

			void NormalizeUV(const double kU, const double kV, double& rNormalizedU, double& rNormalizedV) const
			{
				rNormalizedU = (kU - contextUMin) / (contextUMax - contextUMin);
				rNormalizedV = (kV - contextVMin) / (contextVMax - contextVMin);
			}

			int i;
			int j;
		};*/

		// Iterate against the aperture
		//std::list<CurvedAperturePanel> panels;
		//
		std::list<std::shared_ptr<Topology>>& rContents = kpFace->Contents();
		//std::list<std::list<std::shared_ptr<Vertex>>> allApertureSampleVertices;
		//std::list<std::list<std::shared_ptr<Vertex>>> allApertureSampleVertices;
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
					//apertureSampleVertices.push_back(apertureSampleVertex);
					//vertices.push_back(apertureSampleVertex);

					// Get the UV coordinate of the aperture sample vertex
					double apertureSampleVertexU = 0.0, apertureSampleVertexV = 0.0;
					kpFace->UVParameterAtPoint(apertureSampleVertex, apertureSampleVertexU, apertureSampleVertexV);
					apertureSampleVerticesUV.push_back(new Geom2d_CartesianPoint(apertureSampleVertexU, apertureSampleVertexV));


					/*std::shared_ptr<Vertex> mappedApertureVertex = kpFace->PointAtParameter(
						apertureSampleVertexU,
						apertureSampleVertexV);
					vertices.push_back(mappedApertureVertex);*/
				}
			}
			allApertureSampleVerticesUV.push_back(apertureSampleVerticesUV);

			//double occtApertureUMin = 0.0, occtApertureUMax = 0.0, occtApertureVMin = 0.0, occtApertureVMax = 0.0;
			//ShapeAnalysis::GetFaceUVBounds(TopoDS::Face(*apertureFace->GetOcctShape()), occtApertureUMin, occtApertureUMax, occtApertureVMin, occtApertureVMax);

			//Handle(Geom_Surface) apertureSurface = apertureFace->Surface();

			//// Iterate against the UV subdivision
			//endUIterator = occtUValues.end();
			//endUIterator--;
			//endVIterator = occtVValues.end();
			//endVIterator--;
			//int i = 0;
			//for (std::list<double>::const_iterator uIterator = occtUValues.begin();
			//	uIterator != endUIterator;
			//	uIterator++, ++i)
			//{
			//	const double& rkUMin = *uIterator;
			//	std::list<double>::const_iterator uIterator2 = uIterator;
			//	uIterator2++;
			//	const double& rkUMax = *uIterator2;
			//	
			//	int j = 0;
			//	for (std::list<double>::const_iterator vIterator = occtVValues.begin();
			//		vIterator != endVIterator;
			//		vIterator++, ++j)
			//	{
			//		const double& rkVMin = *vIterator;
			//		std::list<double>::const_iterator vIterator2 = vIterator;
			//		vIterator2++;
			//		const double& rkVMax = *vIterator2;

			//		double occtUPatchMin = rkUMin < occtApertureUMin ? occtApertureUMin : rkUMin;
			//		double occtUPatchMax = rkUMax > occtApertureUMax ? occtApertureUMax : rkUMax;
			//		double occtVPatchMin = rkVMin < occtApertureVMin ? occtApertureVMin : rkVMin;
			//		double occtVPatchMax = rkVMax > occtApertureVMax ? occtApertureVMax : rkVMax;

			//		occtUPatchMin += 0.001;
			//		occtUPatchMax -= 0.001;
			//		occtVPatchMin += 0.001;
			//		occtVPatchMax -= 0.001;

			//		if (occtUPatchMin > occtApertureUMax ||
			//			occtUPatchMax < occtApertureUMin ||
			//			occtVPatchMin > occtApertureVMax ||
			//			occtVPatchMax < occtApertureVMin)
			//		{
			//			continue;
			//		}

			//		// Only get panels which overlap with the apertures
			//		BRepBuilderAPI_MakeFace occtMakeFace(apertureSurface, occtUPatchMin, occtUPatchMax, occtVPatchMin, occtVPatchMax, Precision::Confusion());
			//		
			//		CurvedAperturePanel panel;
			//		panel.face = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(occtMakeFace.Shape()));
			//		panel.panelUMin = occtUPatchMin;
			//		panel.panelUMax = occtUPatchMax;
			//		panel.panelVMin = occtVPatchMin;
			//		panel.panelVMax = occtVPatchMax;
			//		panel.contextUMin = rkUMin;
			//		panel.contextUMax = rkUMax;
			//		panel.contextVMin = rkVMin;
			//		panel.contextVMax = rkVMax;
			//		panel.i = i;
			//		panel.j = j;

			//		panels.push_back(panel);

			//		//faces.push_back(panel.face);
			//	}
			//}
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
		/*for (int i = 0; i < numUPanels; ++i)
		{
			for (int j = 0; j < numVPanels; ++j)
			{*/
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

				ShapeAnalysis_Surface occtSurfaceAnalysis(pOcctPanelSurface);
				std::shared_ptr<Face> pPanelFace = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(rkOcctPanelFace));
				
				// Don't use this; use the actual corners instead.
				//double occtPlanarContextUMin = 0.0, occtPlanarContextUMax = 0.0, occtPlanarContextVMin = 0.0, occtPlanarContextVMax = 0.0;
				//ShapeAnalysis::GetFaceUVBounds(TopoDS::Face(*pPanelFace->GetOcctShape()), occtPlanarContextUMin, occtPlanarContextUMax, occtPlanarContextVMin, occtPlanarContextVMax);

				// (0,0)
				gp_Pnt2d occtUV1 = occtSurfaceAnalysis.ValueOfUV(pVertex1->Point()->Pnt(), Precision::Confusion());
				double panelPoint1U = occtUV1.X(), panelPoint1V = occtUV1.Y();
				//double panelPoint1U = 0.0, panelPoint1V = 0.0;
				//pPanelFace->UVParameterAtPoint(pVertex1, panelPoint1U, panelPoint1V);

				// (0,1)
				gp_Pnt2d occtUV2 = occtSurfaceAnalysis.ValueOfUV(pVertex2->Point()->Pnt(), Precision::Confusion());
				double panelPoint2U = occtUV2.X(), panelPoint2V = occtUV2.Y();
				/*double panelPoint2U = 0.0, panelPoint2V = 0.0;
				pPanelFace->UVParameterAtPoint(pVertex2, panelPoint2U, panelPoint2V);*/

				// (1,1)
				gp_Pnt2d occtUV3 = occtSurfaceAnalysis.ValueOfUV(pVertex3->Point()->Pnt(), Precision::Confusion());
				double panelPoint3U = occtUV3.X(), panelPoint3V = occtUV3.Y();
				/*double panelPoint3U = 0.0, panelPoint3V = 0.0;
				pPanelFace->UVParameterAtPoint(pVertex3, panelPoint3U, panelPoint3V);*/

				// (1,0)
				gp_Pnt2d occtUV4 = occtSurfaceAnalysis.ValueOfUV(pVertex4->Point()->Pnt(), Precision::Confusion());
				double panelPoint4U = occtUV4.X(), panelPoint4V = occtUV4.Y();
				/*double panelPoint4U = 0.0, panelPoint4V = 0.0;
				pPanelFace->UVParameterAtPoint(pVertex4, panelPoint4U, panelPoint4V);*/
				
				//Compute deltas
				double deltaPanelBottomU = panelPoint4U - panelPoint1U;
				double deltaPanelBottomV = panelPoint4V - panelPoint1V;

				double deltaPanelTopU = panelPoint3U - panelPoint2U;
				double deltaPanelTopV = panelPoint3V - panelPoint2V;



				//for (const CurvedAperturePanel& rkAperturePanel : panels)
				//{
				//	if (rkAperturePanel.i == minU && rkAperturePanel.j == minV)
				//	{
				//		// Bilinear interpolations

				//		double apertureNormalizedUMin = 0.0;
				//		double apertureNormalizedUMax = 0.0;
				//		double apertureNormalizedVMin = 0.0;
				//		double apertureNormalizedVMax = 0.0;
				//		rkAperturePanel.NormalizeUV(rkAperturePanel.panelUMin, rkAperturePanel.panelVMin, apertureNormalizedUMin, apertureNormalizedVMin);
				//		rkAperturePanel.NormalizeUV(rkAperturePanel.panelUMax, rkAperturePanel.panelVMax, apertureNormalizedUMax, apertureNormalizedVMax);

				//		double aperturePoint1U = apertureNormalizedUMin;
				//		double aperturePoint1V = apertureNormalizedVMin;
				//		double aperturePoint2U = apertureNormalizedUMin;
				//		double aperturePoint2V = apertureNormalizedVMax;
				//		double aperturePoint3U = apertureNormalizedUMax;
				//		double aperturePoint3V = apertureNormalizedVMax;
				//		double aperturePoint4U = apertureNormalizedUMax;
				//		double aperturePoint4V = apertureNormalizedVMin;
				//		
				//		// Bottom = interpolate between panel points 1 and 4
				//		// Top = interpolate between panel points 2 and 3

				//		//==========================================
				//		// 
				//		// Aperture point 1 
				//		// 1st steps only involve aperturePoint1U
				//		double mappedAperturePoint1UBottom = panelPoint1U + aperturePoint1U * deltaPanelBottomU;
				//		double mappedAperturePoint1VBottom = panelPoint1V + aperturePoint1U * deltaPanelBottomV;
				//		double mappedAperturePoint1UTop = panelPoint2U + aperturePoint1U * deltaPanelTopU;
				//		double mappedAperturePoint1VTop = panelPoint2V + aperturePoint1U * deltaPanelTopV;
				//		
				//		double deltaMappedAperturePoint1U = mappedAperturePoint1UTop - mappedAperturePoint1UBottom;
				//		double deltaMappedAperturePoint1V = mappedAperturePoint1VTop - mappedAperturePoint1VBottom;

				//		// 2nd step uses aperturePoint1V
				//		double mappedAperturePoint1U = mappedAperturePoint1UBottom + aperturePoint1V * deltaMappedAperturePoint1U;
				//		double mappedAperturePoint1V = mappedAperturePoint1VBottom + aperturePoint1V * deltaMappedAperturePoint1V;
				//		//gp_Pnt occtMappedAperturePoint1 = pOcctPanelSurface->Value(panelPoint1U, panelPoint1V);
				//		gp_Pnt occtMappedAperturePoint1 = pOcctPanelSurface->Value(mappedAperturePoint1U, mappedAperturePoint1V);
				//		//
				//		//==========================================

				//		//==========================================
				//		// 
				//		// Aperture point 2
				//		// 1st steps only involve aperturePoint2U
				//		double mappedAperturePoint2UBottom = panelPoint1U + aperturePoint2U * deltaPanelBottomU;
				//		double mappedAperturePoint2VBottom = panelPoint1V + aperturePoint2U * deltaPanelBottomV;
				//		double mappedAperturePoint2UTop = panelPoint2U + aperturePoint2U * deltaPanelTopU;
				//		double mappedAperturePoint2VTop = panelPoint2V + aperturePoint2U * deltaPanelTopV;

				//		double deltaMappedAperturePoint2U = mappedAperturePoint2UTop - mappedAperturePoint2UBottom;
				//		double deltaMappedAperturePoint2V = mappedAperturePoint2VTop - mappedAperturePoint2VBottom;

				//		// 2nd step uses aperturePoint2V
				//		double mappedAperturePoint2U = mappedAperturePoint2UBottom + aperturePoint2V * deltaMappedAperturePoint2U;
				//		double mappedAperturePoint2V = mappedAperturePoint2VBottom + aperturePoint2V * deltaMappedAperturePoint2V;
				//		//gp_Pnt occtMappedAperturePoint2 = pOcctPanelSurface->Value(panelPoint2U, panelPoint2V);
				//		gp_Pnt occtMappedAperturePoint2 = pOcctPanelSurface->Value(mappedAperturePoint2U, mappedAperturePoint2V);
				//		//
				//		//==========================================

				//		//==========================================
				//		// 
				//		// Aperture point 3
				//		// 1st steps only involve aperturePoint3U
				//		double mappedAperturePoint3UBottom = panelPoint1U + aperturePoint3U * deltaPanelBottomU;
				//		double mappedAperturePoint3VBottom = panelPoint1V + aperturePoint3U * deltaPanelBottomV;
				//		double mappedAperturePoint3UTop = panelPoint2U + aperturePoint3U * deltaPanelTopU;
				//		double mappedAperturePoint3VTop = panelPoint2V + aperturePoint3U * deltaPanelTopV;

				//		double deltaMappedAperturePoint3U = mappedAperturePoint3UTop - mappedAperturePoint3UBottom;
				//		double deltaMappedAperturePoint3V = mappedAperturePoint3VTop - mappedAperturePoint3VBottom;

				//		// 2nd step uses aperturePoint3V
				//		double mappedAperturePoint3U = mappedAperturePoint3UBottom + aperturePoint3V * deltaMappedAperturePoint3U;
				//		double mappedAperturePoint3V = mappedAperturePoint3VBottom + aperturePoint3V * deltaMappedAperturePoint3V;
				//		//gp_Pnt occtMappedAperturePoint3 = pOcctPanelSurface->Value(panelPoint3U, panelPoint3V);
				//		gp_Pnt occtMappedAperturePoint3 = pOcctPanelSurface->Value(mappedAperturePoint3U, mappedAperturePoint3V);
				//		//
				//		//==========================================

				//		//==========================================
				//		// 
				//		// Aperture point 4
				//		// 1st steps only involve aperturePoint4U
				//		double mappedAperturePoint4UBottom = panelPoint1U + aperturePoint4U * deltaPanelBottomU;
				//		double mappedAperturePoint4VBottom = panelPoint1V + aperturePoint4U * deltaPanelBottomV;
				//		double mappedAperturePoint4UTop = panelPoint2U + aperturePoint4U * deltaPanelTopU;
				//		double mappedAperturePoint4VTop = panelPoint2V + aperturePoint4U * deltaPanelTopV;

				//		double deltaMappedAperturePoint4U = mappedAperturePoint4UTop - mappedAperturePoint4UBottom;
				//		double deltaMappedAperturePoint4V = mappedAperturePoint4VTop - mappedAperturePoint4VBottom;

				//		// 2nd step uses aperturePoint3V
				//		double mappedAperturePoint4U = mappedAperturePoint4UBottom + aperturePoint4V * deltaMappedAperturePoint4U;
				//		double mappedAperturePoint4V = mappedAperturePoint4VBottom + aperturePoint4V * deltaMappedAperturePoint4V;
				//		//gp_Pnt occtMappedAperturePoint4 = pOcctPanelSurface->Value(panelPoint4U, panelPoint4V);
				//		gp_Pnt occtMappedAperturePoint4 = pOcctPanelSurface->Value(mappedAperturePoint4U, mappedAperturePoint4V);
				//		//
				//		//==========================================

				//		BRepBuilderAPI_MakeEdge occtMappedApertureEdge1(occtMappedAperturePoint1, occtMappedAperturePoint2);
				//		BRepBuilderAPI_MakeEdge occtMappedApertureEdge2(occtMappedAperturePoint2, occtMappedAperturePoint3);
				//		BRepBuilderAPI_MakeEdge occtMappedApertureEdge3(occtMappedAperturePoint3, occtMappedAperturePoint4);
				//		BRepBuilderAPI_MakeEdge occtMappedApertureEdge4(occtMappedAperturePoint4, occtMappedAperturePoint1);

				//		TopTools_ListOfShape occtMappedApertureEdges;
				//		occtMappedApertureEdges.Append(occtMappedApertureEdge1.Shape());
				//		occtMappedApertureEdges.Append(occtMappedApertureEdge2.Shape());
				//		occtMappedApertureEdges.Append(occtMappedApertureEdge3.Shape());
				//		occtMappedApertureEdges.Append(occtMappedApertureEdge4.Shape());

				//		BRepBuilderAPI_MakeWire occtMappedApertureMakeWire;
				//		occtMappedApertureMakeWire.Add(occtMappedApertureEdges);

				//		TopoDS_Wire occtMappedApertureWire = occtMappedApertureMakeWire.Wire();
				//		BRepBuilderAPI_MakeFace occtMappedApertureMakeFace(
				//			pOcctPanelSurface,
				//			occtMappedApertureWire,
				//			kTolerance
				//		);
				//		std::shared_ptr<Face> mappedAperturePanel = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(TopoDS::Face(occtMappedApertureMakeFace.Shape())));

				//		//faces.push_back(pPanelFace); // only those used to map the aperture

				//		faces.push_back(mappedAperturePanel);
				//	}
				//}
				//faces.push_back(pPanelFace); // all panels

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


				//==================
				// Clip the apertures sample points UV against the wall panel's UV using Sutherland-Hodgman
				std::list<Handle(Geom2d_Line)> clipPolygons;
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(u, v), gp_Dir2d(nextU - u, 0)));
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(nextU, v), gp_Dir2d(0, nextV - v)));
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(nextU, nextV), gp_Dir2d(u - nextU, 0)));
				clipPolygons.push_back(new Geom2d_Line(gp_Pnt2d(u, nextV), gp_Dir2d(0, v - nextV)));

				for (const std::list<Handle(Geom2d_CartesianPoint)>& rkApertureSampleVerticesUV : allApertureSampleVerticesUV)
				{
					// The result is in the original surface's UV
					std::list<Handle(Geom2d_CartesianPoint)> clippedApertureSampleVerticesUVOriginalWall;
					SutherlandHodgmanClipping(clipPolygons, rkApertureSampleVerticesUV, clippedApertureSampleVerticesUVOriginalWall);

					std::list<TopoDS_Vertex> occtMappedApertureVertices;
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

						//Testing with the original surface
						/*std::shared_ptr<Vertex> mappedApertureVertex = kpFace->PointAtParameter(
							kpClippedApertureSampleVertexUVOriginalWall->X(),
							kpClippedApertureSampleVertexUVOriginalWall->Y());
						vertices.push_back(mappedApertureVertex);*/

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
						//gp_Pnt occtMappedAperturePoint1 = pOcctPanelSurface->Value(panelPoint1U, panelPoint1V);
						gp_Pnt occtMappedAperturePoint = pOcctPanelSurface->Value(mappedAperturePointU, mappedAperturePointV);
						occtMappedApertureVertices.push_back(BRepBuilderAPI_MakeVertex(occtMappedAperturePoint));

						/*std::shared_ptr<Vertex> mappedApertureVertex = Vertex::ByPoint(new Geom_CartesianPoint(occtMappedAperturePoint));
						vertices.push_back(mappedApertureVertex);*/
					}

					if(!occtMappedApertureVertices.empty())
					{
						std::list<TopoDS_Vertex>::const_iterator kEndVertexIterator = occtMappedApertureVertices.end();
						kEndVertexIterator--;

						TopTools_ListOfShape occtMappedApertureEdges;
						for (std::list<TopoDS_Vertex>::const_iterator kVertexIterator = occtMappedApertureVertices.begin();
							kVertexIterator != kEndVertexIterator;
							kVertexIterator++)
						{
							auto kNextVertexIterator = kVertexIterator;
							kNextVertexIterator++;

							try{
							occtMappedApertureEdges.Append(BRepBuilderAPI_MakeEdge(*kVertexIterator, *kNextVertexIterator));
							}
							catch(...){

							}
						}
						try{
							occtMappedApertureEdges.Append(BRepBuilderAPI_MakeEdge(occtMappedApertureVertices.front(), occtMappedApertureVertices.back()));
						}
						catch (...) {

						}

						BRepBuilderAPI_MakeWire occtMakeMappedApertureWire;
						occtMakeMappedApertureWire.Add(occtMappedApertureEdges);
						TopoDS_Wire mappedApertureWire = occtMakeMappedApertureWire.Wire();
						BRepBuilderAPI_MakeFace occtMakeMappedApertureFace(pOcctPanelSurface, mappedApertureWire);
						std::shared_ptr<Face> mappedApertureFace = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(occtMakeMappedApertureFace.Face()));
						faces.push_back(mappedApertureFace);

						//BRepMesh_IncrementalMesh occtIncrementalMesh(occtMakeMappedApertureFace.Shape(), 0.01);
						//occtIncrementalMesh.Perform();
						//
						//for (TopExp_Explorer occtExplorer(occtIncrementalMesh.Shape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next()) 
						//{
						//	const TopoDS_Face& rkOcctFace = TopoDS::Face(occtExplorer.Current());
						//	TopLoc_Location occtLocation;
						//	// get the triangulation

						//	Handle(Poly_Triangulation) occtTriangulation = BRep_Tool::Triangulation(rkOcctFace, occtLocation);

						//	if (occtTriangulation.IsNull())
						//	{
						//		continue;
						//	}

						//	TColgp_Array1OfPnt occtPoints = occtTriangulation->Nodes();
						//	Poly_Array1OfTriangle occtTriangles = occtTriangulation->Triangles();
						//	for (Poly_Array1OfTriangle::const_iterator kTriangleIterator = occtTriangles.begin();
						//		kTriangleIterator != occtTriangles.end();
						//		kTriangleIterator++)
						//	{
						//		const Poly_Triangle& rkTriangle = *kTriangleIterator;

						//		int index1 = 0, index2 = 0, index3 = 0;
						//		rkTriangle.Get(index1, index2, index3);

						//		gp_Pnt point1 = occtPoints(index1);
						//		gp_Pnt point2 = occtPoints(index2);
						//		gp_Pnt point3 = occtPoints(index3);

						//		BRepBuilderAPI_MakeVertex occtMakeVertex1(point1);
						//		BRepBuilderAPI_MakeVertex occtMakeVertex2(point2);
						//		BRepBuilderAPI_MakeVertex occtMakeVertex3(point3);

						//		BRepBuilderAPI_MakeEdge occtMakeEdge12(occtMakeVertex1.Vertex(), occtMakeVertex2.Vertex());
						//		BRepBuilderAPI_MakeEdge occtMakeEdge23(occtMakeVertex2.Vertex(), occtMakeVertex3.Vertex());
						//		BRepBuilderAPI_MakeEdge occtMakeEdge31(occtMakeVertex3.Vertex(), occtMakeVertex1.Vertex());

						//		TopTools_ListOfShape occtEdges;
						//		occtEdges.Append(occtMakeEdge12);
						//		occtEdges.Append(occtMakeEdge23);
						//		occtEdges.Append(occtMakeEdge31);

						//		BRepBuilderAPI_MakeWire occtMakeWire;
						//		occtMakeWire.Add(occtEdges);

						//		BRepBuilderAPI_MakeFace occtMakeMappedTriangulatedApertureFace(occtMakeWire);
						//		std::shared_ptr<Face> mappedTriangulatedApertureFace = TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(occtMakeMappedTriangulatedApertureFace.Face()));
						//		faces.push_back(mappedTriangulatedApertureFace);
						//	}
						//}
					}
				}






				//// Iterate through all aperture sample points.
				//// These will be the point to the subject polygons in the individual wall panel's UV space.
				//for (const std::list<Handle(Geom2d_CartesianPoint)>& rkApertureSampleVerticesUV : allApertureSampleVerticesUV)
				//{
				//	for (const Handle(Geom2d_CartesianPoint)& kpApertureSampleVertexUV : rkApertureSampleVerticesUV)
				//	{
				//		
				//	}
				//}
				//
				//==================

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
		return std::make_shared<Shell>(TopoDS::Shell(occtSewing.SewedShape()));
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
