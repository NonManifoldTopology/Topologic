using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGH
{
    public class TopologyUtilityScale : GH_Component
    {

        public TopologyUtilityScale()
          : base("TopologyUtility.Scale", "TopologyUtility.Scale", "Scales a Topology.", "TopologicUtilities", "TopologyUtility")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Topology", "Topology", "Topology", GH_ParamAccess.item);
            pManager.AddGenericParameter("Origin", "Origin", "Origin", GH_ParamAccess.item);
            pManager.AddNumberParameter("X Factor", "X Factor", "X Factor", GH_ParamAccess.item);
            pManager.AddNumberParameter("Y Factor", "Y Factor", "Y Factor", GH_ParamAccess.item);
            pManager.AddNumberParameter("Z Factor", "Z Factor", "Z Factor", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Topology", "Topology", "Topology", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Topology topology = null;
            Topologic.Vertex origin = null;
            double xFactor = 0.0;
            double yFactor = 0.0;
            double zFactor = 0.0;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref topology)) { return; }
            if (!DA.GetData(1, ref origin)) { return; }
            if (!DA.GetData(2, ref xFactor)) { return; }
            if (!DA.GetData(3, ref yFactor)) { return; }
            if (!DA.GetData(4, ref zFactor)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (topology == null) { return; }
            if (origin == null) { return; }
            //if (endVertex == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            
            Topologic.Topology rotatedTopology = Topologic.Utilities.TopologyUtility.Scale(topology, origin, xFactor, yFactor, zFactor);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, rotatedTopology);
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
            get { return new Guid("0be82d5e-c288-4dcd-b5de-21ac0d0b3800"); }
        }
    }
}
