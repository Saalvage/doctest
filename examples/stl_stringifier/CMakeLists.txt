add_executable(stl_stringify check_all.cpp)
target_link_libraries(stl_stringify doctest)

doctest_add_test(NAME check_all.cpp COMMAND $<TARGET_FILE:stl_stringify> --no-skipped-summary --no-version)

if(MSVC)
    add_compiler_flags(/wd4866) # something about evaluation order in chrono
endif()
