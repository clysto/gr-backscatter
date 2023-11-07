/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "pdu_rate_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace backscatter {

using output_type = float;

pdu_rate::sptr pdu_rate::make(float update_rate_ms = 1000)
{
    return gnuradio::make_block_sptr<pdu_rate_impl>(update_rate_ms);
}


/*
 * The private constructor
 */
pdu_rate_impl::pdu_rate_impl(float update_rate_ms)
    : gr::sync_block("pdu_rate",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(output_type))),
      d_min_update_time(update_rate_ms),
      d_pdu_count(0),
      d_rate(0)
{
    message_port_register_in(pmt::mp("pdus"));
    set_msg_handler(pmt::mp("pdus"), [this](pmt::pmt_t msg) { this->handle_msg(msg); });
}

/*
 * Our virtual destructor.
 */
pdu_rate_impl::~pdu_rate_impl() {}

int pdu_rate_impl::work(int noutput_items,
                        gr_vector_const_void_star& input_items,
                        gr_vector_void_star& output_items)
{
    auto out = static_cast<output_type*>(output_items[0]);
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> diff = now - d_last_update;
    double diff_ms = diff.count();

    if (diff_ms >= d_min_update_time) {
        d_rate = d_pdu_count / diff_ms * 1000;
        d_last_update = now;
        d_pdu_count = 0;
    }

    for (int i = 0; i < noutput_items; i++) {
        out[i] = d_rate;
    }

    return noutput_items;
}

void pdu_rate_impl::handle_msg(pmt::pmt_t msg) { d_pdu_count++; }

} /* namespace backscatter */
} /* namespace gr */
