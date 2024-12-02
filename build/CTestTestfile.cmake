# CMake generated Testfile for 
# Source directory: /root/cpp-workspace/option_pricing_library
# Build directory: /root/cpp-workspace/option_pricing_library/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTests "/root/cpp-workspace/option_pricing_library/build/bin/PricingTests")
set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/root/cpp-workspace/option_pricing_library/CMakeLists.txt;110;add_test;/root/cpp-workspace/option_pricing_library/CMakeLists.txt;0;")
subdirs("_deps/spdlog-build")
