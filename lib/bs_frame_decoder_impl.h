/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_BS_FRAME_DECODER_IMPL_H
#define INCLUDED_BACKSCATTER_BS_FRAME_DECODER_IMPL_H

#include <gnuradio/backscatter/bs_frame_decoder.h>
#include <cstddef>
#include <cstdint>

namespace gr {
namespace backscatter {

class bs_frame_decoder_impl : public bs_frame_decoder
{
private:
    enum state_t {
        STATE_FRAME_SEARCH,
        STATE_SYNC_SEARCH,
        STATE_DECODE_HEADER,
        STATE_DECODE_PAYLOAD
    };

    state_t d_state;

    unsigned long long d_access_code; // access code to locate start of packet
    unsigned long long d_data_reg;    // used to look for access_code
    unsigned int d_data_reg_bits = 0; // used to makes sure we've seen the whole code
    unsigned long long d_mask; // masks access_code bits (top N bits are set where N is
                               // the number of bits in the access code)
    unsigned int d_threshold;  // how many bits may be wrong in sync vector
    unsigned int d_len;        // the length of the access code
    unsigned int d_pdu_max_len;

    int d_hdr_count;
    unsigned int d_hdr;
    unsigned int d_fm0_pre_symbol;

    uint8_t d_payload_byte;
    unsigned int d_payload_bit;
    std::vector<uint8_t> d_payload;
    // unsigned int d_syncword_max_search_samples;

    void enter_sync_search();
    void enter_header_decode();
    void enter_payload_decode();
    void publish_pdu();

public:
    bs_frame_decoder_impl(unsigned int threshold, unsigned int pdu_max_len);
    ~bs_frame_decoder_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_BS_FRAME_DECODER_IMPL_H */
