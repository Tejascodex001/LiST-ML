add_executable(collector
    src/main.cpp
    src/proc_reader.cpp
    src/sampler.cpp
)

target_include_directories(collector
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)