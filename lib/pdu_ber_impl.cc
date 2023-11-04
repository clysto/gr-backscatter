/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "pdu_ber_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>


namespace gr::backscatter {

using output_type = float;

pdu_ber::sptr pdu_ber::make(std::vector<uint8_t>& payload)
{
    return gnuradio::make_block_sptr<pdu_ber_impl>(payload);
}

pdu_ber_impl::pdu_ber_impl(std::vector<uint8_t>& payload)
    : gr::sync_block("pdu_ber",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(output_type))),
      d_ber_queue(std::queue<float>()),
      d_payload(payload)
{
    message_port_register_in(pmt::mp("pdus"));
    set_msg_handler(pmt::mp("pdus"), [this](pmt::pmt_t msg) { this->handle_msg(msg); });
}

pdu_ber_impl::~pdu_ber_impl() = default;

int pdu_ber_impl::work(int noutput_items,
                       gr_vector_const_void_star& input_items,
                       gr_vector_void_star& output_items)
{
    auto out = static_cast<float*>(output_items[0]);
    int n = 0;
    for (int i = 0; i < noutput_items; i++) {
        if (d_ber_queue.empty()) {
            break;
        }
        out[n++] = d_ber_queue.front();
        d_ber_queue.pop();
    }
    return n;
}

void pdu_ber_impl::handle_msg(pmt::pmt_t msg)
{
    pmt::pmt_t data_vector = pmt::cdr(msg);
    size_t len = pmt::blob_length(data_vector);
    const uint8_t* data = pmt::u8vector_elements(data_vector, len);
    int n = std::min((int)len, (int)d_payload.size());
    uint32_t error_count = 0;
    std::vector<uint8_t> xor_data(n);
    std::transform(
        data, data + n, d_payload.begin(), xor_data.begin(), std::bit_xor<uint8_t>());
    for (int i = 0; i < n; i++) {
        uint32_t xor_data_32 = xor_data[i];
        uint32_t ret;
        volk_32u_popcnt(&ret, xor_data_32);
        error_count += ret;
    }
    d_ber_queue.push(error_count / (float)(n * 8));
}


} // namespace gr::backscatter
