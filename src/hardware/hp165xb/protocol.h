/*
 * This file is part of the libsigrok project.
 *
 * Copyright (C) 2025 Roy van Lierop
 * Copyright (C) 2025 Corne Lukken
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBSIGROK_HARDWARE_HP165XB_PROTOCOL_H
#define LIBSIGROK_HARDWARE_HP165XB_PROTOCOL_H

#include <stdint.h>
#include <libsigrok/libsigrok.h>
#include "libsigrok-internal.h"

#define LOG_PREFIX "HP165XB"

#define DEFAULT_NUM_LOGIC_CHANNELS

struct dev_context {
	uint64_t cur_samplerate;
	uint64_t limit_samples;
	uint64_t limit_msec;
	uint64_t limit_frames;
	uint64_t sent_samples;
	uint64_t sent_frame_samples; /* Number of samples that were sent for current frame. */
	int64_t start_us;
	int64_t spent_us;
	uint64_t step;

	/* Logic */
	int32_t num_logic_channels;
	size_t logic_unitsize;
	uint64_t all_logic_channels_mask;
	uint8_t logic_data[4096];

	/* Analog */
	int32_t num_analog_channels;
	GHashTable *ch_ag;
	size_t enabled_logic_channels;
	size_t enabled_analog_channels;
};

#endif
