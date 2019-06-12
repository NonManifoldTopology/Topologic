using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class GraphByTopology : GH_Component
    {

        public GraphByTopology()
          : base("Graph.ByTopology", "Graph.ByTopology", "Creates a Graph from any topology.", "Topologic", "Graph")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Topology", "Topology", "Topology", GH_ParamAccess.item);
            pManager.AddBooleanParameter("Direct", "Direct", "Direct", GH_ParamAccess.item, true);
            pManager.AddBooleanParameter("Via Shared Topologies", "Via Shared Topologies", "Via Shared Topologies", GH_ParamAccess.item, false);
            pManager.AddBooleanParameter("Via Shared Apertures", "Via Shared Apertures", "Via Shared Apertures", GH_ParamAccess.item, false);
            pManager.AddBooleanParameter("To Exterior Topologies", "To Exterior Topologies", "To Exterior Topologies", GH_ParamAccess.item, false);
            pManager.AddBooleanParameter("To Exterior Apertures", "To Exterior Apertures", "To Exterior Apertures", GH_ParamAccess.item, false);
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
            Topologic.Topology topology = null;
            bool direct = false;
            bool viaSharedTopologies = false;
            bool viaSharedApertures = false;
            bool toExteriorTopologies = false;
            bool toExteriorApertures = false;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref topology)) { return; }
            if (!DA.GetData(1, ref direct)) { return; }
            if (!DA.GetData(2, ref viaSharedTopologies)) { return; }
            if (!DA.GetData(3, ref viaSharedApertures)) { return; }
            if (!DA.GetData(4, ref toExteriorTopologies)) { return; }
            if (!DA.GetData(5, ref toExteriorApertures)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (topology == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            // Reverse the array of character.
            Topologic.Graph graph = Topologic.Graph.ByTopology(topology, direct, viaSharedTopologies, viaSharedApertures, toExteriorTopologies, toExteriorApertures);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, graph);
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
            get { return new Guid("856104c0-c315-4626-bb97-598862109109"); }
        }
    }
}
