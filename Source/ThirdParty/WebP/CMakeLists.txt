#
# Copyright (c) 2008-2022 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
include(CheckIncludeFile)
check_include_file("cpu-features.h" HAVE_CPU_FEATURES_H)

file (GLOB_RECURSE SOURCE_FILES src/*.c src/*.h)
add_library (WebP STATIC ${SOURCE_FILES})
target_include_directories(WebP SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
if (HAVE_CPU_FEATURES_H)
    target_compile_definitions(WebP PRIVATE -DHAVE_CPU_FEATURES_H=1)
endif ()
if (NOT URHO3D_MERGE_STATIC_LIBS)
    install(TARGETS WebP EXPORT Urho3D ARCHIVE DESTINATION ${DEST_ARCHIVE_DIR_CONFIG})
endif ()
