#include <Face.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Shell.h>
#include <Cell.h>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TopoDS.hxx>

#include <algorithm>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Face::AdjacentFaces(Face^ topoLogicFace)
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(topoLogicFace->GetCoreTopology());
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

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Faces", pAdjacentFaces);
		pDictionary->Add("Surfaces", pDynamoAdjacentFaces);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::Apertures(Face ^ topoLogicFace)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Dictionary<String^, Object^>^ Face::Cells(Face ^ topoLogicFace)
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(topoLogicFace->GetCoreTopology());
		std::list<TopoLogicCore::Cell*> pCoreCells;
		pCoreFace->Cells(pCoreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		List<System::Object^>^ pDynamoSolids = gcnew List<System::Object^>();

		for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = pCoreCells.begin();
			kCellIterator != pCoreCells.end();
			kCellIterator++)
		{
			TopoLogicCore::Cell* pCoreCell = *kCellIterator;
			Cell^ pCell = gcnew Cell(pCoreCell);
			pCells->Add(pCell);
			pDynamoSolids->Add(pCell->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cells", pCells);
		pDictionary->Add("Solids", pDynamoSolids);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::Edges(Face ^ topoLogicFace)
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(topoLogicFace->GetCoreTopology());
		std::list<TopoLogicCore::Edge*> pCoreEdges;
		pCoreFace->Edges(pCoreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		List<System::Object^>^ pDynamoCurves = gcnew List<System::Object^>();

		for (std::list<TopoLogicCore::Edge*>::const_iterator kEdgeIterator = pCoreEdges.begin();
			kEdgeIterator != pCoreEdges.end();
			kEdgeIterator++)
		{
			TopoLogicCore::Edge* pCoreEdge = *kEdgeIterator;
			Edge^ pEdge = gcnew Edge(pCoreEdge);
			pEdges->Add(pEdge);
			pDynamoCurves->Add(pEdge->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Edges", pEdges);
		pDictionary->Add("Curves", pDynamoCurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::Shells(Face ^ topoLogicFace)
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(topoLogicFace->GetCoreTopology());
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

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Shells", pShells);
		pDictionary->Add("PolySurfaces", pDynamoPolysurfaces);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::Vertices(Face ^ topoLogicFace)
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(topoLogicFace->GetCoreTopology());
		std::list<TopoLogicCore::Vertex*> pCoreVertices;
		pCoreFace->Vertices(pCoreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		List<System::Object^>^ pDynamoPoints = gcnew List<System::Object^>();

		for (std::list<TopoLogicCore::Vertex*>::const_iterator kVertexIterator = pCoreVertices.begin();
			kVertexIterator != pCoreVertices.end();
			kVertexIterator++)
		{
			TopoLogicCore::Vertex* pCoreVertex = *kVertexIterator;
			Vertex^ pVertex = gcnew Vertex(pCoreVertex);
			pVertices->Add(pVertex);
			pDynamoPoints->Add(pVertex->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertices", pVertices);
		pDictionary->Add("Points", pDynamoPoints);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::Wires(Face ^ topoLogicFace)
	{
		TopoLogicCore::Face* pCoreFace = TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(topoLogicFace->GetCoreTopology());
		std::list<TopoLogicCore::Wire*> pCoreWires;
		pCoreFace->Wires(pCoreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		List<System::Object^>^ pDynamoPolycurves = gcnew List<System::Object^>();

		for (std::list<TopoLogicCore::Wire*>::const_iterator kWireIterator = pCoreWires.begin();
			kWireIterator != pCoreWires.end();
			kWireIterator++)
		{
			TopoLogicCore::Wire* pCoreWire = *kWireIterator;
			Wire^ pWire = gcnew Wire(pCoreWire);
			pWires->Add(pWire);
			pDynamoPolycurves->Add(pWire->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Wires", pWires);
		pDictionary->Add("PolyCurves", pDynamoPolycurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::ApplyApertures(Face ^ topoLogicFace, List<Face^>^ apertures)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Dictionary<String^, Object^>^ Face::ByClosedWire(Wire^ wire)
	{
		Face^ pFace = gcnew Face(wire);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Face", pFace);
		pDictionary->Add("Surface", pFace->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::ByEdges(List<Edge^>^ edges)
	{
		return ByClosedWire(gcnew Wire(edges));
	}

	Dictionary<String^, Object^>^ Face::BySurface(Autodesk::DesignScript::Geometry::Surface^ surface)
	{
		Face^ pFace = gcnew Face(surface);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Face", pFace);
		pDictionary->Add("Surface", pFace->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Face::SharedEdges(Face ^ face1, Face ^ face2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Dictionary<String^, Object^>^ Face::SharedVertices(Face ^ face1, Face ^ face2)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Face::IsApplied::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Object^ Face::Geometry::get()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	TopoLogicCore::Topology * Face::GetCoreTopology()
	{
		return nullptr;
	}
	Face::Face(TopoLogicCore::Face* const kpCoreFace)
	{
		throw gcnew System::NotImplementedException();
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