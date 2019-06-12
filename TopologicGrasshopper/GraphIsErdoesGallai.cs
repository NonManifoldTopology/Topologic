using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;

namespace TopologicGrasshopper
{
    public class GraphIsErdoesGallai : GH_Component
    {

        public GraphIsErdoesGallai()
          : base("Graph.IsErdoesGallai", "Graph.IsErdoesGallai", "Returns True if the input sequence satisfies the Erdoes Gallai theorem. It returns False otherwise.", "Topologic", "Graph")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Graph", "Graph", "Graph", GH_ParamAccess.item);
            pManager.AddIntegerParameter("Sequence", "Sequence", "Sequence", GH_ParamAccess.list);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddBooleanParameter("Bool", "Bool", "Bool", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Graph graph = null;
            List<int> sequence = new List<int>();

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref graph)) { return; }
            if (!DA.GetDataList(1, sequence)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (graph == null) { return; }
            if (sequence == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            // Reverse the array of character.
            bool isErdoesGallai = graph.IsErdoesGallai(sequence);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, isErdoesGallai);
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
            get { return new Guid("bfed753e-7fd4-416c-9399-ae3368c5fff7"); }
        }
    }
}
