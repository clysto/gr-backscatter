/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "power_detector_impl.h"
#include <gnuradio/basic_block.h>
#include <gnuradio/blocks/abs_blk.h>
#include <gnuradio/blocks/delay.h>
#include <gnuradio/blocks/divide.h>
#include <gnuradio/blocks/moving_average.h>
#include <gnuradio/blocks/nlog10_ff.h>
#include <gnuradio/io_signature.h>

namespace gr {
namespace backscatter {

using input_type = float;
using output_type = float;

power_detector::sptr power_detector::make(int window_size)
{
    return gnuradio::make_block_sptr<power_detector_impl>(window_size);
}

power_detector_impl::power_detector_impl(int window_size)
    : gr::hier_block2("power_detector",
                      gr::io_signature::make(1, 1, sizeof(input_type)),
                      gr::io_signature::make(2, 2, sizeof(output_type))),
      d_avg(blocks::moving_average_ff::make(window_size, 1.0 / window_size)),
      d_delay(blocks::delay::make(4, window_size)),
      d_divide(blocks::divide_ff::make(1)),
      d_abs(blocks::abs_ff::make(1)),
      d_20log10(blocks::nlog10_ff::make(20, 1, 0)),
      d_window_size(window_size)
{
    connect(self(), 0, d_abs, 0);
    connect(d_abs, 0, d_avg, 0);
    connect(d_avg, 0, d_delay, 0);
    connect(d_avg, 0, d_divide, 0);
    connect(d_delay, 0, d_divide, 1);
    connect(d_divide, 0, d_20log10, 0);
    connect(d_20log10, 0, self(), 0);
    connect(d_avg, 0, self(), 1);
}

/*
 * Our virtual destructor.
 */
power_detector_impl::~power_detector_impl() {}


} /* namespace backscatter */
} /* namespace gr */
