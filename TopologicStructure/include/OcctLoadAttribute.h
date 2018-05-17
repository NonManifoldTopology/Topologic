#pragma once

#include <TDF_Attribute.hxx>

namespace TopologicStructure
{
	/// <summary>
	/// <para>
	/// </para>
	/// </summary>
	class OcctLoadAttribute : public TDF_Attribute
	{
	public:
		OcctLoadAttribute();
		OcctLoadAttribute(
			const double kXDirection, const double kYDirection, const double kZDirection,
			const double kMagnitude);
		~OcctLoadAttribute();

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
		static Handle(OcctLoadAttribute) SetLoad(
			const TDF_Label& rkOcctLabel, 
			const double kXDirection, const double kYDirection, const double kZDirection,
			const double kMagnitude);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rXDirection"></param>
		/// <param name="rYDirection"></param>
		/// <param name="rZDirection"></param>
		/// <param name="rMagnitude"></param>
		void Get(
			double& rXDirection, double& rYDirection, double& rZDirection,
			double& rMagnitude) const;

	protected:
		double m_xDirection;
		double m_yDirection;
		double m_zDirection;
		double m_magnitude;
	};
}