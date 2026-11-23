include(FetchContent)
FetchContent_Declare(
  googletest
  # v1.18.0
  URL https://github.com/google/googletest/archive/063de7e9578f82b369302001269680b4b1553359.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(rawbytes_test
  ptr_test.cpp
  slice_test.cpp
)

target_link_libraries(rawbytes_test PUBLIC rawbytes GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(rawbytes_test)
