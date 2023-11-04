/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_PDU_BER_IMPL_H
#define INCLUDED_BACKSCATTER_PDU_BER_IMPL_H

#include <gnuradio/backscatter/pdu_ber.h>
#include <cstdint>
#include <queue>

namespace gr {
namespace backscatter {

class pdu_ber_impl : public pdu_ber
{
private:
    std::queue<float> d_ber_queue;
    std::vector<uint8_t> d_payload;

public:
    pdu_ber_impl(std::vector<uint8_t>& payload);
    ~pdu_ber_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);

    void handle_msg(pmt::pmt_t msg);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_PDU_BER_IMPL_H */
