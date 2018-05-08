#pragma once

#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>

namespace TopoLogicCore
{
	/// <summary>
	/// <para>
	/// Provides a "smart pointer"-like counter. Every time a Topologic shape is created from
	/// an existing label, the counter is increased. When the Topologic shape is deleted,
	/// the counter is decreased. When the counter is 0, the label is "deleted".
	/// </para>
	/// </summary>
	class OcctCounterAttribute : public TDF_Attribute
	{
	public:
		OcctCounterAttribute();
		~OcctCounterAttribute();

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
		/// <returns></returns>
		static Handle(OcctCounterAttribute) IncreaseCounter(const TDF_Label& rkOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctLabel"></param>
		/// <returns></returns>
		static Handle(OcctCounterAttribute) DecreaseCounter(const TDF_Label& rkOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="label"></param>
		/// <returns></returns>
		static Handle(OcctCounterAttribute) Set(const TDF_Label& label);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int Get() const;

	protected:
		int m_counter;
	};
}