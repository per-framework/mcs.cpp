add_conventional_library(mcs_v1)
target_link_libraries(mcs_v1 PUBLIC dumpster_v1 polyfill_v1 PRIVATE intrinsics_v1)
