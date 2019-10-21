// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel;
using Grasshopper.Kernel.Data;
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
            pManager.AddNumberParameter("Tolerance", "Tolerance", "Tolerance", GH_ParamAccess.item, 0.0001);
        }

        /// <summary>
        /// Registers all the output parameters for this component.
        /// </summary>
        protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
        {
            pManager.AddGenericParameter("Geometry", "Geometry", "Geometry", GH_ParamAccess.list);
        }

        /// <summary>
        /// This is the method that actually does the work.
        /// </summary>
        /// <param name="DA">The DA object is used to retrieve from inputs and store in outputs.</param>
        protected override void SolveInstance(IGH_DataAccess DA)
        {
            // Declare a variable for the input String
            Topologic.Topology topology = null;
            double tolerance = 0.0001;

            // Use the DA object to retrieve the data inside the first input parameter.
            // If the retieval fails (for example if there is no data) we need to abort.
            if (!DA.GetData(0, ref topology)) { return; }
            if (!DA.GetData(1, ref tolerance)) { return; }

            // If the retrieved data is Nothing, we need to abort.
            // We're also going to abort on a zero-length String.
            if (topology == null) { return; }

            List<Object> geometries = ToGeometry(topology, tolerance);
            
            DA.SetDataList(0, geometries);
        }

        GH_Path ListToTreePath(List<int> path)
        {
            if(path.Count == 0)
            {
                return null;
            }

            if(path.Count == 1)
            {
                return new GH_Path(path[0]);
            }

            return new GH_Path(path.ToArray());
        }

        void AddObjectToTree(Object o, List<int> path, ref Grasshopper.DataTree<Object> output)
        {
            ICollection<Object> innerList = o as ICollection<Object>;
            if (innerList != null)
            {
                List<int> newPath = new List<int>(path);
                newPath.Add(0);
                int i = 0;
                foreach (Object innerO in innerList)
                {
                    List<int> newPath2 = new List<int>(path);
                    newPath2.Add(i);
                    AddObjectToTree(innerO, newPath2, ref output);
                    ++i;
                }
            }
            else
            {
                output.Add(o, ListToTreePath(path));
            }
        }

        private List<Object> ToGeometry(Topology topology, double tolerance)
        {
            if(topology == null)
            {
                return null;
            }

            List<Object> geometries = new List<Object>();
            Vertex vertex = topology as Vertex;
            if (vertex != null)
            {
                geometries.Add(ToPoint(vertex));
                return geometries;
            }

            Edge edge = topology as Edge;
            if (edge != null)
            {
                geometries.Add(ToCurve(edge));
                return geometries;
            }
            
            Wire wire = topology as Wire;
            if (wire != null)
            {
                return ToCurves(wire);
            }

            Face face = topology as Face;
            if (face != null)
            {
                geometries.Add(ToSurface(face, tolerance));
                return geometries;
            }

            Shell shell = topology as Shell;
            if (shell != null)
            {
                return ToBrep(shell, tolerance);
            }

            Cell cell = topology as Cell;
            if (cell != null)
            {
                return ToBrep(cell, tolerance);
            }

            CellComplex cellComplex = topology as CellComplex;
            if (cellComplex != null)
            {
                return ToBreps(cellComplex, tolerance);
            }

            Cluster cluster = topology as Cluster;
            if (cluster != null)
            {
                return ToGeometries(cluster, tolerance);
            }

            Aperture aperture = topology as Aperture;
            if (aperture != null)
            {
                return ToGeometry(aperture.Topology, tolerance);
            }

            throw new Exception("The type of the input topology is not recognized.");
        }

        private List<Object> ToBreps(CellComplex cellComplex, double tolerance)
        {
            List<Cell> cells = cellComplex.Cells;
            List<Object> ghBreps = new List<Object>();
            foreach (Cell cell in cells)
            {
                List<Object> ghBrep = ToBrep(cell, tolerance);
                
                ghBreps.AddRange(ghBrep);
            }
            return ghBreps;
        }

        private List<Object> ToGeometries(Topology topology, double tolerance)
        {
            List<Topology> subTopologies = topology.SubTopologies;
            List<Object> ghGeometries = new List<Object>();
            foreach(Topology subTopology in subTopologies)
            {
                Object ghGeometry = ToGeometry(subTopology, tolerance);

                List<Object> ghGeometryAsList = ghGeometry as List<Object>;
                if (ghGeometryAsList != null)
                {
                    ghGeometries.AddRange(ghGeometryAsList);
                }
                else
                {
                    ghGeometries.Add(ghGeometry);
                }
            }
            return ghGeometries;
        }

        private List<Object> ToBrep(Topology topology, double tolerance)
        {
            List<Face> faces = topology.Faces;
            List<Brep> ghBrepSurfaces = new List<Brep>();
            foreach(Face face in faces)
            {
                Brep ghBrepSurface = ToSurface(face, tolerance);
                ghBrepSurfaces.Add(ghBrepSurface);
            }
            
            if(ghBrepSurfaces.Count == 0)
            {
                return null;
            }

            Brep[] ghJoinedBreps = Brep.JoinBreps(ghBrepSurfaces, 0.1);
            List<Object> ghJoinedBrepsAsObjects = new List<Object>();
            ghJoinedBrepsAsObjects.AddRange(ghJoinedBreps);
            return ghJoinedBrepsAsObjects;
        }

        //private void ProcessFace(
        //    Wire wire, ref Brep ghBrep, BrepFace ghBrepFace, BrepLoopType ghBrepLoopType, Rhino.Geometry.Surface ghSurface, 
        //    Dictionary<Edge, Tuple<int, int>> edge2DIndices, Dictionary<Edge, BrepEdge> edgeIndices)
        //{
        //    List<Edge> edges = wire.Edges;
        //    BrepLoop ghBrepLoop = ghBrep.Loops.Add(ghBrepLoopType, ghBrepFace);

        //    // 2f.For each loop, add a trim(2D edge)
        //    List<BrepEdge> ghEdges = new List<BrepEdge>();
        //    List<Tuple<Curve, int, Curve, int>> gh2DCurves = new List<Tuple<Curve, int, Curve, int>>(); // original curve, index, reverse curve, reverse index
        //    foreach (Edge edge in edges)
        //    {
        //        Tuple<int, int> thisEdge2DIndices = edge2DIndices.
        //            Where(edgeIndexPair => edgeIndexPair.Key.IsSame(edge)).
        //            Select(edgeIndexPair => edgeIndexPair.Value).
        //            FirstOrDefault();

        //        int thisEdge2DIndex = thisEdge2DIndices.Item1;
        //        int thisReverseEdge2DIndex = thisEdge2DIndices.Item2;

        //        Curve ghCurve2D = ghBrep.Curves2D[thisEdge2DIndex];
        //        Curve ghReverseCurve2D = ghBrep.Curves2D[thisReverseEdge2DIndex];
        //        gh2DCurves.Add(Tuple.Create(ghCurve2D, thisEdge2DIndex, ghReverseCurve2D, thisReverseEdge2DIndex));

        //        BrepEdge ghBrepEdge = edgeIndices.
        //            Where(edgeIndexPair => edgeIndexPair.Key.IsSame(edge)).
        //            Select(edgeIndexPair => edgeIndexPair.Value).
        //            FirstOrDefault();

        //        String ghBrepEdgeLog = "";
        //        if (!ghBrepEdge.IsValidWithLog(out ghBrepEdgeLog))
        //        {
        //            throw new Exception("Fails to create a valid Brep with the following message: " + ghBrepEdgeLog);
        //        }

        //        ghEdges.Add(ghBrepEdge);
        //    }

        //    for (int currentEntryID = 0; currentEntryID < gh2DCurves.Count; ++currentEntryID)
        //    {
        //        int previousEntryID = currentEntryID - 1;
        //        if (previousEntryID < 0)
        //        {
        //            previousEntryID = edges.Count - 1;
        //        }

        //        bool isCurrentStartEqualToPreviousStart = gh2DCurves[currentEntryID].Item1.PointAtStart.DistanceTo(
        //                                                    gh2DCurves[previousEntryID].Item1.PointAtStart) < tolerance;
        //        bool isCurrentStartEqualToPreviousEnd = gh2DCurves[currentEntryID].Item1.PointAtStart.DistanceTo(
        //                                                    gh2DCurves[previousEntryID].Item1.PointAtEnd) < tolerance;
        //        bool isTrimReversed = false;
        //        if (!isCurrentStartEqualToPreviousStart && !isCurrentStartEqualToPreviousEnd)
        //        {
        //            // Reverse trim
        //            isTrimReversed = true;
        //        }

        //        BrepTrim ghBrepTrim = ghBrep.Trims.Add(
        //            ghEdges[currentEntryID],       // 3D edge
        //            isTrimReversed,                // is reversed?
        //            ghBrepLoop,                    // 2D loop
        //            isTrimReversed ? gh2DCurves[currentEntryID].Item4 : gh2DCurves[currentEntryID].Item2);  // 2D curve index, use the reversed one if reversed

        //        ghBrepTrim.IsoStatus = ghSurface.IsIsoparametric(gh2DCurves[currentEntryID].Item1);
        //        ghBrepTrim.TrimType = BrepTrimType.Boundary;
        //        ghBrepTrim.SetTolerances(0.0, 0.0);

        //        String ghBrepTrimLog = "";
        //        if (!ghBrepTrim.IsValidWithLog(out ghBrepTrimLog))
        //        {
        //            throw new Exception("Fails to create a valid BrepTrim with the following message: " + ghBrepTrimLog);
        //        }
        //    }

        //    String brepLoopLog = "";
        //    if (!ghBrepLoop.IsValidWithLog(out brepLoopLog))
        //    {
        //        throw new Exception("Fails to create a valid outer BrepLoop with the following message: " + brepLoopLog);
        //    }
        //}

        private Rhino.Geometry.NurbsSurface ToRhinoNurbsSurface(Topologic.NurbsSurface nurbsSurface)
        {
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
                    ghNurbsSurface.Points.SetPoint(u, v, ToPoint(controlVertex));
                    ++i;
                }
            }

            List<double> uKnots = nurbsSurface.UKnots;
            uKnots = uKnots.GetRange(1, uKnots.Count - 2);
            for (int u = 0; u < uKnots.Count; u++)
            {
                ghNurbsSurface.KnotsU[u] = uKnots[u];
            }

            List<double> vKnots = nurbsSurface.VKnots;
            vKnots = vKnots.GetRange(1, vKnots.Count - 2);
            for (int v = 0; v < vKnots.Count; v++)
            {
                ghNurbsSurface.KnotsV[v] = vKnots[v];
            }

            if (!ghNurbsSurface.IsValid)
            {
                throw new Exception("A valid surface cannot be created from this Face.");
            }

            return ghNurbsSurface;
        }

        private Rhino.Geometry.PlaneSurface ToRhinoPlaneSurface(PlanarSurface planarSurface, Face face)
        {
            // From Topologic
            List<double> coefficients = planarSurface.Coefficients;
            double a = coefficients[0];
            double b = coefficients[1];
            double c = coefficients[2];
            double d = coefficients[3];
            Vertex faceCenterOfMass = face.CenterOfMass;
            Point3d ghFaceCenterOfMass = ToPoint(faceCenterOfMass);

            Rhino.Geometry.Plane ghPlane = new Rhino.Geometry.Plane(a, b, c, d);

            double occtXMin = planarSurface.XMin;
            double occtXMax = planarSurface.XMax;
            double occtAbsDeltaX = Math.Abs(occtXMax - occtXMin);
            double occtHalfDeltaX = occtAbsDeltaX / 2.0;

            double occtYMin = planarSurface.YMin;
            double occtYMax = planarSurface.YMax;
            double occtAbsDeltaY = Math.Abs(occtYMax - occtYMin);
            double occtHalfDeltaY = occtAbsDeltaY / 2.0;

            double ghXMin = occtXMin;// - occtHalfDeltaX - safetyMarginX;
            double ghXMax = occtXMax;// - occtHalfDeltaX + safetyMarginX;
            double ghYMin = occtYMin;// - occtHalfDeltaY - safetyMarginY;
            double ghYMax = occtYMax;// - occtHalfDeltaY + safetyMarginY;


            Interval xExtents = new Interval(
                ghXMin,
                ghXMax);
            Interval yExtents = new Interval(
                ghYMin,
                ghYMax);

            PlaneSurface ghPlaneSurface = new PlaneSurface(ghPlane, xExtents, yExtents);
            Point3d ghCentroid = Rhino.Geometry.AreaMassProperties.Compute(ghPlaneSurface).Centroid;
            Vector3d ghTranslationVector = ghFaceCenterOfMass - ghCentroid;
            ghPlaneSurface.Translate(ghTranslationVector);
            if (!ghPlaneSurface.IsValid)
            {
                throw new Exception("A valid surface cannot be created from this Face.");
            }

            return ghPlaneSurface;
        }

        private Rhino.Geometry.Surface ToRhinoSurface(Face face)
        {
            Object faceGeometry = face.BasicGeometry;

            // 1. Compute the base surface
            // Based on https://developer.rhino3d.com/api/RhinoCommon/html/M_Rhino_Geometry_NurbsSurface_Create.htm
            Topologic.NurbsSurface nurbsSurface = faceGeometry as Topologic.NurbsSurface;
            if (nurbsSurface != null)
            {
                return ToRhinoNurbsSurface(nurbsSurface);
            }

            Topologic.PlanarSurface planarSurface = faceGeometry as Topologic.PlanarSurface;
            if (planarSurface != null)
            {
                //Topologic.NurbsSurface planarSurfaceAsNurbs = planarSurface.ToNurbsSurface();
                //return ToRhinoNurbsSurface(planarSurfaceAsNurbs);
                PlaneSurface planeSurface = ToRhinoPlaneSurface(planarSurface, face);
                return planeSurface;
                //Rhino.Geometry.NurbsSurface planeSurfaceAsNurbsSurface = planeSurface.ToNurbsSurface();
                //return planeSurfaceAsNurbsSurface;
            }

            throw new Exception("An invalid surface is created.");
        }

        private Brep ToSurface(Face face, double tolerance)
        {
            Rhino.Geometry.Surface ghSurface = ToRhinoSurface(face);

            double width = 0.0, height = 0.0;
            bool canGetSurfaceSize = ghSurface.GetSurfaceSize(out width, out height);
            if(!canGetSurfaceSize)
            {
                throw new Exception("Fails to get the surface size.");
            }
            double maxSize = Math.Max(width, height);
            double maxSizeAndMargin = maxSize + 2;
            ghSurface = ghSurface.Extend(IsoStatus.North, maxSizeAndMargin, true);
            ghSurface = ghSurface.Extend(IsoStatus.South, maxSizeAndMargin, true);
            ghSurface = ghSurface.Extend(IsoStatus.West, maxSizeAndMargin, true);
            ghSurface = ghSurface.Extend(IsoStatus.East, maxSizeAndMargin, true);
            //List<Wire> wires = face.Wires;

            //List<GeometryBase> ghGeometryBases = new List<GeometryBase>();
            //foreach (Wire wire in wires)
            //{
            //    List<Object> ghCurves = ToCurves(wire);
            //    foreach (Object ghCurve in ghCurves)
            //    {
            //        GeometryBase geometryBase = ghCurve as GeometryBase;
            //        if (geometryBase != null)
            //        {
            //            ghGeometryBases.Add(geometryBase);
            //        }
            //    }
            //}


            //2.Create the Brep
            //Brep ghBrep = ghNurbsSurface.ToBrep();
            //Brep ghBrep = new Brep();

            //// 2a. Add vertices
            //List<Vertex> vertices = face.Vertices;
            //foreach (Vertex vertex in vertices)
            //{
            //    Point3d ghPoint = ToPoint(vertex);
            //    BrepVertex ghBrepVertex = ghBrep.Vertices.Add(ghPoint, 0.0);

            //    String ghBrepVertexLog = "";
            //    if (!ghBrepVertex.IsValidWithLog(out ghBrepVertexLog))
            //    {
            //        throw new Exception("Fails to create a valid BrepVertex with the following message: " + ghBrepVertexLog);
            //    }
            //}

            //// 2b. Add 3D curves and edges. The index dictionaries are used to identify the IDs of the edges.
            //List<Edge> edges = face.Edges;
            //Dictionary<Edge, Tuple<int, int>> edge2DIndices = new Dictionary<Edge, Tuple<int, int>>(); // edge, curve, reverse curve
            //Dictionary<Edge, int> edge3DIndices = new Dictionary<Edge, int>();
            //Dictionary<Edge, BrepEdge> edgeIndices = new Dictionary<Edge, BrepEdge>();
            //foreach (Edge edge in edges)
            //{
            //    Curve ghCurve3D = ToCurve(edge);
            //    Curve ghCurve2D = ghSurface.Pullback(ghCurve3D, 0.0001);

            //    // 2D curves --> need to check if the endpoints are near to previously generated points
            //    // if yes, change the value

            //    int curve3DID = ghBrep.Curves3D.Add(ghCurve3D);
            //    int curve2DID = ghBrep.Curves2D.Add(ghCurve2D);

            //    Curve ghReverseCurve2D = ghCurve2D.DuplicateCurve();
            //    ghReverseCurve2D.Reverse();
            //    int reverseCurve2DID = ghBrep.Curves2D.Add(ghReverseCurve2D);

            //    Point3d ghStartPoint = ghCurve3D.PointAtStart;
            //    Point3d ghEndPoint = ghCurve3D.PointAtEnd;

            //    int startVertexIndex = -1;
            //    int endVertexIndex = -1;
            //    foreach (BrepVertex ghBrepVertex in ghBrep.Vertices)
            //    {
            //        Point3d ghBrepPoint = ghBrepVertex.Location;

            //        if (startVertexIndex == -1 && ghBrepPoint.DistanceTo(ghStartPoint) < 0.0001)
            //        {
            //            startVertexIndex = ghBrepVertex.VertexIndex;
            //        }

            //        if (endVertexIndex == -1 && ghBrepPoint.DistanceTo(ghEndPoint) < 0.0001)
            //        {
            //            endVertexIndex = ghBrepVertex.VertexIndex;
            //        }
            //    }

            //    BrepEdge ghBrepEdge = ghBrep.Edges.Add(startVertexIndex, endVertexIndex, curve3DID, 0.0001);

            //    String brepEdgeLog = "";
            //    if (!ghBrepEdge.IsValidWithLog(out brepEdgeLog))
            //    {
            //        throw new Exception("Fails to create a valid BrepEdge with the following message: " + brepEdgeLog);
            //    }

            //    edge3DIndices.Add(edge, curve3DID);
            //    edge2DIndices.Add(edge, Tuple.Create(curve2DID, reverseCurve2DID));
            //    edgeIndices.Add(edge, ghBrepEdge);
            //}

            //// 2c. Add surface
            //int ghSurfaceIndex = ghBrep.AddSurface(ghSurface);

            //// 2d. Add face
            //BrepFace ghBrepFace = ghBrep.Faces.Add(ghSurfaceIndex);

            //// 2e. Create outer loop
            //Wire outerWire = face.ExternalBoundary;
            //ProcessFace(outerWire, ref ghBrep, ghBrepFace, BrepLoopType.Outer, ghSurface, edge2DIndices, edgeIndices);

            //// 2g. Create inner loops
            //List<Wire> innerWires = face.InternalBoundaries;
            //foreach (Wire innerWire in innerWires)
            //{
            //    ProcessFace(innerWire, ref ghBrep, ghBrepFace, BrepLoopType.Inner, ghSurface, edge2DIndices, edgeIndices);
            //}

            //String brepFaceLog = "";
            //if (!ghBrepFace.IsValidWithLog(out brepFaceLog))
            //{
            //    throw new Exception("Fails to create a valid Face with the following message: " + brepFaceLog);
            //}

            //ghBrep.Compact();

            List<GeometryBase> ghGeometryBases = new List<GeometryBase>();

            List<Edge> outerEdges = face.ExternalBoundary.Edges;
            List<Curve> ghCurves = new List<Curve>();
            foreach (Edge edge in outerEdges)
            {
                Curve ghCurve3D = ToCurve(edge);
                ghGeometryBases.Add(ghCurve3D);
                ghCurves.Add(ghCurve3D);
            }
            //ghGeometryBases.AddRange(ghBrep.Curves2D);
            //ghGeometryBases.AddRange(ghBrep.Trims);
            //ghGeometryBases.AddRange(ghBrep.Vertices);

            Brep ghBrep2 = Brep.CreatePatch(
                ghGeometryBases,
                ghSurface,
                20,
                20,
                true,
                true,
                tolerance,
                100.0,
                1,
                new Boolean[] { true, true, true, true },
                tolerance);
            //return ghBrep2; // always returns the trimmed surface

            BrepFace ghSurfaceAsBrepFace = ghSurface as BrepFace;

            if (ghBrep2 == null)
            {
                return null;
            }

            List<Wire> internalBoundaries = face.InternalBoundaries;
            if(internalBoundaries.Count == 0)
            {
                return ghBrep2;
            }

            BrepFace ghBrepFace = ghBrep2.Faces[0];
            //return ghBrepFace.ToBrep(); // may return the untrimmed surface!

            List<Curve> ghInternalCurves = new List<Curve>();
            foreach(Wire internalBoundary in internalBoundaries)
            {
                List<Object> ghCurvesFromWireAsObjects = ToCurves(internalBoundary);
                foreach(Object ghCurveFromWireAsObject in ghCurvesFromWireAsObjects)
                {
                    Curve ghCurveFromWire = ghCurveFromWireAsObject as Curve;
                    if(ghCurveFromWire != null)
                    {
                        Curve[] ghPulledCurveFromWire = ghCurveFromWire.PullToBrepFace(ghBrepFace, tolerance);
                        ghInternalCurves.AddRange(ghPulledCurveFromWire);
                    }
                }
            }
            //ghCurves.AddRange(ghInternalCurves);
            Brep ghBrep3 = ghBrepFace.Split(ghInternalCurves, tolerance);
            //return ghBrep3.Faces[0].DuplicateFace(true);
            return ghBrep3.Faces.ExtractFace(0);
            //Brep[] ghBreps3 = ghBrep2.Split(ghInternalCurves, 0.0001);
            //return ghBreps3.First();


            //String brepLog = "";
            //if (!ghBrep.IsValidWithLog(out brepLog))
            //{
            //    throw new Exception("Fails to create a valid Brep with the following message: " + brepLog);
            //}

            //if (!ghBrep.IsValidGeometry(out brepLog))
            //{
            //    throw new Exception("Fails to create a valid Brep with the following message: " + brepLog);
            //}

            //if (!ghBrep.IsValidTopology(out brepLog))
            //{
            //    throw new Exception("Fails to create a valid Brep with the following message: " + brepLog);
            //}

            //if (!ghBrep.IsValidTolerancesAndFlags(out brepLog))
            //{
            //    throw new Exception("Fails to create a valid Brep with the following message: " + brepLog);
            //}


            //return ghBrep;
        }

        private List<Object> ToCurves(Wire wire)
        {
            List<Edge> edges = wire.Edges;
            List<Curve> ghOriginalCurves = new List<Curve>();
            foreach (Edge edge in edges)
            {
                Curve ghCurve = ToCurve(edge);
                ghOriginalCurves.Add(ghCurve);
            }

            if(ghOriginalCurves.Count == 0)
            {
                return null;
            }

            List<Curve> ghFinalCurves = Curve.JoinCurves(ghOriginalCurves).ToList();

            List<Object> ghFinalCurvesAsObjects = new List<object>();
            foreach (Curve ghFinalCurve in ghFinalCurves)
            {
                String log = "";
                if (!ghFinalCurve.IsValidWithLog(out log))
                {
                    throw new Exception(log);
                }
                ghFinalCurvesAsObjects.Add(ghFinalCurve);
            }

            return ghFinalCurvesAsObjects;
        }

        private Curve ToCurve(Edge edge)
        {
            Object edgeGeometry = edge.BasicGeometry;
            Curve ghCurve = null;
            Topologic.Line line = edgeGeometry as Topologic.Line;
            if(line != null)
            {
                ghCurve = ToLine(edge);
            }

            Topologic.NurbsCurve nurbsCurve = edgeGeometry as Topologic.NurbsCurve;
            if (nurbsCurve != null)
            {
                ghCurve = ToNurbsCurve(nurbsCurve);
            }

            if (ghCurve == null)
            {
                //throw new Exception("This Edge creates an unrecognized Geometry.");
                return null;
            }

            bool isReversed = edge.IsReversed;

            // IF ENABLED, WILL MAKE THE FACE VALID, BUT CELL INVALID
            //if (isReversed)
            //{
            //    bool successful = ghCurve.Reverse();
            //}

            return ghCurve;
        }

        private Rhino.Geometry.Curve ToNurbsCurve(Topologic.NurbsCurve nurbsCurve)
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

            double t0 = nurbsCurve.FirstParameter;
            double t1 = nurbsCurve.LastParameter;

            Rhino.Geometry.Curve ghTrimmedNurbsCurve = ghNurbsCurve.Trim(t0, t1);

            String log = "";
            if(ghTrimmedNurbsCurve.IsValidWithLog(out log))
            {
                return ghTrimmedNurbsCurve;
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
