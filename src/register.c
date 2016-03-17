/**
 * This file is part of ffmpeg_sender, which belongs to StreamSwitch
 * project. 
 * 
 * Copyright (C) 2014  OpenSight (www.opensight.cn)
 * 
 * StreamSwitch is an extensible and scalable media stream server for 
 * multi-protocol environment. 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/



    
#include "cached_segment.h"

#define REGISTER_MUXER(x)                                            \
    {                                                                   \
        extern AVOutputFormat ff_##x##_muxer;                           \
        av_register_output_format(&ff_##x##_muxer);                 \
    }
    

#define REGISTER_CSEG_WRITER(x)                                            \
    {                                                                   \
        extern CachedSegmentWriter cseg_##x##_writer;                           \
        av_register_output_format(&cseg_##x##_writer);                 \
    }



void ffmpeg_ivr_register(void)
{
    static int initialized;

    if (initialized)
        return;
    initialized = 1;
    
    REGISTER_CSEG_WRITER(file);
    REGISTER_CSEG_WRITER(dummy);
    REGISTER_CSEG_WRITER(ivr);    
    
    REGISTER_MUXER(cached_segment);

}