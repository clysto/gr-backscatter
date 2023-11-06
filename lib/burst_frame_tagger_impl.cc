/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "burst_frame_tagger_impl.h"
#include <gnuradio/io_signature.h>
#include <pmt/pmt.h>

namespace gr {
namespace backscatter {

using input_type = float;
using output_type = float;

burst_frame_tagger::sptr burst_frame_tagger::make(float snr_threshoud, int look_ahead)
{
    return gnuradio::make_block_sptr<burst_frame_tagger_impl>(snr_threshoud, look_ahead);
}


/*
 * The private constructor
 */
burst_frame_tagger_impl::burst_frame_tagger_impl(float snr_threshoud, int look_ahead)
    : gr::sync_block("burst_frame_tagger",
                     gr::io_signature::make(3, 3, sizeof(input_type)),
                     gr::io_signature::make(1, 1, sizeof(output_type))),
      d_snr_threshoud(snr_threshoud),
      d_look_ahead(look_ahead),
      d_state(0),
      d_first_snr(0),
      d_last_snr(0),
      d_below_threshoud_count(0)
{
    std::stringstream str;
    str << name() << unique_id();
    d_id = pmt::string_to_symbol(str.str());
}

/*
 * Our virtual destructor.
 */
burst_frame_tagger_impl::~burst_frame_tagger_impl() {}

int burst_frame_tagger_impl::work(int noutput_items,
                                  gr_vector_const_void_star& input_items,
                                  gr_vector_void_star& output_items)
{
    auto signal = static_cast<const input_type*>(input_items[0]);
    auto snr = static_cast<const input_type*>(input_items[1]);
    auto power = static_cast<const input_type*>(input_items[2]);
    auto out = static_cast<output_type*>(output_items[0]);

    // find the first snr that exceeds the threshold
    if (d_state == 0) {
        for (int i = 0; i < noutput_items; i++) {
            if (snr[i] > d_snr_threshoud) {
                // exceed the threshold
                d_state = 1;
                memcpy(out, signal, i * 4);
                return i;
            }
        }
        memcpy(out, signal, noutput_items * 4);
        return noutput_items;
    }
    // find the peak snr
    else if (d_state == 1) {
        if (noutput_items < d_look_ahead) {
            return 0; // ask for more
        }
        // early late gate
        d_first_snr = snr[0];
        d_last_snr = snr[d_look_ahead - 1];
        if (d_first_snr < d_last_snr) {
            // too early
            out[0] = signal[0];
            return 1;
        } else if (d_state == 1) {
            float max_val = snr[0];
            int max_ind = 0;
            for (int i = 0; i < d_look_ahead; i++) {
                if (snr[i] > max_val) {
                    max_val = snr[i];
                    max_ind = i;
                }
            }
            add_item_tag(0,
                         nitems_written(0) + max_ind,
                         pmt::intern("frame_start"),
                         pmt::from_float(power[max_ind]),
                         d_id);
            memcpy(out, signal, d_look_ahead * 4);
            d_below_threshoud_count = 0;
            d_state = 2;
            return d_look_ahead;
        }
    }
    // find the peak end
    else {
        for (int i = 0; i < noutput_items; i++) {
            if (snr[i] < d_snr_threshoud) {
                d_below_threshoud_count++;
                if (d_below_threshoud_count > d_look_ahead) {
                    d_state = 0;
                    memcpy(out, signal, i * 4);
                    return i;
                }
            } else {
                d_below_threshoud_count = 0;
            }
        }
        memcpy(out, signal, noutput_items * 4);
        return noutput_items;
    }
    return noutput_items;
}

void burst_frame_tagger_impl::set_snr_threshoud(float snr_threshoud)
{
    d_snr_threshoud = snr_threshoud;
}

} /* namespace backscatter */
} /* namespace gr */
