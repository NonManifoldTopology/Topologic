#pragma once

#include <Topology.h>

#include <TDF_Attribute.hxx>

namespace TopologicCore
{
	class OcctContentsAttribute : public TDF_Attribute
	{
	public:
		OcctContentsAttribute();
		~OcctContentsAttribute();

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
		/// <param name="label"></param>
		/// <returns></returns>
		static Handle(OcctContentsAttribute) Set(const TDF_Label& label);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkLabel"></param>
		void AddContent(const TDF_Label& rkLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkLabel"></param>
		void RemoveContent(const TDF_Label& rkLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::list<TDF_Label>& GetContents() const
		{
			return m_contents;
		}

	protected:
		std::list<TDF_Label> m_contents;
	};
}