#pragma once

#include <Topology.h>

#include <TDF_Attribute.hxx>

namespace TopoLogicCore
{
	class OcctContextsAttribute : public TDF_Attribute
	{
	public:
		OcctContextsAttribute();
		~OcctContextsAttribute();

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
		/// <param name="kpTopology"></param>
		void AddContext(const std::shared_ptr<Context>& kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		void RemoveContext(const std::shared_ptr<Context>& kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::list<std::shared_ptr<Context>>& GetContexts() const
		{
			return m_contexts;
		}

	protected:
		std::list<std::shared_ptr<Context>> m_contexts;
	};
}