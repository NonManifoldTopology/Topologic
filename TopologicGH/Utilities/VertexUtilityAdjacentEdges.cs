using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGH.Utilities
{
    public class VertexUtilityAdjacentEdges : GH_Component
    {
        public VertexUtilityAdjacentEdges()
          : base("VertexUtility.AdjacentEdges", "VertexUtility.AdjacentEdges", "Returns a list of Edges that are the adjacent to the input Vertex.", "TopologicUtilities", "VertexUtility")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Vertex", "Vertex", "Vertex", GH_ParamAccess.item);
            pManager.AddGenericParameter("Parent Topology", "Parent Topology", "Parent Topology", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Edges", "Edges", "Edges", GH_ParamAccess.list);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            global::Topologic.Vertex vertex = null;
            global::Topologic.Topology parentTopology = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref vertex)) { return; }
            if (!DA.GetData(1, ref parentTopology)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (vertex == null) { return; }
            if (parentTopology == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            List<global::Topologic.Edge> adjacentEdges = global::Topologic.Utilities.VertexUtility.AdjacentEdges(vertex, parentTopology);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetDataList(0, adjacentEdges);
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
            get { return new Guid("4b4af12c-ab72-4bd2-a8a6-41a14e4bcadb"); }
        }
    }
}
