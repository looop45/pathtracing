# Include directories for headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add other source files (excluding main.cpp)
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/area_light.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/bounding_volume.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/bvh_tree.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/camera.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/color.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/distant_light.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hit_tree.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hittable_list.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hittable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/light.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/material.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix4.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mesh.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/onb.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pdf.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/point_light.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/polygon.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/quad.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rtw_stb_image.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sampler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sphere.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/texture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utilities.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vec2.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vec3.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vec4.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image.h

        
)