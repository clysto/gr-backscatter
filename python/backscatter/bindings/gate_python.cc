/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(gate.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(df94ec136416f97d0d96ac29cb1c1d21)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/backscatter/gate.h>
// pydoc.h is automatically generated in the build directory
#include <gate_pydoc.h>

void bind_gate(py::module& m)
{

    using gate = gr::backscatter::gate;


    py::class_<gate, gr::block, gr::basic_block, std::shared_ptr<gate>>(
        m, "gate", D(gate))

        .def(py::init(&gate::make), py::arg("max_samples"), D(gate, make))


        ;
}
