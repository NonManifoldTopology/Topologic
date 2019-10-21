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

// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it 
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

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