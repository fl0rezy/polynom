set(target ${MP2_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_executable(${target} ${srcs} ${hdrs})

target_link_libraries(${target} gtest)

target_include_directories(${target} PUBLIC 
    ${CMAKE_SOURCE_DIR}/gtest 
    ${CMAKE_SOURCE_DIR}/include
)

add_test(${target} ${target})