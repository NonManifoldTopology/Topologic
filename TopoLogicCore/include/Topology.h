#pragma once

#include "Utilities.h"
#include "TopologicalQuery.h"

#include <BOPCol_ListOfShape.hxx>
#include <BOPCol_DataMapOfShapeShape.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <BRepAlgoAPI_BooleanOperation.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <Standard_Handle.hxx>
#include <Geom_Geometry.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_MapOfShape.hxx>

#include <list>
#include <vector>
#include <map>
#include <memory>

class TopoDS_Shape;

namespace TopologicCore
{
	class Cluster;
	class CellComplex;
	class Cell;
	class Shell;
	class Face;
	class Vertex;
	class Edge;
	class Wire;
	class Context;
	class TopologyFactory;

	/// <summary>
	/// A Topology is an abstract superclass that constructors, properties and methods used by other subclasses that extend it.
	/// </summary>
	class Topology : public TopologicalQuery, public std::enable_shared_from_this<Topology>
	{
	public:
		typedef std::shared_ptr<Topology> Ptr;

	public:
		virtual ~Topology();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rkInstanceGuid"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Topology::Ptr ByOcctShape(const TopoDS_Shape& rkOcctShape, const std::string& rkInstanceGuid = "");

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pGeometry"></param>
		/// <returns></returns>
		static Topology::Ptr ByGeometry(Handle(Geom_Geometry) pGeometry);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpContext"></param>
		/// <returns></returns>
		static Topology::Ptr ByContext(const std::shared_ptr<Context>& kpContext);

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertexCoordinates"></param>
		/// <param name="rkVertexIndices"></param>
		/// <returns></returns>
		static Topology::Ptr ByVertexIndex(const std::list<std::array<double, 3>>& rkVertexCoordinates, const std::list<std::list<int>>& rkVertexIndices);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkVertexIndices"></param>
		/// <param name="rTopologies"></param>
		/// <returns></returns>
		static TOPOLOGIC_API void ByVertexIndex(const std::vector<std::shared_ptr<Vertex>>& rkVertices, const std::list<std::list<int>>& rkVertexIndices, std::list<Topology::Ptr>& rTopologies);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Topology> ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API void AddContent(const Topology::Ptr& rkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopology"></param>
		/// <param name="kTypeFilter"></param>
		/// <returns></returns>
		TOPOLOGIC_API void AddContent(const Topology::Ptr& rkTopology, const int kTypeFilter);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API void RemoveContent(const Topology::Ptr& rkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkContext"></param>
		/// <returns></returns>
		TOPOLOGIC_API void AddContext(const std::shared_ptr<Context>& rkContext);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkContext"></param>
		/// <returns></returns>
		TOPOLOGIC_API void RemoveContext(const std::shared_ptr<Context>& rkContext);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="rkSharedTopologies"></param>
		TOPOLOGIC_API void SharedTopologies(const Topology::Ptr& kpTopology, std::list<Topology::Ptr>& rkSharedTopologies) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpParentTopology"></param>
		/// <param name="kMaxLevels"></param>
		/// <param name="kMaxPaths"></param>
		/// <param name="rkPaths"></param>
		TOPOLOGIC_API void PathsTo(const Topology::Ptr& kpTopology, const Topology::Ptr& kpParentTopology, const int kMaxLevels, const int kMaxPaths, std::list<std::list<std::shared_ptr<TopologicalQuery>>>& rkPaths) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctFaces"></param>
		/// <returns></returns>
		static TopoDS_Shape OcctSewFaces(const TopTools_ListOfShape& rkOcctFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Difference(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Impose(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Imprint(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Intersect(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Merge(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr SelfMerge();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Slice(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Union(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr XOR(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Trim(const Topology::Ptr& kpOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTool"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Divide(const Topology::Ptr& kpTool);

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherTopology"></param>
		/// <param name="rOcctCellsBuilder"></param>
		/// <param name="rOcctMapFaceToFixedFaceA"></param>
		/// <param name="rOcctMapFaceToFixedFaceB"></param>
		/// <returns></returns>
		std::shared_ptr<Topology> TransferBooleanContents(
			const std::shared_ptr<Topology>& kpAnotherTopology,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape1"></param>
		/// <param name="kpTopology2"></param>
		static void TransferContents(const TopoDS_Shape& rkOcctShape1, const Topology::Ptr& kpTopology2);

		
		static void TransferContents(const TopoDS_Shape& rkOcctShape1, const Topology::Ptr& kpTopology2, const BOPCol_ListOfShape& rkOcctDeletedSubshapes);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rContents"></param>
		TOPOLOGIC_API void Contents(std::list<Topology::Ptr>& rContents) const;

		static void Contents(const TopoDS_Shape& rkOcctShape, std::list<Topology::Ptr>& rContents);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rSubContents"></param>
		TOPOLOGIC_API void SubContents(std::list<Topology::Ptr>& rSubContents) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rSubContents"></param>
		static void SubContents(const TopoDS_Shape& rkOcctShape, std::list<Topology::Ptr>& rSubContents);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API bool Contexts(std::list<std::shared_ptr<Context>>& rContexts) const;

		static bool Contexts(const TopoDS_Shape& rkOcctShape, std::list<std::shared_ptr<Context>>& rContexts);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual bool IsManifold() const = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkPath"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool SaveToBrep(const std::string& rkPath) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkPath"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Topology::Ptr LoadFromBrep(const std::string& rkPath);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		static TopoDS_Shape MakeBooleanContainers(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		static TopoDS_Shape Simplify(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API std::string Analyze();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int Dimensionality() const
		{
			return m_dimensionality;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Shape& GetOcctShape() = 0;

		virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape) = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Shape& GetOcctShape() const = 0;

		virtual TopologyType GetType() const = 0;

		/// <summary>
		/// Returns all sub-entities that have no other parents than this topology, i.e. do not belong to other entities.
		/// </summary>
		/// <param name="rSubTopologies">The sub-topologies</param>
		TOPOLOGIC_API void SubTopologies(std::list<Topology::Ptr>& rSubTopologies) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual std::string GetTypeAsString() const = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rAncestors"></param>
		template <class Subclass>
		void UpwardNavigation(std::list<std::shared_ptr<Subclass>>& rAncestors) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctHostTopology"></param>
		/// <param name="rAncestors"></param>
		template <class Subclass>
		void UpwardNavigation(const TopoDS_Shape& rkOcctHostTopology, std::list<std::shared_ptr<Subclass>>& rAncestors) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rkOcctParentShape"></param>
		/// <param name="kShapeEnum"></param>
		/// <param name="rOcctMembers"></param>
		static void UpwardNavigation(const TopoDS_Shape& rkOcctShape, const TopoDS_Shape& rkOcctParentShape, const TopAbs_ShapeEnum kShapeEnum, TopTools_ListOfShape& rOcctMembers);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kOcctShapeType"></param>
		/// <param name="rMembers"></param>
		template <class Subclass>
		void DownwardNavigation(std::list<std::shared_ptr<Subclass>>& rMembers) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rkShapeEnum"></param>
		/// <param name="rOcctMembers"></param>
		static void DownwardNavigation(const TopoDS_Shape& rkOcctShape, const TopAbs_ShapeEnum& rkShapeEnum, TopTools_MapOfShape& rOcctMembers);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr Copy();

		/// <summary>
		/// Copy the whole content/context hierarchy.
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr DeepCopy();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		static TopoDS_Shape CopyOcct(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOriginalSubshape"></param>
		/// <param name="rkNewSubshape"></param>
		/// <returns></returns>
		TOPOLOGIC_API void ReplaceSubentity(const Topology::Ptr& rkOriginalSubshape, const Topology::Ptr& rkNewSubshape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctOriginalSubshape"></param>
		/// <param name="rkOcctNewSubshape"></param>
		void ReplaceSubentity(const TopoDS_Shape& rkOcctOriginalSubshape, const TopoDS_Shape& rkOcctNewSubshape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool IsSame(const Topology::Ptr& kpTopology) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctMembers"></param>
		void Members(TopTools_ListOfShape& rOcctMembers) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rMembers"></param>
		void Members(std::list<Topology::Ptr>& rMembers) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr ClosestSimplestSubshape(const Topology::Ptr& kpTopology) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpSelectorTopology"></param>
		/// <param name="kTypeFilter"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology::Ptr SelectSubtopology(const Topology::Ptr& kpSelectorTopology, const int kTypeFilter = TOPOLOGY_ALL) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual std::shared_ptr<Vertex> CenterOfMass() const = 0;

		/// <summary>
		/// <para>
		/// Identifies the class type by GUID. Used in the factory class system since GUID is easily extendable.
		/// WARNING: Do not use this to identify the instance!
		/// </para>
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual std::string GetClassGUID() const = 0;

		/// <summary>
		/// Identifies the instance type by GUID. Used in the factory class system since GUID is easily extendable.
		/// </summary>
		/// <returns>The GUID</returns>
		TOPOLOGIC_API const std::string GetInstanceGUID() const;

		static const std::string GetInstanceGUID(const TopoDS_Shape& rkOcctShape);

		/*TOPOLOGIC_API void SetInstanceGUID(const std::string& rkGuid) {
			m_guid = rkGuid;
		}*/

		static TopologyType GetTopologyType(const TopAbs_ShapeEnum& rkOcctType);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkShape"></param>
		/// <param name="rSubTopologies"></param>
		static void SubTopologies(const TopoDS_Shape& rkShape, BOPCol_ListOfShape& rSubTopologies);

		virtual bool IsContainerType() = 0;

#ifdef _DEBUG
		TOPOLOGIC_API void GlobalClusterSubTopologies(std::list<Topology::Ptr>& rSubTopologies) const;
#endif

		TOPOLOGIC_API void RegisterFactory(const std::string& rkGuid, const std::shared_ptr<TopologyFactory>& kpTopologyFactory);

	protected:
		Topology(const int kDimensionality, const TopoDS_Shape& rkOcctShape, const std::string& rkGuid = "");

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		static TopoDS_CompSolid MakeCompSolid(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkShape"></param>
		/// <param name="kLevel"></param>
		/// <returns></returns>
		static std::string Analyze(const TopoDS_Shape& rkShape, const int kLevel = 0);
/*
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rUnionArguments"></param>
		static void AddUnionInternalStructure(const TopoDS_Shape& rkOcctShape, BOPCol_ListOfShape& rUnionArguments);*/

		template <class Subclass>
		static TopAbs_ShapeEnum CheckOcctShapeType();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <param name="rOcctCellsBuilder"></param>
		/// <param name="rOcctCellsBuildersOperandsA"></param>
		/// <param name="rOcctCellsBuildersOperandsB"></param>
		/// <param name="rOcctMapFaceToFixedFaceA"></param>
		/// <param name="rOcctMapFaceToFixedFaceB"></param>
		void AddBooleanOperands(
			const Topology::Ptr& kpOtherTopology,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsB,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <param name="rOcctCellsBuildersOperandsA"></param>
		/// <param name="rOcctCellsBuildersOperandsB"></param>
		void AddBooleanOperands(
			const Topology::Ptr& kpOtherTopology,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsB);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <param name="rOcctCellsBuilder"></param>
		/// <param name="rOcctCellsBuildersOperandsA"></param>
		/// <param name="rOcctCellsBuildersOperandsB"></param>
		/// <param name="rOcctMapFaceToFixedFaceA"></param>
		/// <param name="rOcctMapFaceToFixedFaceB"></param>
		void NonRegularBooleanOperation(
			const Topology::Ptr& kpOtherTopology,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsB,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctArgumentsA"></param>
		/// <param name="rkOcctArgumentsB"></param>
		/// <param name="rOcctCellsBuilder"></param>
		static void NonRegularBooleanOperation(
			const BOPCol_ListOfShape& rkOcctArgumentsA,
			const BOPCol_ListOfShape& rkOcctArgumentsB,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctArgumentsA"></param>
		/// <param name="rkOcctArgumentsB"></param>
		/// <param name="rOcctBooleanOperation"></param>
		static void RegularBooleanOperation(
			const BOPCol_ListOfShape& rkOcctArgumentsA,
			const BOPCol_ListOfShape& rkOcctArgumentsB,
			BRepAlgoAPI_BooleanOperation& rOcctBooleanOperation);

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctCellsBuilder"></param>
		/// <returns></returns>
		Topology::Ptr GetBooleanResult(
			const Topology::Ptr& kpOtherTopology,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceA,
			BOPCol_DataMapOfShapeShape& rOcctMapFaceToFixedFaceB);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOtherTopology"></param>
		/// <param name="rkOcctResultShape"></param>
		/// <returns></returns>
		TopoDS_Shape PostprocessBooleanResult(
			const Topology::Ptr& kpOtherTopology,
			const TopoDS_Shape& rkOcctResultShape
			);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctBooleanResult"></param>
		/// <returns></returns>
		TopoDS_Shape PostprocessBooleanResult(
			const TopoDS_Shape& rkOcctBooleanResult
		);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctMakeShape"></param>
		/// <param name="rkShapes"></param>
		static void TransferMakeShapeContents(BRepBuilderAPI_MakeShape & rkOcctMakeShape, const std::list<Topology::Ptr>& rkShapes);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctMakeShape"></param>
		/// <param name="rkOcctShapes"></param>
		static void TransferMakeShapeContents(BRepBuilderAPI_MakeShape& rkOcctMakeShape, const BOPCol_ListOfShape& rkOcctShapes);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		TopoDS_Shape FixBooleanOperandCell(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		TopoDS_Shape FixBooleanOperandShell(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rMapFaceToFixedFace"></param>
		/// <returns></returns>
		TopoDS_Shape FixBooleanOperandFace(const TopoDS_Shape& rkOcctShape, BOPCol_DataMapOfShapeShape& rMapFaceToFixedFace);

		TopoDS_Shape FixBooleanOperandFace(const TopoDS_Shape& rkOcctShape);

		void GetDeletedBooleanSubtopologies(const TopoDS_Shape& rkOcctShape, BOPAlgo_CellsBuilder& rOcctCellsBuilder, BOPCol_ListOfShape& rOcctDeletedShapes);

		void GetDeletedBooleanSubtopologies(const TopoDS_Shape& rkOcctShape, BRepAlgoAPI_BooleanOperation& rOcctBooleanOperation, BOPCol_ListOfShape& rOcctDeletedShapes);

		Topology::Ptr TrackContextAncestor();

		int m_dimensionality;
		static int m_numOfTopologies;
	};

	template <class Subclass>
	void Topology::UpwardNavigation(std::list<std::shared_ptr<Subclass>>& rAncestors) const
	{
		UpwardNavigation(GlobalCluster::GetInstance().GetOcctCompound(), rAncestors);
	}

	template<class Subclass>
	inline void Topology::UpwardNavigation(const TopoDS_Shape& rkOcctHostTopology, std::list<std::shared_ptr<Subclass>>& rAncestors) const
	{
		static_assert(std::is_base_of<Topology, Subclass>::value, "Subclass not derived from Topology");

		TopAbs_ShapeEnum occtShapeType = CheckOcctShapeType<Subclass>();

		TopTools_MapOfShape occtAncestorMap;
		TopTools_IndexedDataMapOfShapeListOfShape occtShapeMap;
		TopExp::MapShapesAndUniqueAncestors(
			rkOcctHostTopology,
			GetOcctShape().ShapeType(),
			occtShapeType,
			occtShapeMap);

		TopTools_ListOfShape occtAncestors;
		bool isInShape = occtShapeMap.FindFromKey(GetOcctShape(), occtAncestors);
		if (!isInShape)
		{
			return;
		}

		for (TopTools_ListIteratorOfListOfShape occtAncestorIterator(occtAncestors);
			occtAncestorIterator.More();
			occtAncestorIterator.Next())
		{
			const TopoDS_Shape& rkOcctAncestor = occtAncestorIterator.Value();
			bool isAncestorAdded = occtAncestorMap.Contains(rkOcctAncestor);
			if (rkOcctAncestor.ShapeType() == occtShapeType && !isAncestorAdded)
			{
				occtAncestorMap.Add(rkOcctAncestor);

				Topology::Ptr pTopology = ByOcctShape(rkOcctAncestor, "");
				rAncestors.push_back(Downcast<Subclass>(pTopology));
			}
		}
	}

	template <class Subclass>
	void Topology::DownwardNavigation(std::list<std::shared_ptr<Subclass>>& rMembers) const
	{
		static_assert(std::is_base_of<Topology, Subclass>::value, "Subclass not derived from Topology");

		TopAbs_ShapeEnum occtShapeType = CheckOcctShapeType<Subclass>();
		TopTools_MapOfShape occtShapes;
		for (TopExp_Explorer occtExplorer(GetOcctShape(), occtShapeType); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtShapes.Contains(occtCurrent))
			{
				occtShapes.Add(occtCurrent);
				Topology::Ptr pChildTopology = ByOcctShape(occtCurrent, "");
				rMembers.push_back(Downcast<Subclass>(pChildTopology));
			}
		}
	}

	template <class Subclass>
	static TopAbs_ShapeEnum Topology::CheckOcctShapeType()
	{
		if (std::is_same<Subclass, Vertex>::value)
		{
			return TopAbs_VERTEX;
		} 
		else if (std::is_same<Subclass, Edge>::value)
		{
			return TopAbs_EDGE;
		}
		else if (std::is_same<Subclass, Wire>::value)
		{
			return TopAbs_WIRE;
		}
		else if (std::is_same<Subclass, Face>::value)
		{
			return TopAbs_FACE;
		}
		else if (std::is_same<Subclass, Shell>::value)
		{
			return TopAbs_SHELL;
		}
		else if (std::is_same<Subclass, Cell>::value)
		{
			return TopAbs_SOLID;
		}
		else if (std::is_same<Subclass, CellComplex>::value)
		{
			return TopAbs_COMPSOLID;
		}
		else if (std::is_same<Subclass, Cluster>::value)
		{
			return TopAbs_COMPOUND;
		}
		
		throw std::exception("Other subclasses are invalid.");
	}

	/// <summary>
	/// Used by TopologyFactory to pass smart pointers
	/// </summary>
	struct TopologyPtr
	{
		TOPOLOGIC_API TopologyPtr(const std::shared_ptr<Topology>& pTopologyPtr)
			: topologyPtr(pTopologyPtr)
		{
		}

		const std::shared_ptr<Topology>& topologyPtr;
	};
}