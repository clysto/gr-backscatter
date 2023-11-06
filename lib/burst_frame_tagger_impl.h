/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_BURST_FRAME_TAGGER_IMPL_H
#define INCLUDED_BACKSCATTER_BURST_FRAME_TAGGER_IMPL_H

#include <gnuradio/backscatter/burst_frame_tagger.h>

namespace gr {
namespace backscatter {

class burst_frame_tagger_impl : public burst_frame_tagger
{
private:
    float d_snr_threshoud;
    int d_look_ahead;
    int d_state;
    float d_first_snr;
    float d_last_snr;
    int d_below_threshoud_count;
    pmt::pmt_t d_id;

public:
    burst_frame_tagger_impl(float snr_threshoud, int look_ahead);
    ~burst_frame_tagger_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_BURST_FRAME_TAGGER_IMPL_H */
