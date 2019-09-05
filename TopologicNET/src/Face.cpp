#include "Face.h"
#include "Vertex.h"
#include "Edge.h"
#include "Wire.h"
#include "Shell.h"
#include "Cell.h"
#include "FaceFactory.h"
#include "FaceUtility.h"
#include "EdgeUtility.h"

#ifndef TOPOLOGIC_DYNAMO
#include "NurbsSurface.h"
#endif

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_OffsetSurface.hxx>
#include <Geom_SurfaceOfLinearExtrusion.hxx>
#include <Geom_SurfaceOfRevolution.hxx>
#include <GeomConvert.hxx>
#include <GeomPlate_Surface.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeFix_Edge.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include <algorithm>
#include <assert.h>

namespace Topologic
{
	List<Face^>^ Face::AdjacentFaces::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Face::Ptr> pAdjacentCoreFaces;
		pCoreFace->AdjacentFaces(pAdjacentCoreFaces);

		List<Face^>^ pAdjacentFaces = gcnew List<Face^>();

		for(std::list<TopologicCore::Face::Ptr>::const_iterator kAdjacentFaceIterator = pAdjacentCoreFaces.begin();
			kAdjacentFaceIterator != pAdjacentCoreFaces.end();
			kAdjacentFaceIterator++)
		{
			const TopologicCore::Face::Ptr& kpCoreFace = *kAdjacentFaceIterator;
			Face^ pFace = gcnew Face(kpCoreFace);
			pAdjacentFaces->Add(pFace);
		}

		return pAdjacentFaces;
	}

	List<Cell^>^ Face::Cells::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Cell::Ptr> pCoreCells;
		pCoreFace->Cells(pCoreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		for (std::list<TopologicCore::Cell::Ptr>::const_iterator kCellIterator = pCoreCells.begin();
			kCellIterator != pCoreCells.end();
			kCellIterator++)
		{
			const TopologicCore::Cell::Ptr& kpCoreCell = *kCellIterator;
			Cell^ pCell = gcnew Cell(kpCoreCell);
			pCells->Add(pCell);
		}

		return pCells;
	}

	List<Shell^>^ Face::Shells::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Shell::Ptr> pCoreShells;
		pCoreFace->Shells(pCoreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();

		for (std::list<TopologicCore::Shell::Ptr>::const_iterator kShellIterator = pCoreShells.begin();
			kShellIterator != pCoreShells.end();
			kShellIterator++)
		{
			const TopologicCore::Shell::Ptr& kpCoreShell = *kShellIterator;
			Shell^ pShell = gcnew Shell(kpCoreShell);
			pShells->Add(pShell);
		}

		return pShells;
	}

	/*Vertex^ Face::CenterOfMass()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreFace->CenterOfMass();
		return gcnew Vertex(pCoreCenterOfMass);
	}*/

	Face^ Face::ByWire(Wire^ wire)
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Face::ByExternalBoundary(
			TopologicCore::Topology::Downcast<TopologicCore::Wire>(wire->GetCoreTopologicalQuery())
		);
		return gcnew Face(pCoreFace);
	}

	Face ^ Face::ByExternalInternalBoundaries(Wire ^ externalBoundary, System::Collections::Generic::IEnumerable<Wire^>^ internalBoundaries)
	{
		TopologicCore::Wire::Ptr pCoreExternalBoundary = TopologicCore::Topology::Downcast<TopologicCore::Wire>(externalBoundary->GetCoreTopologicalQuery());
		std::list<TopologicCore::Wire::Ptr> coreInternalBoundaries;

		for each(Wire^ internalBoundary in internalBoundaries)
		{
			coreInternalBoundaries.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(internalBoundary->GetCoreTopologicalQuery()));
		}

		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Face::ByExternalInternalBoundaries(pCoreExternalBoundary, coreInternalBoundaries);

		return gcnew Face(pCoreFace);
	}

	Face^ Face::ByNurbsParameters(List<List<Vertex^>^>^ controlPoints, List<List<double>^>^ weights, List<double>^ uKnots, List<double>^ vKnots, bool isRational, bool isUPeriodic, bool isVPeriodic, int uDegree, int vDegree)
	{
		if (controlPoints->Count == 0 || controlPoints[0]->Count == 0 ||
			weights->Count == 0 || weights[0]->Count == 0)
		{
			return nullptr;
		}
		// 1. NURBS parameters
		// Transfer the poles/control points
		//array<array<Autodesk::DesignScript::Geometry::Point^>^>^ pDynamoControlPoints = pDynamoNurbsSurface->ControlPoints();

		TColgp_Array2OfPnt occtPoles(0, controlPoints->Count - 1, 0, controlPoints[0]->Count - 1);
		for (int i = occtPoles.LowerRow(); i <= occtPoles.UpperRow(); i++)
		{
			List<Vertex^>^ controlPoints1D = controlPoints[i];
			for (int j = occtPoles.LowerCol(); j <= occtPoles.UpperCol(); j++)
			{
				List<double>^ coordinates = controlPoints1D[j]->Coordinates;
				occtPoles.SetValue(i, j, gp_Pnt(coordinates[0], coordinates[1], coordinates[2]));
			}
		}

		// Transfer the weights
		//array<array<double>^>^ pDynamoWeights = pDynamoNurbsSurface->Weights();
		TColStd_Array2OfReal occtWeights(0, weights->Count - 1, 0, weights[0]->Count - 1);
		for (int i = occtWeights.LowerRow(); i <= occtWeights.UpperRow(); i++)
		{
			List<double>^ weights1D = weights[i];
			for (int j = occtWeights.LowerCol(); j <= occtWeights.UpperCol(); j++)
			{
				double weight = weights1D[j];
				occtWeights.SetValue(i, j, weight);
			}
		}

		// Transfer the U knots and U multiplicities. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
		List<double>^ uniqueUKnots = gcnew List<double>();
		List<int>^ pUMultiplicities = gcnew List<int>();
		double previousUKnot = uKnots[0] - 1.0;
		int uMultiplicity = 0;
		for each(double uKnot in uKnots)
		{
			if (uKnot > previousUKnot)
			{
				if (previousUKnot > uKnots[0] - 1.0)
					pUMultiplicities->Add(uMultiplicity);
				uniqueUKnots->Add(uKnot);
				uMultiplicity = 1;
			}
			else
			{
				uMultiplicity++;
			}
			previousUKnot = uKnot;
		}
		pUMultiplicities->Add(uMultiplicity);

		TColStd_Array1OfReal occtUKnots(0, uniqueUKnots->Count - 1);
		for (int i = occtUKnots.Lower(); i <= occtUKnots.Upper(); i++)
		{
			occtUKnots.SetValue(i, uniqueUKnots[i]);
		}

		TColStd_Array1OfInteger occtUMultiplicities(0, pUMultiplicities->Count - 1);
		for (int i = occtUMultiplicities.Lower(); i <= occtUMultiplicities.Upper(); i++)
		{
			occtUMultiplicities.SetValue(i, pUMultiplicities[i]);
		}

		// Transfer the V knots and V multiplicities. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
		List<double>^ uniqueVKnots = gcnew List<double>();
		List<int>^ pVMultiplicities = gcnew List<int>();
		double previousVKnot = vKnots[0] - 1.0;
		int vMultiplicity = 0;
		for each(double vKnot in vKnots)
		{
			if (vKnot > previousVKnot)
			{
				if (previousVKnot > vKnots[0] - 1.0)
					pVMultiplicities->Add(vMultiplicity);
				uniqueVKnots->Add(vKnot);
				vMultiplicity = 1;
			}
			else
			{
				vMultiplicity++;
			}
			previousVKnot = vKnot;
		}
		pVMultiplicities->Add(vMultiplicity);

		TColStd_Array1OfReal occtVKnots(0, uniqueVKnots->Count - 1);
		for (int i = occtVKnots.Lower(); i <= occtVKnots.Upper(); i++)
		{
			occtVKnots.SetValue(i, uniqueVKnots[i]);
		}

		TColStd_Array1OfInteger occtVMultiplicities(0, pVMultiplicities->Count - 1);
		for (int i = occtVMultiplicities.Lower(); i <= occtVMultiplicities.Upper(); i++)
		{
			occtVMultiplicities.SetValue(i, pVMultiplicities[i]);
		}

		// Get other arguments: degrees, periodicness, and rationality
		
		TopologicCore::Face::Ptr pCoreFace = nullptr;
		try {
			pCoreFace = TopologicCore::Face::BySurface(
				occtPoles,
				occtWeights,
				occtUKnots,
				occtVKnots,
				occtUMultiplicities,
				occtVMultiplicities,
				uDegree,
				vDegree,
				isUPeriodic,
				isVPeriodic,
				isRational,
				nullptr,
				std::list<TopologicCore::Wire::Ptr>()
			);
		}
		catch (const std::exception& rkException)
		{
			throw gcnew Exception(gcnew String(rkException.what()));
		}

		return gcnew Face(pCoreFace);
	}

	Face^ Face::ByEdges(System::Collections::Generic::IEnumerable<Edge^>^ edges)
	{
		return ByWire(Wire::ByEdges(edges));
	}

#ifdef TOPOLOGIC_DYNAMO
	Face^ Face::BySurface(Autodesk::DesignScript::Geometry::Surface^ surface)
	{
		if (surface->GetType() == Autodesk::DesignScript::Geometry::NurbsSurface::typeid)
		{
			return BySurface(safe_cast<Autodesk::DesignScript::Geometry::NurbsSurface^>(surface), surface->PerimeterCurves());
		}
		else if (surface->GetType() == Autodesk::DesignScript::Geometry::PolySurface::typeid)
		{
			throw gcnew System::Exception("Cannot create an Edge from a PolyCurve. Create a Wire instead.");
		}
		else if (surface->GetType() == Autodesk::DesignScript::Geometry::Surface::typeid) // a generic surface
		{
			Autodesk::DesignScript::Geometry::NurbsSurface^ dynamoNurbsSurface = nullptr;
			array<Autodesk::DesignScript::Geometry::Curve^>^ dynamoPerimeterCurves = nullptr;
			try {
				dynamoNurbsSurface = surface->ToNurbsSurface();
				dynamoPerimeterCurves = surface->PerimeterCurves();
			}
			catch (Exception ^ e)
			{
				throw gcnew System::Exception("Dynamo fails to create a face: " + e->Message);
			}
			catch (...)
			{
				throw gcnew System::Exception("Dynamo fails to create a face.");
			}


			Face^ face = BySurface(dynamoNurbsSurface, dynamoPerimeterCurves);
			delete dynamoNurbsSurface;
			for each(Autodesk::DesignScript::Geometry::Curve^ dynamoPerimeterCurve in dynamoPerimeterCurves)
			{
				delete dynamoPerimeterCurve;
			}
			return face;
		}
		else
		{
			throw gcnew ArgumentException("The argument is not a valid Dynamo surface.");
		}
	}

	Autodesk::DesignScript::Geometry::Surface^ Face::Surface()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		Handle(Geom_Surface) pOcctSurface = pCoreFace->Surface();

		Handle(Geom_BezierSurface) pOcctBezierSurface = Handle_Geom_BezierSurface::DownCast(pOcctSurface);
		if (!pOcctBezierSurface.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_BSplineSurface) pOcctBSplineSurface = Handle_Geom_BSplineSurface::DownCast(pOcctSurface);
		if (!pOcctBSplineSurface.IsNull())
		{
			// Transfer the control points
			const TColgp_Array2OfPnt& rkOcctControlPoints = pOcctBSplineSurface->Poles();
			array<array<Autodesk::DesignScript::Geometry::Point^>^>^ pDynamoControlPoints = gcnew array<array<Autodesk::DesignScript::Geometry::Point^>^>(rkOcctControlPoints.ColLength());
			for (int i = rkOcctControlPoints.LowerRow(); i <= rkOcctControlPoints.UpperRow(); i++)
			{
				pDynamoControlPoints[i - rkOcctControlPoints.LowerRow()] = gcnew array<Autodesk::DesignScript::Geometry::Point^>(rkOcctControlPoints.RowLength());
				for (int j = rkOcctControlPoints.LowerCol(); j <= rkOcctControlPoints.UpperCol(); j++)
				{
					const gp_Pnt& rkControlPoint = rkOcctControlPoints.Value(i, j);
					pDynamoControlPoints[i - rkOcctControlPoints.LowerRow()][j - rkOcctControlPoints.LowerCol()] = Autodesk::DesignScript::Geometry::Point::ByCoordinates(rkControlPoint.X(), rkControlPoint.Y(), rkControlPoint.Z());
				}
			}

			// Transfer the control points' weights
			//const TColStd_Array2OfReal* pkOcctWeights = pOcctBSplineSurface->Weights();
			TColStd_Array2OfReal* pkOcctWeights = new TColStd_Array2OfReal(rkOcctControlPoints.LowerRow(), rkOcctControlPoints.UpperRow(), rkOcctControlPoints.LowerCol(), pOcctBSplineSurface->NbVPoles());
			pOcctBSplineSurface->Weights(*pkOcctWeights);
			array<array<double>^>^ pWeights = gcnew array<array<double>^>(pkOcctWeights->ColLength());
			assert(pkOcctWeights->Length() == rkOcctControlPoints.Length() && "Weights and control points have different lengths.");
			for (int i = pkOcctWeights->LowerRow(); i <= pkOcctWeights->UpperRow(); i++)
			{
				pWeights[i - pkOcctWeights->LowerRow()] = gcnew array<double>(pkOcctWeights->RowLength());
				for (int j = pkOcctWeights->LowerCol(); j <= pkOcctWeights->UpperCol(); j++)
				{
					pWeights[i - pkOcctWeights->LowerRow()][j - rkOcctControlPoints.LowerCol()] = pkOcctWeights->Value(i, j);
				}
			}
			delete pkOcctWeights;

			// Transfer the U knots. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
			const TColStd_Array1OfReal& krOcctUKnots = pOcctBSplineSurface->UKnots();
			List<double>^ pUKnots = gcnew List<double>();
			for (int i = krOcctUKnots.Lower(); i <= krOcctUKnots.Upper(); i++)
			{
				int uMultiplicity = pOcctBSplineSurface->UMultiplicity(i);
				for (int j = 0; j < uMultiplicity; j++)
				{
					pUKnots->Add(krOcctUKnots.Value(i));
				}
			}

			// Transfer the V knots. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
			const TColStd_Array1OfReal& krOcctVKnots = pOcctBSplineSurface->VKnots();
			List<double>^ pVKnots = gcnew List<double>();
			for (int i = krOcctVKnots.Lower(); i <= krOcctVKnots.Upper(); i++)
			{
				int vMultiplicity = pOcctBSplineSurface->VMultiplicity(i);
				for (int j = 0; j < vMultiplicity; j++)
				{
					pVKnots->Add(krOcctVKnots.Value(i));
				}
			}

			// OCCT has arbitrary parameters. Dynamo's parameter ranges between 0 and 1.
			// Order: First - Start - End - Last
			TopoDS_Face occtFace = TopoDS::Face(pCoreFace->GetOcctShape());
			TopExp_Explorer ex(occtFace, TopAbs_EDGE);
			for (; ex.More(); ex.Next()) {
				ShapeFix_Edge occtShapeFix;
				occtShapeFix.FixAddPCurve(TopoDS::Edge(ex.Current()), occtFace, false);
			}

			double u0 = 0.0, u1 = 0.0, v0 = 0.0, v1 = 0.0;
			ShapeAnalysis::GetFaceUVBounds(occtFace, u0, u1, v0, v1);

			GeomAdaptor_Surface occtSurfaceAdaptor(pOcctSurface);
			double occtFirstUParameter = occtSurfaceAdaptor.FirstUParameter();
			double occtLastUParameter = occtSurfaceAdaptor.LastUParameter();
			double occtDeltaUParameter = occtLastUParameter - occtFirstUParameter;
			if (occtDeltaUParameter < Precision::Confusion())
				occtDeltaUParameter = Precision::Confusion();
			double dynamoStartUParameter = (u0 - occtFirstUParameter) / occtDeltaUParameter;
			if (dynamoStartUParameter < 0.0)
				dynamoStartUParameter = 0.0;
			double dynamoEndUParameter = (u1 - occtFirstUParameter) / occtDeltaUParameter;
			if (dynamoEndUParameter > 1.0)
				dynamoEndUParameter = 1.0;

			double occtFirstVParameter = occtSurfaceAdaptor.FirstVParameter();
			double occtLastVParameter = occtSurfaceAdaptor.LastVParameter();
			double occtDeltaVParameter = occtLastVParameter - occtFirstVParameter;
			if (occtDeltaVParameter < Precision::Confusion())
				occtDeltaVParameter = Precision::Confusion();
			double dynamoStartVParameter = (v0 - occtFirstVParameter) / occtDeltaVParameter;
			if (dynamoStartVParameter < 0.0)
				dynamoStartVParameter = 0.0;
			double dynamoEndVParameter = (v1 - occtFirstVParameter) / occtDeltaVParameter;
			if (dynamoEndVParameter > 1.0)
				dynamoEndVParameter = 1.0;

			Autodesk::DesignScript::Geometry::UV^ pDynamoUMinVMin = Autodesk::DesignScript::Geometry::UV::ByCoordinates(dynamoStartUParameter, dynamoStartVParameter);
			Autodesk::DesignScript::Geometry::UV^ pDynamoUMinVMax = Autodesk::DesignScript::Geometry::UV::ByCoordinates(dynamoStartUParameter, dynamoEndVParameter);
			Autodesk::DesignScript::Geometry::UV^ pDynamoUMaxVMin = Autodesk::DesignScript::Geometry::UV::ByCoordinates(dynamoEndUParameter, dynamoStartVParameter);
			Autodesk::DesignScript::Geometry::UV^ pDynamoUMaxVMax = Autodesk::DesignScript::Geometry::UV::ByCoordinates(dynamoEndUParameter, dynamoEndVParameter);

			// Create an untrimmed Dynamo surface
			Autodesk::DesignScript::Geometry::NurbsSurface^ pDynamoUntrimmedSurface =
				Autodesk::DesignScript::Geometry::NurbsSurface::ByControlPointsWeightsKnots(
					pDynamoControlPoints,
					pWeights,
					pUKnots->ToArray(),
					pVKnots->ToArray(),
					pOcctBSplineSurface->UDegree(),
					pOcctBSplineSurface->VDegree()
				);

			//return pDynamoUntrimmedSurface;

			// The newly created surface corresponds to the whole surface in OCCT. 
			// It needs to be trimmed by the outer wire and inner wires.
			List<Autodesk::DesignScript::Geometry::PolyCurve^>^ pDynamoEdgeLoops = gcnew List<Autodesk::DesignScript::Geometry::PolyCurve^>();

			List<Wire^>^ pWires = Wires;
			for each(Wire^ pWire in pWires)
			{
				try {
					Object^ pWireGeometry = pWire->BasicGeometry;
					Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoWireGeometry = safe_cast<Autodesk::DesignScript::Geometry::PolyCurve^>(pWireGeometry);
					if (pDynamoWireGeometry != nullptr)
					{

						pDynamoEdgeLoops->Add(pDynamoWireGeometry);
					}
				}
				catch (Exception^)
				{
					int i = 0;
				}
			}

			for each(array<Autodesk::DesignScript::Geometry::Point^>^ pDynamo1DControlPoints in pDynamoControlPoints)
			{
				for each(Autodesk::DesignScript::Geometry::Point^ pDynamoControlPoint in pDynamo1DControlPoints)
				{
					delete pDynamoControlPoint;
				}
			}

			delete pDynamoUMinVMin;
			delete pDynamoUMinVMax;
			delete pDynamoUMaxVMin;
			delete pDynamoUMaxVMax;

			if (pDynamoEdgeLoops->Count == 0)
			{
				return pDynamoUntrimmedSurface;
			}

			try {
				Autodesk::DesignScript::Geometry::Surface^ pDynamoTrimmedSurfaceByParameters =
					pDynamoUntrimmedSurface->TrimWithEdgeLoops(pDynamoEdgeLoops);

				for each(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoEdgeLoop in pDynamoEdgeLoops)
				{
					delete pDynamoEdgeLoop;
				}

				delete pDynamoUntrimmedSurface;

				return pDynamoTrimmedSurfaceByParameters;
			}
			catch (std::exception& e)
			{
				String^ str = gcnew String(e.what());
			}
			catch (Exception^ e)
			{
				//throw e;
				String^ str(e->Message);
				//if (e->Message->Equals(gcnew String("trim_with_edge_loops requires all curves to touch surface")))
				{
					// Project to surface
					List<Autodesk::DesignScript::Geometry::PolyCurve^>^ pDynamoProjectedEdgeLoops = gcnew List<Autodesk::DesignScript::Geometry::PolyCurve^>();

					for each(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoEdgeLoop in pDynamoEdgeLoops)
					{
						List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoProjectedCurves =
							gcnew List<Autodesk::DesignScript::Geometry::Curve^>();
						try {
							array<Autodesk::DesignScript::Geometry::Curve^>^ dynamoLoopCurves = pDynamoEdgeLoop->Curves();

							for each(Autodesk::DesignScript::Geometry::Curve^ dynamoLoopCurve in dynamoLoopCurves)
							{
								Autodesk::DesignScript::Geometry::Curve^ pDynamoProjectedCurve =
									dynamoLoopCurve->PullOntoSurface(pDynamoUntrimmedSurface);
								pDynamoProjectedCurves->Add(pDynamoProjectedCurve);
							}
							Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoProjectedPolyCurve =
								Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoProjectedCurves, 0.001);
							if (pDynamoProjectedPolyCurve->NumberOfCurves == pDynamoProjectedCurves->Count)
							{
								pDynamoProjectedEdgeLoops->Add(pDynamoProjectedPolyCurve);
								pDynamoProjectedCurves->Clear();
							}
						}
						catch (...)
						{
						}

						for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoProjectedCurve in pDynamoProjectedCurves)
						{
							delete pDynamoProjectedCurve;
						}
					}

					Autodesk::DesignScript::Geometry::Surface^ pDynamoTrimmedSurfaceByParameters = nullptr;

					if (pDynamoProjectedEdgeLoops->Count > 0)
					{
						try {
							pDynamoTrimmedSurfaceByParameters =
								pDynamoUntrimmedSurface->TrimWithEdgeLoops(pDynamoProjectedEdgeLoops);
						}
						catch (...)
						{
							pDynamoTrimmedSurfaceByParameters = pDynamoUntrimmedSurface;
						}

						for each(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoEdgeLoop in pDynamoEdgeLoops)
						{
							delete pDynamoEdgeLoop;
						}

						for each(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoProjectedEdgeLoop in pDynamoProjectedEdgeLoops)
						{
							delete pDynamoProjectedEdgeLoop;
						}

						if (pDynamoTrimmedSurfaceByParameters != pDynamoUntrimmedSurface)
						{
							delete pDynamoUntrimmedSurface;
						}

						return pDynamoTrimmedSurfaceByParameters;
					}
					else
					{

						for each(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoEdgeLoop in pDynamoEdgeLoops)
						{
							delete pDynamoEdgeLoop;
						}

						return pDynamoUntrimmedSurface;
					}					
				}
			}
			catch (...)
			{
			}

			for each(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoEdgeLoop in pDynamoEdgeLoops)
			{
				delete pDynamoEdgeLoop;
			}

			return pDynamoUntrimmedSurface;
		}

		Handle(Geom_RectangularTrimmedSurface) pOcctRectangularTrimmedSurface = Handle_Geom_RectangularTrimmedSurface::DownCast(pOcctSurface);
		if (!pOcctRectangularTrimmedSurface.IsNull())
		{
			Handle(Geom_Surface) pOcctBasisSurface = pOcctRectangularTrimmedSurface->BasisSurface();
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_ConicalSurface) pOcctConicalSurface = Handle_Geom_ConicalSurface::DownCast(pOcctSurface);
		if (!pOcctConicalSurface.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_CylindricalSurface) pOcctCylindricalSurface = Handle_Geom_CylindricalSurface::DownCast(pOcctSurface);
		if (!pOcctCylindricalSurface.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_Plane) pOcctPlane = Handle_Geom_Plane::DownCast(pOcctSurface);
		if (!pOcctPlane.IsNull())
		{
			// This is a planar face, and pOcctPlane is simply the supporting infinite plane.
			// In this case, do the following steps.

			// Get the wires and edges.
			List<Wire^>^ pWires = Wires;
			List<Edge^>^ pEdges = Edges;

			// If there is only one wire, create a surface and return it.
			if (pWires->Count < 2)
			{
				List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurves = gcnew List<Autodesk::DesignScript::Geometry::Curve^>();
				for each(Edge^ pEdge in pEdges)
				{
					// 2. Convert the edges to Dynamo curves
					pDynamoCurves->Add(pEdge->Curve());
				}
				Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface = nullptr;

				if (pDynamoCurves->Count > 0)
				{
					Autodesk::DesignScript::Geometry::PolyCurve^ dynamoPolyCurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoCurves, 0.001);
					pDynamoSurface = Autodesk::DesignScript::Geometry::Surface::ByPatch(dynamoPolyCurve);
					delete dynamoPolyCurve;
				}

				for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
				{
					delete pDynamoCurve;
				}

				return pDynamoSurface;
			}

			// Otherwise, identify the outer wire.
			// Create a surface from this wire
			// Trim the surface with all the wires (including the outer wire)

			Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface = nullptr;

			List<Autodesk::DesignScript::Geometry::PolyCurve^>^ pDynamoEdgeLoops = gcnew List<Autodesk::DesignScript::Geometry::PolyCurve^>();
			const TopoDS_Wire& rkOcctOuterWire = ShapeAnalysis::OuterWire(pCoreFace->GetOcctFace());

			for each(Wire^ pWire in pWires)
			{
				if (pDynamoSurface == nullptr)
				{
					if (TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(pWire->GetCoreTopologicalQuery())->GetOcctShape().IsSame(rkOcctOuterWire))
					{
						List<Edge^>^ pOuterEdges = pWire->Edges;
						List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoOuterCurves = gcnew List<Autodesk::DesignScript::Geometry::Curve^>();
						for each(Edge^ pOuterEdge in pOuterEdges)
						{
							// 2. Convert the edges to Dynamo curves
							pDynamoOuterCurves->Add(pOuterEdge->Curve());
						}

						Autodesk::DesignScript::Geometry::PolyCurve^ dynamoPolyCurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoOuterCurves, 0.001);
						if (pDynamoSurface != nullptr)
						{
							delete pDynamoSurface;
						}
						pDynamoSurface = Autodesk::DesignScript::Geometry::Surface::ByPatch(dynamoPolyCurve);

						delete dynamoPolyCurve;
						for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoOuterCurve in pDynamoOuterCurves)
						{
							delete pDynamoOuterCurve;
						}
					}
				}

				Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve = safe_cast<Autodesk::DesignScript::Geometry::PolyCurve^>(pWire->BasicGeometry);
				if (pDynamoPolycurve != nullptr)
				{
					pDynamoEdgeLoops->Add(pDynamoPolycurve);
				}
			}

			Autodesk::DesignScript::Geometry::Surface^ pTrimmedDynamoSurface = nullptr;
			try {
				pTrimmedDynamoSurface = pDynamoSurface->TrimWithEdgeLoops(pDynamoEdgeLoops);
			}
			catch (Exception^ e)
			{
				String^ str(e->Message);
				throw e;
			}
			catch (...)
			{
				throw gcnew Exception("Unknown exception");
			}
			
			if (pDynamoSurface != nullptr)
			{
				delete pDynamoSurface;
			}
			for each(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoEdgeLoop in pDynamoEdgeLoops)
			{
				delete pDynamoEdgeLoop;
			}

			return pTrimmedDynamoSurface;
		}

		Handle(Geom_SphericalSurface) pOcctSphericalSurface = Handle_Geom_SphericalSurface::DownCast(pOcctSurface);
		if (!pOcctSphericalSurface.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_ToroidalSurface) pOcctToroidalSurface = Handle_Geom_ToroidalSurface::DownCast(pOcctSurface);
		if (!pOcctToroidalSurface.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_OffsetSurface) pOcctOffsetSurface = Handle_Geom_OffsetSurface::DownCast(pOcctSurface);
		if (!pOcctOffsetSurface.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_SurfaceOfLinearExtrusion) pOcctSurfaceOfLinearExtrusion = Handle_Geom_SurfaceOfLinearExtrusion::DownCast(pOcctSurface);
		if (!pOcctSurfaceOfLinearExtrusion.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_SurfaceOfRevolution) pOcctSurfaceOfRevolution = Handle_Geom_SurfaceOfRevolution::DownCast(pOcctSurface);
		if (!pOcctSurfaceOfRevolution.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(GeomPlate_Surface) pOcctPlateSurface = Handle_GeomPlate_Surface::DownCast(pOcctSurface);
		if (!pOcctPlateSurface.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		List<Vertex^>^ pVertices = Vertices;
		List<Autodesk::DesignScript::Geometry::Point^>^ pDynamoPoints = gcnew List<Autodesk::DesignScript::Geometry::Point^>();
		for each(Vertex^ pVertex in pVertices)
		{
			pDynamoPoints->Add(safe_cast<Autodesk::DesignScript::Geometry::Point^>(pVertex->BasicGeometry));
		}

		Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface = Autodesk::DesignScript::Geometry::Surface::ByPerimeterPoints(pDynamoPoints);

		for each(Autodesk::DesignScript::Geometry::Point^ pDynamoPoint in pDynamoPoints)
		{
			delete pDynamoPoint;
		}

		return pDynamoSurface;
	}

	Autodesk::DesignScript::Geometry::Mesh^ Face::TriangulatedMesh()
	{
		TopoDS_Face occtFace = TopoDS::Face(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery())->GetOcctShape());
		BRepMesh_IncrementalMesh occtMesh(occtFace, 0.1);
		TopLoc_Location occtLocation;
		Handle_Poly_Triangulation occtTriangulation = BRep_Tool::Triangulation(occtFace, occtLocation);
		if (occtTriangulation.IsNull())
			return nullptr;

		// retrieve the faces
		List<Autodesk::DesignScript::Geometry::Point^>^ pDynamoPoints = gcnew List<Autodesk::DesignScript::Geometry::Point^>();
		const TColgp_Array1OfPnt& rkOcctPoints = occtTriangulation->Nodes();
		TColgp_Array1OfPnt::Iterator occtPointsIterator;
		for (occtPointsIterator.Init(rkOcctPoints); occtPointsIterator.More(); occtPointsIterator.Next())
		{
			const gp_Pnt& rkOcctPoint = occtPointsIterator.Value();
			pDynamoPoints->Add(Autodesk::DesignScript::Geometry::Point::ByCoordinates(rkOcctPoint.X(), rkOcctPoint.Y(), rkOcctPoint.Z()));
		}
		int occtLowerBound = rkOcctPoints.Lower();

		// retrieve the triangle indices
		List<Autodesk::DesignScript::Geometry::IndexGroup^>^ pDynamoTriangleIndices = gcnew List<Autodesk::DesignScript::Geometry::IndexGroup^>();
		const Poly_Array1OfTriangle& rkOcctTriangleIndices = occtTriangulation->Triangles(); Poly_Array1OfTriangle::Iterator occtTriangleIndicesIterator;
		for (occtTriangleIndicesIterator.Init(rkOcctTriangleIndices); occtTriangleIndicesIterator.More(); occtTriangleIndicesIterator.Next())
		{
			const Poly_Triangle& rkOcctTriangleIndices = occtTriangleIndicesIterator.Value();
			int occtIndex1 = -1, occtIndex2 = -1, occtIndex3 = -1;
			rkOcctTriangleIndices.Get(occtIndex1, occtIndex2, occtIndex3);

			// OCCT indices start from occtLowerBound (1), while Dynamo's start from 0.
			pDynamoTriangleIndices->Add(Autodesk::DesignScript::Geometry::IndexGroup::ByIndices(occtIndex1 - occtLowerBound, occtIndex2 - occtLowerBound, occtIndex3 - occtLowerBound));
		}

		Autodesk::DesignScript::Geometry::Mesh^ pDynamoMesh = Autodesk::DesignScript::Geometry::Mesh::ByPointsFaceIndices(pDynamoPoints, pDynamoTriangleIndices);

		for each(Autodesk::DesignScript::Geometry::Point^ pDynamoPoint in pDynamoPoints)
		{
			delete pDynamoPoint;
		}
		for each(Autodesk::DesignScript::Geometry::IndexGroup^ pDynamoTriangleIndex in pDynamoTriangleIndices)
		{
			delete pDynamoTriangleIndex;
		}

		return pDynamoMesh;
	}

	Face^ Face::BySurface(Autodesk::DesignScript::Geometry::NurbsSurface^ pDynamoNurbsSurface,
		array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoPerimeterCurves)
	{
		// 1. NURBS parameters
		// Transfer the poles/control points
		array<array<Autodesk::DesignScript::Geometry::Point^>^>^ pDynamoControlPoints = pDynamoNurbsSurface->ControlPoints();

		TColgp_Array2OfPnt occtPoles(0, pDynamoNurbsSurface->NumControlPointsU - 1, 0, pDynamoNurbsSurface->NumControlPointsV - 1);
		for (int i = occtPoles.LowerRow(); i <= occtPoles.UpperRow(); i++)
		{
			for (int j = occtPoles.LowerCol(); j <= occtPoles.UpperCol(); j++)
			{
				occtPoles.SetValue(i, j, gp_Pnt(pDynamoControlPoints[i][j]->X, pDynamoControlPoints[i][j]->Y, pDynamoControlPoints[i][j]->Z));
			}
		}

		// Transfer the weights
		array<array<double>^>^ pDynamoWeights = pDynamoNurbsSurface->Weights();
		TColStd_Array2OfReal occtWeights(0, pDynamoWeights->Length - 1, 0, pDynamoWeights[0]->Length - 1);
		for (int i = occtWeights.LowerRow(); i <= occtWeights.UpperRow(); i++)
		{
			for (int j = occtWeights.LowerCol(); j <= occtWeights.UpperCol(); j++)
			{
				double weight = pDynamoWeights[i][j];
				occtWeights.SetValue(i, j, weight);
			}
		}

		// Transfer the U knots and U multiplicities. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
		array<double>^ pDynamoUKnots = pDynamoNurbsSurface->UKnots();
		array<double>^ pDynamoUKnotsCopy = gcnew array<double>(pDynamoUKnots->Length);
		Array::Copy(pDynamoUKnots, pDynamoUKnotsCopy, pDynamoUKnots->Length);
		Array::Sort(pDynamoUKnotsCopy);
		List<double>^ pUKnots = gcnew List<double>();
		List<int>^ pUMultiplicities = gcnew List<int>();
		double previousUKnot = pDynamoUKnotsCopy[0] - 1.0;
		int uMultiplicity = 0;
		for each(double uKnot in pDynamoUKnots)
		{
			if (uKnot > previousUKnot)
			{
				if (previousUKnot > pDynamoUKnotsCopy[0] - 1.0)
					pUMultiplicities->Add(uMultiplicity);
				pUKnots->Add(uKnot);
				uMultiplicity = 1;
			}
			else
			{
				uMultiplicity++;
			}
			previousUKnot = uKnot;
		}
		pUMultiplicities->Add(uMultiplicity);

		TColStd_Array1OfReal occtUKnots(0, pUKnots->Count - 1);
		for (int i = occtUKnots.Lower(); i <= occtUKnots.Upper(); i++)
		{
			occtUKnots.SetValue(i, pUKnots[i]);
		}

		TColStd_Array1OfInteger occtUMultiplicities(0, pUMultiplicities->Count - 1);
		for (int i = occtUMultiplicities.Lower(); i <= occtUMultiplicities.Upper(); i++)
		{
			occtUMultiplicities.SetValue(i, pUMultiplicities[i]);
		}

		// Transfer the V knots and V multiplicities. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
		array<double>^ pDynamoVKnots = pDynamoNurbsSurface->VKnots();
		array<double>^ pDynamoVKnotsCopy = gcnew array<double>(pDynamoVKnots->Length);
		Array::Copy(pDynamoVKnots, pDynamoVKnotsCopy, pDynamoVKnots->Length);
		Array::Sort(pDynamoVKnotsCopy);
		List<double>^ pVKnots = gcnew List<double>();
		List<int>^ pVMultiplicities = gcnew List<int>();
		double previousVKnot = pDynamoVKnotsCopy[0] - 1.0;
		int vMultiplicity = 0;
		for each(double vKnot in pDynamoVKnots)
		{
			if (vKnot > previousVKnot)
			{
				if (previousVKnot > pDynamoVKnotsCopy[0] - 1.0)
					pVMultiplicities->Add(vMultiplicity);
				pVKnots->Add(vKnot);
				vMultiplicity = 1;
			}
			else
			{
				vMultiplicity++;
			}
			previousVKnot = vKnot;
		}
		pVMultiplicities->Add(vMultiplicity);

		TColStd_Array1OfReal occtVKnots(0, pVKnots->Count - 1);
		for (int i = occtVKnots.Lower(); i <= occtVKnots.Upper(); i++)
		{
			occtVKnots.SetValue(i, pVKnots[i]);
		}

		TColStd_Array1OfInteger occtVMultiplicities(0, pVMultiplicities->Count - 1);
		for (int i = occtVMultiplicities.Lower(); i <= occtVMultiplicities.Upper(); i++)
		{
			occtVMultiplicities.SetValue(i, pVMultiplicities[i]);
		}

		// Get other arguments: degrees, periodicness, and rationality
		int uDegree = pDynamoNurbsSurface->DegreeU;
		int vDegree = pDynamoNurbsSurface->DegreeV;
		bool isUPeriodic = pDynamoNurbsSurface->IsPeriodicInU;
		bool isVPeriodic = pDynamoNurbsSurface->IsPeriodicInV;
		bool isRational = pDynamoNurbsSurface->IsRational;

		//=================================================
		// 2. Get the polycurves

		Autodesk::DesignScript::Geometry::PolyCurve^ pOuterPolycurve = nullptr;
		bool checkWire = true;

		// Retrieve the perimeters as curves. These comprise of the outer and, if any, inner perimeters.
		{
			List<List<Autodesk::DesignScript::Geometry::Curve^>^>^ pDynamoCurveGroups = gcnew List<List<Autodesk::DesignScript::Geometry::Curve^>^>();
			for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoPerimeterCurve in pDynamoPerimeterCurves)
			{
				// Set this flag to true if a polycurve can be created. But, don't store the polycurve yet. 
				// The final ones will be created at the end.
				List<List<Autodesk::DesignScript::Geometry::Curve^>^>^ pOtherConnectedCurveGroups = gcnew List<List<Autodesk::DesignScript::Geometry::Curve^>^>();
				bool isConnectedToAnotherCurve = false;
				for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurveGroup in pDynamoCurveGroups)
				{
					//Iterate through the individual curve. If the intersection is true, add it to the list and break.
					for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoGroupedCurve in pDynamoCurveGroup)
					{
						bool doesIntersect = false;
						try {
							doesIntersect = pDynamoPerimeterCurve->DoesIntersect(pDynamoGroupedCurve);
						}
						catch (Exception ^ e)
						{
							throw gcnew System::Exception("Dynamo fails to perform intersection test on a group of curve: " + e->Message);
						}
						catch (...)
						{
							throw gcnew System::Exception("Dynamo fails to perform intersection test on a group of curve.");
						}

						if (doesIntersect)
						{
							// Only add once; other groups will be added to this list.
							if (!isConnectedToAnotherCurve)
							{
								isConnectedToAnotherCurve = true;
								pDynamoCurveGroup->Add(pDynamoPerimeterCurve);
							}
							pOtherConnectedCurveGroups->Add(pDynamoCurveGroup);
							break; // No need to check other curves
						}
					}

					// Continue to check other groups
				}

				for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pOtherConnectedCurveGroup in pOtherConnectedCurveGroups)
				{
					if (pOtherConnectedCurveGroup == pOtherConnectedCurveGroups[0])
					{
						continue;
					}

					pOtherConnectedCurveGroups[0]->AddRange(pOtherConnectedCurveGroup);

					// Remove this list from pDynamoCurveGroups
					pDynamoCurveGroups->Remove(pOtherConnectedCurveGroup);
				}

				// If not yet added to any polycurve, create one.
				if (!isConnectedToAnotherCurve)
				{
					List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoNewConnectedCurves = gcnew List<Autodesk::DesignScript::Geometry::Curve^>();
					pDynamoNewConnectedCurves->Add(pDynamoPerimeterCurve);
					pDynamoCurveGroups->Add(pDynamoNewConnectedCurves);
				}
			}

			// Do the rest of operations in OpenCascade. Create wires by edges.
			TopologicCore::Wire::Ptr pCoreOuterWire = nullptr;
			try {
				List<Wire^>^ pWires = gcnew List<Wire^>();
				for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurveGroup in pDynamoCurveGroups)
				{
					List<Edge^>^ pEdges = gcnew List<Edge^>();
					for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurveGroup)
					{
						Edge^ pEdge = Edge::ByCurve(pDynamoCurve);
						pEdges->Add(pEdge);
					}
					Wire^ pWire = Wire::ByEdges(pEdges);
					pWires->Add(pWire);
				}


				std::list<double> surfaceAreas;
				for each(Wire^ pWire in pWires)
				{
					try {
						Face^ pFace = Face::ByWire(pWire);
						surfaceAreas.push_back(Topologic::Utilities::FaceUtility::Area(pFace));
					}
					catch (...)
					{
						surfaceAreas.push_back(0.0);
					}
				}

				if (checkWire)
				{
					std::list<double>::iterator maxAreaIterator = std::max_element(surfaceAreas.begin(), surfaceAreas.end());
					int index = 0;
					for (std::list<double>::iterator areaIterator = surfaceAreas.begin();
						areaIterator != maxAreaIterator;
						areaIterator++)
					{
						++index;
					}
					pOuterPolycurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoCurveGroups[index], 0.001);
					pCoreOuterWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>((Wire::ByPolyCurve(pOuterPolycurve))->GetCoreTopologicalQuery());
					delete pOuterPolycurve;
					pDynamoCurveGroups->RemoveAt(index);
				}
			}
			catch (...)
			{

			}

			// Get the outer polycurve and discard it from the pDynamoCurveGroups
			/*for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurveGroup in pDynamoCurveGroups)
			{
				try {
					Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolyCurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoCurveGroup);
					Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface = Autodesk::DesignScript::Geometry::Surface::ByPatch(pDynamoPolyCurve);
					surfaceAreas.push_back(pDynamoSurface->Area);
				}
				catch (std::exception& e)
				{
					String^ str = gcnew String(e.what());
					checkWire = false;
					break;
				}
				catch (Exception^ e)
				{
					String^ str(e->Message);
					checkWire = false;
					break;
				}
			}*/

			// 3. Bounding wires
			std::list<TopologicCore::Wire::Ptr> coreInnerWires;

			try {
				// Create wires and add them to occtMakeFace to create internal wires
				for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoConnectedCurves in pDynamoCurveGroups)
				{
					Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoConnectedCurves, 0.001);
					Wire^ pWire = Wire::ByPolyCurve(pDynamoPolycurve);
					delete pDynamoPolycurve;
					coreInnerWires.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(pWire->GetCoreTopologicalQuery()));
				}
			}
			catch (...)
			{

			}

			//=================================================
			Exception^ e = nullptr;
			TopologicCore::Face::Ptr pCoreFace = nullptr;
			try {
				pCoreFace = TopologicCore::Face::BySurface(
					occtPoles,
					occtWeights,
					occtUKnots,
					occtVKnots,
					occtUMultiplicities,
					occtVMultiplicities,
					uDegree,
					vDegree,
					isUPeriodic,
					isVPeriodic,
					isRational,
					pCoreOuterWire,
					coreInnerWires
				);
			}
			catch (const std::exception& rkException)
			{
				e = gcnew Exception(gcnew String(rkException.what()));
			}

			for each(array<Autodesk::DesignScript::Geometry::Point^>^ pDynamo1DControlPoints in pDynamoControlPoints)
			{
				for each(Autodesk::DesignScript::Geometry::Point^ pDynamoControlPoint in pDynamo1DControlPoints)
				{
					delete pDynamoControlPoint;
				}
			}

			for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamo1DCurveGroup in pDynamoCurveGroups)
			{
				for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurveGroup in pDynamo1DCurveGroup)
				{
					delete pDynamoCurveGroup;
				}
			}

			if (pOuterPolycurve != nullptr)
			{
				delete pOuterPolycurve;
			}

			if (e != nullptr)
			{
				throw e;
			}

			return gcnew Face(pCoreFace);
		}
	}
#else
	NurbsSurface^ Face::Surface(Handle(Geom_BSplineSurface) pOcctBSplineSurface)
	{
		TopologicCore::NurbsSurface::Ptr pCoreNurbsSurface = std::make_shared<TopologicCore::NurbsSurface>(pOcctBSplineSurface, *m_pCoreFace);
		return gcnew NurbsSurface(pCoreNurbsSurface);
	}

	Object^ Face::Surface()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		Handle(Geom_Surface) pOcctSurface = pCoreFace->Surface();

		Handle(Geom_BSplineSurface) pOcctBSplineSurface = Handle_Geom_BSplineSurface::DownCast(pOcctSurface);

		// If not BSpline Surface, create one.
		if (!pOcctBSplineSurface.IsNull())
		{
			return Surface(pOcctBSplineSurface);
		}

		try {
			pOcctBSplineSurface = GeomConvert::SurfaceToBSplineSurface(pOcctSurface);
			return Surface(pOcctBSplineSurface);
		}
		catch (Standard_DomainError)
		{
			// https://www.opencascade.com/doc/occt-7.2.0/refman/html/class_geom_convert.html#aec7d0c9e937cc0bcbe97fba8b3c360bf
			throw gcnew Exception("This surface is not previously defined.");
		}
	}
#endif

	List<Edge^>^ Face::SharedEdges(Face^ face)
	{
		TopologicCore::Face::Ptr pCoreFace1 = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		TopologicCore::Face::Ptr pCoreFace2 = TopologicCore::Topology::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery());
		std::list<TopologicCore::Edge::Ptr> pCoreEdges;
		pCoreFace1->SharedEdges(pCoreFace2, pCoreEdges);

		List<Edge^>^ pSharedEdges = gcnew List<Edge^>();

		for (std::list<TopologicCore::Edge::Ptr>::const_iterator kEdgeIterator = pCoreEdges.begin();
			kEdgeIterator != pCoreEdges.end();
			kEdgeIterator++)
		{
			const TopologicCore::Edge::Ptr& kpCoreEdge = *kEdgeIterator;
			Edge^ pEdge = gcnew Edge(kpCoreEdge);
			pSharedEdges->Add(pEdge);
		}

		return pSharedEdges;
	}

	List<Vertex^>^ Face::SharedVertices(Face^ face)
	{
		TopologicCore::Face::Ptr pCoreFace1 = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		TopologicCore::Face::Ptr pCoreFace2 = TopologicCore::Topology::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery());
		std::list<TopologicCore::Vertex::Ptr> pCoreVertices;
		pCoreFace1->SharedVertices(pCoreFace2, pCoreVertices);

		List<Vertex^>^ pSharedVertices = gcnew List<Vertex^>();

		for (std::list<TopologicCore::Vertex::Ptr>::const_iterator kVertexIterator = pCoreVertices.begin();
			kVertexIterator != pCoreVertices.end();
			kVertexIterator++)
		{
			const TopologicCore::Vertex::Ptr& kpCoreVertex = *kVertexIterator;
			Vertex^ pVertex = gcnew Vertex(kpCoreVertex);
			pSharedVertices->Add(pVertex);
		}

		return pSharedVertices;
	}

	Wire^ Face::ExternalBoundary::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		return gcnew Wire(pCoreFace->ExternalBoundary());
	}

	List<Wire^>^ Face::InternalBoundaries::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Wire::Ptr> pCoreWires;
		pCoreFace->InternalBoundaries(pCoreWires);

		List<Wire^>^ pInternalBoundaries = gcnew List<Wire^>();
		for (std::list<TopologicCore::Wire::Ptr>::const_iterator kWireIterator = pCoreWires.begin();
			kWireIterator != pCoreWires.end();
			kWireIterator++)
		{
			const TopologicCore::Wire::Ptr& pCoreWire = *kWireIterator;
			Wire^ pWire = gcnew Wire(pCoreWire);
			pInternalBoundaries->Add(pWire);
		}
		return pInternalBoundaries;
	}

	Face^ Face::AddInternalBoundaries(List<Wire^>^ internalBoundaries)
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		TopologicCore::Face::Ptr pCoreCopyFace = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Face>(pCoreFace->DeepCopy());

		std::list<TopologicCore::Wire::Ptr> coreWires;
		for each(Wire^ wire in internalBoundaries)
		{
			coreWires.push_back(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Wire>(wire->GetCoreTopologicalQuery()));
		}
		pCoreCopyFace->AddInternalBoundaries(coreWires);

		return gcnew Face(pCoreCopyFace);
	}

	List<Vertex^>^ Face::Vertices::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Vertex::Ptr> pCoreVertices;
		pCoreFace->Vertices(pCoreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopologicCore::Vertex::Ptr>::const_iterator kVertexIterator = pCoreVertices.begin();
			kVertexIterator != pCoreVertices.end();
			kVertexIterator++)
		{
			const TopologicCore::Vertex::Ptr& kpCoreVertex = *kVertexIterator;
			Vertex^ pVertex = gcnew Vertex(kpCoreVertex);
			pVertices->Add(pVertex);
		}
		return pVertices;
	}

	List<Edge^>^ Face::Edges::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Edge::Ptr> pCoreEdges;
		pCoreFace->Edges(pCoreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<TopologicCore::Edge::Ptr>::const_iterator kEdgeIterator = pCoreEdges.begin();
			kEdgeIterator != pCoreEdges.end();
			kEdgeIterator++)
		{
			const TopologicCore::Edge::Ptr& kpCoreEdge = *kEdgeIterator;
			Edge^ pEdge = gcnew Edge(kpCoreEdge);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Wire^>^ Face::Wires::get()
	{
		TopologicCore::Face::Ptr pCoreFace = TopologicCore::Topology::Downcast<TopologicCore::Face>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Wire::Ptr> pCoreWires;
		pCoreFace->Wires(pCoreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<TopologicCore::Wire::Ptr>::const_iterator kWireIterator = pCoreWires.begin();
			kWireIterator != pCoreWires.end();
			kWireIterator++)
		{
			const TopologicCore::Wire::Ptr& kpCoreWire = *kWireIterator;
			Wire^ pWire = gcnew Wire(kpCoreWire);
			pWires->Add(pWire);
		}

		return pWires;
	}

	Object^ Face::BasicGeometry::get()
	{
#ifdef TOPOLOGIC_DYNAMO
		try {
			return Surface();
		}
		catch (Exception^)
		{
			return TriangulatedMesh();
		}
#else
		return Surface();
#endif
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Face::GetCoreTopologicalQuery()
	{
		assert(m_pCoreFace != nullptr && "Face::m_pCoreFace is null.");
		if (m_pCoreFace == nullptr)
		{
			throw gcnew Exception("Face::m_pCoreFace is null.");
		}

		return *m_pCoreFace;
	}

	int Face::Type()
	{
		return TopologicCore::Face::Type();
	}

	Face::Face()
		: Topologic::Topology()
		, m_pCoreFace(nullptr)
	{

	}

	Face::Face(const TopologicCore::Face::Ptr&  kpCoreFace)
		: Topology()
		, m_pCoreFace(kpCoreFace != nullptr ? new TopologicCore::Face::Ptr(kpCoreFace) : throw gcnew Exception("A null face was created."))
	{
		
	}

	Face::~Face()
	{
		delete m_pCoreFace;
	}

	Face^ Face::AddApertureDesign(Face^ apertureDesign, int numEdgeSamples)
	{
		if (numEdgeSamples <= 0)
		{
			throw gcnew Exception("numEdgeSamples must be positive.");
		}
		// 1. Convert the apertures and boundary as faces.
		Wire^ pOuterApertureWire = apertureDesign->ExternalBoundary;
		List<Wire^>^ pApertureWires = apertureDesign->InternalBoundaries;

		List<Topology^>^ pFaces = gcnew List<Topology^>();

		// 2. For each wires, iterate through the edges, sample points, and map them to the 
		for each(Wire^ pApertureWire in pApertureWires)
		{
			List<Edge^>^ pApertureEdges = pApertureWire->Edges;
			List<Edge^>^ pMappedApertureEdges = gcnew List<Edge^>();

			for each(Edge^ pApertureEdge in pApertureEdges)
			{
				List<Vertex^>^ pMappedSampleVertices = gcnew List<Vertex^>();
				for (int i = 0; i < numEdgeSamples; ++i)
				{
					double t = (double)i / (double)(numEdgeSamples - 1);
					if (t < 0.0)
					{
						t = 0.0;
					}
					else if (t > 1.0)
					{
						t = 1.0;
					}

					// Find the actual point on the edge
					Vertex^ pSampleVertex = Topologic::Utilities::EdgeUtility::VertexAtParameter(pApertureEdge, t);

					// Find the UV-coordinate of the point on the aperture design
					List<double>^ uv = Topologic::Utilities::FaceUtility::ParametersAtVertex(apertureDesign, pSampleVertex);
					assert(uv->Count == 2);
					double checkedU = uv[0], checkedV = uv[1];
					if (checkedU < 0.0)
					{
						checkedU = 0.0;
					}
					else if (checkedU > 1.0)
					{
						checkedU = 1.0;
					}

					if (checkedV < 0.0)
					{
						checkedV = 0.0;
					}
					else if (checkedV > 1.0)
					{
						checkedV = 1.0;
					}

					// Find the point with the same UV-coordinate on the surface, add it to the list
					Vertex^ pMappedSampleVertex = Topologic::Utilities::FaceUtility::VertexAtParameters(this, checkedU, checkedV);
					pMappedSampleVertices->Add(pMappedSampleVertex);
				}

				// Interpolate the mapped vertices to an edge.
				Edge^ pMappedApertureEdge = Topologic::Utilities::EdgeUtility::ByVertices(pMappedSampleVertices);
				pMappedApertureEdges->Add(pMappedApertureEdge);
			}

			// Connect the mapped edges to a wire
			Wire^ pMappedApertureWire = Wire::ByEdges(pMappedApertureEdges);

			//// Use the wire to make a face on the same supporting surface as the input face's
			Face^ pMappedApertureFace = Topologic::Utilities::FaceUtility::TrimByWire(this, pMappedApertureWire, false);
			pFaces->Add(pMappedApertureFace);

			// and attach it as an aperture to the face.
			/*Context^ pFaceContext = Context::ByTopologyParameters(face, 0.0, 0.0, 0.0);
			Aperture^ pMappedAperture = Aperture::ByTopologyContext(pMappedApertureFace, pFaceContext);*/
		}
		Face^ pCopyFace = safe_cast<Face^>(AddApertures(pFaces));

		return pCopyFace;
	}
}