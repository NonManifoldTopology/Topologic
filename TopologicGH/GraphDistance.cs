using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class GraphDistance : GH_Component
    {

        public GraphDistance()
          : base("Graph.Distance", "Graph.Distance", "Returns the number of Edges of the shortest path connecting the two input Vertices.", "Topologic", "Graph")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Graph", "Graph", "Graph", GH_ParamAccess.item);
            pManager.AddGenericParameter("Start Vertex", "Start Vertex", "Start Vertex", GH_ParamAccess.item);
            pManager.AddGenericParameter("End Vertex", "End Vertex", "End Vertex", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddIntegerParameter("Distance", "Distance", "Distance", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Graph graph = null;
            Topologic.Vertex startVertex = null;
            Topologic.Vertex endVertex = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref graph)) { return; }
            if (!DA.GetData(1, ref startVertex)) { return; }
            if (!DA.GetData(2, ref endVertex)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (graph == null) { return; }
            if (startVertex == null) { return; }
            if (endVertex == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            // Reverse the array of character.
            int distance = graph.Distance(startVertex, endVertex);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, distance);
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
            get { return new Guid("c2796671-b874-4b74-b7cb-464d55583bdf"); }
        }
    }
}
