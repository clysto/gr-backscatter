/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "bs_frame_decoder_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <gnuradio/pdu.h>
#include <gnuradio/tags.h>
#include <volk/volk.h>
#include <algorithm>
#include <cstdint>
#include <string>

namespace gr {
namespace backscatter {

using input_type = float;
bs_frame_decoder::sptr bs_frame_decoder::make(unsigned int threshold,
                                              unsigned int pdu_max_len)
{
    return gnuradio::make_block_sptr<bs_frame_decoder_impl>(threshold, pdu_max_len);
}


/*
 * The private constructor
 */
bs_frame_decoder_impl::bs_frame_decoder_impl(unsigned int threshold,
                                             unsigned int pdu_max_len)
    : gr::sync_block("bs_frame_decoder",
                     gr::io_signature::make(1, 1, sizeof(input_type)),
                     gr::io_signature::make(0, 0, 0)),
      d_state(STATE_FRAME_SEARCH),
      d_data_reg(0),
      d_mask(0),
      d_threshold(threshold),
      d_len(0),
      d_pdu_max_len(pdu_max_len),
      d_payload(std::vector<uint8_t>())

{
    std::string access_code = "110100100011";
    d_len = access_code.length();
    d_mask = ((~0ULL) >> (64 - d_len));
    d_access_code = 0;
    for (unsigned i = 0; i < d_len; i++) {
        d_access_code = (d_access_code << 1) | (access_code[i] & 1);
    }
    message_port_register_out(msgport_names::pdus());
}

/*
 * Our virtual destructor.
 */
bs_frame_decoder_impl::~bs_frame_decoder_impl() {}

inline void bs_frame_decoder_impl::enter_sync_search()
{
    d_state = STATE_SYNC_SEARCH;
    d_data_reg_bits = 0;
}

inline void bs_frame_decoder_impl::enter_header_decode()
{
    d_state = STATE_DECODE_HEADER;
    d_hdr_count = 0;
}

inline void bs_frame_decoder_impl::enter_payload_decode()
{
    d_state = STATE_DECODE_PAYLOAD;
    d_payload_bit = 0;
    d_payload_byte = 0;
    d_payload.clear();
}

inline void bs_frame_decoder_impl::publish_pdu()
{
    auto pdu = pmt::init_u8vector(d_payload.size(), (const unsigned char*)&d_payload[0]);
    message_port_pub(msgport_names::pdus(), pmt::cons(pmt::PMT_NIL, pdu));
}


int bs_frame_decoder_impl::work(int noutput_items,
                                gr_vector_const_void_star& input_items,
                                gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    int index = 0;

    std::vector<tag_t> tags;
    get_tags_in_range(tags,
                      0,
                      nitems_read(0),
                      nitems_read(0) + noutput_items,
                      pmt::string_to_symbol("frame_start"));
    std::set<int> idx;
    for (auto tag : tags) {
        idx.insert(tag.offset - nitems_read(0));
    }

    uint64_t wrong_bits;
    uint64_t nwrong;
    unsigned int bit;

    while (index < noutput_items) {
        if (idx.count(index)) {
            enter_sync_search();
        }
        switch (d_state) {
        case STATE_FRAME_SEARCH:
            index++;
            break;
        case STATE_SYNC_SEARCH:
            // shift in new data
            d_data_reg =
                (d_data_reg << 1) | (gr::branchless_binary_slicer(in[index++]) & 0x1);
            if (d_data_reg_bits + 1 < d_len) {
                d_data_reg_bits++;
                continue;
            }
            // compute hamming distance between desired access code and data_reg
            wrong_bits = 0;
            nwrong = d_threshold + 1;

            wrong_bits = (d_data_reg ^ d_access_code) & d_mask;
            volk_64u_popcnt(&nwrong, wrong_bits);

            if (nwrong <= d_threshold) {
                enter_header_decode();
            }
            break;
        case STATE_DECODE_HEADER:
            bit = gr::branchless_binary_slicer(in[index]) & 0x1;
            if (d_hdr_count & 1) {
                bit = bit ^ d_fm0_pre_symbol ^ 1;
                d_hdr = d_hdr << 1 | bit;
            } else {
                d_fm0_pre_symbol = bit;
            }
            d_hdr_count++;
            index++;
            if (d_hdr_count < 64) {
                continue;
            }
            d_hdr = __builtin_bswap32(d_hdr);
            // limit the payload size to 1500 bytes
            d_hdr = std::min(d_hdr, d_pdu_max_len);
            enter_payload_decode();
            break;
        case STATE_DECODE_PAYLOAD:
            bit = gr::branchless_binary_slicer(in[index]) & 0x1;
            if (d_payload_bit & 1) {
                bit = bit ^ d_fm0_pre_symbol ^ 1;
                d_payload_byte = d_payload_byte << 1 | bit;
            } else {
                d_fm0_pre_symbol = bit;
            }
            d_payload_bit++;
            if (!(d_payload_bit & 15)) {
                d_payload.push_back(d_payload_byte);
                d_payload_byte = 0;
            }
            index++;
            if (d_payload_bit < d_hdr * 8 * 2) {
                continue;
            }
            publish_pdu();
            d_state = STATE_FRAME_SEARCH;
            break;
        }
    }

    return noutput_items;
}

} /* namespace backscatter */
} /* namespace gr */
