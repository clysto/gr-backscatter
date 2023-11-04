/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_GATE_IMPL_H
#define INCLUDED_BACKSCATTER_GATE_IMPL_H

#include <gnuradio/backscatter/gate.h>

namespace gr {
namespace backscatter {

class gate_impl : public gate
{
private:
    float d_scale;
    float d_max_samples;
    int d_expected;
    bool d_have_sync;

public:
    gate_impl(unsigned int max_samples);
    ~gate_impl();

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);

    void handle_msg(pmt::pmt_t msg);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_GATE_IMPL_H */
