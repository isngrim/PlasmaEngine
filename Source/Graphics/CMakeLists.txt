# Libraries.

add_subdirectory(GraphicsRuntime)
add_subdirectory(RendererGL)
if(PlasmaVulkanInstaleld)
	add_subdirectory(RendererVK)
endif()

set_property(TARGET "GraphicsRuntime" PROPERTY FOLDER "Graphics")
set_property(TARGET "RendererGL" PROPERTY FOLDER "Graphics")
if(PlasmaVulkanInstaleld)
	set_property(TARGET "RendererVK" PROPERTY FOLDER "Graphics")
endif()