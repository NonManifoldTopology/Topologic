//#pragma once
//
//#include <Topology.h>
//
//#include <TDF_Attribute.hxx>
//
//namespace TopologicCore
//{
//	class OcctContextListAttribute : public TDF_Attribute
//	{
//	public:
//		OcctContextListAttribute();
//		~OcctContextListAttribute();
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <returns></returns>
//		static const Standard_GUID& GetID();
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <returns></returns>
//		virtual const Standard_GUID& ID() const;
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <param name="kpOcctAttribute"></param>
//		void Restore(const Handle(TDF_Attribute)& kpOcctAttribute);
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <returns></returns>
//		Handle(TDF_Attribute) NewEmpty() const;
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <param name="kpOcctAttribute"></param>
//		/// <param name="Handle"></param>
//		void Paste(const Handle(TDF_Attribute)& kpOcctAttribute,
//			const Handle(TDF_RelocationTable)& /*RT*/) const;
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <param name="rkLabel"></param>
//		void AddContext(const TDF_Label& rkLabel);
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <param name="rkLabel"></param>
//		void RemoveContext(const TDF_Label& rkLabel);
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <param name="label"></param>
//		/// <returns></returns>
//		static Handle(OcctContextListAttribute) Set(const TDF_Label& label);
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <returns></returns>
//		const std::list<TDF_Label>& GetContexts() const
//		{
//			return m_contexts;
//		}
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <returns></returns>
//		std::list<TDF_Label>& GetContexts()
//		{
//			return m_contexts;
//		}
//
//	protected:
//		std::list<TDF_Label> m_contexts;
//	};
//}