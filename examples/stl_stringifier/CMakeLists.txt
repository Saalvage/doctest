set(files check_basic.cpp)
set(latestVer 03)

macro(doctest_test_std ver)
    if(cxx_std_${ver} IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list(APPEND files check${ver}.cpp)
        set(latestVer ${ver})
    endif()
endmacro()

doctest_test_std(11)
#doctest_test_std(14)
#doctest_test_std(17)
#doctest_test_std(20)

message("Latest avaiable C++ version is C++" ${latestVer})

add_executable(stl_stringify header.h ${files})
target_compile_features(stl_stringify INTERFACE cxx_std_${latestVer})
target_link_libraries(stl_stringify doctest)

foreach(f ${files})
    doctest_add_test(NAME ${f} COMMAND $<TARGET_FILE:stl_stringify> --no-skipped-summary --no-version -sf=*${f})
endforeach()

if(MSVC)
    add_compiler_flags(/wd4866) # something about evaluation order in chrono
endif()
