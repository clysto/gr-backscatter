/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <gnuradio/hier_block2.h>
#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/backscatter/power_detector.h>
// pydoc.h is automatically generated in the build directory
#include <power_detector_pydoc.h>

void bind_power_detector(py::module& m)
{

    using power_detector = gr::backscatter::power_detector;


    py::class_<power_detector,
               gr::hier_block2,
               gr::basic_block,
               std::shared_ptr<power_detector>>(m, "power_detector", D(power_detector))

        .def(py::init(&power_detector::make),
             py::arg("window_size"),
             D(power_detector, make))


        ;
}
