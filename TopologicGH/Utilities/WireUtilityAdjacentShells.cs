using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGH.Utilities
{
    public class WireUtilityAdjacentShells : GH_Component
    {
        public WireUtilityAdjacentShells()
          : base("WireUtility.AdjacentShells", "WireUtility.AdjacentShells", "Returns a list of Shells that are the adjacent to the input Wire.", "TopologicUtilities", "WireUtility")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Wire", "Wire", "Wire", GH_ParamAccess.item);
            pManager.AddGenericParameter("Parent Topology", "Parent Topology", "Parent Topology", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Shells", "Shells", "Shells", GH_ParamAccess.list);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            global::Topologic.Wire wire = null;
            global::Topologic.Topology parentTopology = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref wire)) { return; }
            if (!DA.GetData(1, ref parentTopology)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (wire == null) { return; }
            if (parentTopology == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            List<global::Topologic.Shell> adjacentShells = global::Topologic.Utilities.WireUtility.AdjacentShells(wire, parentTopology);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetDataList(0, adjacentShells);
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
            get { return new Guid("f00c22e9-cc73-4bd8-9ff7-6498ba5360cd"); }
        }
    }
}
