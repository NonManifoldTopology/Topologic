#pragma once

using namespace System;

/// <summary>
/// Topologic is an open-source software modelling library enabling hierarchical and topological representations of architectural spaces, buildings and artefacts through non-manifold topology (NMT).
/// </summary>
namespace Topologic {
	/// <summary>
	/// The About class includes information about Topologic.
	/// </summary>
	public ref class About
	{

		/// <summary>
		/// Returns the current version of Topologic.
		/// </summary>
		/// <returns>The current version of Topologic</returns>
	public:
		static String^ Version();

	private:
		About() {}
		~About() {}
	};
}