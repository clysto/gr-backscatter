/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "multiply_by_tag_value_ff_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>


namespace gr {
namespace backscatter {

multiply_by_tag_value_ff::sptr multiply_by_tag_value_ff::make(const std::string& tag_name,
                                                              size_t vlen)
{
    return gnuradio::make_block_sptr<multiply_by_tag_value_ff_impl>(tag_name, vlen);
}


multiply_by_tag_value_ff_impl::multiply_by_tag_value_ff_impl(const std::string& tag_name,
                                                             size_t vlen)
    : gr::sync_block("multiply_by_tag_value_ff",
                     io_signature::make(1, 1, sizeof(float) * vlen),
                     io_signature::make(1, 1, sizeof(float) * vlen)),
      d_vlen(vlen),
      d_k(1)
{
    d_tag_key = pmt::intern(tag_name);

    const int alignment_multiple = volk_get_alignment() / sizeof(float);
    set_alignment(std::max(1, alignment_multiple));
}


multiply_by_tag_value_ff_impl::~multiply_by_tag_value_ff_impl() {}

float multiply_by_tag_value_ff_impl::k() const { return d_k; }

int multiply_by_tag_value_ff_impl::work(int noutput_items,
                                        gr_vector_const_void_star& input_items,
                                        gr_vector_void_star& output_items)
{
    const float* in = (const float*)input_items[0];
    float* out = (float*)output_items[0];
    std::vector<tag_t> tags;
    get_tags_in_window(tags, 0, 0, noutput_items, d_tag_key);

    int start = 0, end;
    for (const auto& tag : tags) {
        end = tag.offset - nitems_read(0);
        end *= d_vlen;

        // Multiply based on the current value of k from 'start' to 'end'
        volk_32f_s32f_multiply_32f(&out[start], &in[start], d_k, (end - start));
        start = end;

        // Extract new value of k
        pmt::pmt_t k = tag.value;
        if (pmt::is_number(k)) {
            d_k = pmt::to_double(k);
        } else {
            d_logger->warn("Got key '{:s}' with incompatible value of '{:s}'",
                           pmt::write_string(d_tag_key),
                           pmt::write_string(k));
        }
    }

    volk_32f_s32f_multiply_32f(
        &out[start], &in[start], d_k, (d_vlen * noutput_items - start));

    return noutput_items;
}

} /* namespace backscatter */
} /* namespace gr */
