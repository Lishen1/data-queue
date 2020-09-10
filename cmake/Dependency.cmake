include ( cmake/get_cpm.cmake )

function (add_data_queue_tests_dependency target)
	CPMAddPackage(
	  NAME googletest
	  GITHUB_REPOSITORY google/googletest
	  GIT_TAG 703bd9c
	  OPTIONS
	    "BUILD_GMOCK OFF"
		"gtest_force_shared_crt on"
	)
	
	CPMAddPackage(
	  NAME benchmark
	  GITHUB_REPOSITORY google/benchmark
	  VERSION 1.4.1
	  OPTIONS
		"BENCHMARK_ENABLE_TESTING OFF"
		"BENCHMARK_ENABLE_EXCEPTIONS OFF"
		"BENCHMARK_ENABLE_GTEST_TESTS OFF"
		"BENCHMARK_ENABLE_INSTALL OFF"
	)
	
	if (benchmark_ADDED)
	  # compile with C++17
	  set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)
	endif()
	
	target_link_libraries( ${target} PRIVATE gtest gtest_main benchmark)
endfunction()


