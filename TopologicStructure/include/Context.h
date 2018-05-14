#pragma once

using namespace System::Collections::Generic;
using namespace Topologic;

namespace TopologicStructure
{
	/// <summary>
	/// 
	/// </summary>
	public ref class Context
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <param name="u"></param>
		/// <param name="v"></param>
		/// <param name="w"></param>
		/// <param name="uScale"></param>
		/// <param name="vScale"></param>
		/// <param name="wScale"></param>
		/// <param name="uShift"></param>
		/// <param name="vShift"></param>
		/// <param name="wShift"></param>
		/// <returns></returns>
		static Context^ Create(
			Topology^ hostTopology, 
			double u, double v, double w, 
			double uScale, double vScale, double wScale, 
			double uShift, double vShift, double wShift);

		property Topology^ HostTopology
		{
			Topology^ get() { return m_pHostTopology; }
		}

		property double U
		{
			double get() { return m_u; }
		}

		property double V
		{
			double get() { return m_v; }
		}

		property double W
		{
			double get() { return m_w; }
		}

		property double UScale
		{
			double get() { return m_uScale; }
		}

		property double VScale
		{
			double get() { return m_vScale; }
		}

		property double WSale
		{
			double get() { return m_wScale; }
		}

		property double UShift
		{
			double get() { return m_uShift; }
		}

		property double VShift
		{
			double get() { return m_vShift; }
		}

		property double WShift
		{
			double get() { return m_wShift; }
		}

	public protected:

	protected:
		Context(
			Topology^ hostTopology,
			double u, double v, double w,
			double uScale, double vScale, double wScale,
			double uShift, double vShift, double wShift);
		~Context();

		Topology^ m_pHostTopology;
		double m_u;
		double m_v;
		double m_w;
		double m_uScale;
		double m_vScale;
		double m_wScale;
		double m_uShift;
		double m_vShift;
		double m_wShift;
	};
}