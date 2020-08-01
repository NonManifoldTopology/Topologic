# Topologic
Topologic is a software modelling library enabling hierarchical and topological representations of architectural spaces, buildings and artefacts through non-manifold topology.

The project website is located here: https://topologic.app/


## Introduction
Topologic is a software modelling library enabling hierarchical and topological representations of architectural spaces, buildings and artefacts through non-manifold topology (NMT). Topologic is designed as a core library and additional plugins to visual data flow programming (VDFP) applications and parametric modelling platforms commonly used in architectural design practice. These applications provide workspaces with visual programming nodes and connections for architects to interact with Topologic and perform architectural design and analysis tasks.

Topologic is well-suited to create a lightweight representation of a building as an external envelope and the subdivision of the enclosed space into separate spaces and zones using zero-thickness internal surfaces. Because Topologic maintains topological consistency, a user can query these cellular spaces and surfaces regarding their topological data and thus conduct various analyses. For example, this lightweight and consistent representation was found to be well-matched with the input data requirements for energy analysis simulation software. Because Topologic allows entities with mixed dimensionalities and those that are optionally independent (e.g. a line, a surface, a volume) to co-exist, structural models can be represented in a coherent manner where lines can represent columns and beams, surfaces can represent walls and slabs, and volumes can represent solids. In addition, non-building entities, such as structural loads can be efficiently attached to the structure. This creates a lightweight model that is well-matched with the input data requirements for structural analysis simulation software.


## Rigorous Class Hierarchy
TopologicCore contains the following main classes:

*Topology: A Topology is an abstract superclass that stores constructors, properties and methods used by other subclasses that extend it.
*Vertex: A Vertex is a zero-dimensional entity equivalent to a geometry point.
*Edge: An Edge is a one-dimensional entity defined by two vertices. It is important to note that while a topologic edge is made of two vertices, its geometry can be a curve with multiple control vertices.
*Wire: A Wire is a contiguous collection of Edges where adjacent Edges are connected by shared Vertices. It may be open or closed and may be manifold or non-manifold.
*Face: A Face is a two-dimensional region defined by a collection of closed Wires. The geometry of a face can be flat or undulating.
*Shell: A Shell is a contiguous collection of Faces, where adjacent Faces are connected by shared Edges. It may be open or closed and may be manifold or non-manifold.
*Cell: A Cell is a three-dimensional region defined by a collection of closed Shells. It may be manifold or non- manifold.
*CellComplex: A CellComplex is a contiguous collection of Cells where adjacent Cells are connected by shared Faces. It is non- manifold.
*Cluster: A Cluster is a collection of any topologic entities. It may be contiguous or not and may be manifold or non- manifold. Clusters can be nested within other Clusters.

## Works with Dynamo
Topologic works within Dynamo 2.x (Sandbox/Revit) or newer in Microsoft Windows OS. You can integrate workflows using nodes from Dynamo, Topologic, and other third-party packages. Topologic is installed automatically as a thrid-party package in Dynamo. After installation, launch Dynamo and you can see Topologic in the menu on the left.

While we recommend using Topologic for building, editing and querying consistent and robust topological entities, you are not locked into using Topologic exclusively. You can use regular Dynamo geometric entities and convert them to Topologic entities by using the Topology.ByGeometry constructor node.

Topologic entities are not rendered to the view. If you wish to view any Topologic entity, you can convert it to a Dynamo geometry by using the Topology.Geometry node.

## Works with Grasshopper
Topologic works within Rhino/Grasshopper 6.18 or newer in Microsoft Windows OS. You can integrate workflows using nodes from Grasshopper, Topologic, and other third-party packages. To use Topologic, you simply choose Topologic from the top menu or search for Topologic methods in the canvas itself.

While we recommend using Topologic for building, editing and querying consistent and robust topological entities, you are not locked into using Topologic exclusively. You can use regular Grasshopper geometric entities and convert them to Topologic entities by using the Topology.ByGeometry constructor node.

Topologic entities are not rendered to the view. If you wish to view any Topologic entity, you can convert it to a Grasshopper geometry by using the Topology.Geometry node.
