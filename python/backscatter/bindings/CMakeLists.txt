# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT backscatter_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT backscatter_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND backscatter_python_files
    power_detector_python.cc
    burst_frame_tagger_python.cc
    bs_frame_decoder_python.cc
    gate_python.cc
    pdu_ber_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(backscatter
   ../../..
   gr::backscatter
   "${backscatter_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET backscatter_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:backscatter_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/backscatter/
)

install(TARGETS backscatter_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/backscatter COMPONENT pythonapi)
