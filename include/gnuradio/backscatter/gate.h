/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_GATE_H
#define INCLUDED_BACKSCATTER_GATE_H

#include <gnuradio/backscatter/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace backscatter {

/*!
 * \brief <+description of block+>
 * \ingroup backscatter
 *
 */
class BACKSCATTER_API gate : virtual public gr::block
{
public:
    typedef std::shared_ptr<gate> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of backscatter::gate.
     *
     * To avoid accidental use of raw pointers, backscatter::gate's
     * constructor is in a private implementation
     * class. backscatter::gate::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned int max_samples);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_GATE_H */
