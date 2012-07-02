/*
 * Copyright (C) 2006 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with FFmpeg; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "libavutil/attributes.h"
#include "libavutil/cpu.h"
#include "libavutil/x86_cpu.h"
#include "libavcodec/x86/dsputil_mmx.h"
#include "libavfilter/yadif.h"

DECLARE_ASM_CONST(16, const xmm_reg, pb_1) = {0x0101010101010101ULL, 0x0101010101010101ULL};
DECLARE_ASM_CONST(16, const xmm_reg, pw_1) = {0x0001000100010001ULL, 0x0001000100010001ULL};

#if HAVE_SSSE3
#define COMPILE_TEMPLATE_SSE 1
#define COMPILE_TEMPLATE_SSSE3 1
#undef RENAME
#define RENAME(a) a ## _ssse3
#include "yadif_template.c"
#undef COMPILE_TEMPLATE_SSSE3
#endif

#if HAVE_SSE
#undef RENAME
#define RENAME(a) a ## _sse2
#include "yadif_template.c"
#undef COMPILE_TEMPLATE_SSE
#endif

#if HAVE_MMX
#undef RENAME
#define RENAME(a) a ## _mmx
#include "yadif_template.c"
#endif

av_cold void ff_yadif_init_x86(YADIFContext *yadif)
{
    int cpu_flags = av_get_cpu_flags();

    if (HAVE_MMX && cpu_flags & AV_CPU_FLAG_MMX)
        yadif->filter_line = yadif_filter_line_mmx;
    if (HAVE_SSE && cpu_flags & AV_CPU_FLAG_SSE2)
        yadif->filter_line = yadif_filter_line_sse2;
    if (HAVE_SSSE3 && cpu_flags & AV_CPU_FLAG_SSSE3)
        yadif->filter_line = yadif_filter_line_ssse3;
}
