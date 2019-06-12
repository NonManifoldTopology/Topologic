using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;
using Rhino.Geometry.Collections;

namespace TopologicGH
{
    public class TopologyByGeometry : GH_Component
    {
        public TopologyByGeometry()
          : base("Topology.ByGeometry", "Topology.ByGeometry", "Creates a Topology by geometry.", "Topologic", "Topology")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGeometryParameter("Geometry", "Geometry", "Geometry", GH_ParamAccess.item);
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
            GeometryBase ghGeometryBase = null;
            if (!DA.GetData(0, ref ghGeometryBase)) { return; }

            if (ghGeometryBase == null) { return; }
            
            Topologic.Topology topology = null;
            Point ghPoint = ghGeometryBase as Point;
            if (ghPoint != null)
            {
                topology = ByPoint(ghPoint.Location);
                DA.SetData(0, topology);
                return;
            }

            Curve ghCurve = ghGeometryBase as Curve;
            if (ghCurve != null)
            {
                LineCurve ghLine = ghCurve as LineCurve;
                if (ghLine != null)
                {
                    topology = ByLine(ghLine);
                    DA.SetData(0, topology);
                    return;
                }

                NurbsCurve ghNurbsCurve = ghCurve as NurbsCurve;
                if(ghNurbsCurve != null)
                {
                    //ghNurbsCurve.points
                }

                PolylineCurve ghPolylineCurve = ghCurve as PolylineCurve;
                if (ghPolylineCurve != null)
                {
                    topology = ByPolylineCurve(ghPolylineCurve);
                    DA.SetData(0, topology);
                    return;
                }

                throw new Exception("This type of curve is not yet supported.");
            }

            throw new Exception("This type of geometry is not yet supported.");

            //Surface ghSurface = ghGeometryBase as Surface;
            //if (ghSurface != null)
            //{
            //    BrepFace ghBrepFace = ghSurface as BrepFace;
            //    if (ghBrepFace != null)
            //    {
            //        topology = ByBrepFace(ghBrepFace);
            //        DA.SetData(0, topology);
            //        return;
            //    }

            //    throw new Exception("This type of surface is not yet supported.");
            //}


            //Brep ghBrep = ghGeometryBase as Brep;
            //if(ghBrep != null)
            //{
            //    topology = ByBrep(ghBrep);
            //    DA.SetData(0, topology);
            //    return;
            //}

        }

        Topologic.Vertex ByPoint(Point3d ghPoint)
        {
            return Topologic.Vertex.ByCoordinates(ghPoint.X, ghPoint.Y, ghPoint.Z);
        }

        Topologic.Edge ByLine(LineCurve ghLine)
        {
            Topologic.Vertex vertex1 = ByPoint(ghLine.PointAtStart);
            Topologic.Vertex vertex2 = ByPoint(ghLine.PointAtEnd);
            return Topologic.Edge.ByStartVertexEndVertex(vertex1, vertex2);
        }

        Topologic.Wire ByPolylineCurve(PolylineCurve ghPolylineCurve)
        {
            int numPoints = ghPolylineCurve.PointCount;
            if(numPoints < 1)
            {
                return null;
            }

            List<Topologic.Vertex> vertices = new List<Topologic.Vertex>();
            List<int> indices = new List<int>();
            for(int i = 0; i < numPoints; ++i)
            {
                Point3d ghPoint = ghPolylineCurve.Point(i);
                Topologic.Vertex vertex = ByPoint(ghPoint);
                vertices.Add(vertex);
                indices.Add(0);
            }

            if(ghPolylineCurve.IsClosed)
            {
                vertices.Add(vertices[0]);
                indices.Add(0);
                List<List<int>> listOfIndices = new List<List<int>>();
                listOfIndices.Add(indices);
                return Topologic.Topology.ByVerticesIndices(vertices, listOfIndices)[0].Wires[0];
            }
            else
            {
                List<List<int>> listOfIndices = new List<List<int>>();
                listOfIndices.Add(indices);
                return Topologic.Topology.ByVerticesIndices(vertices, listOfIndices)[0] as Topologic.Wire;
            }
        }

        //Topologic.Face ByBrepFace(BrepFace ghBrepFace)
        //{
        //    BrepLoop ghOuterLoop = ghBrepFace.OuterLoop;
        //    BrepLoopList ghLoops = ghBrepFace.Loops;
        //    Topologic.Face face = Topologic.Face.ByExternalInternalBoundaries();
        //}

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
            get { return new Guid("6f38afdb-d218-4b22-8b50-853dc46fce9c"); }
        }
    }
}
