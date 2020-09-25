project(TopologicCore CXX)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "include/About.h"
    "include/Aperture.h"
    "include/Cell.h"
    "include/CellComplex.h"
    "include/Cluster.h"
    "include/ContentManager.h"
    "include/Context.h"
    "include/ContextManager.h"
    "include/Edge.h"
    "include/Face.h"
    "include/GlobalCluster.h"
    "include/Graph.h"
    "include/InstanceGUIDManager.h"
    "include/Shell.h"
    "include/TopologicalQuery.h"
    "include/Topology.h"
    "include/Utilities.h"
    "include/Vertex.h"
    "include/Wire.h"
    "src/About.cpp"
    "src/Aperture.cpp"
    "src/Cell.cpp"
    "src/CellComplex.cpp"
    "src/Cluster.cpp"
    "src/ContentManager.cpp"
    "src/Context.cpp"
    "src/ContextManager.cpp"
    "src/Edge.cpp"
    "src/Face.cpp"
    "src/GlobalCluster.cpp"
    "src/Graph.cpp"
    "src/InstanceGUIDManager.cpp"
    "src/Shell.cpp"
    "src/Topology.cpp"
    "src/Utilities.cpp"
    "src/Vertex.cpp"
    "src/Wire.cpp"
)
source_group("" FILES ${no_group_source_files})

set(Attributes
    "include/Attribute.h"
    "include/AttributeManager.h"
    "include/DoubleAttribute.h"
    "include/IntAttribute.h"
    "include/ListAttribute.h"
    "include/StringAttribute.h"
    "src/AttributeManager.cpp"
    "src/DoubleAttribute.cpp"
    "src/IntAttribute.cpp"
    "src/ListAttribute.cpp"
    "src/StringAttribute.cpp"
)
source_group("Attributes" FILES ${Attributes})

set(Factories
    "include/ApertureFactory.h"
    "include/CellComplexFactory.h"
    "include/CellFactory.h"
    "include/ClusterFactory.h"
    "include/EdgeFactory.h"
    "include/FaceFactory.h"
    "include/ShellFactory.h"
    "include/TopologyFactory.h"
    "include/TopologyFactoryManager.h"
    "include/VertexFactory.h"
    "include/WireFactory.h"
    "src/ApertureFactory.cpp"
    "src/CellComplexFactory.cpp"
    "src/CellFactory.cpp"
    "src/ClusterFactory.cpp"
    "src/EdgeFactory.cpp"
    "src/FaceFactory.cpp"
    "src/ShellFactory.cpp"
    "src/TopologyFactoryManager.cpp"
    "src/VertexFactory.cpp"
    "src/WireFactory.cpp"
)
source_group("Factories" FILES ${Factories})

set(Geometries
    "include/Geometry.h"
    "include/Line.h"
    "include/NurbsCurve.h"
    "include/NurbsSurface.h"
    "include/PlanarSurface.h"
    "include/Surface.h"
    "src/Geometry.cpp"
    "src/Line.cpp"
    "src/NurbsCurve.cpp"
    "src/NurbsSurface.cpp"
    "src/PlanarSurface.cpp"
    "src/Surface.cpp"
)
source_group("Geometries" FILES ${Geometries})

set(Utilities
    "include/Bitwise.h"
    "include/Utilities/CellUtility.h"
    "include/Utilities/Direction.h"
    "include/Utilities/EdgeUtility.h"
    "include/Utilities/FaceUtility.h"
    "include/Utilities/ShellUtility.h"
    "include/Utilities/TopologyUtility.h"
    "include/Utilities/TransformationMatrix2D.h"
    "include/Utilities/Vector.h"
    "include/Utilities/VertexUtility.h"
    "include/Utilities/WireUtility.h"
    "src/Bitwise.cpp"
    "src/Utilities/CellUtility.cpp"
    "src/Utilities/Direction.cpp"
    "src/Utilities/EdgeUtility.cpp"
    "src/Utilities/FaceUtility.cpp"
    "src/Utilities/ShellUtility.cpp"
    "src/Utilities/TopologyUtility.cpp"
    "src/Utilities/TransformationMatrix2D.cpp"
    "src/Utilities/Vector.cpp"
    "src/Utilities/VertexUtility.cpp"
    "src/Utilities/WireUtility.cpp"
)
source_group("Utilities" FILES ${Utilities})

set(ALL_FILES
    ${no_group_source_files}
    ${Attributes}
    ${Factories}
    ${Geometries}
    ${Utilities}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE TopologicCore)

################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/output/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/output/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/output/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/output/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/output/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/output/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "/usr/include/opencascade"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "/usr/include/opencascade"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "/usr/include/opencascade"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "TOPOLOGIC_EXPORTS;"
        "_WINDLL;"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "TOPOLOGICCORE_EXPORTS;"
        "_WINDLL;"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "TOPOLOGIC_EXPORTS;"
        "_WINDLL;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG:FULL
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG:FULL
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG:FULL
        )
    endif()
endif()

################################################################################
# Post build events
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND $<CONFIG:Release> copy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>references\\opencascade-7.2.0\\win32\\vc14\\bin\\*.dll" "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>output\\${CMAKE_VS_PLATFORM_NAME}\\$<CONFIG>"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND   $<CONFIG:Debug> copy "C:\\OpenCASCADE-7.4.0-vc14-64\\opencascade-7.4.0\\win64\\vc14\\bin\\*.dll" "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>output\\${CMAKE_VS_PLATFORM_NAME}\\$<CONFIG>"
        COMMAND $<CONFIG:Release> copy "C:\\OpenCASCADE-7.4.0-vc14-64\\opencascade-7.4.0\\win64\\vc14\\bin\\*.dll" "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>output\\${CMAKE_VS_PLATFORM_NAME}\\$<CONFIG>"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND $<CONFIG:Release> copy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>references\\opencascade-7.2.0\\win32\\vc14\\bin\\*.dll" "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>output\\${CMAKE_VS_PLATFORM_NAME}\\$<CONFIG>"
    )
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "TKCAF;"
        "TKLCAF;"
        "TKOffset;"
        "TKIGES;"
        "TKPrim;"
        "TKMesh;"
        "TKXSBase;"
        "TKBool;"
        "TKBO;"
        "TKShHealing;"
        "TKG3d;"
        "TKG2d;"
        "TKGeomBase;"
        "TKGeomAlgo;"
        "TKTopAlgo;"
        "TKernel;"
        "TKMath;"
        "uuid;"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "TKCAF;"
        "TKLCAF;"
        "TKOffset;"
        "TKIGES;"
        "TKPrim;"
        "TKMesh;"
        "TKXSBase;"
        "TKBool;"
        "TKBO;"
        "TKShHealing;"
        "TKG3d;"
        "TKG2d;"
        "TKGeomBase;"
        "TKGeomAlgo;"
        "TKTopAlgo;"
        "TKernel;"
        "TKMath;"
        "uuid;"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "TKCAF;"
        "TKLCAF;"
        "TKOffset;"
        "TKIGES;"
        "TKPrim;"
        "TKMesh;"
        "TKXSBase;"
        "TKBool;"
        "TKBO;"
        "TKShHealing;"
        "TKG3d;"
        "TKG2d;"
        "TKGeomBase;"
        "TKGeomAlgo;"
        "TKTopAlgo;"
        "TKernel;"
        "TKMath;"
        "uuid;"
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "/usr/lib64"
        ">"
        "$<$<CONFIG:Release>:"
            "/usr/lib64"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "/usr/lib64"
        ">"
        "$<$<CONFIG:Release>:"
            "/usr/lib64"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "/usr/lib64"
        ">"
        "$<$<CONFIG:Release>:"
            "/usr/lib64"
        ">"
    )
endif()
