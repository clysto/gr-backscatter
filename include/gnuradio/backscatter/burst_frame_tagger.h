/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_BURST_FRAME_TAGGER_H
#define INCLUDED_BACKSCATTER_BURST_FRAME_TAGGER_H

#include <gnuradio/backscatter/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace backscatter {

/*!
 * \brief <+description of block+>
 * \ingroup backscatter
 *
 */
class BACKSCATTER_API burst_frame_tagger : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<burst_frame_tagger> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of backscatter::burst_frame_tagger.
     *
     * To avoid accidental use of raw pointers, backscatter::burst_frame_tagger's
     * constructor is in a private implementation
     * class. backscatter::burst_frame_tagger::make is the public interface for
     * creating new instances.
     */
    static sptr make(float snr_threshoud, int look_ahead);

    //! \brief Set the threshold for the SNR.
    virtual void set_snr_threshoud(float snr_threshoud) = 0;
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_BURST_FRAME_TAGGER_H */
