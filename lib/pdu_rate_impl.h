/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_PDU_RATE_IMPL_H
#define INCLUDED_BACKSCATTER_PDU_RATE_IMPL_H

#include <gnuradio/backscatter/pdu_rate.h>

namespace gr {
namespace backscatter {

class pdu_rate_impl : public pdu_rate
{
private:
    const float d_min_update_time;
    uint64_t d_pdu_count;
    std::chrono::time_point<std::chrono::steady_clock> d_last_update;
    float d_rate;


public:
    pdu_rate_impl(float update_rate_ms);
    ~pdu_rate_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);

    void handle_msg(pmt::pmt_t msg);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_PDU_RATE_IMPL_H */
