/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_BS_FRAME_DECODER_H
#define INCLUDED_BACKSCATTER_BS_FRAME_DECODER_H

#include <gnuradio/backscatter/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace backscatter {

/*!
 * \brief <+description of block+>
 * \ingroup backscatter
 *
 */
class BACKSCATTER_API bs_frame_decoder : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<bs_frame_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of backscatter::bs_frame_decoder.
     *
     * To avoid accidental use of raw pointers, backscatter::bs_frame_decoder's
     * constructor is in a private implementation
     * class. backscatter::bs_frame_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned int threshold, unsigned int pdu_max_len);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_BS_FRAME_DECODER_H */
