using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGH
{
    public class EdgeByStartVertexEndVertex : GH_Component
    {

        public EdgeByStartVertexEndVertex()
          : base("Edge.ByStartVertexEndVertex", "Edge.ByStartVertexEndVertex", "Creates a straight Edge by the startVertex and endVertex", "Topologic", "Edge")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("startVertex", "startVertex", "startVertex", GH_ParamAccess.item);
            pManager.AddGenericParameter("endVertex", "endVertex", "endVertex", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Edge", "Edge", "Edge", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Vertex startVertex = null;
            Topologic.Vertex endVertex = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref startVertex)) { return; }
            if (!DA.GetData(1, ref endVertex)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (startVertex == null) { return; }
            if (endVertex == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            
            Topologic.Edge edge = Topologic.Edge.ByStartVertexEndVertex(startVertex, endVertex);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, edge);
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
            get { return new Guid("093ebebf-0aa8-4e8c-ae5f-017f9eb69faa"); }
        }
    }
}
