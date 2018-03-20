#pragma once

#include "Attribute.h"
#include "Utilities.h"
#include "TopologicalQuery.h"

#include <BOPCol_ListOfShape.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <Standard_Handle.hxx>
#include <Geom_Geometry.hxx>
#include <TopoDS_CompSolid.hxx>

#include <list>
#include <map>
#include <memory>

class TopoDS_Shape;

namespace TopoLogicCore
{
	enum TopologyType
	{
		TOPOLOGY_VERTEX = 0,
		TOPOLOGY_EDGE = 1,
		TOPOLOGY_WIRE = 2,
		TOPOLOGY_FACE = 3,
		TOPOLOGY_SHELL = 4,
		TOPOLOGY_CELL = 5,
		TOPOLOGY_CELLCOMPLEX = 6,
		TOPOLOGY_CLUSTER = 7
	};

	class Vertex;
	class Context;

	/// <summary>
	/// A Topology is an abstract superclass that constructors, properties and methods used by other subclasses that extend it.
	/// </summary>
	class Topology : public TopologicalQuery
	{
	public:
		typedef std::map<std::string, Attribute*> AttributeMap;
		typedef std::map<std::string, Attribute*>::iterator AttributeMapIterator;
		typedef std::map<std::string, Attribute*>::const_iterator AttributeMapConstIterator;

	public:
		virtual ~Topology();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		static Topology* ByOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pGeometry"></param>
		/// <returns></returns>
		static Topology* ByGeometry(Handle(Geom_Geometry) pGeometry);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkContext"></param>
		/// <returns></returns>
		static Topology* ByContext(Context const * const kpkContext);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertexCoordinates"></param>
		/// <param name="rkVertexIndices"></param>
		/// <returns></returns>
		static Topology* ByVertexIndex(const std::list<std::array<double, 3>>& rkVertexCoordinates, const std::list<std::list<int>>& rkVertexIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkVertexIndices"></param>
		/// <returns></returns>
		static Topology* ByVertexIndex(const std::list<Vertex*>& rkVertices, const std::list<std::list<int>>& rkVertexIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* AddContent(Topology * const kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* RemoveContent(Topology * const kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpContext"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* AddContext(Context * const kpContext);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpContext"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* RemoveContext(Context * const kpContext);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="rkSharedTopologies"></param>
		TOPOLOGIC_API void SharedTopologies(Topology * const kpTopology, std::list<Topology*>& rkSharedTopologies) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kMaxLevels"></param>
		/// <param name="kMaxPaths"></param>
		/// <param name="rkPaths"></param>
		TOPOLOGIC_API void PathsTo(Topology * const kpTopology, const int kMaxLevels, const int kMaxPaths, std::list<std::list<TopologicalQuery*>>& rkPaths) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <param name="kArgumentImagesInArguments"></param>
		/// <param name="kArgumentImagesInTools"></param>
		/// <param name="kToolsImagesInArguments"></param>
		/// <param name="kToolsImagesInTools"></param>
		/// <returns></returns>
		TOPOLOGIC_API void BooleanImages(
			Topology const * const kpkOtherTopology,
			std::list<Topology*>& kArgumentImagesInArguments,
			std::list<Topology*>& kArgumentImagesInTools,
			std::list<Topology*>& kToolsImagesInArguments,
			std::list<Topology*>& kToolsImagesInTools);

		/// <summary>
		/// Does not perform cells builder.
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <param name="rOcctCellsBuilder"></param>
		/// <param name="kOcctArgumentImagesInArguments"></param>
		/// <param name="kOcctArgumentImagesInTools"></param>
		/// <param name="kOcctToolsImagesInArguments"></param>
		/// <param name="kOcctToolsImagesInTools"></param>
		void BooleanImages(
			Topology const * const kpkOtherTopology,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_ListOfShape& rOcctExclusivelyArgumentImages,
			BOPCol_ListOfShape& rOcctExclusivelyToolImages,
			BOPCol_ListOfShape& rOcctSharedImages,
			BOPCol_ListOfShape& rOcctExclusivelyArgumentImageFaces,
			BOPCol_ListOfShape& rOcctExclusivelyToolImageFaces,
			BOPCol_ListOfShape& rOcctSharedImageFaces,
			BOPCol_ListOfShape& kOcctArgumentImagesInArguments,
			BOPCol_ListOfShape& kOcctArgumentImagesInTools,
			BOPCol_ListOfShape& kOcctToolsImagesInArguments,
			BOPCol_ListOfShape& kOcctToolsImagesInTools);


		TOPOLOGIC_API void BooleanParts(
			Topology const * const kpkOtherTopology,
			std::list<Topology*>& rSpaceBetween_A_A_and_B_A,
			std::list<Topology*>& rSpaceBetween_B_A_and_A_B,
			std::list<Topology*>& rSpaceBetween_A_B_and_B_B);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Difference(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Impose(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Imprint(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Intersection(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Merge(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Slice(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Union(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* XOR(Topology const * const kpkOtherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::list<Topology*>& Contents()
		{
			return m_contents;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::list<Topology*>& Contents() const
		{
			return m_contents;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::list<Context*>& Contexts()
		{
			return m_contexts;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::list<Context*>& Contexts() const
		{
			return m_contexts;
		}

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
		static TOPOLOGIC_API Topology* LoadFromBrep(const std::string& rkPath);

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
		virtual TopoDS_Shape* GetOcctShape() const = 0;

		virtual TopologyType GetType() const = 0;

		/// <summary>
		/// Returns all sub-entities that have no other parents than this topology, i.e. do not belong to other entities.
		/// </summary>
		/// <param name="rImmediateMembers">The immediate members</param>
		TOPOLOGIC_API void ImmediateMembers(std::list<Topology*>& rImmediateMembers) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API AttributeMap& Attributes() { return m_attributeMap; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API const AttributeMap& Attributes() const { return m_attributeMap; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::string GetTypeAsString() const;

		void SetInGlobalCluster(const bool kInGlobalCluster)
		{
			m_isInGlobalCluster = kInGlobalCluster;
		}

		bool GetInGlobalCluster() const
		{
			return m_isInGlobalCluster;
		}

	protected:
		Topology(const int kDimensionality);
		
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
		/// <param name="rImmediateMembers"></param>
		static void ImmediateMembers(const TopoDS_Shape& rkShape, BOPCol_ListOfShape& rImmediateMembers);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkShape"></param>
		/// <param name="kLevel"></param>
		/// <returns></returns>
		static std::string Analyze(const TopoDS_Shape& rkShape, const int kLevel = 0);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rUnionArguments"></param>
		static void AddUnionInternalStructure(const TopoDS_Shape& rkOcctShape, BOPCol_ListOfShape& rUnionArguments);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <param name="rOcctCellsBuilder"></param>
		void AddBooleanOperands(
			Topology const * const kpkOtherTopology, 
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsB);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <param name="kBooleanFlag"></param>
		void BooleanOperation(
			Topology const * const kpkOtherTopology,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
			BOPCol_ListOfShape& rOcctCellsBuildersOperandsB);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctCellsBuilder"></param>
		/// <returns></returns>
		Topology* GetBooleanResult(BOPAlgo_CellsBuilder& rOcctCellsBuilder);

		AttributeMap m_attributeMap;
		std::list<Topology*> m_contents;
		std::list<Context*> m_contexts;
		int m_dimensionality;
		bool m_isInGlobalCluster;
	};
}