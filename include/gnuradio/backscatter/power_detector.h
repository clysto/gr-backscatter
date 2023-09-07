/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_POWER_DETECTOR_H
#define INCLUDED_BACKSCATTER_POWER_DETECTOR_H

#include <gnuradio/backscatter/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
namespace backscatter {

/*!
 * \brief <+description of block+>
 * \ingroup backscatter
 *
 */
class BACKSCATTER_API power_detector : virtual public gr::hier_block2
{
public:
    typedef std::shared_ptr<power_detector> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of backscatter::power_detector.
     *
     * To avoid accidental use of raw pointers, backscatter::power_detector's
     * constructor is in a private implementation
     * class. backscatter::power_detector::make is the public interface for
     * creating new instances.
     */
    static sptr make(int window_size);
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_POWER_DETECTOR_H */
