/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "burst_frame_scaler_impl.h"
#include <gnuradio/io_signature.h>
#include <pmt/pmt.h>
#include <string>

namespace gr {
namespace backscatter {

using input_type = float;
using output_type = float;

burst_frame_scaler::sptr burst_frame_scaler::make()
{
    return gnuradio::make_block_sptr<burst_frame_scaler_impl>();
}


/*
 * The private constructor
 */
burst_frame_scaler_impl::burst_frame_scaler_impl()
    : gr::sync_block("burst_frame_scaler",
                     gr::io_signature::make(1, 1, sizeof(input_type)),
                     gr::io_signature::make(1, 1, sizeof(output_type))),
      d_scale(1)
{
}

/*
 * Our virtual destructor.
 */
burst_frame_scaler_impl::~burst_frame_scaler_impl() {}

int burst_frame_scaler_impl::work(int noutput_items,
                                  gr_vector_const_void_star& input_items,
                                  gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    std::vector<tag_t> tags;
    get_tags_in_range(tags,
                      0,
                      nitems_read(0),
                      nitems_read(0) + noutput_items,
                      pmt::string_to_symbol("frame_start"));

    if (!tags.empty()) {
        int n = tags[0].offset - nitems_written(0) + 1;
        for (int i = 0; i < n; i++) {
            out[i] = in[i] * d_scale;
        }
        float val = pmt::to_float(tags[0].value);
        if (val == 0) {
            d_scale = 1;
        } else {
            d_scale = 1 / val;
        }
        return n;
    } else {
        for (int i = 0; i < noutput_items; i++) {
            out[i] = in[i] * d_scale;
        }
        return noutput_items;
    }
}

} /* namespace backscatter */
} /* namespace gr */
