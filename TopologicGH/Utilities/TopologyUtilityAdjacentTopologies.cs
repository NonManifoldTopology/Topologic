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

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGH.Utilities
{
    public class TopologyUtilityAdjacentTopologies : GH_Component
    {
        public TopologyUtilityAdjacentTopologies()
          : base("TopologyUtility.AdjacentTopologies", "TopologyUtility.AdjacentTopologies", "Returns a list of Topologies that are the adjacent to the input Topology.", "TopologicUtilities", "TopologyUtility")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Topology", "Topology", "Topology", GH_ParamAccess.item);
            pManager.AddGenericParameter("Parent Topology", "Parent Topology", "Parent Topology", GH_ParamAccess.item);
            pManager.AddIntegerParameter("Type Filter", "Type Filter", "Type Filter", GH_ParamAccess.item);
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
            global::Topologic.Topology topology = null;
            global::Topologic.Topology parentTopology = null;
            int typeFilter = 0;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref topology)) { return; }
            if (!DA.GetData(1, ref parentTopology)) { return; }
            if (!DA.GetData(2, ref typeFilter)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (topology == null) { return; }
            if (parentTopology == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();


            List<global::Topologic.Topology> adjacentTopologies = global::Topologic.Utilities.TopologyUtility.AdjacentTopologies(topology, parentTopology, typeFilter);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetDataList(0, adjacentTopologies);
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
            get { return new Guid("f178a6e8-6780-44f0-bf07-a93590391e30"); }
        }
    }
}
