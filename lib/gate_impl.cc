/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "gate_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace backscatter {

using input_type = float;
using output_type = float;

gate::sptr gate::make(unsigned int max_samples)
{
    return gnuradio::make_block_sptr<gate_impl>(max_samples);
}


/*
 * The private constructor
 */
gate_impl::gate_impl(unsigned int max_samples)
    : gr::block("gate",
                gr::io_signature::make(1, 1, sizeof(input_type)),
                gr::io_signature::make(1, 1, sizeof(output_type))),
      d_scale(1),
      d_max_samples(max_samples),
      d_expected(max_samples),
      d_have_sync(false)
{
    message_port_register_in(pmt::mp("close"));
    set_msg_handler(pmt::mp("close"), [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    set_tag_propagation_policy(gr::block::tag_propagation_policy_t::TPP_DONT);
}


gate_impl::~gate_impl() {}

int gate_impl::general_work(int noutput_items,
                            gr_vector_int& ninput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items)
{
    auto in = static_cast<const float*>(input_items[0]);
    auto out = static_cast<float*>(output_items[0]);

    std::vector<tag_t> tags;
    get_tags_in_range(tags,
                      0,
                      nitems_read(0),
                      nitems_read(0) + ninput_items[0],
                      pmt::string_to_symbol("frame_start"));
    if (d_expected <= 0) {
        d_have_sync = false;
        d_expected = d_max_samples;
    }
    if (d_have_sync) {
        int ncp = std::min(noutput_items, ninput_items[0]);
        ncp = std::min(ncp, d_expected);
        for (int i = 0; i < ncp; i++) {
            out[i] = in[i] * d_scale;
        }
        for (size_t i = 0; i < tags.size(); i++) {
            gr::tag_t t = tags[i];
            if (t.offset >= nitems_read(0) + ncp)
                continue;
            int offset = (nitems_read(0) - nitems_written(0));
            t.offset -= offset;
            add_item_tag(0, t);
        }
        d_expected -= ncp;
        consume_each(ncp);
        return ncp;
    } else {
        if (!tags.empty()) {
            d_have_sync = true;
            consume_each(tags[0].offset - nitems_read(0));
            float val = pmt::to_float(tags[0].value);
            if (val != 0) {
                d_scale = 1 / val;
            }
        } else {
            consume_each(ninput_items[0]);
        }
        return 0;
    }
}

void gate_impl::handle_msg(pmt::pmt_t msg) { d_expected = 0; }

} /* namespace backscatter */
} /* namespace gr */
