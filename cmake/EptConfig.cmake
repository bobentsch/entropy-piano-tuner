# on mobile platforms and mac/ios  use static algorithm build
# android|ios|winrt|winphone|macx:EPT_CONFIG += static static_core
# android|ios|macx:EPT_CONFIG += static_deps
# contains(EPT_CONFIG, static_core) {
#     EPT_CONFIG += no_shared_algorithms static_algorithms static_core no_shared_core
# }
# else {
#     EPT_CONFIG += shared_algorithms no_static_algorithms shared_core no_static_core
# }

if(CMAKE_SYSTEM_NAME IN_LIST "Android;iOS;WindowsPhone;Darwin")
    set(EPT_STATIC_CORE ON)
endif()

if(CMAKE_SYSTEM_NAME IN_LIST "Android;iOS;Darwin")
    set(EPT_STATIC_DEPS ON)
endif()


if(EPT_STATIC_CORE)
    set(EPT_STATIC_ALGORITHMS ON)
    set(EPT_STATIC_CORE ON)
else()
    set(EPT_STATIC_ALGORITHMS OFF)
    set(EPT_STATIC_CORE OFF)
endif()


message(STATUS "EPT Static Core: ${EPT_STATIC_CORE}")
message(STATUS "EPT Static Algorithms: ${EPT_STATIC_ALGORITHMS}")


if(EPT_STATIC_ALGORITHMS)
add_compile_definitions(EPT_NO_SHARED_ALGORITHMS)
add_compile_definitions(EPT_STATIC_ALGORITHMS)
else()
add_compile_definitions(EPT_SHARED_ALGORITHMS)
endif()

if(EPT_STATIC_CORE)
add_compile_definitions(EPT_STATIC_CORE)
else()
add_compile_definitions(EPT_DYNAMIC_CORE)
endif()