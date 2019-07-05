using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Rhino.Geometry;
using Topologic;

namespace TopologicGH
{
    public class TopologyGeometry : GH_Component
    {
        public TopologyGeometry()
          : base("Topology.Geometry", "Topology.Geometry", "Creates a geometry from the Topology.", "Topologic", "Topology")
        {
        }

        /// <summary>
        /// Registers all the input parameters for this component.
        /// </summary>
        protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
        {
            pManager.AddGenericParameter("Topology", "Topology", "Topology", GH_ParamAccess.item);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Geometry", "Geometry", "Geometry", GH_ParamAccess.item);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Topology topology = null;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref topology)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (topology == null) { return; }
            //if (data.Length == 0) { return; }

            // Convert the String to a character array.
            //char[] chars = data.ToCharArray();

            // Reverse the array of character.
            Object geometry = ToGeometry(topology);

            // Use the DA object to assign a new String to the first output parameter.
            DA.SetData(0, geometry);
        }

        private object ToGeometry(Topology topology)
        {
            Vertex vertex = topology as Vertex;
            if (vertex != null)
            {
                return ToPoint(vertex);
            }

            Edge edge = topology as Edge;
            if (edge != null)
            {
                return ToCurve(edge);
            }
            
            Wire wire = topology as Wire;
            if (wire != null)
            {
                return ToPolyCurve(wire);
            }

            Face face = topology as Face;
            if (face != null)
            {
                return ToSurface(face);
            }

            Aperture aperture = topology as Aperture;
            if (aperture != null)
            {
                return ToGeometry(aperture.Topology);
            }

            throw new Exception("The type of the input topology is not recognized.");
        }

        private Brep ToSurface(Face face)
        {
            // 1. Compute the base NURBS surface
            // Based on https://developer.rhino3d.com/api/RhinoCommon/html/M_Rhino_Geometry_NurbsSurface_Create.htm
            Topologic.NurbsSurface nurbsSurface = face.BasicGeometry as Topologic.NurbsSurface;
            if(nurbsSurface == null)
            {
                throw new Exception("An invalid surface is created.");
            }

            int uDegree = nurbsSurface.UDegree;
            int vDegree = nurbsSurface.VDegree;
            bool isRational = nurbsSurface.IsURational && nurbsSurface.IsVRational;
            int uCount = nurbsSurface.NumOfUControlVertices;
            int vCount = nurbsSurface.NumOfVControlVertices;

            Rhino.Geometry.NurbsSurface ghNurbsSurface = Rhino.Geometry.NurbsSurface.Create(
                3, 
                isRational,
                uDegree + 1,
                vDegree + 1,
                uCount,
                vCount
                );

            int i = 0;
            for (int u = 0; u < uCount; ++u)
            {
                for (int v = 0; v < vCount; ++v)
                {
                    Topologic.Vertex controlVertex = nurbsSurface.ControlVertex(u, v);
                    ghNurbsSurface.Points.SetControlPoint(u, v, ToPoint(controlVertex));
                    ++i;
                }
            }

            List<double> uKnots = nurbsSurface.UKnots;
            for (int u = 0; u < uKnots.Count; u++)
            {
                ghNurbsSurface.KnotsU[u] = uKnots[u];
            }

            List<double> vKnots = nurbsSurface.VKnots;
            for (int v = 0; v < vKnots.Count; v++)
            {
                ghNurbsSurface.KnotsV[v] = vKnots[v];
            }
            
            if (!ghNurbsSurface.IsValid)
            {
                throw new Exception("A valid surface cannot be created from this Face.");
            }

            // 2. Create the Brep
            Brep ghBrep = new Brep();

            // 2a. Add vertices
            List<Vertex> vertices = face.Vertices;
            foreach (Vertex vertex in vertices)
            {
                Point3d ghPoint = ToPoint(vertex);
                ghBrep.Vertices.Add(ghPoint, 0.0);
            }

            // 2b. Add 3D curves and edges
            List<Edge> edges = face.Edges;
            Dictionary<Edge, int> edge2DIndices = new Dictionary<Edge, int>();
            Dictionary<Edge, int> edge3DIndices = new Dictionary<Edge, int>();
            Dictionary<Edge, BrepEdge> edgeIndices = new Dictionary<Edge, BrepEdge>();
            foreach (Edge edge in edges)
            {
                Curve ghCurve3D = ToCurve(edge);
                Curve ghCurve2D = ghNurbsSurface.Pullback(ghCurve3D, 0.0);
                int curve3DID = ghBrep.Curves3D.Add(ghCurve3D);
                int curve2DID = ghBrep.Curves2D.Add(ghCurve2D);

                BrepEdge ghBrepEdge = ghBrep.Edges.Add(curve3DID);
                edge3DIndices.Add(edge, curve3DID);

                edge2DIndices.Add(edge, curve2DID);

                edgeIndices.Add(edge, ghBrepEdge);
            }

            // 2c. Add surface
            int ghSurfaceIndex = ghBrep.AddSurface(ghNurbsSurface);

            // 2d. Add face
            BrepFace ghBrepFace = ghBrep.Faces.Add(ghSurfaceIndex);

            // 2e. Create outer loop
            Wire outerWire = face.ExternalBoundary;
            List<Edge> outerEdges = outerWire.Edges;
            BrepLoop ghBrepOuterLoop = ghBrep.Loops.Add(BrepLoopType.Outer, ghBrepFace);

            // 2f. For each loop, add a trim (2D edge)
            foreach (Edge outerEdge in outerEdges)
            {
                int outerEdge2DIndex = edge2DIndices.
                    Where(edgeIndexPair => edgeIndexPair.Key.IsSame(outerEdge)).
                    Select(edgeIndexPair => edgeIndexPair.Value).
                    FirstOrDefault();

                //int outerEdge3DIndex = edge3DIndices.
                //    Where(edgeIndexPair => edgeIndexPair.Key.IsSame(outerEdge)).
                //    Select(edgeIndexPair => edgeIndexPair.Value).
                //    FirstOrDefault();

                Curve ghOuterCurve2D = ghBrep.Curves2D[outerEdge2DIndex];
                //Curve ghOuterCurve3D = ghBrep.Curves3D[outerEdge3DIndex];
                
                BrepEdge ghBrepEdge = edgeIndices.
                    Where(edgeIndexPair => edgeIndexPair.Key.IsSame(outerEdge)).
                    Select(edgeIndexPair => edgeIndexPair.Value).
                    FirstOrDefault();
                
                BrepTrim ghBrepTrim = ghBrep.Trims.Add(ghBrepEdge, false, ghBrepOuterLoop, outerEdge2DIndex);
                ghBrepTrim.IsoStatus = IsoStatus.None;
                ghBrepTrim.TrimType = BrepTrimType.Boundary;
            }
            
            // 2g. Create inner loops
            List<Wire> innerWires= face.InternalBoundaries;
            foreach (Wire innerWire in innerWires)
            {
                BrepLoop ghBrepInnerLoop = ghBrep.Loops.Add(BrepLoopType.Inner, ghBrepFace);
                List<Edge> innerEdges = innerWire.Edges;
                foreach (Edge innerEdge in innerEdges)
                {
                    int innerEdge2DIndex = edge2DIndices.
                        Where(edgeIndexPair => edgeIndexPair.Key.IsSame(innerEdge)).
                        Select(edgeIndexPair => edgeIndexPair.Value).
                        FirstOrDefault();

                    //int innerEdge3DIndex = edge3DIndices.
                    //    Where(edgeIndexPair => edgeIndexPair.Key.IsSame(innerEdge)).
                    //    Select(edgeIndexPair => edgeIndexPair.Value).
                    //    FirstOrDefault();

                    Curve ghinnerCurve2D = ghBrep.Curves2D[innerEdge2DIndex];
                    //Curve ghinnerCurve3D = ghBrep.Curves3D[innerEdge3DIndex];

                    BrepEdge ghBrepEdge = edgeIndices.
                        Where(edgeIndexPair => edgeIndexPair.Key.IsSame(innerEdge)).
                        Select(edgeIndexPair => edgeIndexPair.Value).
                        FirstOrDefault();

                    BrepTrim ghBrepTrim = ghBrep.Trims.Add(ghBrepEdge, false, ghBrepInnerLoop, innerEdge2DIndex);
                    ghBrepTrim.IsoStatus = IsoStatus.None;
                    ghBrepTrim.TrimType = BrepTrimType.Boundary;
                }
            }
            ghBrepFace.OrientationIsReversed = false;

            String brepFaceLog = "";
            if (!ghBrepFace.IsValidWithLog(out brepFaceLog))
            {
                throw new Exception("Fails to create a valid Face with the following message: " + brepFaceLog);
            }

            String brepLog = "";
            if (!ghBrep.IsValidWithLog(out brepLog))
            {
                throw new Exception("Fails to create a valid Brep with the following message: " + brepLog);
            }

            return ghBrep;
        }

        private PolyCurve ToPolyCurve(Wire wire)
        {
            PolyCurve ghPolyCurve = new PolyCurve();
            List<Edge> edges = wire.Edges;
            foreach (Edge edge in edges)
            {
                Curve ghCurve = ToCurve(edge);

                try
                {
                    ghPolyCurve.Append(ghCurve);
                }
                catch
                {
                    throw new Exception("Fails creating a PolyCurve from a Wire.");
                }
            }

            String log = "";
            if(!ghPolyCurve.IsValidWithLog(out log))
            {
                throw new Exception(log);
            }

            return ghPolyCurve;
        }

        private Curve ToCurve(Edge edge)
        {
            Object edgeGeometry = edge.BasicGeometry;

            Topologic.Line line = edgeGeometry as Topologic.Line;
            if(line != null)
            {
                return ToLine(edge);
            }

            Topologic.NurbsCurve nurbsCurve = edgeGeometry as Topologic.NurbsCurve;
            if (nurbsCurve != null)
            {
                return ToNurbsCurve(nurbsCurve);
            }

            throw new Exception("This Edge creates an unrecognized Geometry.");
        }

        private Rhino.Geometry.NurbsCurve ToNurbsCurve(Topologic.NurbsCurve nurbsCurve)
        {
            // Based on https://developer.rhino3d.com/api/RhinoCommon/html/P_Rhino_Geometry_NurbsCurve_Knots.htm
            bool isPeriodic = nurbsCurve.IsPeriodic;
            bool isRational = nurbsCurve.IsRational;
            int degree = nurbsCurve.Degree;
            List<Topologic.Vertex> controlVertices = nurbsCurve.ControlVertices;
            List<Point3d> ghControlPoints = new List<Point3d>();

            Rhino.Geometry.NurbsCurve ghNurbsCurve = new Rhino.Geometry.NurbsCurve(3, isRational, degree + 1, controlVertices.Count);
            
            int i = 0;
            foreach(Topologic.Vertex controlVertex in controlVertices)
            {
                Point3d ghControlPoint = ToPoint(controlVertex);
                ghControlPoints.Add(ghControlPoint);
                ghNurbsCurve.Points.SetPoint(i, ghControlPoint);
                ++i;
            }

            List<double> knots = nurbsCurve.Knots;
            knots = knots.GetRange(1, knots.Count - 2);
            i = 0;
            foreach (double knot in knots)
            {
                ghNurbsCurve.Knots[i] = knot;
                ++i;
            }

            String log = "";
            if(ghNurbsCurve.IsValidWithLog(out log))
            {
                return ghNurbsCurve;
            }

            throw new Exception("A valid curve cannot be created from this Edge.");
        }

        private LineCurve ToLine(Edge edge)
        {
            Vertex startVertex = edge.StartVertex;
            Point3d ghStartPoint = ToPoint(startVertex);
            Vertex endVertex = edge.EndVertex;
            Point3d ghEndPoint = ToPoint(endVertex);
            return new LineCurve(ghStartPoint, ghEndPoint);
        }

        private Point3d ToPoint(Vertex vertex)
        {
            List<double> coordinates = vertex.Coordinates;
            return new Point3d(coordinates[0], coordinates[1], coordinates[2]);
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
            get { return new Guid("25a0bb53-da53-4789-9e43-a57060481a53"); }
        }
    }
}
