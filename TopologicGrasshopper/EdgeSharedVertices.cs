using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class EdgeSharedVertices : GH_Component
    {

        public EdgeSharedVertices()
          : base("Edge.SharedVertices", "Edge.SharedVertices", "Returns the shared Vertices between two Edges.", "Topologic", "Edge")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Edge", "Edge", "Edge", GH_ParamAccess.item);
            pManager.AddGenericParameter("Edge", "Edge", "Edge", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Vertex", "Vertex", "Vertex", GH_ParamAccess.list);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Edge edge1 = null;
            Topologic.Edge edge2 = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref edge1)) { return; }
            if (!DA.GetData(1, ref edge2)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (edge1 == null) { return; }
            if (edge2 == null) { return; }
            
            List<Topologic.Vertex> vertices = edge1.SharedVertices(edge2);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, vertices);
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
            get { return new Guid("9275bcc2-c420-4a84-ad52-3a2b37a68213"); }
        }
    }
}
