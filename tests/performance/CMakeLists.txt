add_definitions("-fno-omit-frame-pointer") 
add_executable(analysis_stage1 analysis_stage1.cpp)
target_link_libraries(analysis_stage1 PUBLIC FCCAnalyses)
# target_include_directories(bench PUBLIC ${VDT_INCLUDE_DIR})
