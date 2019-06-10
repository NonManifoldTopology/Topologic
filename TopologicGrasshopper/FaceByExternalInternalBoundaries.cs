using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class FaceByExternalInternalBoundaries : GH_Component
    {

        public FaceByExternalInternalBoundaries()
          : base("Face.ByExternalInternalBoundaries", "Face.ByExternalInternalBoundaries", "Creates a Face by an external boundary (Wire) and internal boundaries (Wires).", "Topologic", "Face")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("External Boundary", "External Boundary", "External Boundary", GH_ParamAccess.item);
            pManager.AddGenericParameter("Internal Boundaries", "Internal Boundaries", "Internal Boundaries", GH_ParamAccess.list);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Face", "Face", "Face", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Wire externalBoundary = null;
            List<Topologic.Wire> internalBoundaries = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref externalBoundary)) { return; }
            if (!DA.GetData(1, ref internalBoundaries)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (externalBoundary == null) { return; }
            if (internalBoundaries == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            // Reverse the array of character.
            Topologic.Face face = Topologic.Face.ByExternalInternalBoundaries(externalBoundary, internalBoundaries);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, face);
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
            get { return new Guid("742aaf39-dc23-4549-8105-5f73d9071808"); }
        }
    }
}
