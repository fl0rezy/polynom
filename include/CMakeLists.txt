set(target ${MP2_CUSTOM_PROJECT})

project(${target})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_executable(${target} ${srcs} ${hdrs})

target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)