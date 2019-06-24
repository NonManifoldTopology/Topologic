using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;
using Rhino.Geometry.Collections;
using Topologic;

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
                topology = ByCurve(ghCurve);
                DA.SetData(0, topology);
                return;
            }

            Surface ghSurface = ghGeometryBase as Surface;
            if (ghSurface != null)
            {
                topology = BySurface(ghSurface);
                DA.SetData(0, topology);
                return;
            }

            Brep ghBrep = ghGeometryBase as Brep;
            if (ghBrep != null)
            {
                topology = ByBrep(ghBrep);
                DA.SetData(0, topology);
                return;
            }

            Mesh ghMesh = ghGeometryBase as Mesh;
            if (ghMesh != null)
            {
                topology = ByMesh(ghMesh);
                DA.SetData(0, topology);
                return;
            }

            //BrepLoop ghBrepLoop = ghGeometryBase as BrepLoop;
            //if (ghBrepLoop != null)
            //{
            //    topology = ByBrepLoop(ghBrepLoop);
            //    DA.SetData(0, topology);
            //    return;
            //}

            throw new Exception("This type of geometry is not yet supported.");
        }

        //private Topology ByBrepLoop(BrepLoop ghBrepLoop)
        //{
        //    throw new NotImplementedException();
        //}

        private Topologic.Vertex ByPoint(Point3d ghPoint)
        {
            return Topologic.Vertex.ByCoordinates(ghPoint.X, ghPoint.Y, ghPoint.Z);
        }

        private Topologic.Topology ByCurve(Curve ghCurve)
        {
            LineCurve ghLine = ghCurve as LineCurve;
            if (ghLine != null)
            {
                return ByLine(ghLine);
            }

            NurbsCurve ghNurbsCurve = ghCurve as NurbsCurve;
            if (ghNurbsCurve != null)
            {
                return ByNurbsCurve(ghNurbsCurve);
            }

            ArcCurve ghArcCurve = ghCurve as ArcCurve;
            if (ghArcCurve != null)
            {
                return ByArcCurve(ghArcCurve);
            }

            BrepEdge ghBrepEdge = ghCurve as BrepEdge;
            if (ghBrepEdge != null)
            {
                return ByBrepEdge(ghBrepEdge);
            }

            //BrepTrim ghBrepTrim = ghCurve as BrepTrim;
            //if (ghBrepTrim != null)
            //{
            //    return ByBrepTrim(ghBrepTrim);
            //}

            PolylineCurve ghPolylineCurve = ghCurve as PolylineCurve;
            if (ghPolylineCurve != null)
            {
                return ByPolylineCurve(ghPolylineCurve);
            }

            PolyCurve ghPolyCurve = ghCurve as PolyCurve;
            if (ghPolyCurve != null)
            {
                return ByPolyCurve(ghPolyCurve);
            }
            
            throw new Exception("This type of curve is not yet supported.");
        }

        private Topologic.Wire ByPolyCurve(PolyCurve ghPolyCurve)
        {
            Curve[] ghCurves = ghPolyCurve.Explode();
            List<Topologic.Edge> edges = new List<Topologic.Edge>();
            foreach(Curve ghCurve in ghCurves)
            {
                Topologic.Topology topology = ByCurve(ghCurve);

                Topologic.Edge edge = topology as Topologic.Edge;
                if(edge != null)
                {
                    edges.Add(edge);
                    continue;
                }

                Topologic.Wire wire = topology as Topologic.Wire;
                if(wire != null)
                {
                    edges.AddRange(wire.Edges);
                    continue;
                }
            }

            return Topologic.Wire.ByEdges(edges);
        }

        private Topologic.Edge ByBrepEdge(BrepEdge ghBrepEdge)
        {
            NurbsCurve ghNurbsCurve = ghBrepEdge.ToNurbsCurve();
            return ByNurbsCurve(ghNurbsCurve);
        }

        private Topologic.Edge ByArcCurve(ArcCurve ghArcCurve)
        {
            NurbsCurve ghNurbsCurve = ghArcCurve.ToNurbsCurve();
            return ByNurbsCurve(ghNurbsCurve);
        }

        private Topologic.Edge ByNurbsCurve(NurbsCurve ghNurbsCurve)
        {
            int degree = ghNurbsCurve.Degree;
            bool isClosed = ghNurbsCurve.IsClosed;
            bool isPeriodic = ghNurbsCurve.IsPeriodic;
            bool isRational = ghNurbsCurve.IsRational;
            NurbsCurveKnotList ghKnots = ghNurbsCurve.Knots;
            List<double> knots = ghKnots.ToList();
            NurbsCurvePointList ghControlPoints = ghNurbsCurve.Points;
            List<Topologic.Vertex> controlPoints = new List<Topologic.Vertex>();
            List<double> weights = new List<double>();
            for (int i = 0; i < ghControlPoints.Count; ++i)
            {
                controlPoints.Add(ByPoint(ghControlPoints[i].Location));
                weights.Add(ghControlPoints[i].Weight);
            }

            return Topologic.Edge.ByNurbsParameters(controlPoints, weights, knots, isRational, isPeriodic, degree);
        }

        private Topologic.Edge ByLine(LineCurve ghLine)
        {
            Topologic.Vertex vertex1 = ByPoint(ghLine.PointAtStart);
            Topologic.Vertex vertex2 = ByPoint(ghLine.PointAtEnd);
            return Topologic.Edge.ByStartVertexEndVertex(vertex1, vertex2);
        }

        private Topologic.Wire ByPolylineCurve(PolylineCurve ghPolylineCurve)
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

        private Topologic.Face BySurface(Surface ghSurface)
        {
            SumSurface ghSumSurface = ghSurface as SumSurface;
            if (ghSumSurface != null)
            {
                return BySumSurface(ghSumSurface);
            }

            RevSurface ghRevSurface = ghSurface as RevSurface;
            if (ghRevSurface != null)
            {
                return ByRevSurface(ghRevSurface);
            }

            PlaneSurface ghPlaneSurface = ghSurface as PlaneSurface;
            if (ghPlaneSurface != null)
            {
                return ByPlaneSurface(ghPlaneSurface);
            }

            //ClippingPlaneSurface ghClippingPlaneSurface = ghSurface as ClippingPlaneSurface;
            //if (ghClippingPlaneSurface != null)
            //{
            //    return ByClippingPlaneSurface(ghClippingPlaneSurface);
            //}

            Extrusion ghExtrusion = ghSurface as Extrusion;
            if (ghExtrusion != null)
            {
                return ByExtrusion(ghExtrusion);
            }

            NurbsSurface ghNurbsSurface = ghSurface as NurbsSurface;
            if (ghNurbsSurface != null)
            {
                return ByNurbsSurface(ghNurbsSurface);
            }

            BrepFace ghBrepFace = ghSurface as BrepFace;
            if (ghBrepFace != null)
            {
                return ByBrepFace(ghBrepFace);
            }

            throw new Exception("This type of surface is not yet supported.");
        }

        private Topologic.Face ByPlaneSurface(PlaneSurface ghPlaneSurface)
        {
            NurbsSurface ghNurbsSurface = ghPlaneSurface.ToNurbsSurface();
            return ByNurbsSurface(ghNurbsSurface);
        }

        private Topologic.Face ByRevSurface(RevSurface ghRevSurface)
        {
            NurbsSurface ghNurbsSurface = ghRevSurface.ToNurbsSurface();
            return ByNurbsSurface(ghNurbsSurface);
        }

        private Topologic.Face BySumSurface(SumSurface ghSumSurface)
        {
            NurbsSurface ghNurbsSurface = ghSumSurface.ToNurbsSurface();
            return ByNurbsSurface(ghNurbsSurface);
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
