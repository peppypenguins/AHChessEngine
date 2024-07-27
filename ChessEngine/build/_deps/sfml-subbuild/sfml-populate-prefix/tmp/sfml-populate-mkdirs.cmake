# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-src")
  file(MAKE_DIRECTORY "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-build"
  "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-subbuild/sfml-populate-prefix"
  "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Adam/projects/ChessEngine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()