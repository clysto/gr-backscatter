/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_POWER_DETECTOR_IMPL_H
#define INCLUDED_BACKSCATTER_POWER_DETECTOR_IMPL_H

#include <gnuradio/backscatter/power_detector.h>
#include <gnuradio/blocks/delay.h>
#include <gnuradio/blocks/divide.h>
#include <gnuradio/blocks/moving_average.h>
#include <gnuradio/blocks/abs_blk.h>
#include <gnuradio/blocks/nlog10_ff.h>

namespace gr {
namespace backscatter {

class power_detector_impl : public power_detector
{
private:
    blocks::moving_average_ff::sptr d_avg;
    blocks::delay::sptr d_delay;
    blocks::divide_ff::sptr d_divide;
    blocks::abs_ff::sptr d_abs;
    blocks::nlog10_ff::sptr d_20log10;
    int d_window_size;

public:
    power_detector_impl(int window_size);
    ~power_detector_impl();
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_POWER_DETECTOR_IMPL_H */
