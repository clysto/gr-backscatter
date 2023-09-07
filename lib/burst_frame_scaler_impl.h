/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_BURST_FRAME_SCALER_IMPL_H
#define INCLUDED_BACKSCATTER_BURST_FRAME_SCALER_IMPL_H

#include <gnuradio/backscatter/burst_frame_scaler.h>

namespace gr {
namespace backscatter {

class burst_frame_scaler_impl : public burst_frame_scaler
{
private:
    float d_scale;

public:
    burst_frame_scaler_impl();
    ~burst_frame_scaler_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_BURST_FRAME_SCALER_IMPL_H */
