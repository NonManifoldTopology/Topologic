#pragma once

#include <Topology.h>

#include <TDF_Attribute.hxx>

namespace TopologicCore
{
	class OcctRelationshipAttribute : public TDF_Attribute
	{
	public:
		OcctRelationshipAttribute();
		~OcctRelationshipAttribute();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		static const Standard_GUID& GetID();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const Standard_GUID& ID() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctAttribute"></param>
		void Restore(const Handle(TDF_Attribute)& kpOcctAttribute);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Handle(TDF_Attribute) NewEmpty() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctAttribute"></param>
		/// <param name="Handle"></param>
		void Paste(const Handle(TDF_Attribute)& kpOcctAttribute,
			const Handle(TDF_RelocationTable)& /*RT*/) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctLabel"></param>
		/// <param name="rkRelationshipType"></param>
		/// <returns></returns>
		static Handle(OcctRelationshipAttribute) Set(const TDF_Label& rkOcctLabel, const TopologyRelationshipType rkRelationshipType);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TopologyRelationshipType Get() const;

	protected:
		TopologyRelationshipType m_relationshipType;
	};
}