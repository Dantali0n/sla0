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

#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libsigrok/libsigrok.h>
#include "libsigrok-internal.h"
#include "protocol.h"

#define DEFAULT_NUM_LOGIC_CHANNELS		8
#define DEFAULT_LOGIC_PATTERN			PATTERN_SIGROK

#define DEFAULT_NUM_ANALOG_CHANNELS		5

static const uint32_t scanopts[] = {
	SR_CONF_NUM_LOGIC_CHANNELS,
	SR_CONF_NUM_ANALOG_CHANNELS,
	SR_CONF_LIMIT_FRAMES,
};

static const uint32_t drvopts[] = {
	SR_CONF_DEMO_DEV,
	SR_CONF_LOGIC_ANALYZER,
	SR_CONF_OSCILLOSCOPE,
};

static const uint32_t devopts[] = {
	SR_CONF_CONTINUOUS,
	SR_CONF_LIMIT_SAMPLES | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_LIMIT_MSEC | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_LIMIT_FRAMES | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_SAMPLERATE | SR_CONF_GET | SR_CONF_SET | SR_CONF_LIST,
	SR_CONF_AVERAGING | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_AVG_SAMPLES | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_TRIGGER_MATCH | SR_CONF_LIST,
	SR_CONF_CAPTURE_RATIO | SR_CONF_GET | SR_CONF_SET,
};

static const uint32_t devopts_cg_logic[] = {
	SR_CONF_PATTERN_MODE | SR_CONF_GET | SR_CONF_SET | SR_CONF_LIST,
};

static const uint32_t devopts_cg_analog_group[] = {
	SR_CONF_AMPLITUDE | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_OFFSET | SR_CONF_GET | SR_CONF_SET,
};

static const uint32_t devopts_cg_analog_channel[] = {
	SR_CONF_MEASURED_QUANTITY | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_PATTERN_MODE | SR_CONF_GET | SR_CONF_SET | SR_CONF_LIST,
	SR_CONF_AMPLITUDE | SR_CONF_GET | SR_CONF_SET,
	SR_CONF_OFFSET | SR_CONF_GET | SR_CONF_SET,
};

static const int32_t trigger_matches[] = {
	SR_TRIGGER_ZERO,
	SR_TRIGGER_ONE,
	SR_TRIGGER_RISING,
	SR_TRIGGER_FALLING,
	SR_TRIGGER_EDGE,
};

static const uint64_t samplerates[] = {
	SR_HZ(1),
	SR_GHZ(1),
	SR_HZ(1),
};

static GSList *scan(struct sr_dev_driver *di, GSList *options)
{
	struct dev_context *devc;
	struct sr_dev_inst *sdi;
	struct sr_channel *ch;
	struct sr_channel_group *cg, *acg;
	struct sr_config *src;
	struct analog_gen *ag;
	GSList *l;
	int num_logic_channels, num_analog_channels, pattern, i;
	uint64_t limit_frames;
	char channel_name[16];

	num_logic_channels = DEFAULT_NUM_LOGIC_CHANNELS;
	num_analog_channels = DEFAULT_NUM_ANALOG_CHANNELS;
	for (l = options; l; l = l->next) {
		src = l->data;
		switch (src->key) {
		case SR_CONF_NUM_LOGIC_CHANNELS:
			num_logic_channels = g_variant_get_int32(src->data);
			break;
		case SR_CONF_NUM_ANALOG_CHANNELS:
			num_analog_channels = g_variant_get_int32(src->data);
			break;
		case SR_CONF_LIMIT_FRAMES:
			limit_frames = g_variant_get_uint64(src->data);
			break;
		}
	}

	sdi = g_malloc0(sizeof(struct sr_dev_inst));
	sdi->status = SR_ST_INACTIVE;
	sdi->model = g_strdup("Psuedo HP1650b");

	devc = g_malloc0(sizeof(struct dev_context));
	devc->cur_samplerate = SR_KHZ(200);
	devc->num_logic_channels = num_logic_channels;
	devc->logic_unitsize = (devc->num_logic_channels + 7) / 8;
	devc->all_logic_channels_mask = 1UL << 0;
	devc->all_logic_channels_mask <<= devc->num_logic_channels;
	devc->all_logic_channels_mask--;
	devc->num_analog_channels = num_analog_channels;
	devc->limit_frames = limit_frames;

	sdi->priv = devc;

	return std_scan_complete(di, g_slist_append(NULL, sdi));
}

static void clear_helper(struct dev_context *devc)
{

}

static int dev_clear(const struct sr_dev_driver *di)
{
	return std_dev_clear_with_callback(di, (std_dev_clear_callback)clear_helper);
}

static int config_get(uint32_t key, GVariant **data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{

}

static int config_set(uint32_t key, GVariant *data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{

}

static int config_list(uint32_t key, GVariant **data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{
}

static int dev_acquisition_start(const struct sr_dev_inst *sdi)
{

}

static int dev_acquisition_stop(struct sr_dev_inst *sdi)
{

}

static struct sr_dev_driver hp165xb_driver_info = {
	.name = "HP1650XB",
	.longname = "Driver for HP 1650B series logic analyzers",
	.api_version = 1,
	.init = std_init,
	.cleanup = std_cleanup,
	.scan = scan,
	.dev_list = std_dev_list,
	.dev_clear = dev_clear,
	.config_get = config_get,
	.config_set = config_set,
	.config_list = config_list,
	.dev_open = std_dummy_dev_open,
	.dev_close = std_dummy_dev_close,
	.dev_acquisition_start = dev_acquisition_start,
	.dev_acquisition_stop = dev_acquisition_stop,
	.context = NULL,
};
SR_REGISTER_DEV_DRIVER(hp165xb_driver_info);
