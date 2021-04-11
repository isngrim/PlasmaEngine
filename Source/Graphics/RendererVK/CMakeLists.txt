add_library(RendererVK)

plasma_setup_library(RendererVK ${CMAKE_CURRENT_LIST_DIR} TRUE)
plasma_use_precompiled_header(RendererVK ${CMAKE_CURRENT_LIST_DIR})

target_sources(RendererVK
  PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
      ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
      ${CMAKE_CURRENT_LIST_DIR}/RendererVKStandard.hpp
      ${CMAKE_CURRENT_LIST_DIR}/RendererVKStandard.cpp
      ${CMAKE_CURRENT_LIST_DIR}/VulkanRenderer.hpp
      ${CMAKE_CURRENT_LIST_DIR}/VulkanRenderer.cpp
      ${CMAKE_CURRENT_LIST_DIR}/VulkanStructures.hpp 
      ${CMAKE_CURRENT_LIST_DIR}/VulkanInitialization.hpp
      ${CMAKE_CURRENT_LIST_DIR}/VulkanInstance.hpp 
      ${CMAKE_CURRENT_LIST_DIR}/VulkanValidation.hpp
      ${CMAKE_CURRENT_LIST_DIR}/VulkanExtensions.hpp
      ${CMAKE_CURRENT_LIST_DIR}/VulkanSurface.hpp
)

plasma_target_includes(RendererVK
  PUBLIC
    Common
    Support
    tracy
)

target_link_libraries(RendererVK
  PRIVATE
    volk_headers
)