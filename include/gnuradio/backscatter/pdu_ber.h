/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_PDU_BER_H
#define INCLUDED_BACKSCATTER_PDU_BER_H

#include <gnuradio/backscatter/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace backscatter {

/*!
 * \brief <+description of block+>
 * \ingroup backscatter
 *
 */
class BACKSCATTER_API pdu_ber : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<pdu_ber> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of backscatter::pdu_ber.
     *
     * To avoid accidental use of raw pointers, backscatter::pdu_ber's
     * constructor is in a private implementation
     * class. backscatter::pdu_ber::make is the public interface for
     * creating new instances.
     */
    static sptr make(std::vector<uint8_t> &payload);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_PDU_BER_H */
