add_library(util utilityCore.cpp tiny_obj_loader.cc)

target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
