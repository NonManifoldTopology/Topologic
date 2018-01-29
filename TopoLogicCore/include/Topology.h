#pragma once

#include <Attribute.h>
#include <Utilities.h>

#include <Standard_Handle.hxx>
#include <Geom_Geometry.hxx>

#include <list>
#include <map>

namespace TopoLogicCore
{
	/// <summary>
	/// The base class for all topology classes in TopoLogic.
	/// </summary>
	class Topology
	{
	public:
		typedef std::map<std::string, Attribute*> AttributeMap;
		typedef std::map<std::string, Attribute*>::iterator AttributeMapIterator;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology1"></param>
		/// <param name="topology2"></param>
		/// <returns></returns>
		static Topology* Difference(Topology const * const kpkTopology1, Topology const * const kpkTopology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology1"></param>
		/// <param name="kpkTopology2"></param>
		/// <returns></returns>
		static Topology* Impose(Topology const * const kpkTopology1, Topology const * const kpkTopology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology1"></param>
		/// <param name="kpkTopology2"></param>
		/// <returns></returns>
		static Topology* Imprint(Topology const * const kpkTopology1, Topology const * const kpkTopology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology1"></param>
		/// <param name="kpkTopology2"></param>
		/// <returns></returns>
		static Topology* Intersection(Topology const * const kpkTopology1, Topology const * const kpkTopology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology1"></param>
		/// <param name="kpkTopology2"></param>
		/// <returns></returns>
		static Topology* Merge(Topology const * const kpkTopology1, Topology const * const kpkTopology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology1"></param>
		/// <param name="kpkTopology2"></param>
		/// <returns></returns>
		static Topology* Slice(Topology const * const kpkTopology1, Topology const * const kpkTopology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology1"></param>
		/// <param name="kpkTopology2"></param>
		/// <returns></returns>
		static Topology* Union(Topology const * const kpkTopology1, Topology const * const kpkTopology2);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
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
		/// <returns></returns>
		int Dimensionality() const
		{ 
			return m_dimensionality; 
		}

	protected:
		Topology(const int kDimensionality);
		virtual ~Topology();

		AttributeMap m_attributeMap;
		std::list<Topology*> m_members;
		std::list<Topology*> m_membersOf;
		bool m_isLocked;
		int m_modelingPriority;
		int m_dimensionality;
	};
}