/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2012 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_AGG_HELPERS_HPP
#define MAPNIK_AGG_HELPERS_HPP

// agg 
#include "agg_basics.h"
#include "agg_gamma_functions.h"
#include "agg_math_stroke.h"
#include "agg_pixfmt_rgba.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_scanline.h"

namespace mapnik {

template <typename T0, typename T1>
void set_gamma_method(T0 const& obj, T1 & ras_ptr)
{
    switch (obj.get_gamma_method())
    {
    case GAMMA_POWER:
        ras_ptr->gamma(agg::gamma_power(obj.get_gamma()));
        break;
    case GAMMA_LINEAR:
        ras_ptr->gamma(agg::gamma_linear(0.0, obj.get_gamma()));
        break;
    case GAMMA_NONE:
        ras_ptr->gamma(agg::gamma_none());
        break;
    case GAMMA_THRESHOLD:
        ras_ptr->gamma(agg::gamma_threshold(obj.get_gamma()));
        break;
    case GAMMA_MULTIPLY:
        ras_ptr->gamma(agg::gamma_multiply(obj.get_gamma()));
        break;
    default:
        ras_ptr->gamma(agg::gamma_power(obj.get_gamma()));
    }
}

template <typename Stroke,typename PathType>
void set_join_caps(Stroke const& stroke_, PathType & stroke)
{
    line_join_e join=stroke_.get_line_join();
    switch (join)
    {
    case MITER_JOIN:
        stroke.generator().line_join(agg::miter_join);
        break;
    case MITER_REVERT_JOIN:
        stroke.generator().line_join(agg::miter_join);
        break;
    case ROUND_JOIN:
        stroke.generator().line_join(agg::round_join);
        break;
    default:
        stroke.generator().line_join(agg::bevel_join);
    }

    line_cap_e cap=stroke_.get_line_cap();
    switch (cap)
    {
    case BUTT_CAP:
        stroke.generator().line_cap(agg::butt_cap);
        break;
    case SQUARE_CAP:
        stroke.generator().line_cap(agg::square_cap);
        break;
    default:
        stroke.generator().line_cap(agg::round_cap);
    }
}


template <typename PixelFormat>
struct renderer_ : private boost::noncopyable
{
    typedef PixelFormat pixfmt_type;
    typedef typename pixfmt_type::color_type color_type;
    typedef typename pixfmt_type::row_data row_data;
    typedef agg::renderer_base<pixfmt_type> ren_base;  
    typedef agg::renderer_scanline_aa_solid<ren_base> renderer;
    typedef agg::scanline_u8 scanline_type;
    
    renderer_()
        : renb_(),
          ren_(renb_)
    {}
    
    template <typename PF>
    void attach(PF & pf)
    {
        renb_.attach(pf);
    }
    
    void color(color_type const& c)
    {
        ren_.color(c);
    }
    
    template <typename Rasterizer>
    void render(Rasterizer & ras)
    {
        agg::render_scanlines(ras, sl_, ren_);
        sl_. reset_spans();
    }

    scanline_type sl_;
    ren_base renb_;
    renderer ren_;
};

struct aa_renderer : renderer_<agg::pixfmt_rgba32_plain> {};

}

#endif //MAPNIK_AGG_HELPERS_HPP
