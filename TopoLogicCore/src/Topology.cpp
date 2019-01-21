#include "Topology.h"
#include "Cluster.h"
#include "CellComplex.h"
#include "Cell.h"
#include "Shell.h"
#include "Face.h"
#include "Wire.h"
#include "Edge.h"
#include "Vertex.h"
#include "Aperture.h"
#include "Context.h"
#include "ContentManager.h"
#include "ContextManager.h"
#include "InstanceGUIDManager.h"
#include "TopologyFactory.h"
#include "TopologyFactoryManager.h"
#include "GlobalCluster.h"
#include "Bitwise.h"

#include <BOPAlgo_MakerVolume.hxx>
#include <BRep_Builder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepCheck_Wire.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepTools.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>

#include <array>

#include <assert.h>

namespace TopologicCore
{
	int Topology::m_numOfTopologies = 0;

	void AddOcctListShapeToAnotherList(const BOPCol_ListOfShape& rkAList, BOPCol_ListOfShape& rAnotherList)
	{
		for (BOPCol_ListIteratorOfListOfShape kIterator(rkAList);
			kIterator.More();
			kIterator.Next())
		{
			rAnotherList.Append(kIterator.Value());
		}
	}

	Topology::Ptr Topology::ClosestSimplestSubshape(const Topology::Ptr& kpTopology) const
	{
		//TopTools_DataMapOfShapeInteger occtShapeToDistanceMap;
		TopoDS_Shape occtClosestSubshape;
		double minDistance = std::numeric_limits<double>::max();
		const TopoDS_Shape& kOcctThisShape = GetOcctShape();
		const TopoDS_Shape& kOcctQueryShape = kpTopology->GetOcctShape();
		TopAbs_ShapeEnum occtShapeTypes[4] = { TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE, TopAbs_SOLID };
		for (int i = 0; i < 4; ++i)
		{
			TopAbs_ShapeEnum occtShapeType = occtShapeTypes[i];
			TopTools_MapOfShape occtCells;
			for (TopExp_Explorer occtExplorer(kOcctThisShape, occtShapeType); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape rkCurrentChildShape = occtExplorer.Current();
				TopoDS_Shape checkDistanceShape = rkCurrentChildShape;
				if (i == 3)
				{
					ShapeFix_Solid occtSolidFix(TopoDS::Solid(rkCurrentChildShape));
					occtSolidFix.Perform();
					checkDistanceShape = occtSolidFix.Shape();
				}
				BRepExtrema_DistShapeShape occtDistanceCalculation(checkDistanceShape, kOcctQueryShape);
				bool isDone = occtDistanceCalculation.Perform();
				if (isDone)
				{
					double distance = occtDistanceCalculation.Value();
					if (distance < minDistance)
					{
						minDistance = distance;
						occtClosestSubshape = rkCurrentChildShape;
					}
					else if (minDistance <= distance && 
						     distance <= minDistance + Precision::Confusion() && 
							 rkCurrentChildShape.ShapeType() > occtClosestSubshape.ShapeType()) // larger value = lower dimension
					{
						TopAbs_ShapeEnum closestShapeType = occtClosestSubshape.ShapeType();
						TopAbs_ShapeEnum currentShapeType = rkCurrentChildShape.ShapeType();

						minDistance = distance;
						occtClosestSubshape = rkCurrentChildShape;
					}
				}
			}
		}

		if (occtClosestSubshape.IsNull())
		{
			return nullptr;
		}

		return Topology::ByOcctShape(occtClosestSubshape, "");
	}

	Topology::Ptr Topology::SelectSubtopology(const Topology::Ptr & kpSelectorTopology, const int kTypeFilter) const
	{
		//TopTools_DataMapOfShapeInteger occtShapeToDistanceMap;
		TopoDS_Shape occtClosestSubshape;
		double minDistance = std::numeric_limits<double>::max();
		const TopoDS_Shape& kOcctThisShape = GetOcctShape();
		const TopoDS_Shape& kOcctSelectorShape = kpSelectorTopology->GetOcctShape();
		TopAbs_ShapeEnum occtShapeTypes[4] = { TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE, TopAbs_SOLID };
		TopologyType shapeTypes[4] = { TOPOLOGY_VERTEX, TOPOLOGY_EDGE, TOPOLOGY_FACE, TOPOLOGY_CELL };
		for (int i = 0; i < 4; ++i)
		{
			// If this is not the requested topology type, skip.
			int result = Bitwise::And(kTypeFilter, shapeTypes[i]);
			if (Bitwise::And(kTypeFilter, shapeTypes[i]) == 0)
			{
				continue;
			}
			
			TopAbs_ShapeEnum occtShapeType = occtShapeTypes[i];
			TopTools_MapOfShape occtCells;
			for (TopExp_Explorer occtExplorer(kOcctThisShape, occtShapeType); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape rkCurrentChildShape = occtExplorer.Current();
				TopoDS_Shape checkDistanceShape = rkCurrentChildShape;
				if (i == 3)
				{
					ShapeFix_Solid occtSolidFix(TopoDS::Solid(rkCurrentChildShape));
					occtSolidFix.Perform();
					checkDistanceShape = occtSolidFix.Shape();
				}
				BRepExtrema_DistShapeShape occtDistanceCalculation(checkDistanceShape, kOcctSelectorShape);
				bool isDone = occtDistanceCalculation.Perform();
				if (isDone)
				{
					double distance = occtDistanceCalculation.Value();
					if (distance < minDistance)
					{
						minDistance = distance;
						occtClosestSubshape = rkCurrentChildShape;
					}
					else if (minDistance <= distance &&
						distance <= minDistance + Precision::Confusion() &&
						rkCurrentChildShape.ShapeType() > occtClosestSubshape.ShapeType()) // larger value = lower dimension
					{
						TopAbs_ShapeEnum closestShapeType = occtClosestSubshape.ShapeType();
						TopAbs_ShapeEnum currentShapeType = rkCurrentChildShape.ShapeType();

						minDistance = distance;
						occtClosestSubshape = rkCurrentChildShape;
					}
				}
			}
		}

		if (occtClosestSubshape.IsNull())
		{
			return nullptr;
		}

		return Topology::ByOcctShape(occtClosestSubshape, "");
	}

	TopologyType Topology::GetTopologyType(const TopAbs_ShapeEnum & rkOcctType)
	{
		switch (rkOcctType)
		{
		case TopAbs_VERTEX: return TOPOLOGY_VERTEX;
		case TopAbs_EDGE: return TOPOLOGY_EDGE;
		case TopAbs_WIRE: return TOPOLOGY_WIRE;
		case TopAbs_FACE: return TOPOLOGY_FACE;
		case TopAbs_SHELL: return TOPOLOGY_SHELL;
		case TopAbs_SOLID: return TOPOLOGY_CELL;
		case TopAbs_COMPSOLID: return TOPOLOGY_CELLCOMPLEX;
		case TopAbs_COMPOUND: return TOPOLOGY_CLUSTER;
		default:
			throw std::exception("Unrecognised topology");
		}
	}

	Topology::Topology(const int kDimensionality, const TopoDS_Shape& rkOcctShape, const std::string& rkGuid)
		: m_dimensionality(kDimensionality)
	{
		// If no guid is given, use the default class GUID in TopologicCore classes.
		InstanceGUIDManager::GetInstance().Add(rkOcctShape, rkGuid);
		m_numOfTopologies++;
	}

	void Topology::RegisterFactory(const std::string & rkGuid, const TopologyFactory::Ptr & kpTopologyFactory)
	{
		TopologyFactoryManager::GetInstance().Add(rkGuid, kpTopologyFactory);
	}

	Topology::Ptr Topology::ByOcctShape(const TopoDS_Shape& rkOcctShape, const std::string& rkInstanceGuid)
	{
		if (rkOcctShape.IsNull())
		{
			return nullptr;
		}
		// If there is no subtopologies, return null.
		BOPCol_ListOfShape occtSubTopologies;
		SubTopologies(rkOcctShape, occtSubTopologies);
		if (rkOcctShape.ShapeType() != TopAbs_VERTEX && occtSubTopologies.Size() == 0)
		{
			return nullptr;
		}

		TopologyFactory::Ptr pTopologyFactory = nullptr;
		if (rkInstanceGuid.compare("") == 0)
		{
			pTopologyFactory = TopologyFactoryManager::GetDefaultFactory(rkOcctShape.ShapeType());
		}
		else
		{
			TopologyFactoryManager::GetInstance().Find(rkInstanceGuid, pTopologyFactory);
		}
		assert(pTopologyFactory != nullptr);
		Topology::Ptr pTopology = pTopologyFactory->Create(rkOcctShape);

		return pTopology;
	}

	TopoDS_CompSolid Topology::MakeCompSolid(const TopoDS_Shape& rkOcctShape)
	{
		TopoDS_CompSolid occtCompSolid;
		BRep_Builder occtBuilder;
		occtBuilder.MakeCompSolid(occtCompSolid);

		TopTools_MapOfShape occtCells;
		for (TopExp_Explorer occtExplorer(rkOcctShape, TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtCells.Contains(occtCurrent))
			{
				occtCells.Add(occtCurrent);
				try {
					occtBuilder.Add(occtCompSolid, occtCurrent);
				}
				catch (TopoDS_FrozenShape&)
				{
					throw std::exception("The cell is not free and cannot be modified.");
				}
				catch (TopoDS_UnCompatibleShapes&)
				{
					throw std::exception("The cell and face are not compatible.");
				}
			}
		}

		return occtCompSolid;
	}

	Topology::~Topology()
	{
		m_numOfTopologies--;
		if (m_numOfTopologies < 0)
		{
			m_numOfTopologies = 0;
		}
		if (m_numOfTopologies == 0)
		{
			GlobalCluster::GetInstance().Clear();
		}
	}

	Topology::Ptr Topology::ByGeometry(Handle(Geom_Geometry) pGeometry)
	{
		return nullptr;
	}

	Topology::Ptr Topology::ByContext(const std::shared_ptr<Context>& kpContext)
	{
		return nullptr;
	}

	/*Topology::Ptr Topology::ByVertexIndex(const std::list<std::array<double, 3>>& rkVertexCoordinates, const std::list<std::list<int>>& rkVertexIndices)
	{
		return nullptr;
	}*/

	void Topology::ByVertexIndex(const std::vector<Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkVertexIndices, std::list<Topology::Ptr>& rTopologies)
	{
		if (rkVertices.empty() || rkVertexIndices.empty())
		{
			return;
		}

		for (const std::list<int>& rkVertex1DIndices : rkVertexIndices)
		{
			if (rkVertex1DIndices.empty())
				continue;

			std::list<TopoDS_Vertex> occtVertices;
			for (int vertexIndex : rkVertex1DIndices)
			{
				//occtVertices.push_back(copyVertices[vertexIndex]->GetOcctVertex());
				occtVertices.push_back(rkVertices[vertexIndex]->GetOcctVertex());
			}

			if (occtVertices.size() > 2)
			{
				//// Always assume it tries to build a face, so add the first index again.
				//occtVertices.push_back(rkVertices[*rkVertex1DIndices.begin()]->GetOcctVertex());

				// Create edges
				TopTools_ListOfShape occtEdges;
				std::list<TopoDS_Vertex>::const_iterator kEndVertexIterator = occtVertices.end();
				kEndVertexIterator--;

				for (std::list<TopoDS_Vertex>::const_iterator kVertexIterator = occtVertices.begin();
					kVertexIterator != kEndVertexIterator;
					kVertexIterator++)
				{
					std::list<TopoDS_Vertex>::const_iterator kNextVertexIterator = kVertexIterator;
					kNextVertexIterator++;

					BRepBuilderAPI_MakeEdge occtMakeEdge(TopoDS::Vertex(*kVertexIterator), TopoDS::Vertex(*kNextVertexIterator));
					occtEdges.Append(occtMakeEdge);
				}
				// No need to connect the first and last vertices

				// Creating a face
				BRepBuilderAPI_MakeWire occtMakeWire;
				occtMakeWire.Add(occtEdges);
				if (occtMakeWire.Error() == BRepBuilderAPI_WireDone)
				{
					const TopoDS_Wire& rkOcctWire = occtMakeWire.Wire();

					if (BRepCheck_Wire(rkOcctWire).Closed() == BRepCheck_NoError)
					{
						BRepBuilderAPI_MakeFace occtMakeFace(rkOcctWire);
						if (occtMakeFace.Error() == BRepBuilderAPI_FaceDone)
						{
							rTopologies.push_back(std::make_shared<Face>(occtMakeFace)->DeepCopy());
						}
						else
						{
							// Add the closed wire
							rTopologies.push_back(std::make_shared<Wire>(rkOcctWire)->DeepCopy());
						}
					}
					else
					{
						// Add the open wire
						rTopologies.push_back(std::make_shared<Wire>(rkOcctWire)->DeepCopy());
					}
				}
				else
				{
					// Add the edges
					for (TopTools_ListIteratorOfListOfShape occtEdgeIterator(occtEdges);
						occtEdgeIterator.More();
						occtEdgeIterator.Next())
					{
						rTopologies.push_back(std::make_shared<Edge>(TopoDS::Edge(occtEdgeIterator.Value()))->DeepCopy());
					}
				}
			}
			else if (occtVertices.size() == 2)
			{
				// Try creating an edge
				BRepBuilderAPI_MakeEdge occtMakeEdge(TopoDS::Vertex(occtVertices.front()),
					TopoDS::Vertex(occtVertices.back()));
				rTopologies.push_back(std::make_shared<Edge>(occtMakeEdge)->DeepCopy());
			}
			else if (occtVertices.size() == 1)
			{
				// Insert the vertices
				rTopologies.push_back(std::make_shared<Vertex>(occtVertices.front())->DeepCopy());
			}
		}
	}

	std::shared_ptr<Topology> Topology::ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces, const double kTolerance)
	{
		if (rkFaces.empty())
		{
			throw std::exception("No face is passed.");
		}

		TopTools_ListOfShape occtShapes;
		for (const Face::Ptr& kpFace : rkFaces)
		{
			//Face::Ptr pCopyFace = std::dynamic_pointer_cast<Face>(kpFace->DeepCopy());
			occtShapes.Append(kpFace->GetOcctShape());
		}

		TopoDS_Shape occtShape = OcctSewFaces(occtShapes, kTolerance);
		Topology::Ptr pTopology = Topology::ByOcctShape(occtShape, "");
		Topology::Ptr pCopyTopology = std::dynamic_pointer_cast<Topology>(pTopology->DeepCopy());
		GlobalCluster::GetInstance().AddTopology(pTopology->GetOcctShape());
		return pTopology;
	}

	void Topology::AddContent(const Topology::Ptr & rkTopology)
	{
		bool hasContent = ContentManager::GetInstance().HasContent(GetOcctShape(), rkTopology->GetOcctShape());
		if (hasContent)
		{
			return;
		}

		ContentManager::GetInstance().Add(GetOcctShape(), rkTopology);

		const double kDefaultParameter = 0.0;
		ContextManager::GetInstance().Add(
			rkTopology->GetOcctShape(),
			Context::ByTopologyParameters(Topology::ByOcctShape(GetOcctShape()),
				kDefaultParameter, 
				kDefaultParameter,
				kDefaultParameter)
		);
	}

	void Topology::AddContent(const Topology::Ptr& rkTopology, const int kTypeFilter)
	{
		// 1. Get the center of mass of the content
		Vertex::Ptr pCenterOfMass = rkTopology->CenterOfMass();

		// 2. Find the closest simplest topology of the copy topology
		Topology::Ptr selectedSubtopology = SelectSubtopology(pCenterOfMass, kTypeFilter);
		bool hasContent = ContentManager::GetInstance().HasContent(selectedSubtopology->GetOcctShape(), rkTopology->GetOcctShape());
		if (hasContent)
		{
			return;
		}

		// 3. Register to ContentManager
		ContentManager::GetInstance().Add(selectedSubtopology->GetOcctShape(), rkTopology);

		// 4. Add closestSimplestSubshape as the context of pCoreCopyContentTopology
		const double kDefaultParameter = 0.0; // TODO: calculate the parameters
		ContextManager::GetInstance().Add(
			rkTopology->GetOcctShape(),
			TopologicCore::Context::ByTopologyParameters(
			selectedSubtopology,
			kDefaultParameter, kDefaultParameter, kDefaultParameter
		));
	}

	void Topology::RemoveContent(const Topology::Ptr& rkTopology)
	{
		ContentManager::GetInstance().Remove(GetOcctShape(), rkTopology->GetOcctShape());

		ContextManager::GetInstance().Remove(rkTopology->GetOcctShape(), GetOcctShape());
	}

	void Topology::AddContext(const std::shared_ptr<Context>& rkContext)
	{
		// 1. Get the center of mass of the content
		Vertex::Ptr pCenterOfMass = CenterOfMass();

		// 4. Register to ContextManager
		ContextManager::GetInstance().Add(GetOcctShape(), rkContext);

		// 5. Register to ContentManager
		ContentManager::GetInstance().Add(rkContext->Topology()->GetOcctShape(), Topology::ByOcctShape(GetOcctShape(), GetInstanceGUID()));
	}

	void Topology::RemoveContext(const std::shared_ptr<Context>& rkContext)
	{
		// 1. Remove from ContextManager
		ContextManager::GetInstance().Remove(GetOcctShape(), rkContext->Topology()->GetOcctShape());

		// 2. Remove from ContentManager
		ContentManager::GetInstance().Remove(rkContext->Topology()->GetOcctShape(), GetOcctShape());
	}

	void Topology::SharedTopologies(const Topology::Ptr& kpTopology, std::list<Topology::Ptr>& rkSharedTopologies) const
	{
	}

	void Topology::PathsTo(const Topology::Ptr& kpTopology, const Topology::Ptr& kpParentTopology, const int kMaxLevels, const int kMaxPaths, std::list<std::list<std::shared_ptr<TopologicalQuery>>>& rkPaths) const
	{
		// Use BFS.
		// https://en.wikipedia.org/wiki/Breadth-first_search
		//std::queue<Topology const *> openSetTopology;
		//std::set<Topology const *> closedSetTopology;
		//topologyToVisit.push_back(this);
		//std::list<Topology const *> visitedTopologies;
		//while (!topologyToVisit.empty())
		//{
		//	Topology const * kpCurrentTopology = topologyToVisit.front();
		//	topologyToVisit.pop_front();
		//	visitedTopologies.push_back(kpCurrentTopology);

		//	// Get neighbours
		//	std::list<Topology const *> neighbours; // up and down

		//	DownwardNavigation(neighbours);
		//	UpwardNavigation(kpParentTopology, neighbours);

		//	for(Topology const * kpNeighbour : neighbours)
		//	{
		//		if()
		//	}
		//}
	}

	TopoDS_Shape Topology::OcctSewFaces(const TopTools_ListOfShape & rkOcctFaces, const double kTolerance)
	{
		BRepBuilderAPI_Sewing occtSewing(kTolerance, true, true, true, true);
		for (TopTools_ListIteratorOfListOfShape occtEdgeIterator(rkOcctFaces);
			occtEdgeIterator.More();
			occtEdgeIterator.Next())
		{
			occtSewing.Add(occtEdgeIterator.Value());
		}

		occtSewing.Perform();
		if (occtSewing.SewedShape().IsNull())
		{
			throw std::exception("A null shape is created.");
		}
		TopAbs_ShapeEnum type = occtSewing.SewedShape().ShapeType();

		for (TopTools_ListIteratorOfListOfShape occtEdgeIterator(rkOcctFaces);
			occtEdgeIterator.More();
			occtEdgeIterator.Next())
		{
			const TopoDS_Shape& rkModifiedShape = occtSewing.Modified(occtEdgeIterator.Value());
			Topology::Ptr pChildTopology = Topology::ByOcctShape(rkModifiedShape, "");

			// Map the aperture to the modifed shell faces.
			std::list<Topology::Ptr> contents;
			ContentManager::GetInstance().Find(occtEdgeIterator.Value(), contents);
			//kShellFace->Contents(false, contents);
			for (const Topology::Ptr& rkContent : contents)
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
			}
		}

		return occtSewing.SewedShape();
	}

	Topology::Ptr Topology::Difference(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BOPAlgo_CellsBuilder occtCellsBuilder;
		NonRegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtCellsBuilder);

		// 2. Select the parts to be included in the final result.
		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorB.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		occtCellsBuilder.MakeContainers();

		TopoDS_Shape occtResultShape = occtCellsBuilder.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		TransferContents(GetOcctShape(), pPostprocessedShape);
		TransferContents(kpOtherTopology->GetOcctShape(), pPostprocessedShape);
		GlobalCluster::GetInstance().AddTopology(occtPostprocessedShape);
		return pPostprocessedShape;
	}

	void Topology::Contents(std::list<Topology::Ptr>& rContents) const
	{
		Contents(GetOcctShape(), rContents);
	}

	void Topology::Contents(const TopoDS_Shape & rkOcctShape, std::list<Topology::Ptr>& rContents)
	{
		ContentManager::GetInstance().Find(rkOcctShape, rContents);
	}

	void Topology::SubContents(std::list<Topology::Ptr>& rSubContents) const
	{
		SubContents(GetOcctShape(), rSubContents);
	}
	
	void Topology::SubContents(const TopoDS_Shape & rkOcctShape, std::list<Topology::Ptr>& rSubContents)
	{
		Contents(rkOcctShape, rSubContents);
		TopAbs_ShapeEnum occtType = rkOcctShape.ShapeType();
		int occtTypeInt = (int)occtType + 1; // +1 for the next lower type
		for (int occtTypeIntIteration = occtTypeInt; occtTypeIntIteration != (int)TopAbs_SHAPE; occtTypeIntIteration++)
		{
			// Get members in each level
			TopAbs_ShapeEnum occtTypeIteration = (TopAbs_ShapeEnum)occtTypeIntIteration;
			TopTools_MapOfShape occtMembers;
			DownwardNavigation(rkOcctShape, occtTypeIteration, occtMembers);

			// For each member, get the contents
			for (TopTools_MapIteratorOfMapOfShape occtMemberIterator(occtMembers);
				occtMemberIterator.More();
				occtMemberIterator.Next())
			{
				ContentManager::GetInstance().Find(occtMemberIterator.Value(), rSubContents);
			}
		}
	}

	bool Topology::Contexts(std::list<std::shared_ptr<Context>>& rContexts) const
	{
		return Contexts(GetOcctShape(), rContexts);
	}

	bool Topology::Contexts(const TopoDS_Shape & rkOcctShape, std::list<std::shared_ptr<Context>>& rContexts)
	{
		return ContextManager::GetInstance().Find(rkOcctShape, rContexts);
	}

	bool Topology::SaveToBrep(const std::string & rkPath) const
	{
		return BRepTools::Write(GetOcctShape(), rkPath.c_str());;
	}

	Topology::Ptr Topology::LoadFromBrep(const std::string & rkPath)
	{
		TopoDS_Shape occtShape;
		BRep_Builder occtBRepBuilder;
		bool returnValue = BRepTools::Read(occtShape, rkPath.c_str(), occtBRepBuilder);
		Topology::Ptr pTopology = Topology::ByOcctShape(occtShape, "");

		GlobalCluster::GetInstance().AddTopology(pTopology);
		return pTopology;
	}

	std::string Topology::Analyze(const TopoDS_Shape& rkShape, const int kLevel)
	{
		BOPCol_ListOfShape occtSubTopologies;
		SubTopologies(rkShape, occtSubTopologies);

		std::array<std::string, 8> occtShapeNameSingular;
		occtShapeNameSingular[0] = "a cluster";
		occtShapeNameSingular[1] = "a cellComplex";
		occtShapeNameSingular[2] = "a cell";
		occtShapeNameSingular[3] = "a shell";
		occtShapeNameSingular[4] = "a face";
		occtShapeNameSingular[5] = "a wire";
		occtShapeNameSingular[6] = "an edge";
		occtShapeNameSingular[7] = "a vertex";

		std::array<std::string, 8> occtShapeNamePlural;
		occtShapeNamePlural[0] = "clusters";
		occtShapeNamePlural[1] = "cellComplexes";
		occtShapeNamePlural[2] = "cells";
		occtShapeNamePlural[3] = "shells";
		occtShapeNamePlural[4] = "faces";
		occtShapeNamePlural[5] = "wires";
		occtShapeNamePlural[6] = "edges";
		occtShapeNamePlural[7] = "vertices";

		TopAbs_ShapeEnum occtShapeType = rkShape.ShapeType();
		std::stringstream ssCurrentIndent;
		for (int i = 0; i < kLevel; ++i)
		{
			ssCurrentIndent << "  ";
		}
		std::string currentIndent = ssCurrentIndent.str();
		int numberOfSubentities[8] = { 0,0,0,0,0,0,0,0 };
		for (BOPCol_ListIteratorOfListOfShape kMemberIterator(occtSubTopologies);
			kMemberIterator.More();
			kMemberIterator.Next())
		{
			const TopoDS_Shape& rkMemberTopology = kMemberIterator.Value();
			TopAbs_ShapeEnum occtShapeMemberType = rkMemberTopology.ShapeType();
			numberOfSubentities[occtShapeMemberType]++;
		}

		std::stringstream ssCurrentResult;

		// For the topmost level only, print the overall subentities result
		if (kLevel == 0)
		{
			ShapeAnalysis_ShapeContents occtShapeAnalysis;
			occtShapeAnalysis.Perform(rkShape);

			// No method is provided in ShapeAnalysis_ShapeContents to compute the number of CompSolids.
			// Do this manually.
			int numberCompSolids = 0;
			BOPCol_ListOfShape occtCompSolids;
			for (TopExp_Explorer occtExplorer(rkShape, TopAbs_COMPSOLID); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtExplorer.Current();
				if (!occtCompSolids.Contains(rkOcctCurrent))
				{
					occtCompSolids.Append(rkOcctCurrent);
					numberCompSolids++;
				}
			}
			ssCurrentResult <<
				"OVERALL ANALYSIS" << std::endl <<
				"================" << std::endl <<
				"The shape is " << occtShapeNameSingular[occtShapeType] << "." << std::endl;

			if (occtShapeType == 0) // Only for cluster
			{
				ssCurrentResult << "Number of cell complexes = " << numberCompSolids << std::endl;
			}

			if (occtShapeType <= 1) // Only up to cellcomplex
			{
				ssCurrentResult << "Number of cells = " << occtShapeAnalysis.NbSharedSolids() << std::endl;
			}

			if (occtShapeType <= 2) // Only up to cell
			{
				ssCurrentResult << "Number of shells = " << occtShapeAnalysis.NbSharedShells() << std::endl;
			}

			if (occtShapeType <= 3) // Only up to shell
			{
				ssCurrentResult << "Number of faces = " << occtShapeAnalysis.NbSharedFaces() << std::endl;
			}
			if (occtShapeType <= 4) // Only up to face
			{
				ssCurrentResult << "Number of wires = " << occtShapeAnalysis.NbSharedWires() << std::endl;
			}

			if (occtShapeType <= 5) // Only up to wire
			{
				ssCurrentResult << "Number of edges = " << occtShapeAnalysis.NbSharedEdges() << std::endl;
			}

			if (occtShapeType <= 6) // Only up to shell
			{
				ssCurrentResult << "Number of vertices = " << occtShapeAnalysis.NbSharedVertices() << std::endl;
			}

			ssCurrentResult << std::endl << std::endl <<
				"INDIVIDUAL ANALYSIS" << std::endl <<
				"================" << std::endl;
		}

		ssCurrentResult << currentIndent << "The shape is " << occtShapeNameSingular[occtShapeType] << "." << std::endl;

		for (int i = occtShapeType + 1; i < 8; ++i)
		{
			if(numberOfSubentities[i] > 0)
			{
				ssCurrentResult << currentIndent << "Number of " << occtShapeNamePlural[i] << " = " << numberOfSubentities[i] << std::endl;
			}
		}
		ssCurrentResult << currentIndent << "================" << std::endl;

		for (BOPCol_ListIteratorOfListOfShape kMemberIterator(occtSubTopologies);
			kMemberIterator.More();
			kMemberIterator.Next())
		{
			const TopoDS_Shape& rkMemberTopology = kMemberIterator.Value();
			std::string strMemberAnalyze = Analyze(rkMemberTopology, kLevel + 1);
			ssCurrentResult << strMemberAnalyze;
		}

		return ssCurrentResult.str();
	}

	TopoDS_Shape Topology::Simplify(TopoDS_Shape & rOcctShape)
	{
		// Simplify needs to do the followings.
		// 1. The input is a container type, otherwise return itself.
		// 2. If the input is an empty cluster: return null
		// 3. Else if the input just contains one container element: recursively dive deeper until a non-container element OR
		//    a container with more than one elements is found.
		// 4. Else if the input contains more than elements:
		//    a. For a non-container element: leave it.
		//    b. For a container element: recursively dive deeper until a non-container element OR
		//       a container with more than one elements is found.
		if (!IsContainerType(rOcctShape))
			return rOcctShape;

		BOPCol_ListOfShape occtSubTopologies;
		SubTopologies(rOcctShape, occtSubTopologies);

		if (occtSubTopologies.Size() == 0)
		{
			return TopoDS_Shape();
		}
		else if (occtSubTopologies.Size() == 1)
		{
			TopoDS_Shape occtCurrentShape = rOcctShape;
			BOPCol_ListOfShape occtShapes;
			bool isSimplestShapeFound = false;
			do
			{
				// Only do this for wire, shell, cellcomplex, cluster
				if (!IsContainerType(occtCurrentShape))
				{
					break;
				}

				SubTopologies(occtCurrentShape, occtShapes);

				int numOfSubTopologies = occtShapes.Size();
				if (numOfSubTopologies != 1)
				{
					// occtCurrentShape does not change.
					isSimplestShapeFound = true;
				}
				else // if (occtShapes.Size() == 1)
				{
					// Go deeper
					occtCurrentShape = *occtShapes.begin();
				}
				occtShapes.Clear();
			} while (!isSimplestShapeFound);
			return occtCurrentShape;
		}
		else // occtSubTopologies.Size() > 1
		{
			BOPCol_ListOfShape occtShapesToRemove;
			BOPCol_ListOfShape occtShapesToAdd;

			for (BOPCol_ListIteratorOfListOfShape occtSubTopologyIterator(occtSubTopologies);
				occtSubTopologyIterator.More();
				occtSubTopologyIterator.Next())
			{
				TopoDS_Shape& rOcctSubShape = occtSubTopologyIterator.Value();
				if (!IsContainerType(rOcctSubShape))
				{
					continue;
				}

				TopoDS_Shape occtCurrentShape = rOcctSubShape;
				BOPCol_ListOfShape occtShapes;
				bool isSimplestShapeFound = false;
				do
				{
					// Only do this for wire, shell, cellcomplex, cluster
					if (!IsContainerType(occtCurrentShape))
					{
						break;
					}

					SubTopologies(occtCurrentShape, occtShapes);

					int numOfSubTopologies = occtShapes.Size();
					if (numOfSubTopologies != 1)
					{
						// occtCurrentShape does not change.
						isSimplestShapeFound = true;
					}
					else // if (occtShapes.Size() == 1)
					{
						// Go deeper
						occtCurrentShape = *occtShapes.begin();
					}
					occtShapes.Clear();
				} while (!isSimplestShapeFound);

				if (!occtCurrentShape.IsSame(rOcctSubShape))
				{
					// Do this so as to not modify the list in the iteration.
					occtShapesToRemove.Append(rOcctSubShape);
					occtShapesToAdd.Append(occtCurrentShape);
				}
			}

			for (BOPCol_ListIteratorOfListOfShape occtSubTopologyToRemoveIterator(occtShapesToRemove);
				occtSubTopologyToRemoveIterator.More();
				occtSubTopologyToRemoveIterator.Next())
			{
				TopoDS_Builder occtBuilder;
				try {
					occtBuilder.Remove(rOcctShape, occtSubTopologyToRemoveIterator.Value());
				}
				catch (TopoDS_FrozenShape)
				{
					throw std::exception("Topology is locked, cannot remove subtopology. Please contact the developer.");
				}
			}

			for (BOPCol_ListIteratorOfListOfShape occtSubTopologyToAddIterator(occtShapesToAdd);
				occtSubTopologyToAddIterator.More();
				occtSubTopologyToAddIterator.Next())
			{
				TopoDS_Builder occtBuilder;
				try {
					occtBuilder.Add(rOcctShape, occtSubTopologyToAddIterator.Value());
				}
				catch (TopoDS_FrozenShape)
				{
					throw std::exception("Topology is locked, cannot remove subtopology.");
				}
				catch (TopoDS_UnCompatibleShapes)
				{
					throw std::exception("Cannot add incompatible subtopology.");
				}
			}

			return rOcctShape;
		}
	}

	TopoDS_Shape Topology::BooleanSubTopologyContainment(TopoDS_Shape & rOcctShape)
	{
		// 1. Only for cluster
		// 2. If the input is an empty cluster: return null
		// 3. For each subtopology A, check against each other subtopology B. If A is inside B, remove A.

		if (rOcctShape.ShapeType() != TopAbs_COMPOUND)
		{
			return rOcctShape;
		}

		BOPCol_ListOfShape occtSubTopologies;
		SubTopologies(rOcctShape, occtSubTopologies);

		if (occtSubTopologies.Size() == 0)
		{
			return TopoDS_Shape();
		}

		BOPCol_MapOfShape occtShapesToRemove;
		for (BOPCol_ListIteratorOfListOfShape occtSubTopologyIteratorA(occtSubTopologies);
			occtSubTopologyIteratorA.More();
			occtSubTopologyIteratorA.Next())
		{
			TopoDS_Shape& rOcctSubtopologyA = occtSubTopologyIteratorA.Value();	

			bool isShapeAToRemove = false;
			for (BOPCol_ListIteratorOfListOfShape occtSubTopologyIteratorB(occtSubTopologies);
				occtSubTopologyIteratorB.More() && !isShapeAToRemove;
				occtSubTopologyIteratorB.Next())
			{
				TopoDS_Shape& rOcctSubtopologyB = occtSubTopologyIteratorB.Value();

				if (rOcctSubtopologyA.IsSame(rOcctSubtopologyB))
				{
					continue;
				}

				// Does B contain A?
				TopTools_MapOfShape occtSubTopologiesB;
				DownwardNavigation(rOcctSubtopologyB, rOcctSubtopologyA.ShapeType(), occtSubTopologiesB);
				if (occtSubTopologiesB.Contains(rOcctSubtopologyA))
				{
					isShapeAToRemove = true;
					occtShapesToRemove.Add(rOcctSubtopologyA);
				}
			}
		}
		
		// Remove the shapes
		for (BOPCol_MapIteratorOfMapOfShape occtShapesToRemoveIterator(occtShapesToRemove);
			occtShapesToRemoveIterator.More();
			occtShapesToRemoveIterator.Next())
		{
			TopoDS_Builder occtBuilder;
			try {
				occtBuilder.Remove(rOcctShape, occtShapesToRemoveIterator.Value());
			}
			catch (TopoDS_FrozenShape)
			{
				throw std::exception("Topology is locked, cannot remove subtopology. Please contact the developer.");
			}
		}

		return rOcctShape;
	}

	std::string Topology::Analyze()
	{
		return Analyze(GetOcctShape(), 0);
	}

	void Topology::NonRegularBooleanOperation(
		const Topology::Ptr& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsB,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	{
		AddBooleanOperands(kpOtherTopology, rOcctCellsBuilder, rOcctCellsBuildersOperandsA, rOcctCellsBuildersOperandsB, rOcctMapFaceToFixedFaceA, rOcctMapFaceToFixedFaceB);

		// Split the arguments and tools
		try {
			rOcctCellsBuilder.Perform();
		}
		catch (Standard_Failure& e)
		{
			const char* str = e.GetMessageString();
			std::string stlStr(str);
		}
		catch (std::exception& e)
		{
			const char* str = e.what();
			std::string stlStr(str);
		}


		if (rOcctCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			rOcctCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
	}

	void Topology::NonRegularBooleanOperation(
		const BOPCol_ListOfShape & rkOcctArgumentsA,
		const BOPCol_ListOfShape & rkOcctArgumentsB,
		BOPAlgo_CellsBuilder & rOcctCellsBuilder)
	{
		BOPCol_ListOfShape occtArguments;
		for (BOPCol_ListIteratorOfListOfShape occtArgumentIteratorA(rkOcctArgumentsA);
			occtArgumentIteratorA.More();
			occtArgumentIteratorA.Next())
		{
			occtArguments.Append(occtArgumentIteratorA.Value());
		}
		for (BOPCol_ListIteratorOfListOfShape occtArgumentIteratorB(rkOcctArgumentsB);
			occtArgumentIteratorB.More();
			occtArgumentIteratorB.Next())
		{
			occtArguments.Append(occtArgumentIteratorB.Value());
		}
		rOcctCellsBuilder.SetArguments(occtArguments);
		//rOcctCellsBuilder.SetNonDestructive(false);

		// Split the arguments and tools
		try {
			rOcctCellsBuilder.Perform();
		}
		catch (Standard_Failure&)
		{
		}
		catch (std::exception&)
		{
		}

		if (rOcctCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			rOcctCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
	}

	//std::shared_ptr<Topology> Topology::TransferBooleanContents(
	//	const std::shared_ptr<Topology>& kpAnotherTopology,
	//	BOPAlgo_CellsBuilder& rOcctCellsBuilder,
	//	BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
	//	BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	//{
	//	std::list<Topology::Ptr> thisContents;
	//	SubContents(thisContents);

	//	BOPCol_ListOfShape occtSubTopologies;
	//	SubTopologies(rOcctCellsBuilder.Shape(), occtSubTopologies);
	//	//TopoDS_Shape queryShape = rOcctCellsBuilder.Shape();
	//	TopoDS_Shape queryShape = Simplify(rOcctCellsBuilder.Shape());
	//	queryShape = MakeBooleanContainers(queryShape);
	//	GlobalCluster::GetInstance().AddTopology(queryShape);

	//	Topology::Ptr pBooleanResult = Topology::ByOcctShape(queryShape, "");
	//	// queryShape (pBooleanResult) is the member of the Boolean result, it doesn't have any content yet.

	//	// For now, handle the case where queryShape is a cellComplex and t
	//	if (GetType() != TOPOLOGY_CELL || pBooleanResult->GetType() != TOPOLOGY_CELLCOMPLEX)
	//	{
	//		return pBooleanResult;
	//	}

	//	// For now, only handle a cell topology
	//	Cell* pCell = Topology::Downcast<Cell>(this);

	//	std::list<Face::Ptr> originalFaces;
	//	pCell->Faces(originalFaces);
	//	for (const Face::Ptr& kpOriginalFace : originalFaces)
	//	{
	//		// Get the fixed face
	//		TopoDS_Shape occtFixedFace;
	//		try {
	//			occtFixedFace = rOcctMapFaceToFixedFaceA.Find(kpOriginalFace->GetOcctFace());
	//		}
	//		catch (...)
	//		{
	//			continue;
	//		}

	//		// Get the modified fixed faces
	//		TopTools_ListOfShape occtModifiedFixedFaces = rOcctCellsBuilder.Modified(occtFixedFace);
	//		for (TopTools_ListIteratorOfListOfShape occtModifiedFixedFaceIterator(occtModifiedFixedFaces);
	//			occtModifiedFixedFaceIterator.More();
	//			occtModifiedFixedFaceIterator.Next())
	//		{
	//			const TopoDS_Shape& rkOcctModifiedFixedFace = occtModifiedFixedFaceIterator.Value();
	//			std::shared_ptr<Topology> pModifiedFixedFace = Topology::ByOcctShape(rkOcctModifiedFixedFace, "");

	//			// Transfer the contents from kpOriginalFace to pModifiedFixedFace
	//			std::list<Topology::Ptr> contents;
	//			kpOriginalFace->Contents(contents);
	//			for(const Topology::Ptr& kpTopology : contents)
	//			{
	//				pModifiedFixedFace->AddContent(kpTopology);
	//			}
	//		}
	//	}

	//	return pBooleanResult;
	//}

	void Topology::TransferContents(const TopoDS_Shape& rkOcctShape1, const Topology::Ptr& kpTopology2)
	{
		std::list<Topology::Ptr> subContents;
		SubContents(rkOcctShape1, subContents);

		for (const Topology::Ptr kpSubContent : subContents)
		{
			// Attach to the same context type
			int contextType = 0;
			std::list<Context::Ptr> contexts;
			kpSubContent->Contexts(contexts);
			for (const Context::Ptr& kpContext : contexts)
			{
				Topology::Ptr pContextTopology = kpContext->Topology();
				TopologyType contextTopologyType = pContextTopology->GetType();
				contextType = Bitwise::Or(contextType, contextTopologyType);

				// Remove content from current contexts
				pContextTopology->RemoveContent(kpSubContent);
				kpSubContent->RemoveContext(kpContext);
			}

			kpTopology2->AddContent(kpSubContent, contextType);
		}
	}

	void Topology::TransferContents(const TopoDS_Shape & rkOcctShape1, const Topology::Ptr & kpTopology2, const BOPCol_ListOfShape & rkOcctDeletedSubshapes)
	{
		std::list<Topology::Ptr> subContents;
		SubContents(rkOcctShape1, subContents);

		for (const Topology::Ptr kpSubContent : subContents)
		{
			// Check if the context topology is part of kpTopology2. Use OCCT IsDeleted()
			bool allContextsDisappear = true;
			std::list<Context::Ptr> contexts;
			kpSubContent->Contexts(contexts);
			for (const Context::Ptr& kpContext : contexts)
			{
				if (!rkOcctDeletedSubshapes.Contains(kpContext->Topology()->GetOcctShape()))
				{
					allContextsDisappear = false;
					break;
				}
			}
			if (allContextsDisappear)
			{
				continue;
			}
			
			// Attach to the same context type
			int contextType = 0;
			for (const Context::Ptr& kpContext : contexts)
			{
				Topology::Ptr pContextTopology = kpContext->Topology();
				TopologyType contextTopologyType = pContextTopology->GetType();
				contextType = Bitwise::Or(contextType, contextTopologyType);

				// Remove content from current contexts
				pContextTopology->RemoveContent(kpSubContent);
				kpSubContent->RemoveContext(kpContext);
			}

			kpTopology2->AddContent(kpSubContent, contextType);
		}
	}

	void Topology::RegularBooleanOperation(
		const BOPCol_ListOfShape & rkOcctArgumentsA, 
		const BOPCol_ListOfShape & rkOcctArgumentsB, 
		BRepAlgoAPI_BooleanOperation& rOcctBooleanOperation)
	{
		rOcctBooleanOperation.SetArguments(rkOcctArgumentsA);
		rOcctBooleanOperation.SetTools(rkOcctArgumentsB);
		rOcctBooleanOperation.SetNonDestructive(true);
		rOcctBooleanOperation.Build();
	}

	/*Topology::Ptr Topology::GetBooleanResult(
		const Topology::Ptr& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	{
		rOcctCellsBuilder.MakeContainers();

		return TransferBooleanContents(kpOtherTopology, rOcctCellsBuilder, rOcctMapFaceToFixedFaceA, rOcctMapFaceToFixedFaceB);
	}*/

	TopoDS_Shape Topology::PostprocessBooleanResult(TopoDS_Shape & rOcctBooleanResult)
	{
		TopoDS_Shape occtPostprocessedShape = Simplify(rOcctBooleanResult);
		if (!occtPostprocessedShape.IsNull())
		{
			occtPostprocessedShape = BooleanSubTopologyContainment(occtPostprocessedShape);
		}
		if (!occtPostprocessedShape.IsNull())
		{
			occtPostprocessedShape = Simplify(occtPostprocessedShape);
		}
		//occtPostprocessedShape = MakeBooleanContainers(occtPostprocessedShape);
		return occtPostprocessedShape;
	}

	void Topology::TransferMakeShapeContents(BRepBuilderAPI_MakeShape & rkOcctMakeShape, const std::list<Topology::Ptr>& rkShapes)
	{
		BOPCol_ListOfShape occtShapes;
		for (const Topology::Ptr& kpShape : rkShapes)
		{
			occtShapes.Append(kpShape->GetOcctShape());
		}
		TransferMakeShapeContents(rkOcctMakeShape, occtShapes);
	}

	void Topology::TransferMakeShapeContents(BRepBuilderAPI_MakeShape & rkOcctMakeShape, const BOPCol_ListOfShape & rkOcctShapes)
	{
		// 1. For each shape in rkOcctShapes, find the generated shapes in rkOcctMakeShape
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIterator(rkOcctShapes);
			kOcctShapeIterator.More();
			kOcctShapeIterator.Next())
		{
			const TopoDS_Shape& rkOcctOriginalShape = kOcctShapeIterator.Value();
			Topology::Ptr pOriginalShape = Topology::ByOcctShape(rkOcctOriginalShape, "");
			TopTools_ListOfShape occtGeneratedShapes = rkOcctMakeShape.Modified(rkOcctOriginalShape);

			// 2. Transfer the contents from the original shape to the generated shapes
			std::list<Topology::Ptr> contents;
			pOriginalShape->Contents(contents);
			for (TopTools_ListIteratorOfListOfShape kOcctGeneratedShapeIterator(occtGeneratedShapes);
				kOcctGeneratedShapeIterator.More();
				kOcctGeneratedShapeIterator.Next())
			{
				const TopoDS_Shape& rkOcctGeneratedShape = kOcctGeneratedShapeIterator.Value();
				Topology::Ptr pGeneratedShape = Topology::ByOcctShape(rkOcctGeneratedShape, "");

				for (const Topology::Ptr& kpContent : contents)
				{
					pGeneratedShape->AddContent(kpContent);
				}
			}
		}
	}

	Topology::Ptr Topology::Impose(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BOPAlgo_CellsBuilder occtCellsBuilder;
		NonRegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtCellsBuilder);

		// 2. Select the parts to be included in the final result.
		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		// Get part of A not in B
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorB.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		// Add B
		int i = 1;
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
			kOcctShapeIteratorB.More();
			kOcctShapeIteratorB.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorB.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, i++, true);
		}

		occtCellsBuilder.MakeContainers();

		TopoDS_Shape occtResultShape = occtCellsBuilder.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		Topology::Ptr pCopyPostprocessedShape = pPostprocessedShape->DeepCopy();
		TransferContents(GetOcctShape(), pCopyPostprocessedShape);
		TransferContents(kpOtherTopology->GetOcctShape(), pCopyPostprocessedShape);
		GlobalCluster::GetInstance().AddTopology(pCopyPostprocessedShape);
		return pCopyPostprocessedShape;
	}

	Topology::Ptr Topology::Imprint(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BOPAlgo_CellsBuilder occtCellsBuilder;
		NonRegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtCellsBuilder);

		// 2. Select the parts to be included in the final result.

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToTake.Clear();
				occtListToAvoid.Clear();
				occtListToTake.Append(kOcctShapeIteratorA.Value());
				occtListToTake.Append(kOcctShapeIteratorB.Value());
				occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			}
		}

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			occtListToAvoid.Append(occtArgumentsB);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		occtCellsBuilder.MakeContainers();

		TopoDS_Shape occtResultShape = occtCellsBuilder.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		Topology::Ptr pCopyPostprocessedShape = pPostprocessedShape->DeepCopy();
		TransferContents(GetOcctShape(), pCopyPostprocessedShape);
		TransferContents(kpOtherTopology->GetOcctShape(), pCopyPostprocessedShape);
		GlobalCluster::GetInstance().AddTopology(pCopyPostprocessedShape);
		return pCopyPostprocessedShape;
	}

	Topology::Ptr Topology::Intersect(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BOPAlgo_CellsBuilder occtCellsBuilder;
		NonRegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtCellsBuilder);

		// 2. Select the parts to be included in the final result.

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToTake.Clear();
				occtListToAvoid.Clear();
				occtListToTake.Append(kOcctShapeIteratorA.Value());
				occtListToTake.Append(kOcctShapeIteratorB.Value());
				occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			}
		}

		occtCellsBuilder.MakeContainers();

		TopoDS_Shape occtResultShape = occtCellsBuilder.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		if (pPostprocessedShape == nullptr)
		{
			return pPostprocessedShape;
		}
		Topology::Ptr pCopyPostprocessedShape = pPostprocessedShape->DeepCopy();
		
		TransferContents(GetOcctShape(), pCopyPostprocessedShape);
		TransferContents(kpOtherTopology->GetOcctShape(), pCopyPostprocessedShape);
		GlobalCluster::GetInstance().AddTopology(pCopyPostprocessedShape);
		return pCopyPostprocessedShape;
	}

	Topology::Ptr Topology::Merge(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BOPAlgo_CellsBuilder occtCellsBuilder;
		NonRegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtCellsBuilder);

		// 2. Select the parts to be included in the final result.
		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
			kOcctShapeIteratorB.More();
			kOcctShapeIteratorB.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorB.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		occtCellsBuilder.MakeContainers();

		TopoDS_Shape occtResultShape = occtCellsBuilder.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		Topology::Ptr pCopyPostprocessedShape = pPostprocessedShape->DeepCopy();
		TransferContents(GetOcctShape(), pCopyPostprocessedShape);
		TransferContents(kpOtherTopology->GetOcctShape(), pCopyPostprocessedShape);

		GlobalCluster::GetInstance().AddTopology(pCopyPostprocessedShape);
		return pCopyPostprocessedShape;
	}

	Topology::Ptr Topology::SelfMerge()
	{
		// 1
		BOPCol_ListOfShape occtShapes;
		SubTopologies(GetOcctShape(), occtShapes);

		// 2
		BOPAlgo_CellsBuilder occtCellsBuilder;
		occtCellsBuilder.SetArguments(occtShapes);
		//occtCellsBuilder.SetFuzzyValue(kTolerance);
		try {
			occtCellsBuilder.Perform();
		}
		catch (Standard_Failure& e)
		{
			const char* str = e.GetMessageString();
			std::string stlStr(str);
		}
		catch (std::exception& e)
		{
			const char* str = e.what();
			std::string stlStr(str);
		}

		if (occtCellsBuilder.HasErrors() || occtCellsBuilder.HasWarnings())
		{
			std::ostringstream errorStream;
			occtCellsBuilder.DumpErrors(errorStream);
			std::ostringstream warningStream;
			occtCellsBuilder.DumpWarnings(warningStream);

			// Exit here and return occtShapes as a cluster.
			TopoDS_Compound occtCompound;
			BRep_Builder occtBuilder;
			occtBuilder.MakeCompound(occtCompound);
			for (TopTools_ListIteratorOfListOfShape occtShapeIterator(occtShapes);
				occtShapeIterator.More();
				occtShapeIterator.Next())
			{
				occtBuilder.Add(occtCompound, occtShapeIterator.Value());
			}
			return Topology::ByOcctShape(occtCompound, "");
		}
		occtCellsBuilder.AddAllToResult();
		// DEBUG
		//return Topology::ByOcctShape(occtCellsBuilder.Shape());

		// 2b. Get discarded faces from Cells Builder
		BOPCol_ListOfShape occtDiscardedFaces;
		TopoDS_Compound occtCompound;
		BRep_Builder occtBuilder;
		occtBuilder.MakeCompound(occtCompound);
		for (BOPCol_ListIteratorOfListOfShape occtFaceIterator(occtShapes);
			occtFaceIterator.More();
			occtFaceIterator.Next())
		{
			TopoDS_Shape& rCurrent = occtFaceIterator.Value();
			if (occtCellsBuilder.IsDeleted(rCurrent))
			{
				occtBuilder.Add(occtCompound, rCurrent);
				occtDiscardedFaces.Append(rCurrent);
			}
		}
		//return Topology::ByOcctShape(occtCompound);


		// 3. Get Face[] from Topology[]
		BOPCol_ListOfShape occtFaces;
		TopoDS_Compound occtCompound3;
		BRep_Builder occtBuilder3;
		occtBuilder3.MakeCompound(occtCompound3);
		for (TopExp_Explorer occtExplorer(occtCellsBuilder.Shape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& rkOcctCurrent = occtExplorer.Current();
			if (!occtFaces.Contains(rkOcctCurrent))
			{
				occtFaces.Append(rkOcctCurrent);
				occtBuilder3.Add(occtCompound3, rkOcctCurrent);
			}
		}
		//return Topology::ByOcctShape(occtCompound3);

		// 5. Topology = VolumeMaker(Face[])--> first result
		BOPAlgo_MakerVolume occtVolumeMaker;
		Standard_Boolean bRunParallel = Standard_False; /* parallel or single mode (the default value is FALSE)*/
		Standard_Boolean bIntersect = Standard_True; /* intersect or not the arguments (the default value is TRUE)*/
		Standard_Real aTol = 0.0; /* fuzzy option (default value is 0)*/
								  //
		occtVolumeMaker.SetArguments(occtFaces);
		occtVolumeMaker.SetRunParallel(bRunParallel);
		occtVolumeMaker.SetIntersect(bIntersect);
		occtVolumeMaker.SetFuzzyValue(aTol);
		//
		occtVolumeMaker.Perform(); //perform the operation

		if (occtVolumeMaker.HasErrors() || occtVolumeMaker.HasWarnings()) { //check error status
			// BUG: Should not quit, but instead 
			//std::ostringstream errorStream;
			//occtVolumeMaker.DumpErrors(errorStream);
			///*throw std::exception(errorStream.str().c_str());*/
			//std::ostringstream warningStream;
			//occtVolumeMaker.DumpWarnings(warningStream);
			//TopoDS_Compound occtCompound;
			//BRep_Builder occtBuilder;
			//occtBuilder.MakeCompound(occtCompound);
			//for (TopTools_ListIteratorOfListOfShape iterator(occtShapes);
			//	iterator.More();
			//	iterator.Next())
			//{
			//	occtBuilder.Add(occtCompound, iterator.Value());
			//}
			//return Topology::ByOcctShape(occtCompound, "");
			////throw std::exception();
		}else
		{
			//DEBUG
			//return Topology::ByOcctShape(occtVolumeMaker.Shape());

			// 6. Get discarded faces from VolumeMaker--> second result
			TopoDS_Compound occtCompound2;
			BRep_Builder occtBuilder2;
			occtBuilder2.MakeCompound(occtCompound2);
			for (BOPCol_ListIteratorOfListOfShape occtFaceIterator(occtFaces);
				occtFaceIterator.More();
				occtFaceIterator.Next())
			{
				TopoDS_Shape& rCurrent = occtFaceIterator.Value();
				if (occtVolumeMaker.IsDeleted(rCurrent))
				{
					occtDiscardedFaces.Append(rCurrent);
					occtBuilder2.Add(occtCompound2, rCurrent);
				}
			}
			//return Topology::ByOcctShape(occtCompound2);
		}

		// 7. Get the rest from Topology[] --> third result
		BOPCol_ListOfShape occtOtherShapes; // for step #7
		for (BOPCol_ListIteratorOfListOfShape occtShapeIterator(occtShapes);
			occtShapeIterator.More();
			occtShapeIterator.Next())
		{
			if (occtShapeIterator.Value().ShapeType() != TopAbs_FACE)
			{
				occtOtherShapes.Append(occtShapeIterator.Value());
			}
		}

		//DEBUG: Draw discarded faces
		TopoDS_Compound occtCompound4;
		BRep_Builder occtBuilder4;
		occtBuilder4.MakeCompound(occtCompound4);
		for (BOPCol_ListIteratorOfListOfShape occtFaceIterator(occtDiscardedFaces);
			occtFaceIterator.More();
			occtFaceIterator.Next())
		{
			TopoDS_Shape& rCurrent = occtFaceIterator.Value();
			occtBuilder4.Add(occtCompound4, rCurrent);
		}
		//return Topology::ByOcctShape(occtCompound4);
		// 8. Merge results #1 #2 #3
		BOPCol_ListOfShape occtFinalArguments;
		if (!occtVolumeMaker.HasErrors() && !occtVolumeMaker.HasWarnings()) { //check error status
			occtFinalArguments.Append(occtVolumeMaker.Shape());
		}
		occtFinalArguments.Append(occtDiscardedFaces);
		occtFinalArguments.Append(occtOtherShapes);

		if (occtFinalArguments.Size() == 1)
		{
			return Topology::ByOcctShape(occtVolumeMaker.Shape(), "");
		}
		BOPAlgo_CellsBuilder occtCellsBuilder2;
		occtCellsBuilder2.SetArguments(occtFinalArguments);
		try {
			occtCellsBuilder2.Perform();
		}
		catch (Standard_Failure& e)
		{
			const char* str = e.GetMessageString();
			std::string stlStr(str);
		}
		catch (std::exception& e)
		{
			const char* str = e.what();
			std::string stlStr(str);
		}

		if (occtCellsBuilder2.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder2.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		occtCellsBuilder2.AddAllToResult();
		occtCellsBuilder2.MakeContainers();

		// 9. If there is still a discarded face, add to merge2Topology as a cluster.
		BOPCol_ListOfShape clusterCandidates;
		for (BOPCol_ListIteratorOfListOfShape merge2Topologies(occtFinalArguments);
			merge2Topologies.More();
			merge2Topologies.Next())
		{
			if (occtCellsBuilder2.IsDeleted(merge2Topologies.Value()) &&
				merge2Topologies.Value().ShapeType() == TopAbs_FACE) // currently only face
			{
				TopTools_ListOfShape modifiedShapes = occtCellsBuilder2.Modified(merge2Topologies.Value());
				TopTools_ListOfShape generatedShapes = occtCellsBuilder2.Generated(merge2Topologies.Value());
				clusterCandidates.Append(merge2Topologies.Value());
			}
		}
		if (clusterCandidates.Size() > 0)
		{
			SubTopologies(occtCellsBuilder2.Shape(), clusterCandidates);
			TopoDS_Compound occtFinalCompound;
			BRep_Builder occtFinalBuilder;
			occtFinalBuilder.MakeCompound(occtFinalCompound);
			for (BOPCol_ListIteratorOfListOfShape clusterCandidateIterator(clusterCandidates);
				clusterCandidateIterator.More();
				clusterCandidateIterator.Next())
			{
				occtFinalBuilder.Add(occtFinalCompound, clusterCandidateIterator.Value());
			}
			return Topology::ByOcctShape(occtFinalCompound, "");
		}

		// Otherwise, return the result final merge result
		return Topology::ByOcctShape(occtCellsBuilder2.Shape(), "")->DeepCopy();
	}

	Topology::Ptr Topology::Slice(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BOPAlgo_CellsBuilder occtCellsBuilder;
		NonRegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtCellsBuilder);

		// 2. Select the parts to be included in the final result.
		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		occtCellsBuilder.MakeContainers();

		TopoDS_Shape occtResultShape = occtCellsBuilder.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		Topology::Ptr pCopyPostprocessedShape = pPostprocessedShape->DeepCopy();
		TransferContents(GetOcctShape(), pCopyPostprocessedShape);
		GlobalCluster::GetInstance().AddTopology(pCopyPostprocessedShape);
		return pCopyPostprocessedShape;
	}

	Topology::Ptr Topology::Union(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BRepAlgoAPI_Fuse occtFuse;
		/*TopTools_ListOfShape occtArguments;
		occtArguments.Append(GetOcctShape());
		TopTools_ListOfShape occtTools;
		occtTools.Append(kpOtherTopology->GetOcctShape());
		fuse.SetArguments(occtArguments);
		fuse.SetTools(occtTools);
		fuse.SetNonDestructive(true);
		fuse.Build();*/

		RegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtFuse);

		TopoDS_Shape occtResultShape = occtFuse.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		Topology::Ptr pCopyPostprocessedShape = pPostprocessedShape->DeepCopy();
		TransferContents(GetOcctShape(), pCopyPostprocessedShape);
		TransferContents(kpOtherTopology->GetOcctShape(), pCopyPostprocessedShape);
		GlobalCluster::GetInstance().AddTopology(pCopyPostprocessedShape);
		return pCopyPostprocessedShape;
	}

	TopoDS_Shape Topology::FixBooleanOperandCell(const TopoDS_Shape& rkOcctShape)
	{
		TopTools_MapOfShape occtCells;
		TopoDS_Shape occtNewShape(rkOcctShape);
		for (TopExp_Explorer occtExplorer(rkOcctShape, TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Solid &occtCurrentSolid = TopoDS::Solid(occtExplorer.Current());
			//create tools for fixing a face 
			Handle(ShapeFix_Solid) occtShapeFixSolid = new ShapeFix_Solid();

			// create tool for rebuilding a shape and initialize it by shape 
			Handle(ShapeBuild_ReShape) occtContext = new ShapeBuild_ReShape();
			occtContext->Apply(occtNewShape);

			//set a tool for rebuilding a shape in the tool for fixing 
			occtShapeFixSolid->SetContext(occtContext);

			//initialize the fixing tool by one face 
			occtShapeFixSolid->Init(occtCurrentSolid);
			//fix the set face 
			occtShapeFixSolid->Perform();

			//get the result 
			occtNewShape = occtContext->Apply(occtNewShape);
		}

		return occtNewShape;
	}

	TopoDS_Shape Topology::FixBooleanOperandShell(const TopoDS_Shape& rkOcctShape)
	{
		TopTools_MapOfShape occtCells;
		TopoDS_Shape occtNewShape(rkOcctShape);
		for (TopExp_Explorer occtExplorer(rkOcctShape, TopAbs_SHELL); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shell& rkOcctCurrentShell = TopoDS::Shell(occtExplorer.Current());
			//create tools for fixing a face 
			Handle(ShapeFix_Shell) occtShapeFixShell = new ShapeFix_Shell();

			// create tool for rebuilding a shape and initialize it by shape 
			Handle(ShapeBuild_ReShape) occtContext = new ShapeBuild_ReShape();
			occtContext->Apply(occtNewShape);

			//set a tool for rebuilding a shape in the tool for fixing 
			occtShapeFixShell->SetContext(occtContext);

			//initialize the fixing tool by one face 
			occtShapeFixShell->Init(rkOcctCurrentShell);
			//fix the set face 
			occtShapeFixShell->Perform();

			//get the result 
			occtNewShape = occtContext->Apply(occtNewShape);
		}

		return occtNewShape;
	}

	TopoDS_Shape Topology::FixBooleanOperandFace(const TopoDS_Shape& rkOcctShape, BOPCol_DataMapOfShapeShape& rMapFaceToFixedFace)
	{
		TopTools_MapOfShape occtCells;
		TopoDS_Shape occtNewShape(rkOcctShape);
		for (TopExp_Explorer occtExplorer(rkOcctShape, TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Face& occtCurrentFace = TopoDS::Face(occtExplorer.Current());

			//create tools for fixing a face 
			Handle(ShapeFix_Face) occtShapeFixFace = new ShapeFix_Face();

			// create tool for rebuilding a shape and initialize it by shape 
			Handle(ShapeBuild_ReShape) occtContext = new ShapeBuild_ReShape();
			occtContext->Apply(occtNewShape);

			//set a tool for rebuilding a shape in the tool for fixing 
			occtShapeFixFace->SetContext(occtContext);

			//initialize the fixing tool by one face 
			occtShapeFixFace->Init(occtCurrentFace);
			//fix the set face 
			occtShapeFixFace->Perform();

			// Map occtCurrentFace and occtShapeFixFace.Shape()
			rMapFaceToFixedFace.Bind(occtCurrentFace, occtShapeFixFace->Face());

			//get the result 
			occtNewShape = occtContext->Apply(occtNewShape);
		}

		return occtNewShape;
	}

	TopoDS_Shape Topology::FixBooleanOperandFace(const TopoDS_Shape & rkOcctShape)
	{
		BOPCol_DataMapOfShapeShape mapFaceToFixedFace;
		return FixBooleanOperandFace(rkOcctShape, mapFaceToFixedFace);
	}

	void Topology::GetDeletedBooleanSubtopologies(const TopoDS_Shape& rkOcctShape, BOPAlgo_CellsBuilder & rOcctCellsBuilder, BOPCol_ListOfShape & rOcctDeletedShapes)
	{
		TopAbs_ShapeEnum subshapeTypes[3] = {TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE};
		for (int i = 0; i < 3; ++i)
		{
			if (rkOcctShape.ShapeType() == subshapeTypes[i])
			{
				if (rOcctCellsBuilder.IsDeleted(rkOcctShape))
				{
					rOcctDeletedShapes.Append(rkOcctShape);
				}

				const TopTools_ListOfShape& rkOcctModifiedShapes = rOcctCellsBuilder.Modified(rkOcctShape);
				if (!rkOcctModifiedShapes.IsEmpty())
				{
					rOcctDeletedShapes.Append(rkOcctShape);
				}
			}

			TopTools_MapOfShape occtSubshapes;
			DownwardNavigation(rkOcctShape, subshapeTypes[i], occtSubshapes);
			for (TopTools_MapIteratorOfMapOfShape occtSubshapeIterator(occtSubshapes);
				occtSubshapeIterator.More();
				occtSubshapeIterator.Next())
			{
				if (rOcctCellsBuilder.IsDeleted(occtSubshapeIterator.Value()))
				{
					rOcctDeletedShapes.Append(occtSubshapeIterator.Value());
				}
				const TopTools_ListOfShape& rkOcctModifiedShapes = rOcctCellsBuilder.Modified(occtSubshapeIterator.Value());
				if (!rkOcctModifiedShapes.IsEmpty())
				{
					rOcctDeletedShapes.Append(occtSubshapeIterator.Value());
				}
			}
		}
	}

	void Topology::GetDeletedBooleanSubtopologies(const TopoDS_Shape& rkOcctShape, BRepAlgoAPI_BooleanOperation & rOcctBooleanOperation, BOPCol_ListOfShape & rOcctDeletedShapes)
	{
		TopAbs_ShapeEnum subshapeTypes[3] = { TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE };
		for (int i = 0; i < 3; ++i)
		{
			if (rkOcctShape.ShapeType() == subshapeTypes[i])
			{
				if (rOcctBooleanOperation.IsDeleted(rkOcctShape))
				{
					rOcctDeletedShapes.Append(rkOcctShape);
				}
			}

			TopTools_MapOfShape occtSubshapes;
			DownwardNavigation(rkOcctShape, subshapeTypes[i], occtSubshapes);
			for (TopTools_MapIteratorOfMapOfShape occtSubshapeIterator(occtSubshapes);
				occtSubshapeIterator.More();
				occtSubshapeIterator.Next())
			{
				if (rOcctBooleanOperation.IsDeleted(occtSubshapeIterator.Value()))
				{
					rOcctDeletedShapes.Append(occtSubshapeIterator.Value());
				}
			}
		}
	}

	Topology::Ptr Topology::TrackContextAncestor()
	{
		std::list<Context::Ptr> contexts;
		Contexts(contexts);

		if (contexts.size() == 1)
		{
			// Go farther
			return (*contexts.begin())->Topology()->TrackContextAncestor();
		}

		// If empty or > 2
		return shared_from_this();
	}

	void Topology::AddBooleanOperands(
		const Topology::Ptr& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsB,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	{
		// Buffer lists are currently provided for cell complexes to be processed per cells.
		BOPCol_ListOfShape occtCellsBuildersArguments;
		TopologyType typeA = GetType();
		if (typeA == TOPOLOGY_CELLCOMPLEX || typeA == TOPOLOGY_CLUSTER)
		{
			std::list<Topology::Ptr> members;
			SubTopologies(members);
			for (const Topology::Ptr& kpMember : members)
			{
				TopoDS_Shape occtNewShape = FixBooleanOperandFace(kpMember->GetOcctShape(), rOcctMapFaceToFixedFaceA);
				occtNewShape = FixBooleanOperandShell(kpMember->GetOcctShape());
				occtNewShape = FixBooleanOperandCell(kpMember->GetOcctShape());
				rOcctCellsBuildersOperandsA.Append(kpMember->GetOcctShape());
				occtCellsBuildersArguments.Append(kpMember->GetOcctShape());
			}
		}
		else
		{
			BOPCol_DataMapOfShapeShape occtMapFaceToFixedFace;
			TopoDS_Shape occtNewShape = FixBooleanOperandFace(GetOcctShape(), rOcctMapFaceToFixedFaceA);
			occtNewShape = FixBooleanOperandShell(occtNewShape);
			occtNewShape = FixBooleanOperandCell(occtNewShape);
			rOcctCellsBuildersOperandsA.Append(occtNewShape);
			occtCellsBuildersArguments.Append(occtNewShape);
		}

		TopologyType typeB = kpOtherTopology->GetType();
		if (typeB == TOPOLOGY_CELLCOMPLEX || typeB == TOPOLOGY_CLUSTER)
		{
			std::list<Topology::Ptr> members;
			kpOtherTopology->SubTopologies(members);
			for (const Topology::Ptr& kpMember : members)
			{
				TopoDS_Shape occtNewShape = FixBooleanOperandFace(kpMember->GetOcctShape(), rOcctMapFaceToFixedFaceA);
				occtNewShape = FixBooleanOperandShell(kpMember->GetOcctShape());
				occtNewShape = FixBooleanOperandCell(kpMember->GetOcctShape());
				rOcctCellsBuildersOperandsB.Append(kpMember->GetOcctShape());
				occtCellsBuildersArguments.Append(kpMember->GetOcctShape());
			}
		}
		else
		{
			TopoDS_Shape occtNewShape = FixBooleanOperandFace(kpOtherTopology->GetOcctShape(), rOcctMapFaceToFixedFaceB);
			occtNewShape = FixBooleanOperandShell(kpOtherTopology->GetOcctShape());
			occtNewShape = FixBooleanOperandCell(kpOtherTopology->GetOcctShape());
			rOcctCellsBuildersOperandsB.Append(occtNewShape);
			occtCellsBuildersArguments.Append(occtNewShape);
		}

		rOcctCellsBuilder.SetArguments(occtCellsBuildersArguments);
	}

	void Topology::AddBooleanOperands(const Topology::Ptr & kpOtherTopology, BOPCol_ListOfShape & rOcctCellsBuildersOperandsA, BOPCol_ListOfShape & rOcctCellsBuildersOperandsB)
	{
		{
			BOPCol_ListOfShape occtOperandsA;
			if (IsContainerType())
			{
				std::list<Topology::Ptr> subTopologies;
				SubTopologies(subTopologies);
				for (const Topology::Ptr kpTopology: subTopologies)
				{
					occtOperandsA.Append(kpTopology->GetOcctShape());
				}
			}
			else
			{
				occtOperandsA.Append(GetOcctShape());
			}

			for (BOPCol_ListIteratorOfListOfShape occtOperandAIterator(occtOperandsA);
				occtOperandAIterator.More();
				occtOperandAIterator.Next())
			{
				TopoDS_Shape occtNewShape = FixBooleanOperandFace(occtOperandAIterator.Value());
				occtNewShape = FixBooleanOperandShell(occtNewShape);
				occtNewShape = FixBooleanOperandCell(occtNewShape);
				rOcctCellsBuildersOperandsA.Append(occtNewShape);
			}
		}

		{
			BOPCol_ListOfShape occtOperandsB;
			if (kpOtherTopology->IsContainerType())
			{
				std::list<Topology::Ptr> subTopologies;
				kpOtherTopology->SubTopologies(subTopologies);
				for (const Topology::Ptr kpTopology : subTopologies)
				{
					occtOperandsB.Append(kpTopology->GetOcctShape());
				}
			}
			else
			{
				occtOperandsB.Append(kpOtherTopology->GetOcctShape());
			}

			for (BOPCol_ListIteratorOfListOfShape occtOperandBIterator(occtOperandsB);
				occtOperandBIterator.More();
				occtOperandBIterator.Next())
			{
				TopoDS_Shape occtNewShape = FixBooleanOperandFace(occtOperandBIterator.Value());
				occtNewShape = FixBooleanOperandShell(occtNewShape);
				occtNewShape = FixBooleanOperandCell(occtNewShape);
				rOcctCellsBuildersOperandsB.Append(occtNewShape);
			}
		}
	}

	Topology::Ptr Topology::XOR(const Topology::Ptr & kpOtherTopology)
	{
		BOPCol_ListOfShape occtArgumentsA;
		BOPCol_ListOfShape occtArgumentsB;
		AddBooleanOperands(kpOtherTopology, occtArgumentsA, occtArgumentsB);

		BOPAlgo_CellsBuilder occtCellsBuilder;
		NonRegularBooleanOperation(occtArgumentsA, occtArgumentsB, occtCellsBuilder);

		// 2. Select the parts to be included in the final result.
		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorB.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}


		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtArgumentsB);
			kOcctShapeIteratorB.More();
			kOcctShapeIteratorB.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorB.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtArgumentsA);
				kOcctShapeIteratorA.More();
				kOcctShapeIteratorA.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorA.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		occtCellsBuilder.MakeContainers();

		TopoDS_Shape occtResultShape = occtCellsBuilder.Shape();
		TopoDS_Shape occtPostprocessedShape = PostprocessBooleanResult(occtResultShape);
		Topology::Ptr pPostprocessedShape = Topology::ByOcctShape(occtPostprocessedShape, "");
		TransferContents(GetOcctShape(), pPostprocessedShape);
		TransferContents(kpOtherTopology->GetOcctShape(), pPostprocessedShape);
		GlobalCluster::GetInstance().AddTopology(occtPostprocessedShape);
		return pPostprocessedShape;
	}

	Topology::Ptr Topology::Trim(const Topology::Ptr & kpOtherTopology)
	{
		throw std::exception("Not implemented yet");
	}

	Topology::Ptr Topology::Divide(const Topology::Ptr & kpTool)
	{
		// For now, only works if this topology is a cell
		TopologyType topologyType = GetType();

		if (topologyType != TOPOLOGY_CELL && topologyType != TOPOLOGY_FACE && topologyType != TOPOLOGY_EDGE)
		{
			return nullptr;
		}

		//Topology::Ptr farthestContextTopology = TrackContextAncestor();

		Topology::Ptr pSlicedTopology = Slice(kpTool);

		if (topologyType == TOPOLOGY_CELL)
		{
			std::list<Cell::Ptr> cells;
			pSlicedTopology->DownwardNavigation(cells);
			for (const Cell::Ptr& kpCell : cells)
			{
				AddContent(kpCell);
			}
		}else if (topologyType == TOPOLOGY_FACE)
		{
			std::list<Face::Ptr> faces;
			pSlicedTopology->DownwardNavigation(faces);
			for (const Face::Ptr& kpFace : faces)
			{
				AddContent(kpFace);
			}
		}else //if (topologyType == TOPOLOGY_EDGE)
		{
			std::list<Edge::Ptr> edges;
			pSlicedTopology->DownwardNavigation(edges);
			for (const Edge::Ptr& kpEdge : edges)
			{
				AddContent(kpEdge);
			}
		}

		//Topology::Ptr thisTopology = shared_from_this();
		Topology::Ptr farthestTopology = TrackContextAncestor();

		return farthestTopology->DeepCopy();
	}

	void Topology::SubTopologies(const TopoDS_Shape& rkShape, BOPCol_ListOfShape& rSubTopologies)
	{
		for (TopoDS_Iterator occtShapeIterator(rkShape); occtShapeIterator.More(); occtShapeIterator.Next())
		{
			rSubTopologies.Append(occtShapeIterator.Value());
		}
	}

	void Topology::SubTopologies(std::list<Topology::Ptr>& rSubTopologies) const
	{
		BOPCol_ListOfShape occtListMembers;
		Topology::SubTopologies(GetOcctShape(), occtListMembers);
		for (BOPCol_ListIteratorOfListOfShape occtIterator(occtListMembers);
			occtIterator.More();
			occtIterator.Next())
		{
			Topology::Ptr pMemberTopology = Topology::ByOcctShape(occtIterator.Value(), "");
			rSubTopologies.push_back(pMemberTopology);
		}
	}

	void Topology::Shells(std::list<std::shared_ptr<Shell>>& rShells) const
	{
		Navigate(rShells);
	}

	void Topology::Edges(std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		Navigate(rEdges);
	}

	void Topology::Faces(std::list<std::shared_ptr<Face>>& rFaces) const
	{
		Navigate(rFaces);
	}

	void Topology::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		Navigate(rVertices);
	}

	void Topology::Wires(std::list<std::shared_ptr<Wire>>& rWires) const
	{
		Navigate(rWires);
	}

	void Topology::Cells(std::list<std::shared_ptr<Cell>>& rCells) const
	{
		Navigate(rCells);
	}

	void Topology::CellComplexes(std::list<std::shared_ptr<CellComplex>>& rCellComplexes) const
	{
		Navigate(rCellComplexes);
	}

	bool Topology::IsContainerType(const TopoDS_Shape& rkOcctShape)
	{
		TopAbs_ShapeEnum occtShapeType = rkOcctShape.ShapeType();
		if (occtShapeType == TopAbs_WIRE || occtShapeType == TopAbs_SHELL || occtShapeType == TopAbs_COMPSOLID || occtShapeType == TopAbs_COMPOUND)
		{
			return true;
		}

		return false;
	}

#ifdef _DEBUG
	void Topology::GlobalClusterSubTopologies(std::list<Topology::Ptr>& rSubTopologies) const
	{
		BOPCol_ListOfShape occtListMembers;
		Topology::SubTopologies(GlobalCluster::GetInstance().GetOcctCompound(), occtListMembers);
		for (BOPCol_ListIteratorOfListOfShape occtIterator(occtListMembers);
			occtIterator.More();
			occtIterator.Next())
		{
			Topology::Ptr pMemberTopology = Topology::ByOcctShape(occtIterator.Value(), "");
			rSubTopologies.push_back(pMemberTopology);
		}
	}
#endif

	void Topology::UpwardNavigation(const TopoDS_Shape & rkOcctShape, const TopoDS_Shape & rkOcctParentShape, const TopAbs_ShapeEnum kShapeEnum, TopTools_ListOfShape & rOcctMembers)
	{
		TopTools_ListOfShape occtAncestors;
		TopTools_IndexedDataMapOfShapeListOfShape occtShapeMap;
		TopExp::MapShapesAndUniqueAncestors(
			rkOcctShape,
			rkOcctShape.ShapeType(),
			kShapeEnum,
			occtShapeMap);

		occtShapeMap.FindFromKey(rkOcctShape, occtAncestors);
	}

	void Topology::DownwardNavigation(const TopoDS_Shape& rkOcctShape, const TopAbs_ShapeEnum & rkShapeEnum, TopTools_MapOfShape & rOcctMembers)
	{
		for (TopExp_Explorer occtExplorer(rkOcctShape, rkShapeEnum); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!rOcctMembers.Contains(occtCurrent))
			{
				rOcctMembers.Add(occtCurrent);
			}
		}
	}

	void DeepCopyExplodeShape(const TopoDS_Shape& rkOcctOriginalShape, BRepBuilderAPI_Copy& rOcctCopy, TopTools_DataMapOfShapeShape& rOcctShapeCopyShapeMap)
	{
		rOcctShapeCopyShapeMap.Bind(rkOcctOriginalShape, rOcctCopy.Shape());
		rOcctShapeCopyShapeMap.Bind(rOcctCopy.Shape(), rkOcctOriginalShape); // 2-way

		TopTools_ListOfShape occtMembers;
		Topology::Members(rkOcctOriginalShape, occtMembers);
		for (TopTools_ListIteratorOfListOfShape occtMemberIterator(occtMembers);
			occtMemberIterator.More();
			occtMemberIterator.Next())
		{
			TopoDS_Shape& rOcctMember = occtMemberIterator.Value();
			try{
				TopoDS_Shape occtMemberCopy = rOcctCopy.ModifiedShape(rOcctMember);
				rOcctShapeCopyShapeMap.Bind(rOcctMember, occtMemberCopy);
				rOcctShapeCopyShapeMap.Bind(occtMemberCopy, rOcctMember);
			}
			catch (Standard_NoSuchObject)
			{

			}
		}
	}

	// Based on https://stackoverflow.com/questions/14536702/algorithm-to-clone-a-graph
	Topology::Ptr DeepCopyImpl(const TopoDS_Shape& rkOcctShape, TopTools_DataMapOfShapeShape& rOcctShapeCopyShapeMap)
	{
		BRepBuilderAPI_Copy occtShapeCopier(rkOcctShape);
		TopoDS_Shape occtShapeCopy = occtShapeCopier.Shape();
		DeepCopyExplodeShape(rkOcctShape, occtShapeCopier, rOcctShapeCopyShapeMap);
		// Explode
		Topology::Ptr pShapeCopy = Topology::ByOcctShape(occtShapeCopy, Topology::GetInstanceGUID(rkOcctShape));

		std::list<Context::Ptr> contexts;
		Topology::Contexts(rkOcctShape, contexts);
		for (const Context::Ptr& kpContext : contexts)
		{
			Topology::Ptr pContextTopology = kpContext->Topology();
			
			TopoDS_Shape occtCopyShape;
			bool isContextCopied = rOcctShapeCopyShapeMap.Find(pContextTopology->GetOcctShape(), occtCopyShape);
			Topology::Ptr pCopyContextTopology;
			if (isContextCopied)
			{
				pCopyContextTopology = Topology::ByOcctShape(occtCopyShape, Topology::GetInstanceGUID(pContextTopology->GetOcctShape()));
			}else
			{
				pCopyContextTopology = DeepCopyImpl(pContextTopology->GetOcctShape(), rOcctShapeCopyShapeMap);
			}

			pCopyContextTopology->AddContent(pShapeCopy);
		}

		std::list<Topology::Ptr> subContents;
		Topology::SubContents(rkOcctShape, subContents);
		for (const Topology::Ptr& kpSubContent : subContents)
		{
			TopoDS_Shape occtCopyShape;
			bool isContentCopied = rOcctShapeCopyShapeMap.Find(kpSubContent->GetOcctShape(), occtCopyShape);
			Topology::Ptr pCopyContentTopology;
			if (isContentCopied)
			{
				pCopyContentTopology = Topology::ByOcctShape(occtCopyShape, Topology::GetInstanceGUID(kpSubContent->GetOcctShape()));
			}else
			{
				pCopyContentTopology = DeepCopyImpl(kpSubContent->GetOcctShape(), rOcctShapeCopyShapeMap);
			}
			int filterType = 0;
			std::list<Context::Ptr> contexts;
			kpSubContent->Contexts(contexts);
			for (const Context::Ptr& pContext : contexts)
			{
				int contextType = pContext->Topology()->GetType();
				filterType = Bitwise::Or(filterType, contextType);
			}
			pShapeCopy->AddContent(pCopyContentTopology, filterType);
		}
		GlobalCluster::GetInstance().AddTopology(pShapeCopy);
		return pShapeCopy;
	}

	Topology::Ptr Topology::DeepCopy()
	{
		TopTools_DataMapOfShapeShape occtShapeCopyShapeMap;
		Topology::Ptr pShapeCopy = DeepCopyImpl(GetOcctShape(), occtShapeCopyShapeMap);
		return pShapeCopy;
	}

	Topology::Ptr Topology::ShallowCopy()
	{
		BRepBuilderAPI_Copy occtShapeCopier(GetOcctShape());
		return Topology::ByOcctShape(occtShapeCopier.Shape(), GetInstanceGUID());
	}

	TopoDS_Shape Topology::CopyOcct(const TopoDS_Shape& rkOcctShape)
	{
		BRepBuilderAPI_Copy occtShapeCopy(rkOcctShape);
		TopoDS_Shape occtCopyShape = occtShapeCopy.Shape();

		GlobalCluster::GetInstance().AddTopology(occtCopyShape);
		return occtCopyShape;
	}

	void Topology::ReplaceSubentity(const Topology::Ptr& rkOriginalSubshape, const Topology::Ptr& rkNewSubshape)
	{
		ReplaceSubentity(rkOriginalSubshape->GetOcctShape(), rkNewSubshape->GetOcctShape());
	}

	void Topology::ReplaceSubentity(const TopoDS_Shape& rkOcctOriginalSubshape, const TopoDS_Shape& rkOcctNewSubshape)
	{
		Handle(ShapeBuild_ReShape) occtReshaper = new ShapeBuild_ReShape();
		occtReshaper->Replace(rkOcctOriginalSubshape, rkOcctNewSubshape);
		TopoDS_Shape newShape = occtReshaper->Apply(GetOcctShape());
	}

	bool Topology::IsSame(const Topology::Ptr& kpTopology) const
	{
		bool isSame = GetOcctShape().IsSame(kpTopology->GetOcctShape());
		return isSame;
	}

	void Topology::Members(TopTools_ListOfShape& rOcctMembers) const
	{
		const TopoDS_Shape& rkOcctShape = GetOcctShape();
		Members(rkOcctShape, rOcctMembers);
	}

	void Topology::Members(std::list<Topology::Ptr>& rMembers) const
	{
		TopTools_ListOfShape occtMembers;
		Members(occtMembers);
		for (TopTools_ListIteratorOfListOfShape occtMemberIterator(occtMembers); occtMemberIterator.More(); occtMemberIterator.Next())
		{
			rMembers.push_back(Topology::ByOcctShape(occtMemberIterator.Value(), ""));
		}
	}

	void Topology::Members(const TopoDS_Shape & rkOcctShape, TopTools_ListOfShape & rOcctMembers)
	{
		// Store the children
		for (int i = ((int)rkOcctShape.ShapeType()) + 1; i < (int)TopAbs_SHAPE; ++i)
		{
			TopAbs_ShapeEnum occtShapeEnum = (TopAbs_ShapeEnum)i;
			TopTools_MapOfShape occtMembers;
			Topology::DownwardNavigation(rkOcctShape, occtShapeEnum, occtMembers);
			for (TopTools_MapIteratorOfMapOfShape occtMembersIterator(occtMembers);
				occtMembersIterator.More();
				occtMembersIterator.Next())
			{
				rOcctMembers.Append(occtMembersIterator.Value());
			}
		}
	}

	const std::string Topology::GetInstanceGUID() const
	{
		return GetInstanceGUID(GetOcctShape());
	}

	const std::string Topology::GetInstanceGUID(const TopoDS_Shape & rkOcctShape)
	{
		std::string guid;
		bool value = InstanceGUIDManager::GetInstance().Find(rkOcctShape, guid);
		assert(value);
		return guid;
	}
}