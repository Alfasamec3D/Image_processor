# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/alfasamec3d/media/programs/Image_processor/build/_deps/googletest-src"
  "/home/alfasamec3d/media/programs/Image_processor/build/_deps/googletest-build"
  "/mnt/9C468886468862BE/media/programs/Image_processor/build/_deps/googletest-subbuild/googletest-populate-prefix"
  "/mnt/9C468886468862BE/media/programs/Image_processor/build/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "/mnt/9C468886468862BE/media/programs/Image_processor/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "/mnt/9C468886468862BE/media/programs/Image_processor/build/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "/mnt/9C468886468862BE/media/programs/Image_processor/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/9C468886468862BE/media/programs/Image_processor/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/mnt/9C468886468862BE/media/programs/Image_processor/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
