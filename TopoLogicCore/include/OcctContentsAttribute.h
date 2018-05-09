#pragma once

#include <Topology.h>

#include <TDF_Attribute.hxx>

namespace TopoLogicCore
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
		/// <param name="kpTopology"></param>
		void AddContent(const std::shared_ptr<Topology>& kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		void RemoveContent(const std::shared_ptr<Topology>& kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::list<std::shared_ptr<Topology>>& GetContents() const 
		{
			return m_contents;
		}

	protected:
		std::list<std::shared_ptr<Topology>> m_contents;
	};
}