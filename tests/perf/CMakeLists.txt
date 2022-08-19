add_definitions("-fno-omit-frame-pointer") 
add_executable(perf perf.cpp)
target_link_libraries(perf PUBLIC FCCAnalyses)
# target_include_directories(bench PUBLIC ${VDT_INCLUDE_DIR})
