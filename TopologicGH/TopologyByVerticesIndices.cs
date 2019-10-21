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

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGH
{
    public class TopologyByVerticesIndices : GH_Component
    {

        public TopologyByVerticesIndices()
          : base("Topology.ByVerticesIndices", "Topology.ByVerticesIndices", "Creates a Topology by a list of Vertices and a 2D list of indices of the Vertices in the first argument. An array of 1 index makes a Vertex. An array of 2 indices makes an Edge. An array of 3 indices either makes a Wire, or, when the array has at least 4 vertices and the first index is the same as the last index, a Face will be created instead. NOTE: This node currently can only create planar Faces.", "Topologic", "Topology")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Vertices", "Vertices", "Vertices", GH_ParamAccess.list);
            pManager.AddGenericParameter("Indices", "Indices", "Indices", GH_ParamAccess.list);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Topologies", "Topologies", "Topologies", GH_ParamAccess.list);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            List<Topologic.Vertex> vertices = new List<Topologic.Vertex>();
            List<List<int>> indices = new List<List<int>>();

            //// Use the DA object to retrieve the data inside the first input parameter.
            //// If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetDataList(0, vertices)) { return; }
            if (!DA.GetDataList(1, indices)) { return; }

            //// If the retrieved data is Nothing, we need to abort.
            //// We're also going to abort on a zero-length String.
            if (vertices == null) { return; }
            if (indices == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            
            List<Topologic.Topology> topologies = Topologic.Topology.ByVerticesIndices(vertices, indices);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetDataList(0, topologies);
        }

        /// <summary>
        /// Provides an Icon for the component.
        /// </summary>
        protected override System.Drawing.Bitmap Icon
        {
            get
            {
                //You can add image files to your project resources and access them like this:
                // return Resources.IconForThisComponent;
                return Resources.NMT_borderless_logo_small;
            }
        }

        /// <summary>
        /// Gets the unique ID for this component. Do not change this ID after release.
        /// </summary>
        public override Guid ComponentGuid
        {
            get { return new Guid("067df633-21c5-41e6-b081-40263736d9c6"); }
        }
    }
}
