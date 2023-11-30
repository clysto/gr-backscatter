/* -*- c++ -*- */
/*
 * Copyright 2023 gr-backscatter author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BACKSCATTER_MULTIPLY_BY_TAG_VALUE_FF_H
#define INCLUDED_BACKSCATTER_MULTIPLY_BY_TAG_VALUE_FF_H

#include <gnuradio/backscatter/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace backscatter {

/*!
 * \brief <+description of block+>
 * \ingroup backscatter
 *
 */
class BACKSCATTER_API multiply_by_tag_value_ff : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<multiply_by_tag_value_ff> sptr;

    /*!
     * \brief Create an instance of multiply_by_tag_value_ff
     * \param tag_name Tag's key that it will use to get the
     *                 multiplicative constant.
     * \param vlen Vector length of incoming stream
     */
    static sptr make(const std::string& tag_name, size_t vlen = 1);

    /*!
     * Get the current multiplicative constant.
     * This block does not allow external setters.
     */
    virtual float k() const = 0;
};

} // namespace backscatter
} // namespace gr

#endif /* INCLUDED_BACKSCATTER_MULTIPLY_BY_TAG_VALUE_FF_H */
