using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class FaceAddApertureDesign : GH_Component
    {

        public FaceAddApertureDesign()
          : base("Face.AddApertureDesign", "Face.AddApertureDesign", "Adds an Aperture design to a Face.", "Topologic", "Face")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Face", "Face", "Face", GH_ParamAccess.item);
            pManager.AddGenericParameter("Aperture Design", "Aperture Design", "Aperture Design", GH_ParamAccess.item);
            pManager.AddIntegerParameter("Number of Edge Samples", "Number of Edge Samples", "Number of Edge Samples", GH_ParamAccess.item);
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
            Topologic.Face face = null;
            Topologic.Face apertureDesign = null;
            int numOfEdgeSamples = 0;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref face)) { return; }
            if (!DA.GetData(1, ref apertureDesign)) { return; }
            if (!DA.GetData(2, ref numOfEdgeSamples)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (face == null) { return; }
            if (apertureDesign == null) { return; }
            if (numOfEdgeSamples < 2) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            // Reverse the array of character.
            Topologic.Face newFace = face.AddApertureDesign(apertureDesign, numOfEdgeSamples);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, newFace);
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
            get { return new Guid("98c35541-72cd-4a73-b6fa-08fb57be6b1d"); }
        }
    }
}
