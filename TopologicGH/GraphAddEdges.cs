using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class GraphAddEdges : GH_Component
    {

        public GraphAddEdges()
          : base("Graph.AddEdges", "Graph.AddEdges", "Adds the list of Edges to the Graph. The Vertices of the Edges are merged with existing Vertices if they fall within the input tolerance. No new Edges will be added if they already exist. A zero or null tolerance means that the Vertices of the input Edges will be added to the Graph regardless of their overlap with existing Vertices.", "Topologic", "Graph")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Graph", "Graph", "Graph", GH_ParamAccess.item);
            pManager.AddGenericParameter("Edges", "Edges", "Edges", GH_ParamAccess.list);
            pManager.AddNumberParameter("Tolerance", "Tolerance", "Tolerance", GH_ParamAccess.item, 0.0001);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Graph", "Graph", "Graph", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Graph graph = null;
            List<Topologic.Edge> edges = new List<Topologic.Edge>();
            double tolerance = 0.0;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref graph)) { return; }
            if (!DA.GetDataList(1, edges)) { return; }
            if (!DA.GetData(2, ref tolerance)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (graph == null) { return; }
            if (edges == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            // Reverse the array of character.
            Topologic.Graph newGraph = graph.AddEdges(edges, tolerance);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, newGraph);
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
            get { return new Guid("166504df-b722-4a8e-9a2a-3507cb34d1f1"); }
        }
    }
}
