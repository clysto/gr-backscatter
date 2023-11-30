/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_MULTIPLY_BY_TAG_VALUE_FF_IMPL_H
#define INCLUDED_BACKSCATTER_MULTIPLY_BY_TAG_VALUE_FF_IMPL_H

#include <gnuradio/backscatter/multiply_by_tag_value_ff.h>

namespace gr {
namespace backscatter {

class multiply_by_tag_value_ff_impl : public multiply_by_tag_value_ff
{
private:
    const size_t d_vlen;
    pmt::pmt_t d_tag_key;
    float d_k;

public:
    multiply_by_tag_value_ff_impl(const std::string& tag_name, size_t vlen);
    ~multiply_by_tag_value_ff_impl() override;

    float k() const override;

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_MULTIPLY_BY_TAG_VALUE_FF_IMPL_H */
