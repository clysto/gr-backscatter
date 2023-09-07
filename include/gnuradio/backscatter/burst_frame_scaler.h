/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_BURST_FRAME_SCALER_H
#define INCLUDED_BACKSCATTER_BURST_FRAME_SCALER_H

#include <gnuradio/backscatter/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace backscatter {

/*!
 * \brief <+description of block+>
 * \ingroup backscatter
 *
 */
class BACKSCATTER_API burst_frame_scaler : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<burst_frame_scaler> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of backscatter::burst_frame_scaler.
     *
     * To avoid accidental use of raw pointers, backscatter::burst_frame_scaler's
     * constructor is in a private implementation
     * class. backscatter::burst_frame_scaler::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_BURST_FRAME_SCALER_H */
