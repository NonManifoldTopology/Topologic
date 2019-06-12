using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class VertexCoordinates : GH_Component
    {

        public VertexCoordinates()
          : base("Vertex.Coordinates", "Vertex.Coordinates", "Returns the coordinates of the Vertex", "Topologic", "Vertex")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Vertex", "Vertex", "Vertex", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddNumberParameter("X", "X", "X", GH_ParamAccess.item);
            pManager.AddNumberParameter("Y", "Y", "Y", GH_ParamAccess.item);
            pManager.AddNumberParameter("Z", "Z", "Z", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Vertex vertex = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref vertex)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (vertex == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            List<double> coordinates = vertex.Coordinates;

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, coordinates[0]);
            DA.SetData(1, coordinates[1]);
            DA.SetData(2, coordinates[2]);
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
            get { return new Guid("cfd7e542-4328-4776-82cc-abcbe1aca807"); }
        }
    }
}
