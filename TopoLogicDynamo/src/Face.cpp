#include "Face.h"
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Shell.h>
#include <Cell.h>

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
#include <GeomPlate_Surface.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeFix_Edge.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include <algorithm>
#include <assert.h>

namespace TopoLogic
{
	List<Face^>^ Face::AdjacentFaces()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		std::list<TopoLogicCore::Face*> pAdjacentCoreFaces;
		pCoreFace->AdjacentFaces(pAdjacentCoreFaces);

		List<Face^>^ pAdjacentFaces = gcnew List<Face^>();
		List<System::Object^>^ pDynamoAdjacentFaces = gcnew List<System::Object^>();

		for(std::list<TopoLogicCore::Face*>::const_iterator kAdjacentFaceIterator = pAdjacentCoreFaces.begin();
			kAdjacentFaceIterator != pAdjacentCoreFaces.end();
			kAdjacentFaceIterator++)
		{
			TopoLogicCore::Face* pCoreFace = *kAdjacentFaceIterator;
			Face^ pFace = gcnew Face(pCoreFace);
			pAdjacentFaces->Add(pFace);
			pDynamoAdjacentFaces->Add(pFace->Geometry);
		}

		return pAdjacentFaces;
	}

	List<Face^>^ Face::Apertures()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	List<Cell^>^ Face::Cells()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		std::list<TopoLogicCore::Cell*> pCoreCells;
		pCoreFace->Cells(pCoreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = pCoreCells.begin();
			kCellIterator != pCoreCells.end();
			kCellIterator++)
		{
			TopoLogicCore::Cell* pCoreCell = *kCellIterator;
			Cell^ pCell = gcnew Cell(pCoreCell);
			pCells->Add(pCell);
		}

		return pCells;
	}

	List<Shell^>^ Face::Shells()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		std::list<TopoLogicCore::Shell*> pCoreShells;
		pCoreFace->Shells(pCoreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();
		List<System::Object^>^ pDynamoPolysurfaces = gcnew List<System::Object^>();

		for (std::list<TopoLogicCore::Shell*>::const_iterator kShellIterator = pCoreShells.begin();
			kShellIterator != pCoreShells.end();
			kShellIterator++)
		{
			TopoLogicCore::Shell* pCoreShell = *kShellIterator;
			Shell^ pShell = gcnew Shell(pCoreShell);
			pShells->Add(pShell);
			pDynamoPolysurfaces->Add(pShell->Geometry);
		}

		return pShells;
	}

	Face^ Face::ApplyApertures(List<Face^>^ apertures)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Face^ Face::ByClosedWire(Wire^ wire)
	{
		return gcnew Face(wire);
	}

	Face^ Face::ByEdges(List<Edge^>^ edges)
	{
		return ByClosedWire(gcnew Wire(edges));
	}

	Face^ Face::BySurface(Autodesk::DesignScript::Geometry::Surface^ surface)
	{
		return gcnew Face(surface);
	}

	List<Edge^>^ Face::SharedEdges(Face^ face)
	{
		TopoLogicCore::Face* pCoreFace1 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		TopoLogicCore::Face* pCoreFace2 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(face->GetCoreTopology());
		std::list<TopoLogicCore::Edge*> pCoreEdges;
		pCoreFace1->SharedEdges(pCoreFace2, pCoreEdges);

		List<Edge^>^ pSharedEdges = gcnew List<Edge^>();

		for (std::list<TopoLogicCore::Edge*>::const_iterator kEdgeIterator = pCoreEdges.begin();
			kEdgeIterator != pCoreEdges.end();
			kEdgeIterator++)
		{
			TopoLogicCore::Edge* pCoreEdge = *kEdgeIterator;
			Edge^ pEdge = gcnew Edge(pCoreEdge);
			pSharedEdges->Add(pEdge);
		}

		return pSharedEdges;
	}

	List<Vertex^>^ Face::SharedVertices(Face^ face)
	{
		TopoLogicCore::Face* pCoreFace1 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		TopoLogicCore::Face* pCoreFace2 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(face->GetCoreTopology());
		std::list<TopoLogicCore::Vertex*> pCoreVertices;
		pCoreFace1->SharedVertices(pCoreFace2, pCoreVertices);

		List<Vertex^>^ pSharedVertices = gcnew List<Vertex^>();

		for (std::list<TopoLogicCore::Vertex*>::const_iterator kVertexIterator = pCoreVertices.begin();
			kVertexIterator != pCoreVertices.end();
			kVertexIterator++)
		{
			TopoLogicCore::Vertex* pCoreVertex = *kVertexIterator;
			Vertex^ pVertex = gcnew Vertex(pCoreVertex);
			pSharedVertices->Add(pVertex);
		}

		return pSharedVertices;
	}

	Wire^ Face::OuterWire()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		return gcnew Wire(pCoreFace->OuterWire());
	}

	List<Vertex^>^ Face::Vertices()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		std::list<TopoLogicCore::Vertex*> pCoreVertices;
		pCoreFace->Vertices(pCoreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopoLogicCore::Vertex*>::const_iterator kVertexIterator = pCoreVertices.begin();
			kVertexIterator != pCoreVertices.end();
			kVertexIterator++)
		{
			TopoLogicCore::Vertex* pCoreVertex = *kVertexIterator;
			Vertex^ pVertex = gcnew Vertex(pCoreVertex);
			pVertices->Add(pVertex);
		}
		return pVertices;
	}

	List<Edge^>^ Face::Edges()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		std::list<TopoLogicCore::Edge*> pCoreEdges;
		pCoreFace->Edges(pCoreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<TopoLogicCore::Edge*>::const_iterator kEdgeIterator = pCoreEdges.begin();
			kEdgeIterator != pCoreEdges.end();
			kEdgeIterator++)
		{
			TopoLogicCore::Edge* pCoreEdge = *kEdgeIterator;
			Edge^ pEdge = gcnew Edge(pCoreEdge);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Wire^>^ Face::Wires()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
		std::list<TopoLogicCore::Wire*> pCoreWires;
		pCoreFace->Wires(pCoreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<TopoLogicCore::Wire*>::const_iterator kWireIterator = pCoreWires.begin();
			kWireIterator != pCoreWires.end();
			kWireIterator++)
		{
			TopoLogicCore::Wire* pCoreWire = *kWireIterator;
			Wire^ pWire = gcnew Wire(pCoreWire);
			pWires->Add(pWire);
		}

		return pWires;
	}

	bool Face::IsApplied::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Object^ Face::Geometry::get()
	{
		try {
			return Surface();
		}
		catch (ApplicationException^)
		{
			return TriangulatedMesh();
		}
	}

	TopoLogicCore::Topology* Face::GetCoreTopology()
	{
		assert(m_pCoreFace != nullptr && "Face::m_pCoreFace is null.");
		if (m_pCoreFace == nullptr)
		{
			throw gcnew Exception("Face::m_pCoreFace is null.");
		}

		return m_pCoreFace;
	}

	Face::Face(TopoLogicCore::Face* const kpCoreFace)
		: Topology()
		, m_pCoreFace(kpCoreFace)
	{

	}

	Face::Face(Wire^ pWire)
		: Topology()
		, m_pCoreFace(
			TopoLogicCore::Face::ByClosedWire(
				TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(pWire->GetCoreTopology())
			))
	{
		
	}
	Face::Face(Autodesk::DesignScript::Geometry::Surface ^ pDynamoSurface)
		: Topology()
		, m_pCoreFace(nullptr)
	{
		if (pDynamoSurface->GetType() == Autodesk::DesignScript::Geometry::NurbsSurface::typeid)
		{
			Init(safe_cast<Autodesk::DesignScript::Geometry::NurbsSurface^>(pDynamoSurface), pDynamoSurface->PerimeterCurves());
		}
		else if (pDynamoSurface->GetType() == Autodesk::DesignScript::Geometry::PolySurface::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else if (pDynamoSurface->GetType() == Autodesk::DesignScript::Geometry::Surface::typeid) // a generic surface
		{
			Init(pDynamoSurface->ToNurbsSurface(), pDynamoSurface->PerimeterCurves());
		}
		else
		{
			throw gcnew ArgumentException("The argument is not a valid Dynamo surface.");
		}
	}

	Autodesk::DesignScript::Geometry::Surface^ Face::Surface()
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(GetCoreTopology());
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
			TopoDS_Face occtFace = TopoDS::Face(*pCoreFace->GetOcctShape());
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

			// The newly created surface corresponds to the whole surface in OCCT. 
			// It needs to be trimmed by the outer wire and inner wires.
			List<Autodesk::DesignScript::Geometry::PolyCurve^>^ pDynamoEdgeLoops = gcnew List<Autodesk::DesignScript::Geometry::PolyCurve^>();

			List<Wire^>^ pWires = Wires();
			for each(Wire^ pWire in pWires)
			{
				pDynamoEdgeLoops->Add(safe_cast<Autodesk::DesignScript::Geometry::PolyCurve^>(pWire->Geometry));
			}

			try {
				Autodesk::DesignScript::Geometry::Surface^ pDynamoTrimmedSurfaceByParameters = 
					pDynamoUntrimmedSurface->TrimWithEdgeLoops(pDynamoEdgeLoops);
				return pDynamoTrimmedSurfaceByParameters;
			}
			catch (std::exception& e)
			{
				String^ str = gcnew String(e.what());
				return pDynamoUntrimmedSurface;
			}
			catch (Exception^ e)
			{
				String^ str(e->Message);
				return pDynamoUntrimmedSurface;
			}
			catch (...)
			{
				return pDynamoUntrimmedSurface;
			}
		}

		Handle(Geom_RectangularTrimmedSurface) pOcctRectangularTrimmedSurface = Handle_Geom_RectangularTrimmedSurface::DownCast(pOcctSurface);
		if (!pOcctRectangularTrimmedSurface.IsNull())
		{
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
			List<Wire^>^ pWires = Wires();
			List<Edge^>^ pEdges = Edges();

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
					pDynamoSurface = Autodesk::DesignScript::Geometry::Surface::ByPatch(
						Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoCurves)
					);
				}
				return pDynamoSurface;
			}

			// Otherwise, identify the outer wire.
			// Create a surface from this wire
			// Trim the surface with all the wires (including the outer wire)

			Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface = nullptr;

			List<Autodesk::DesignScript::Geometry::PolyCurve^>^ pDynamoEdgeLoops = gcnew List<Autodesk::DesignScript::Geometry::PolyCurve^>();
			const TopoDS_Wire& rkOcctOuterWire = ShapeAnalysis::OuterWire(TopoDS::Face(*pCoreFace->GetOcctShape()));

			for each(Wire^ pWire in pWires)
			{
				if (pDynamoSurface == nullptr)
				{
					if (pWire->GetCoreTopology()->GetOcctShape()->IsSame(rkOcctOuterWire))
					{
						List<Edge^>^ pOuterEdges = pWire->Edges();
						List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoOuterCurves = gcnew List<Autodesk::DesignScript::Geometry::Curve^>();
						for each(Edge^ pOuterEdge in pOuterEdges)
						{
							// 2. Convert the edges to Dynamo curves
							pDynamoOuterCurves->Add(pOuterEdge->Curve());
						}
						pDynamoSurface = Autodesk::DesignScript::Geometry::Surface::ByPatch(
							Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoOuterCurves)
						);
					}
				}

				Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve = 
					safe_cast<Autodesk::DesignScript::Geometry::PolyCurve^>(pWire->Geometry);
				if (pDynamoPolycurve != nullptr)
				{
					pDynamoEdgeLoops->Add(pDynamoPolycurve);
				}
			}

			// this may raise exception
			try{
				Autodesk::DesignScript::Geometry::Surface^ pTrimmedDynamoSurface = pDynamoSurface->TrimWithEdgeLoops(pDynamoEdgeLoops);
				return pTrimmedDynamoSurface;
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

		List<Vertex^>^ pVertices = Vertices();
		List<Autodesk::DesignScript::Geometry::Point^>^ pDynamoPoints = gcnew List<Autodesk::DesignScript::Geometry::Point^>();
		for each(Vertex^ pVertex in pVertices)
		{
			pDynamoPoints->Add(safe_cast<Autodesk::DesignScript::Geometry::Point^>(pVertex->Geometry));
		}

		return Autodesk::DesignScript::Geometry::Surface::ByPerimeterPoints(pDynamoPoints);
	}

	Autodesk::DesignScript::Geometry::Mesh^ Face::TriangulatedMesh()
	{
		TopoDS_Face occtFace = TopoDS::Face(*GetCoreTopology()->GetOcctShape());
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

		return Autodesk::DesignScript::Geometry::Mesh::ByPointsFaceIndices(pDynamoPoints, pDynamoTriangleIndices);
	}

	Face::~Face()
	{
		delete m_pCoreFace;
	}

	void Face::Init(Autodesk::DesignScript::Geometry::NurbsSurface^ pDynamoNurbsSurface,
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
		List<List<Autodesk::DesignScript::Geometry::Curve^>^>^ pDynamoListOfConnectedCurves = gcnew List<List<Autodesk::DesignScript::Geometry::Curve^>^>();
		for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoPerimeterCurve in pDynamoPerimeterCurves)
		{
			// Set this flag to true if a polycurve can be created. But, don't store the polycurve yet. 
			// The final ones will be created at the end.
			bool isConnectedToAnotherCurve = false;
			for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoConnectedCurves in pDynamoListOfConnectedCurves)
			{
				pDynamoConnectedCurves->Add(pDynamoPerimeterCurve);
				try {
					Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoUpdatedPolycurve =
						Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoConnectedCurves);
					isConnectedToAnotherCurve = true;
					break;
				}
				catch (...)
				{
					pDynamoConnectedCurves->Remove(pDynamoPerimeterCurve);
				}
			}

			if (isConnectedToAnotherCurve)
			{
				continue;
			}

			// not yet added to any polycurve, create one.
			List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoNewConnectedCurves = gcnew List<Autodesk::DesignScript::Geometry::Curve^>();
			pDynamoNewConnectedCurves->Add(pDynamoPerimeterCurve);
			pDynamoListOfConnectedCurves->Add(pDynamoNewConnectedCurves);
		}

		// Get the outer polycurve and discard it from the pDynamoListOfConnectedCurves
		std::list<double> surfaceAreas;
		for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoConnectedCurves in pDynamoListOfConnectedCurves)
		{
			try {
				Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolyCurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoConnectedCurves);
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
		}

		TopoLogicCore::Wire* pOuterCoreWire = nullptr;
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
			pOuterPolycurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoListOfConnectedCurves[index]);
			pOuterCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>((gcnew Wire(pOuterPolycurve))->GetCoreTopology());
			pDynamoListOfConnectedCurves->RemoveAt(index);
		}

		// 3. Bounding wires
		std::list<TopoLogicCore::Wire*> coreWires;

		// Create wires and add them to occtMakeFace to create internal wires;
		for each(List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoConnectedCurves in pDynamoListOfConnectedCurves)
		{
			Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoConnectedCurves);
			Wire^ pWire = gcnew Wire(pDynamoPolycurve);
			coreWires.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(pWire->GetCoreTopology()));
		}
		
		//=================================================
		m_pCoreFace = TopoLogicCore::Face::BySurface(
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
			pOuterCoreWire,
			coreWires
		);
	}
}