// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

//#pragma once
//
//#include <TDF_Attribute.hxx>
//
//namespace TopologicStructure
//{
//	/// <summary>
//	/// <para>
//	/// </para>
//	/// </summary>
//	class OcctLoadAttribute : public TDF_Attribute
//	{
//	public:
//		OcctLoadAttribute();
//		OcctLoadAttribute(
//			const double kXDirection, const double kYDirection, const double kZDirection,
//			const double kMagnitude);
//		~OcctLoadAttribute();
//
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
//		/// <param name="rkOcctLabel"></param>
//		/// <returns></returns>
//		static Handle(OcctLoadAttribute) SetLoad(
//			const TDF_Label& rkOcctLabel, 
//			const double kXDirection, const double kYDirection, const double kZDirection,
//			const double kMagnitude);
//
//		/// <summary>
//		/// 
//		/// </summary>
//		/// <param name="rXDirection"></param>
//		/// <param name="rYDirection"></param>
//		/// <param name="rZDirection"></param>
//		/// <param name="rMagnitude"></param>
//		void Get(
//			double& rXDirection, double& rYDirection, double& rZDirection,
//			double& rMagnitude) const;
//
//		double X() const { return m_xDirection; }
//		double Y() const { return m_yDirection; }
//		double Z() const { return m_zDirection; }
//
//	protected:
//		double m_xDirection;
//		double m_yDirection;
//		double m_zDirection;
//		double m_magnitude;
//	};
//}