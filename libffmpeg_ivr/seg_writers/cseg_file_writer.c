/**
 * This file is part of ffmpeg_ivr
 * 
 * Copyright (C) 2016  OpenSight (www.opensight.cn)
 * 
 * ffmpeg_ivr is an extension of ffmpeg to implements the new feature for IVR
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



#include <float.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>

#include "libavutil/avassert.h"
#include "libavutil/mathematics.h"
#include "libavutil/parseutils.h"
#include "libavutil/avstring.h"
#include "libavutil/opt.h"
#include "libavutil/log.h"
#include "libavutil/fifo.h"
#include "libavutil/dict.h"

#include "libavformat/avformat.h"
#include "libavformat/avio.h"    
#include "../cached_segment.h"

struct URLContext;


static int file_init(CachedSegmentContext *cseg)
{
    return 0;
}


#define MAX_FILE_NAME 1024
static int file_write_segment(CachedSegmentContext *cseg, CachedSegment *segment)
{
    char base_name[MAX_FILE_NAME];
    char file_name[MAX_FILE_NAME];
    char ext_name[32] = "";
    char *p;
    AVIOContext *file_context;
    int ret;
    uint8_t *buf;
    
    //printf("file_write_segment is calle\n");
    
    av_strlcpy(base_name, cseg->filename, MAX_FILE_NAME);

    p = strrchr(base_name, '/');  
    if(p){
        p = strrchr(p, '.');
        if (p){
            av_strlcpy(ext_name, p, 32);
            *p = '\0';
        }
    }

    
    snprintf(file_name, MAX_FILE_NAME - 1, "%s_%.3f_%.3f_%lld%s", 
             base_name, segment->start_ts, segment->duration, 
             (long long)segment->sequence, ext_name);
    
    ret = avio_open(&file_context, file_name, AVIO_FLAG_WRITE);
    if(ret < 0){
        return ret;
    }
    
    avio_write(file_context, segment->buffer, segment->size);
    ret = file_context->error;
    if(ret < 0){
        avio_closep(&file_context);
        return ret;
    }
    avio_closep(&file_context);
    
    return 0;
}

static void file_uninit(CachedSegmentContext *cseg)
{
   
}
CachedSegmentWriter cseg_file_writer = {
    .name           = "file_writer",
    .long_name      = "OpenSight file segment writer", 
    .protos         = "file", 
    .init           = file_init, 
    .write_segment  = file_write_segment, 
    .uninit         = file_uninit,
};

