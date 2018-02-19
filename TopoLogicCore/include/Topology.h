#pragma once

#include "Attribute.h"
#include "Utilities.h"

#include <BOPCol_ListOfShape.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <Standard_Handle.hxx>
#include <Geom_Geometry.hxx>
#include <TopoDS_CompSolid.hxx>

#include <list>
#include <map>

class TopoDS_Shape;

namespace TopoLogicCore
{
	enum BooleanFlag
	{
		BOOLEAN_UNION,
		BOOLEAN_DIFFERENCE,
		BOOLEAN_INTERSECTION,
		BOOLEAN_MERGE,
		BOOLEAN_SLICE,
		BOOLEAN_IMPOSE,
		BOOLEAN_IMPRINT,
		BOOLEAN_XOR
	};

	/// <summary>
	/// The base class for all topology classes in TopoLogic.
	/// </summary>
	class Topology
	{
	public:
		typedef std::map<std::string, Attribute*> AttributeMap;
		typedef std::map<std::string, Attribute*>::iterator AttributeMapIterator;

	public:
		virtual ~Topology();

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
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <param name="kOcctArgumentImagesInArguments"></param>
		/// <param name="kOcctArgumentImagesInTools"></param>
		/// <param name="kOcctToolsImagesInArguments"></param>
		/// <param name="kOcctToolsImagesInTools"></param>
		void BooleanImages(
			Topology const * const kpkOtherTopology,
			BOPAlgo_CellsBuilder& rOcctCellsBuilder,
			BOPCol_ListOfShape& kOcctArgumentImagesInArguments,
			BOPCol_ListOfShape& kOcctArgumentImagesInTools,
			BOPCol_ListOfShape& kOcctToolsImagesInArguments,
			BOPCol_ListOfShape& kOcctToolsImagesInTools);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOtherTopology"></param>
		/// <param name="kBooleanFlag"></param>
		/// <returns></returns>
		TOPOLOGIC_API Topology* BooleanOperation(
			Topology const * const kpkOtherTopology, 
			const BooleanFlag kBooleanFlag);

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
		std::list<Topology*>& Members()
		{
			return m_members;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::list<Topology*>& Members() const
		{
			return m_members;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::list<Topology*>& MembersOf()
		{
			return m_membersOf;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::list<Topology*>& MembersOf() const
		{
			return m_membersOf;
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
		TOPOLOGIC_API bool LoadFromBrep(const std::string& rkPath) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology"></param>
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
		TOPOLOGIC_API bool Locked() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kLocked"></param>
		TOPOLOGIC_API void Locked(const bool kLocked);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Shape* GetOcctShape() const = 0;

		template <typename T>
		static T* Downcast(Topology *const kpTopology)
		{
			T* pSubclassTopology = dynamic_cast<T*>(kpTopology);
			if (pSubclassTopology == nullptr)
			{
				throw std::exception("Failed downcasting topology");
			}
			return pSubclassTopology;
		}

	protected:
		Topology(const int kDimensionality);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		static Topology* ByOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <returns></returns>
		static TopoDS_CompSolid MakeCompSolid(const TopoDS_Shape& rkOcctShape);

		AttributeMap m_attributeMap;
		std::list<Topology*> m_members;
		std::list<Topology*> m_membersOf;
		bool m_isLocked;
		int m_modelingPriority;
		int m_dimensionality;
	};
}