# CMake generated Testfile for 
# Source directory: /ssd_scratch/hermes3
# Build directory: /ssd_scratch/hermes3/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(1D-fluid "runtest")
set_tests_properties(1D-fluid PROPERTIES  WORKING_DIRECTORY "tests/integrated/1D-fluid" _BACKTRACE_TRIPLES "/ssd_scratch/hermes3/CMakeLists.txt;180;add_test;/ssd_scratch/hermes3/CMakeLists.txt;185;hermes_add_integrated_test;/ssd_scratch/hermes3/CMakeLists.txt;0;")
add_test(diffusion "runtest")
set_tests_properties(diffusion PROPERTIES  WORKING_DIRECTORY "tests/integrated/diffusion" _BACKTRACE_TRIPLES "/ssd_scratch/hermes3/CMakeLists.txt;180;add_test;/ssd_scratch/hermes3/CMakeLists.txt;186;hermes_add_integrated_test;/ssd_scratch/hermes3/CMakeLists.txt;0;")
add_test(evolve_density "runtest")
set_tests_properties(evolve_density PROPERTIES  WORKING_DIRECTORY "tests/integrated/evolve_density" _BACKTRACE_TRIPLES "/ssd_scratch/hermes3/CMakeLists.txt;180;add_test;/ssd_scratch/hermes3/CMakeLists.txt;187;hermes_add_integrated_test;/ssd_scratch/hermes3/CMakeLists.txt;0;")
add_test(neutral_mixed "runtest")
set_tests_properties(neutral_mixed PROPERTIES  WORKING_DIRECTORY "tests/integrated/neutral_mixed" _BACKTRACE_TRIPLES "/ssd_scratch/hermes3/CMakeLists.txt;180;add_test;/ssd_scratch/hermes3/CMakeLists.txt;188;hermes_add_integrated_test;/ssd_scratch/hermes3/CMakeLists.txt;0;")
add_test(vorticity "runtest")
set_tests_properties(vorticity PROPERTIES  WORKING_DIRECTORY "tests/integrated/vorticity" _BACKTRACE_TRIPLES "/ssd_scratch/hermes3/CMakeLists.txt;180;add_test;/ssd_scratch/hermes3/CMakeLists.txt;189;hermes_add_integrated_test;/ssd_scratch/hermes3/CMakeLists.txt;0;")
add_test(hermes_unit_tests "/ssd_scratch/hermes3/build/hermes_unit_tests")
set_tests_properties(hermes_unit_tests PROPERTIES  _BACKTRACE_TRIPLES "/ssd_scratch/hermes3/CMakeLists.txt;206;add_test;/ssd_scratch/hermes3/CMakeLists.txt;0;")
subdirs("external/googletest")
