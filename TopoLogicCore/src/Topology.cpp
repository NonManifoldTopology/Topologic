#include <Topology.h>
#include <Cluster.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Shell.h>
#include <Face.h>
#include <Wire.h>
#include <Edge.h>
#include <Vertex.h>
#include <Context.h>
#include <GlobalCluster.h>
#include <LabelManager.h>

#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Face.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <TNaming_Builder.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDataStd_Integer.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>

#include <BOPAlgo_Builder.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BOPCol_ListOfShape.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_MapOfShape.hxx>
#include <TNaming_NamedShape.hxx>
#include <TDataStd_Integer.hxx>
#include <BRepCheck_Wire.hxx>
#include <BOPAlgo_MakerVolume.hxx>
#include <TopTools_DataMapOfShapeInteger.hxx>
#include <BRepExtrema_DistShapeShape.hxx>

#include <ShapeFix_Shape.hxx>

#include <array>
#include <algorithm>

namespace TopoLogicCore
{
	void AddOcctListShapeToAnotherList(const BOPCol_ListOfShape& rkAList, BOPCol_ListOfShape& rAnotherList)
	{
		for (BOPCol_ListIteratorOfListOfShape kIterator(rkAList);
			kIterator.More();
			kIterator.Next())
		{
			rAnotherList.Append(kIterator.Value());
		}
	}

	std::string Topology::GetTypeAsString() const
	{
		switch (GetType())
		{
		case TOPOLOGY_CLUSTER:
			return std::string("Cluster");
		case TOPOLOGY_CELLCOMPLEX:
			return std::string("CellComplex");
		case TOPOLOGY_CELL:
			return std::string("Cell");
		case TOPOLOGY_SHELL:
			return std::string("Shell");
		case TOPOLOGY_FACE:
			return std::string("Face");
		case TOPOLOGY_WIRE:
			return std::string("Wire");
		case TOPOLOGY_EDGE:
			return std::string("Edge");
		default: // case TOPOLOGY_VERTEX:
			return std::string("Vertex");
		}
	}

	void Topology::AddChildLabel(std::shared_ptr<Topology>& pTopology, const TopologyRelationshipType kRelationshipType)
	{
		TDF_Label occtChildLabel = TDF_TagSource::NewChild(GetOcctLabel());
		TNaming_Builder cellAttributeBuilder(occtChildLabel);
		cellAttributeBuilder.Generated(pTopology->GetOcctShape());
		TDataStd_Integer::Set(occtChildLabel, kRelationshipType);
		pTopology->SetOcctLabel(occtChildLabel);
	}

	TDF_Label& Topology::GetOcctLabel()
	{
		//assert(m_occtLabel.IsNull() && "OCAF label is null.");
		if (m_occtLabel.IsNull())
		{
			//throw std::exception("OCAF label is null.");
		}

		return m_occtLabel;
	}

	const TDF_Label & Topology::GetOcctLabel() const
	{
		//assert(m_occtLabel.IsNull() && "OCAF label is null.");
		if (m_occtLabel.IsNull())
		{
			//throw std::exception("OCAF label is null.");
		}

		return m_occtLabel;
	}

	void Topology::SetOcctLabel(const TDF_Label & rkOcctLabel)
	{
		// Only register the label if it is not under the root
		if (rkOcctLabel.IsNull() || !LabelManager::GetInstance().HasLabel(rkOcctLabel))
		{
			RegisterLabel();
		}
		else
		{
			m_occtLabel = rkOcctLabel;
		}
	}

	std::shared_ptr<Topology> Topology::ClosestLowestSubshapeTo(const std::shared_ptr<Topology>& kpTopology) const
	{
		//TopTools_DataMapOfShapeInteger occtShapeToDistanceMap;
		TopoDS_Shape occtClosestSubshape;
		double minDistance = std::numeric_limits<double>::max();
		const TopoDS_Shape& kOcctThisShape = GetOcctShape();
		const TopoDS_Shape& kOcctQueryShape = kpTopology->GetOcctShape();
		TopAbs_ShapeEnum occtShapeTypes[3] = { TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE };
		for (int i = 0; i < 3; ++i)
		{
			TopAbs_ShapeEnum occtShapeType = occtShapeTypes[i];
			TopTools_MapOfShape occtCells;
			for (TopExp_Explorer occtExplorer(kOcctThisShape, occtShapeType); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& rkCurrentChildShape = occtExplorer.Current();
				BRepExtrema_DistShapeShape occtDistanceCalculation(rkCurrentChildShape, kOcctQueryShape);
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

		return Topology::ByOcctShape(occtClosestSubshape);
	}

	Topology::Topology(const int kDimensionality)
		: m_dimensionality(kDimensionality)
		, m_isInGlobalCluster(false)
		, m_occtLabel()
	{

	}

	std::shared_ptr<Topology> Topology::ByOcctShape(const TopoDS_Shape& rkOcctShape, const TDF_Label& rkOcctLabel)
	{
		if (rkOcctShape.TShape().IsNull()) 
		{
			return nullptr;
		}
		TopAbs_ShapeEnum occtShapeType = rkOcctShape.ShapeType();
		switch (occtShapeType)
		{
		case TopAbs_COMPOUND: return std::make_shared<Cluster>(TopoDS::Compound(rkOcctShape));// , rkOcctLabel);
		case TopAbs_COMPSOLID: return std::make_shared<CellComplex>(TopoDS::CompSolid(rkOcctShape));//, rkOcctLabel);
		case TopAbs_SOLID: return std::make_shared<Cell>(TopoDS::Solid(rkOcctShape));//, rkOcctLabel);
		case TopAbs_SHELL: return std::make_shared<Shell>(TopoDS::Shell(rkOcctShape));//, rkOcctLabel);
		case TopAbs_FACE: return std::make_shared<Face>(TopoDS::Face(rkOcctShape), rkOcctLabel);
		case TopAbs_WIRE: return std::make_shared<Wire>(TopoDS::Wire(rkOcctShape), rkOcctLabel);
		case TopAbs_EDGE: return std::make_shared<Edge>(TopoDS::Edge(rkOcctShape), rkOcctLabel);
		case TopAbs_VERTEX: return std::make_shared<Vertex>(TopoDS::Vertex(rkOcctShape), rkOcctLabel);
		default:
			throw std::exception("Topology::ByOcctShape: unknown topology.");
		}
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
		// Clear ingredients information
		/*for (std::list<Topology*>::const_iterator kTopologyIterator = m_ingredientTo.begin();
			kTopologyIterator != m_ingredientTo.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			pTopology->m_ingredients.remove(this);
		}*/
		m_ingredientTo.clear();
		m_ingredients.clear();
		m_contents.clear();
		m_contexts.clear();

		/*for (AttributeMapIterator attributeMapIterator = m_attributeMap.begin();
			attributeMapIterator != m_attributeMap.end();
			attributeMapIterator++)
		{
			delete attributeMapIterator->second;
		}*/
		m_attributeMap.clear();
	}

	std::shared_ptr<Topology> Topology::ByGeometry(Handle(Geom_Geometry) pGeometry)
	{
		return nullptr;
	}

	std::shared_ptr<Topology> Topology::ByContext(const std::shared_ptr<Context>& kpContext)
	{
		return nullptr;
	}

	std::shared_ptr<Topology> Topology::ByVertexIndex(const std::list<std::array<double, 3>>& rkVertexCoordinates, const std::list<std::list<int>>& rkVertexIndices)
	{
		return nullptr;
	}

	std::shared_ptr<Topology> Topology::ByVertexIndex(const std::vector<std::shared_ptr<Vertex>>& rkVertices, const std::list<std::list<int>>& rkVertexIndices)
	{
		if (rkVertices.empty() || rkVertexIndices.empty())
		{
			return nullptr;
		}
		// Input: vertex[], int[][] indices

		// - Topology[] = iterate through indices and create either a face(if > 2, planar), a wire (if > 2, non-planar), an edge(= 2), or a vertex.
		TopTools_ListOfShape occtShapes;
		for (const std::list<int>& rkVertex1DIndices : rkVertexIndices)
		{
			std::list<TopoDS_Vertex> occtVertices;
			for (int vertexIndex : rkVertex1DIndices)
			{
				occtVertices.push_back(rkVertices[vertexIndex]->GetOcctVertex());
			}

			if (occtVertices.size() > 2)
			{
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
					const TopoDS_Wire& rkocctWire = occtMakeWire.Wire();
					
					if(BRepCheck_Wire(rkocctWire).Closed() == BRepCheck_NoError)
					{
						// Try creating a face
						BRepBuilderAPI_MakeFace occtMakeFace(rkocctWire);
						if (occtMakeFace.Error() == BRepBuilderAPI_FaceDone)
						{
							// Add the face
							occtShapes.Append(occtMakeFace.Face());
						}
						else
						{
							// Add the closed wire
							occtShapes.Append(rkocctWire);
						}
					}
					else
					{
						// Add the opem wire
						occtShapes.Append(rkocctWire);
					}
				}
				else
				{
					// Add the edges
					occtShapes.Append(occtEdges);
				}
			}
			else if (occtVertices.size() == 2)
			{
				// Try creating an edge
				BRepBuilderAPI_MakeEdge occtMakeEdge(TopoDS::Vertex(occtVertices.front()), 
					                                 TopoDS::Vertex(occtVertices.back()));
				occtShapes.Append(occtMakeEdge);
			}
			else if (occtVertices.size() == 1)
			{
				// Insert the vertices
				occtShapes.Append(occtVertices.front());
			}
		}

		/*TopoDS_Compound compound;
		TopoDS_Builder builder;
		builder.MakeCompound(compound);
		for (TopTools_ListOfShape::Iterator occtShapeIterator(occtShapes);
			occtShapeIterator.More();
			occtShapeIterator.Next())
		{
			TopoDS_Shape& rCurrentShape = occtShapeIterator.Value();
			builder.Add(compound, rCurrentShape);
		}

		return Topology::ByOcctShape(compound);*/


		// - Topology[] = Merge--> this divides implicitly intersecting topologies.
		BOPAlgo_CellsBuilder occtCellsBuilder;
		occtCellsBuilder.SetArguments(occtShapes);
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

		if (occtCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape occtShapeIterator(occtShapes);
			occtShapeIterator.More();
			occtShapeIterator.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(occtShapeIterator.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		//return Topology::ByOcctShape(occtCellsBuilder.Shape());

		// - Get Face[] from Topology[]
		BOPCol_ListOfShape occtFaces;
		for (TopExp_Explorer occtExplorer(occtCellsBuilder.Shape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& rkOcctCurrent = occtExplorer.Current();
			if (!occtFaces.Contains(rkOcctCurrent))
			{
				occtFaces.Append(rkOcctCurrent);
			}
		}

		// - Topology = VolumeMaker(Face[])--> first result
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
		if (occtVolumeMaker.HasErrors()) { //check error status
			throw std::exception();
		}

		// - Get discarded faces from VolumeMaker--> second result
		BOPCol_ListOfShape occtDiscardedFaces;
		for (BOPCol_ListIteratorOfListOfShape occtFaceIterator(occtFaces);
			occtFaceIterator.More();
			occtFaceIterator.Next())
		{
			TopoDS_Shape& rCurrent = occtFaceIterator.Value();
			if (occtVolumeMaker.IsDeleted(rCurrent))
			{
				occtDiscardedFaces.Append(rCurrent);
			}
		}

		// - Get the rest from Topology[] --> third result
		BOPCol_ListOfShape occtOtherShapes;
		for (BOPCol_ListIteratorOfListOfShape occtShapeIterator(occtShapes);
			occtShapeIterator.More();
			occtShapeIterator.Next())
		{
			if (occtShapeIterator.Value().ShapeType() != TopAbs_FACE)
			{
				occtOtherShapes.Append(occtShapeIterator.Value());
			}
		}

		// - Merge results #1 #2 #3
		BOPCol_ListOfShape occtFinalArguments;
		occtFinalArguments.Append(occtVolumeMaker.Shape());
		occtFinalArguments.Append(occtDiscardedFaces);
		occtFinalArguments.Append(occtOtherShapes);

		if (occtFinalArguments.Size() == 1)
		{
			return Topology::ByOcctShape(occtVolumeMaker.Shape());
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

		BOPCol_ListOfShape occtListToTake2;
		BOPCol_ListOfShape occtListToAvoid2;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIterator(occtFinalArguments);
			kOcctShapeIterator.More();
			kOcctShapeIterator.Next())
		{
			occtListToTake2.Clear();
			occtListToAvoid2.Clear();
			occtListToTake2.Append(kOcctShapeIterator.Value());
			occtCellsBuilder2.AddToResult(occtListToTake2, occtListToAvoid2);
		}
		
		return Topology::ByOcctShape(occtCellsBuilder2.Shape());
	}

	void Topology::AddContent(const std::shared_ptr<Topology>& rkTopology)
	{
		LabelManager::GetInstance().AddContent(rkTopology->GetOcctLabel(), GetOcctLabel());
	}

	void Topology::RemoveContent(const std::shared_ptr<Topology>& rkTopology)
	{
		m_contents.remove(rkTopology);
	}

	void Topology::AddContext(const std::shared_ptr<Context>& rkContext)
	{
		if (std::find(m_contexts.begin(), m_contexts.end(), rkContext) == m_contexts.end())
		{
			m_contexts.push_back(rkContext);
		}
	}

	void Topology::RemoveContext(const std::shared_ptr<Context>& rkContext)
	{
		m_contexts.remove(rkContext);
	}

	void Topology::SharedTopologies(const std::shared_ptr<Topology>& kpTopology, std::list<std::shared_ptr<Topology>>& rkSharedTopologies) const
	{
	}

	void Topology::PathsTo(const std::shared_ptr<Topology>& kpTopology, const int kMaxLevels, const int kMaxPaths, std::list<std::list<std::shared_ptr<TopologicalQuery>>>& rkPaths) const
	{
	}

	void Topology::Contents(std::list<std::shared_ptr<Topology>>& rContents) const
	{
		LabelManager::Contents(GetOcctLabel(), rContents);
	}

	void Topology::ContentsV2(const bool kAllLevels, std::list<std::shared_ptr<Topology>>& rContents) const
	{
		const TDF_Label& rkOcctLabel = GetOcctLabel();
		if (rkOcctLabel.IsNull())
		{
			return;
		}

		// Get the list of attribute shapes in the child labels in all levels, therefore no need to iterate hierarchically.
		for (TDF_ChildIterator occtLabelIterator(rkOcctLabel, kAllLevels); occtLabelIterator.More(); occtLabelIterator.Next())
		{
			TDF_Label childLabel = occtLabelIterator.Value();
			// Only care about those labels with aperture or other non-constituent relationships.
			Handle(TNaming_NamedShape) occtApertureAttribute;
			Handle(TDataStd_Integer) occtRelationshipType;
			bool result1 = childLabel.FindAttribute(TNaming_NamedShape::GetID(), occtApertureAttribute);
			bool result2 = childLabel.FindAttribute(TDataStd_Integer::GetID(), occtRelationshipType);
			int result3 = occtRelationshipType->Get();
			if (result1 &&
				result2 &&
				occtRelationshipType->Get() != REL_CONSTITUENT)
			{
				std::shared_ptr<Topology> pTopology = Topology::ByOcctShape(occtApertureAttribute->Get());
				pTopology->SetOcctLabel(childLabel);
				rContents.push_back(pTopology);
			}
		}
	}

	bool Topology::SaveToBrep(const std::string & rkPath) const
	{
		return BRepTools::Write(GetOcctShape(), rkPath.c_str());;
	}

	std::shared_ptr<Topology> Topology::LoadFromBrep(const std::string & rkPath)
	{
		TopoDS_Shape occtShape;
		BRep_Builder occtBRepBuilder;
		bool returnValue = BRepTools::Read(occtShape, rkPath.c_str(), occtBRepBuilder);

		return Topology::ByOcctShape(occtShape);
	}

	std::string Topology::Analyze(const TopoDS_Shape& rkShape, const int kLevel)
	{
		BOPCol_ListOfShape occtImmediateMembers;
		ImmediateMembers(rkShape, occtImmediateMembers);

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
		for (BOPCol_ListIteratorOfListOfShape kMemberIterator(occtImmediateMembers);
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
				"The shape is " << occtShapeNameSingular[occtShapeType] << "." << std::endl <<
				"Number of cell complexes = " << numberCompSolids << std::endl <<
				"Number of cells = " << occtShapeAnalysis.NbSharedSolids() << std::endl <<
				"Number of shells = " << occtShapeAnalysis.NbSharedShells() << std::endl <<
				"Number of faces = " << occtShapeAnalysis.NbSharedFaces() << std::endl <<
				"Number of wires = " << occtShapeAnalysis.NbSharedWires() << std::endl <<
				"Number of edges = " << occtShapeAnalysis.NbSharedEdges() << std::endl <<
				"Number of vertices = " << occtShapeAnalysis.NbSharedVertices() << std::endl <<
				std::endl << std::endl <<
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

		for (BOPCol_ListIteratorOfListOfShape kMemberIterator(occtImmediateMembers);
			kMemberIterator.More();
			kMemberIterator.Next())
		{
			const TopoDS_Shape& rkMemberTopology = kMemberIterator.Value();
			std::string strMemberAnalyze = Analyze(rkMemberTopology, kLevel + 1);
			ssCurrentResult << strMemberAnalyze;
		}

		return ssCurrentResult.str();
	}

	std::string Topology::Analyze()
	{
		return Analyze(GetOcctShape(), 0);
	}

	void Topology::BooleanImages(
		const std::shared_ptr<Topology>& kpOtherTopology,
		std::list<std::shared_ptr<Topology>>& kArgumentImagesInArguments,
		std::list<std::shared_ptr<Topology>>& kArgumentImagesInTools,
		std::list<std::shared_ptr<Topology>>& kToolsImagesInArguments,
		std::list<std::shared_ptr<Topology>>& kToolsImagesInTools)
	{
		BOPCol_ListOfShape occtArgumentImagesInArguments;
		BOPCol_ListOfShape occtArgumentImagesInTools;
		BOPCol_ListOfShape occtToolsImagesInArguments;
		BOPCol_ListOfShape occtToolsImagesInTools;
		BOPCol_ListOfShape occtExclusivelyArgumentImages;
		BOPCol_ListOfShape occtExclusivelyToolImages;
		BOPCol_ListOfShape occtSharedImages;
		BOPCol_ListOfShape occtExclusivelyArgumentImageFaces;
		BOPCol_ListOfShape occtExclusivelyToolImageFaces;
		BOPCol_ListOfShape occtSharedImageFaces;
;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BooleanImages(
			kpOtherTopology,
			occtCellsBuilder,
			occtExclusivelyArgumentImages,
			occtExclusivelyToolImages,
			occtSharedImages,
			occtExclusivelyArgumentImageFaces,
			occtExclusivelyToolImageFaces,
			occtSharedImageFaces,
			occtArgumentImagesInArguments, 
			occtArgumentImagesInTools, 
			occtToolsImagesInArguments, 
			occtToolsImagesInTools);

		for (BOPCol_ListIteratorOfListOfShape kImageIterator(occtArgumentImagesInArguments);
			kImageIterator.More();
			kImageIterator.Next())
		{
			kArgumentImagesInArguments.push_back(Topology::ByOcctShape(kImageIterator.Value()));
		}
		for (BOPCol_ListIteratorOfListOfShape kImageIterator(occtArgumentImagesInTools);
			kImageIterator.More();
			kImageIterator.Next())
		{
			kArgumentImagesInTools.push_back(Topology::ByOcctShape(kImageIterator.Value()));
		}
		for (BOPCol_ListIteratorOfListOfShape kImageIterator(occtToolsImagesInArguments);
			kImageIterator.More();
			kImageIterator.Next())
		{
			kToolsImagesInArguments.push_back(Topology::ByOcctShape(kImageIterator.Value()));
		}
		for (BOPCol_ListIteratorOfListOfShape kImageIterator(occtToolsImagesInTools);
			kImageIterator.More();
			kImageIterator.Next())
		{
			kToolsImagesInTools.push_back(Topology::ByOcctShape(kImageIterator.Value()));
		}
	}

	void Topology::BooleanImages(
		const std::shared_ptr<Topology>& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctExclusivelyArgumentImages,
		BOPCol_ListOfShape& rOcctExclusivelyToolImages,
		BOPCol_ListOfShape& rOcctSharedImages,
		BOPCol_ListOfShape& rOcctExclusivelyArgumentImageFaces,
		BOPCol_ListOfShape& rOcctExclusivelyToolImageFaces,
		BOPCol_ListOfShape& rOcctSharedImageFaces,
		BOPCol_ListOfShape& rOcctArgumentImagesInArguments, 
		BOPCol_ListOfShape& rOcctArgumentImagesInTools, 
		BOPCol_ListOfShape& rOcctToolsImagesInArguments, 
		BOPCol_ListOfShape& rOcctToolsImagesInTools)
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		if (GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex* pCellComplex = TopologicalQuery::Downcast<CellComplex>(this);
			std::list<std::shared_ptr<Cell>> cells;
			pCellComplex->Cells(cells);
			for (const std::shared_ptr<Cell>& kCell : cells)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(kCell->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsA.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsA.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		if (kpOtherTopology->GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			std::shared_ptr<CellComplex> pCellComplex = TopologicalQuery::Downcast<CellComplex>(kpOtherTopology);
			std::list<std::shared_ptr<Cell>> cells;
			pCellComplex->Cells(cells);
			for (const std::shared_ptr<Cell>& kCell : cells)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(kCell->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsB.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(kpOtherTopology->GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsB.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		rOcctCellsBuilder.SetArguments(occtCellsBuildersArguments);

		// Split the arguments and tools
		rOcctCellsBuilder.Perform();

		if (rOcctCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			rOcctCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
		const TopoDS_Shape& rMergeResult = rOcctCellsBuilder.Shape();

		const BOPCol_DataMapOfShapeListOfShape& rkImages = rOcctCellsBuilder.Images();
		int size = rkImages.Size();

		const TopoDS_Shape& rkParts = rOcctCellsBuilder.GetAllParts();
		std::shared_ptr<Topology> pTopologyParts = Topology::ByOcctShape(rkParts);
		std::string strParts = pTopologyParts->Analyze();

		// 3. Classify the images: exclusively from argument, exclusively from tools, or shared.
		for (BOPCol_ListIteratorOfListOfShape kArgumentIterator(occtCellsBuildersOperandsA);
			kArgumentIterator.More();
			kArgumentIterator.Next())
		{
			const BOPCol_ListOfShape& rkArgumentImages = rkImages.Find(kArgumentIterator.Value());

			for (BOPCol_ListIteratorOfListOfShape kArgumentImageIterator(rkArgumentImages);
				kArgumentImageIterator.More();
				kArgumentImageIterator.Next())
			{
				const TopoDS_Shape& rkArgumentImage = kArgumentImageIterator.Value();

				// Is this in tool too?
				bool isArgumentImageInTools = false;
				for (BOPCol_ListIteratorOfListOfShape kToolIterator(occtCellsBuildersOperandsB);
					kArgumentImageIterator.More() && !isArgumentImageInTools;
					kArgumentImageIterator.Next())
				{
					const BOPCol_ListOfShape& rkToolImages = rkImages.Find(kToolIterator.Value());

					for (BOPCol_ListIteratorOfListOfShape kToolImageIterator(rkToolImages);
						kToolImageIterator.More() && !isArgumentImageInTools;
						kToolImageIterator.Next())
					{
						if (rkArgumentImage.IsSame(kToolImageIterator.Value()))
						{
							isArgumentImageInTools = true;
						}
					}
				}

				if (isArgumentImageInTools)
				{
					rOcctSharedImages.Append(rkArgumentImage);
				}
				else
				{
					rOcctExclusivelyArgumentImages.Append(rkArgumentImage);
				}
			}
		}

		for (BOPCol_ListIteratorOfListOfShape kToolIterator(occtCellsBuildersOperandsB);
			kToolIterator.More();
			kToolIterator.Next())
		{
			const BOPCol_ListOfShape& rkToolImages = rkImages.Find(kToolIterator.Value());

			for (BOPCol_ListIteratorOfListOfShape kToolImageIterator(rkToolImages);
				kToolImageIterator.More();
				kToolImageIterator.Next())
			{
				const TopoDS_Shape& rkToolImage = kToolImageIterator.Value();

				// Is this in tool too?
				bool isToolInArguments = false;
				for (BOPCol_ListIteratorOfListOfShape kArgumentIterator(occtCellsBuildersOperandsA);
					kArgumentIterator.More() && !isToolInArguments;
					kArgumentIterator.Next())
				{
					const BOPCol_ListOfShape& rkArgumentImages = rkImages.Find(kArgumentIterator.Value());

					for (BOPCol_ListIteratorOfListOfShape kArgumentImageIterator(rkArgumentImages);
						kArgumentImageIterator.More() && !isToolInArguments;
						kArgumentImageIterator.Next())
					{
						if (rkToolImage.IsSame(kArgumentImageIterator.Value()))
						{
							isToolInArguments = true;
						}
					}
				}

				if (!isToolInArguments)
				{
					rOcctExclusivelyToolImages.Append(rkToolImage);
				}

				// The shared images have been taken care of; no need to check them.
			}
		}

		// 4. Get the faces
		for (BOPCol_ListIteratorOfListOfShape kArgumentImageIterator(rOcctExclusivelyArgumentImages);
			kArgumentImageIterator.More();
			kArgumentImageIterator.Next())
		{
			for (TopExp_Explorer occtArgumentImageExplorer(kArgumentImageIterator.Value(), TopAbs_FACE); 
				 occtArgumentImageExplorer.More(); 
				 occtArgumentImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtArgumentImageExplorer.Current();
				if (!rOcctExclusivelyArgumentImageFaces.Contains(rkOcctCurrent))
				{
					rOcctExclusivelyArgumentImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		for (BOPCol_ListIteratorOfListOfShape kToolImageIterator(rOcctExclusivelyToolImages);
			kToolImageIterator.More();
			kToolImageIterator.Next())
		{
			for (TopExp_Explorer occtToolImageExplorer(kToolImageIterator.Value(), TopAbs_FACE); 
				 occtToolImageExplorer.More(); 
				 occtToolImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtToolImageExplorer.Current();
				if (!rOcctExclusivelyToolImageFaces.Contains(rkOcctCurrent))
				{
					rOcctExclusivelyToolImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		for (BOPCol_ListIteratorOfListOfShape kSharedImageIterator(rOcctSharedImages);
			kSharedImageIterator.More();
			kSharedImageIterator.Next())
		{
			for (TopExp_Explorer occtSharedImageExplorer(kSharedImageIterator.Value(), TopAbs_FACE); 
				 occtSharedImageExplorer.More(); 
				 occtSharedImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtSharedImageExplorer.Current();
				if (!rOcctSharedImageFaces.Contains(rkOcctCurrent))
				{
					rOcctSharedImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		// 5. Identify the argument faces in a tool by doing IsSame Comparison
		for (BOPCol_ListIteratorOfListOfShape kArgumentImageFaceIterator(rOcctExclusivelyArgumentImageFaces);
			kArgumentImageFaceIterator.More();
			kArgumentImageFaceIterator.Next())
		{
			bool isInSharedImage = false;
			const TopoDS_Shape& rkArgumentImageFace = kArgumentImageFaceIterator.Value();
			for (BOPCol_ListIteratorOfListOfShape kSharedImageFaceIterator(rOcctSharedImageFaces);
				kSharedImageFaceIterator.More() && !isInSharedImage;
				kSharedImageFaceIterator.Next())
			{
				if (rkArgumentImageFace.IsSame(kSharedImageFaceIterator.Value()))
				{
					isInSharedImage = true;
				}
			}

			if (isInSharedImage)
			{
				// part of tools in the arguments
				rOcctToolsImagesInArguments.Append(rkArgumentImageFace);
			}
			else
			{
				rOcctArgumentImagesInArguments.Append(rkArgumentImageFace);
			}
		}

		// 6. Identify the tool faces in an image by doing IsSame Comparison
		for (BOPCol_ListIteratorOfListOfShape kToolImageFaceIterator(rOcctExclusivelyToolImageFaces);
			kToolImageFaceIterator.More();
			kToolImageFaceIterator.Next())
		{
			bool isInSharedImage = false;
			const TopoDS_Shape& rkToolImageFace = kToolImageFaceIterator.Value();
			for (BOPCol_ListIteratorOfListOfShape kSharedImageFaceIterator(rOcctSharedImageFaces);
				kSharedImageFaceIterator.More() && !isInSharedImage;
				kSharedImageFaceIterator.Next())
			{
				if (rkToolImageFace.IsSame(kSharedImageFaceIterator.Value()))
				{
					isInSharedImage = true;
				}
			}

			if (isInSharedImage)
			{
				// part of arguments in the tools
				rOcctArgumentImagesInTools.Append(rkToolImageFace);
			}
			else
			{
				rOcctToolsImagesInTools.Append(rkToolImageFace);
			}
		}

		// 7. Identify faces inside the shared images that are inside the arguments or tools.
		for (BOPCol_ListIteratorOfListOfShape kSharedImageFaceIterator(rOcctSharedImageFaces);
			kSharedImageFaceIterator.More();
			kSharedImageFaceIterator.Next())
		{
			bool isInTools = false;
			const TopoDS_Shape& rkSharedImageFace = kSharedImageFaceIterator.Value();
			for (BOPCol_ListIteratorOfListOfShape kToolImageFaceIterator(rOcctExclusivelyToolImageFaces);
				kToolImageFaceIterator.More() && !isInTools;
				kToolImageFaceIterator.Next())
			{
				if (rkSharedImageFace.IsSame(kToolImageFaceIterator.Value()))
				{
					isInTools = true;
					rOcctArgumentImagesInTools.Append(rkSharedImageFace);
				}
			}

			// If not in the arguments, then it is in the tools.
			if (!isInTools)
			{
				rOcctToolsImagesInArguments.Append(rkSharedImageFace);
			}
		}
	}

	void Topology::BooleanParts(
		const std::shared_ptr<Topology>& kpOtherTopology,
		std::list<std::shared_ptr<Topology>>& rSpaceBetween_A_A_and_B_A,
		std::list<std::shared_ptr<Topology>>& rSpaceBetween_B_A_and_A_B,
		std::list<std::shared_ptr<Topology>>& rSpaceBetween_A_B_and_B_B)
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		if (GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex* pCellComplex = TopologicalQuery::Downcast<CellComplex>(this);
			std::list<std::shared_ptr<Cell>> cells;
			pCellComplex->Cells(cells);
			for (const std::shared_ptr<Cell>& kpCell : cells)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(kpCell->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsA.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsA.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		if (kpOtherTopology->GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			std::shared_ptr<CellComplex> pCellComplex = TopologicalQuery::Downcast<CellComplex>(kpOtherTopology);
			std::list<std::shared_ptr<Cell>> cells;
			pCellComplex->Cells(cells);
			for (const std::shared_ptr<Cell>& kpCell : cells)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(kpCell->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsB.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(kpOtherTopology->GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsB.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		BOPAlgo_CellsBuilder occtCellsBuilder;
		occtCellsBuilder.SetArguments(occtCellsBuildersArguments);

		// Split the arguments and tools
		occtCellsBuilder.Perform();

		if (occtCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
		const TopoDS_Shape& rMergeResult = occtCellsBuilder.Shape();

		const BOPCol_DataMapOfShapeListOfShape& rkImages = occtCellsBuilder.Images();


		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		// A_A and B_A
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIterator(occtCellsBuildersOperandsA);
			kOcctShapeIterator.More();
			kOcctShapeIterator.Next())
		{
			occtCellsBuilder.RemoveAllFromResult();
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIterator.Value());

			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorB.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			occtCellsBuilder.MakeContainers();
			const TopoDS_Shape& occtBooleanResult = occtCellsBuilder.Shape();
			std::shared_ptr<Topology> pTopology = Topology::ByOcctShape(occtBooleanResult);
			rSpaceBetween_A_A_and_B_A.push_back(pTopology);
		}

		// B_A and A_B
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIterator(occtCellsBuildersOperandsA);
			kOcctShapeIterator.More();
			kOcctShapeIterator.Next())
		{
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtCellsBuilder.RemoveAllFromResult();
				occtListToTake.Clear();
				occtListToAvoid.Clear();
				occtListToTake.Append(kOcctShapeIterator.Value());
				occtListToTake.Append(kOcctShapeIteratorB.Value());
				occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
				occtCellsBuilder.MakeContainers();
				const TopoDS_Shape& occtBooleanResult = occtCellsBuilder.Shape();
				std::shared_ptr<Topology> pTopology = Topology::ByOcctShape(occtBooleanResult);
				rSpaceBetween_B_A_and_A_B.push_back(pTopology);
			}
		}

		// A_B and B_B
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
			kOcctShapeIteratorB.More();
			kOcctShapeIteratorB.Next())
		{
			occtCellsBuilder.RemoveAllFromResult();
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorB.Value());

			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIterator(occtCellsBuildersOperandsA);
				kOcctShapeIterator.More();
				kOcctShapeIterator.Next())
			{
				occtListToAvoid.Append(kOcctShapeIterator.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			occtCellsBuilder.MakeContainers();
			const TopoDS_Shape& occtBooleanResult = occtCellsBuilder.Shape();
			std::shared_ptr<Topology> pTopology = Topology::ByOcctShape(occtBooleanResult);
			rSpaceBetween_A_B_and_B_B.push_back(pTopology);
		}
	}

	void Topology::BooleanOperation(
		const std::shared_ptr<Topology>& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsB,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	{
		// Buffer lists are currently provided for cell complexes to be processed per cells.
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

	std::shared_ptr<Topology> Topology::GetBooleanResult(
		const std::shared_ptr<Topology>& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	{
		rOcctCellsBuilder.MakeContainers();
		const TopoDS_Shape& occtBooleanResult = rOcctCellsBuilder.Shape();
		std::shared_ptr<Topology> pTopology = ManageBooleanLabels(kpOtherTopology, rOcctCellsBuilder, rOcctMapFaceToFixedFaceA, rOcctMapFaceToFixedFaceB);
		
		return pTopology;
	}

	std::shared_ptr<Topology> Topology::ManageBooleanLabels(
		const std::shared_ptr<Topology>& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	{
		// Add the output cluster to the root.
		const TopoDS_Shape& rkBooleanResult = rOcctCellsBuilder.Shape();
		std::shared_ptr<Topology> pBooleanResult = Topology::ByOcctShape(rkBooleanResult);
		GlobalCluster::GetInstance().GetCluster()->AddChildLabel(pBooleanResult, REL_CONSTITUENT);

		// For now, only map the faces and their apertures to the only cell complex in the output cluster.
		TopTools_MapOfShape occtCompSolids; 
		for (TopExp_Explorer occtExplorer(rOcctCellsBuilder.Shape(), TopAbs_COMPSOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtCompSolids.Contains(occtCurrent))
			{
				occtCompSolids.Add(occtCurrent);
			}
		}
		if (occtCompSolids.Size() == 0)
		{
			return pBooleanResult;
		}

		// Add the cell complex as the constituent child label to the cluster.
		const TopoDS_Shape& rkCompSolid = *occtCompSolids.cbegin();
		std::shared_ptr<Topology> pCellComplex = Topology::ByOcctShape(rkCompSolid);
		pBooleanResult->AddChildLabel(pCellComplex, REL_CONSTITUENT);
		
		// Get the input face labels. These are the labels of the original faces which are not fixed yet,
		// so we need to map these faces to the fixed ones.
		for (TDF_ChildIterator occtLabelIterator(GetOcctLabel()); occtLabelIterator.More(); occtLabelIterator.Next())
		{
			TDF_Label childLabel = occtLabelIterator.Value();
			Handle(TNaming_NamedShape) occtChildShapeAttribute;
			Handle(TDataStd_Integer) occtRelationshipType;
			bool result1 = childLabel.FindAttribute(TNaming_NamedShape::GetID(), occtChildShapeAttribute);
			bool result2 = childLabel.FindAttribute(TDataStd_Integer::GetID(), occtRelationshipType);
			int result3 = occtRelationshipType->Get();

			TopoDS_Shape occtFixedFace;
			try{
				occtFixedFace = rOcctMapFaceToFixedFaceA.Find(occtChildShapeAttribute->Get());
			}
			catch (...)
			{

			}

			if (result1 &&
				result2 &&
				occtRelationshipType->Get() == REL_CONSTITUENT &&
				!occtFixedFace.IsNull())
			{
				// Add this label to fixed face label to the cell complex
				TopTools_ListOfShape occtModifiedFixedFaces = rOcctCellsBuilder.Modified(occtFixedFace);

				for(TopTools_ListIteratorOfListOfShape occtModifiedFixedFaceIterator(occtModifiedFixedFaces);
					occtModifiedFixedFaceIterator.More();
					occtModifiedFixedFaceIterator.Next())
				{
					const TopoDS_Shape& rkOcctModifiedFixedFace = occtModifiedFixedFaceIterator.Value();
					std::shared_ptr<Topology> pModifiedFixedFace = Topology::ByOcctShape(rkOcctModifiedFixedFace);
					pCellComplex->AddChildLabel(pModifiedFixedFace, REL_CONSTITUENT);

					// Transfer the apertures from the original face (if any) to the fixed face
					for (TDF_ChildIterator occtFaceLabelIterator(childLabel); occtFaceLabelIterator.More(); occtFaceLabelIterator.Next())
					{
						TDF_Label apertureLabel = occtFaceLabelIterator.Value();
						Handle(TNaming_NamedShape) occtApertureShapeAttribute;
						Handle(TDataStd_Integer) occtApertureRelationshipType;
						bool result1 = apertureLabel.FindAttribute(TNaming_NamedShape::GetID(), occtApertureShapeAttribute);
						bool result2 = apertureLabel.FindAttribute(TDataStd_Integer::GetID(), occtApertureRelationshipType);
						int result3 = occtApertureRelationshipType->Get();

						if (result1 &&
							result2 &&
							occtApertureRelationshipType->Get() != REL_CONSTITUENT)
						{
							std::shared_ptr<Topology> pAperture = Topology::ByOcctShape(occtApertureShapeAttribute->Get());
							pModifiedFixedFace->AddChildLabel(pAperture, (TopologyRelationshipType) occtApertureRelationshipType->Get());
						}
					}
				}
			}
		}

		return pBooleanResult;
	}

	std::shared_ptr<Topology> Topology::Difference(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorB.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);
	}

	std::shared_ptr<Topology> Topology::Impose(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		// Get part of A not in B
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorB.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		// Add B
		int i = 1;
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
			kOcctShapeIteratorB.More();
			kOcctShapeIteratorB.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorB.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, i++, true);
		}

		return GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);
	}

	std::shared_ptr<Topology> Topology::Imprint(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
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

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			occtListToAvoid.Append(occtCellsBuildersOperandsB);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);
	}

	std::shared_ptr<Topology> Topology::Intersection(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
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

		return GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);
	}

	std::shared_ptr<Topology> Topology::Merge(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
			kOcctShapeIteratorB.More();
			kOcctShapeIteratorB.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorB.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);
	}

	std::shared_ptr<Topology> Topology::Slice(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		// Map the input faces of the topology to the 
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
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

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			occtListToAvoid.Append(occtCellsBuildersOperandsB);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		std::shared_ptr<Topology> pResult = GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);

		return pResult;
	}

	void Topology::AddUnionInternalStructure(const TopoDS_Shape& rkOcctShape, BOPCol_ListOfShape& rUnionArguments)
	{
		TopAbs_ShapeEnum occtShapeType = rkOcctShape.ShapeType();
		std::shared_ptr<Topology> pTopology = Topology::ByOcctShape(rkOcctShape);
		std::list<std::shared_ptr<Face>> faces;
		// Cell complex -> faces not part of the envelope
		// Cell -> inner shells
		// Shell --> inner wires of the faces
		// Face --> inner wires
		// Wire --> n/a
		// Edge --> n/a
		// Vertex --> n/a
		if (occtShapeType == TopAbs_COMPOUND)
		{
			std::shared_ptr<Cluster> pCluster = Topology::Downcast<Cluster>(pTopology);
			std::list<std::shared_ptr<Topology>> immediateMembers;
			pCluster->ImmediateMembers(immediateMembers);
			for (const std::shared_ptr<Topology>& kpImmediateMember : immediateMembers)
			{
				AddUnionInternalStructure(kpImmediateMember->GetOcctShape(), rUnionArguments);
			}
		} else if (occtShapeType == TopAbs_COMPSOLID)
		{
			std::shared_ptr<CellComplex> pCellComplex = Topology::Downcast<CellComplex>(pTopology);
			pCellComplex->InnerBoundaries(faces);
			for (const std::shared_ptr<Face>& kpInternalFace : faces)
			{
				rUnionArguments.Append(kpInternalFace->GetOcctShape());
			}
		}
		else if (occtShapeType == TopAbs_SOLID)
		{
			std::shared_ptr<Cell> pCell = Topology::Downcast<Cell>(pTopology);
			std::list<std::shared_ptr<Shell>> shells;
			pCell->InnerBoundaries(shells);
			for (const std::shared_ptr<Shell>& kpInternalShell : shells)
			{
				rUnionArguments.Append(kpInternalShell->GetOcctShape());
			}
		}
		else if (occtShapeType == TopAbs_SHELL)
		{
			for (TopExp_Explorer occtShellExplorer(rkOcctShape, TopAbs_FACE); occtShellExplorer.More(); occtShellExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrentFace = occtShellExplorer.Current();
				TopoDS_Wire occtOuterWire = BRepTools::OuterWire(TopoDS::Face(rkOcctCurrentFace));

				for (TopExp_Explorer occtFaceExplorer(rkOcctShape, TopAbs_WIRE); occtFaceExplorer.More(); occtFaceExplorer.Next())
				{
					const TopoDS_Shape& rkOcctCurrentFace = occtFaceExplorer.Current();
					if (!rUnionArguments.Contains(rkOcctCurrentFace) && !rkOcctCurrentFace.IsSame(occtOuterWire))
					{
						rUnionArguments.Append(rkOcctCurrentFace);
					}
				}
			}
		}
		else if (occtShapeType == TopAbs_FACE)
		{
			TopoDS_Wire occtOuterWire = BRepTools::OuterWire(TopoDS::Face(rkOcctShape));

			for (TopExp_Explorer occtExplorer(rkOcctShape, TopAbs_WIRE); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& occtCurrent = occtExplorer.Current();
				if (!rUnionArguments.Contains(occtCurrent) && !occtCurrent.IsSame(occtOuterWire))
				{
					rUnionArguments.Append(occtCurrent);
				}
			}
		}
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

	void Topology::RegisterLabel()
	{
		LabelManager::GetInstance().AddLabelToRoot(GetOcctLabel());

		// Add the essential attributes:
		// - Shape
		LabelManager::AddShapeToLabel(GetOcctShape(), GetOcctLabel());

		// - Counter
		LabelManager::AddCounterToLabel(GetOcctLabel());

		// - Contents and contexts
		LabelManager::AddContentsContextsToLabel(GetOcctLabel());
	}

	void Topology::DecreaseCounter()
	{
		LabelManager::GetInstance().DecreaseCounter(GetOcctLabel());
	}

	void Topology::AddBooleanOperands(
		const std::shared_ptr<Topology>& kpOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsB,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
		BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB)
	{
		// Buffer lists are currently provided for cell complexes to be processed per cells.
		BOPCol_ListOfShape occtCellsBuildersArguments;
		if (GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex* pCellComplex = TopologicalQuery::Downcast<CellComplex>(this);
			std::list<std::shared_ptr<Cell>> cells;
			pCellComplex->Cells(cells);
			for (const std::shared_ptr<Cell>& kpCell : cells)
			{
				TopoDS_Shape occtNewShape = FixBooleanOperandFace(kpCell->GetOcctShape(), rOcctMapFaceToFixedFaceA);
				occtNewShape = FixBooleanOperandShell(kpCell->GetOcctShape());
				occtNewShape = FixBooleanOperandCell(kpCell->GetOcctShape());
				rOcctCellsBuildersOperandsA.Append(kpCell->GetOcctShape());
				occtCellsBuildersArguments.Append(kpCell->GetOcctShape());
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

		if (kpOtherTopology->GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			std::shared_ptr<CellComplex> kpkCellComplex = TopologicalQuery::Downcast<CellComplex>(kpOtherTopology);
			std::list<std::shared_ptr<Cell>> cells;
			kpkCellComplex->Cells(cells);
			for (const std::shared_ptr<Cell>& kpCell : cells)
			{
				TopoDS_Shape occtNewShape = FixBooleanOperandFace(kpCell->GetOcctShape(), rOcctMapFaceToFixedFaceB);
				occtNewShape = FixBooleanOperandShell(kpCell->GetOcctShape());
				occtNewShape = FixBooleanOperandCell(kpCell->GetOcctShape());
				rOcctCellsBuildersOperandsB.Append(kpCell->GetOcctShape());
				occtCellsBuildersArguments.Append(kpCell->GetOcctShape());
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

	std::shared_ptr<Topology> Topology::Union(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIterator(occtCellsBuildersOperandsA);
			kOcctShapeIterator.More();
			kOcctShapeIterator.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIterator.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}

		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIterator(occtCellsBuildersOperandsB);
			kOcctShapeIterator.More();
			kOcctShapeIterator.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIterator.Value());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}

		std::shared_ptr<Topology> pTopology = GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);

		BOPAlgo_CellsBuilder occtCellsBuilder2;
		BOPCol_ListOfShape occtCellsBuildersArguments;
		occtCellsBuildersArguments.Append(pTopology->GetOcctShape());

		// Get the internal boundaries
		AddUnionInternalStructure(GetOcctShape(), occtCellsBuildersArguments);
		AddUnionInternalStructure(kpOtherTopology->GetOcctShape(), occtCellsBuildersArguments);
		
		if (occtCellsBuildersArguments.Size() < 2)
		{
			return Topology::ByOcctShape(occtCellsBuildersArguments.First());
		}

		occtCellsBuilder2.SetArguments(occtCellsBuildersArguments);
		occtCellsBuilder2.Perform();

		if (occtCellsBuilder2.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder2.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
		occtCellsBuilder2.AddAllToResult();
		TopoDS_Shape occtUnionResult = occtCellsBuilder2.Shape();
		std::shared_ptr<Topology> pUnionTopology = Topology::ByOcctShape(occtUnionResult);
		return pUnionTopology;
	}

	std::shared_ptr<Topology> Topology::XOR(const std::shared_ptr<Topology>& kpOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceA;
		BOPCol_DataMapOfShapeShape occtMapFaceToFixedFaceB;

		BooleanOperation(kpOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB, occtMapFaceToFixedFaceA, occtMapFaceToFixedFaceB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;
		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
			kOcctShapeIteratorA.More();
			kOcctShapeIteratorA.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorA.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
				kOcctShapeIteratorB.More();
				kOcctShapeIteratorB.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorB.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}


		for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorB(occtCellsBuildersOperandsB);
			kOcctShapeIteratorB.More();
			kOcctShapeIteratorB.Next())
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(kOcctShapeIteratorB.Value());
			for (BOPCol_ListIteratorOfListOfShape kOcctShapeIteratorA(occtCellsBuildersOperandsA);
				kOcctShapeIteratorA.More();
				kOcctShapeIteratorA.Next())
			{
				occtListToAvoid.Append(kOcctShapeIteratorA.Value());
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(
			kpOtherTopology,
			occtCellsBuilder,
			occtMapFaceToFixedFaceA,
			occtMapFaceToFixedFaceB);
	}

	void Topology::ImmediateMembers(const TopoDS_Shape& rkShape, BOPCol_ListOfShape& rImmediateMembers)
	{
		for (TopoDS_Iterator occtShapeIterator(rkShape); occtShapeIterator.More(); occtShapeIterator.Next())
		{
			rImmediateMembers.Append(occtShapeIterator.Value());
		}
	}

	void Topology::ImmediateMembers(std::list<std::shared_ptr<Topology>>& rImmediateMembers) const
	{
		BOPCol_ListOfShape occtListMembers;
		Topology::ImmediateMembers(GetOcctShape(), occtListMembers);
		for (BOPCol_ListIteratorOfListOfShape occtIterator(occtListMembers);
			occtIterator.More();
			occtIterator.Next())
		{
			std::shared_ptr<Topology> pMemberTopology = Topology::ByOcctShape(occtIterator.Value());

			if (!GetOcctLabel().IsNull())
			{
				// Does this topology have a label in the hierachy under the topology? If yes, set the label.
				for (TDF_ChildIterator occtLabelIterator(GetOcctLabel(), true); occtLabelIterator.More(); occtLabelIterator.Next())
				{
					TDF_Label childLabel = occtLabelIterator.Value();
					Handle(TNaming_NamedShape) occtChildShape;
					Handle(TDataStd_Integer) occtRelationshipType;
					bool result1 = childLabel.FindAttribute(TNaming_NamedShape::GetID(), occtChildShape);
					bool result2 = childLabel.FindAttribute(TDataStd_Integer::GetID(), occtRelationshipType);
					int result3 = occtRelationshipType->Get();
					if (result1 &&
						result2 &&
						occtRelationshipType->Get() == REL_CONSTITUENT &&
						occtChildShape->Get().IsSame(occtIterator.Value()))
					{
						pMemberTopology->SetOcctLabel(childLabel);
					}
				}
			}

			rImmediateMembers.push_back(pMemberTopology);
		}
	}

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

	void Topology::DownwardNavigation(const TopoDS_Shape& rkOcctShape, const TopAbs_ShapeEnum & rkShapeEnum, TopTools_ListOfShape & rOcctMembers)
	{
		for (TopExp_Explorer occtExplorer(rkOcctShape, rkShapeEnum); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!rOcctMembers.Contains(occtCurrent))
			{
				rOcctMembers.Append(occtCurrent);
			}
		}
	}

	void Topology::AddIngredientTo(const std::shared_ptr<Topology>& kpTopology)
	{
		if(!IsIngredientTo(kpTopology))
		{
			m_ingredientTo.push_back(kpTopology);
			kpTopology->m_ingredients.push_back(shared_from_this());

			GlobalCluster::GetInstance().Remove(this);
		}
	}

	void Topology::RemoveIngredientTo(const std::shared_ptr<Topology>& kpTopology)
	{
		m_ingredientTo.remove(kpTopology);
		kpTopology->m_ingredients.remove(shared_from_this());
		if (m_ingredientTo.empty())
		{
			GlobalCluster::GetInstance().Add(this);
		}
	}

	bool Topology::IsIngredientTo(const std::shared_ptr<Topology>& kpTopology) const
	{
		return std::find(m_ingredientTo.begin(), m_ingredientTo.end(), kpTopology) != m_ingredientTo.end();
	}

	bool Topology::IsIngredient() const
	{
		return !m_ingredientTo.empty();
	}

	bool Topology::IsSame(const std::shared_ptr<Topology>& kpTopology) const
	{
		bool isSame = GetOcctShape().IsSame(kpTopology->GetOcctShape());
		return isSame;
	}

	void Topology::Members(TopTools_ListOfShape& rOcctMembers) const
	{
		const TopoDS_Shape& rkOcctShape = GetOcctShape();
		// Store the children
		for (int i = ((int)rkOcctShape.ShapeType()) + 1; i < (int)TopAbs_SHAPE; ++i)
		{
			TopAbs_ShapeEnum occtShapeEnum = (TopAbs_ShapeEnum)i;
			TopTools_ListOfShape occtMembers;
			Topology::DownwardNavigation(rkOcctShape, occtShapeEnum, occtMembers);
			for (TopTools_ListIteratorOfListOfShape occtMembersIterator(occtMembers);
				occtMembersIterator.More();
				occtMembersIterator.Next())
			{
				rOcctMembers.Append(occtMembersIterator.Value());
			}
		}
	}

	void Topology::Members(std::list<std::shared_ptr<Topology>>& rMembers) const
	{
		TopTools_ListOfShape occtMembers;
		Members(occtMembers);
		for (TopTools_ListIteratorOfListOfShape occtMemberIterator(occtMembers); occtMemberIterator.More(); occtMemberIterator.Next())
		{
			TDF_Label occtLabel;
			LabelManager::GetInstance().FindChildLabelByShape(occtMemberIterator.Value(), occtLabel);
			rMembers.push_back(Topology::ByOcctShape(occtMemberIterator.Value(), occtLabel));
		}
	}

	/*void Topology::AddIngredient(Topology * const kpTopology)
	{
		m_ingredients.push_back(kpTopology);
	}

	void Topology::RemoveIngredient(Topology * const kpTopology)
	{
		m_ingredients.remove(kpTopology);
	}*/
}