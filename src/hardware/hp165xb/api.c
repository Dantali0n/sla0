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

static const uint32_t scanopts[] = {
	SR_CONF_CONN,
	SR_CONF_SERIALCOMM
};

static const uint32_t drvopts[] = {
	SR_CONF_LOGIC_ANALYZER
};

static const uint32_t devopts[] = {
	SR_CONF_CONN | SR_CONF_GET,
	// SR_CONF_ENABLED | SR_CONF_SET,
};

static const uint32_t devopts_cg[] = {
	// SR_CONF_ENABLED | SR_CONF_SET,
};

static struct sr_dev_driver hp_1650b_driver_info;

/**
 * TODO(Corne): Fix me
 */
SR_PRIV int hp_1650b_get_model(struct sr_serial_dev_inst *serial,
	uint8_t *model_code, uint8_t *hw_version, uint8_t *fw_version)
{
	return SR_ERR;
	sr_log(SR_LOG_DBG, "Serial Scan!");
}

static struct sr_dev_inst *probe_device_conn(const char *conn)
{
	struct sr_dev_inst *sdi;
	struct dev_context *devc;
	struct sr_serial_dev_inst *ser;
	uint8_t code, hwver, fwver;
	char snr_txt[16];
	struct channel_group_context *cgc;
	size_t ch_idx, nr, do_idx, di_idx, ai_idx;
	struct sr_channel_group *cg;
	char cg_name[24];
	int ret;

	sdi = g_malloc0(sizeof(*sdi));
	devc = g_malloc0(sizeof(*devc));
	sdi->priv = devc;
	ser = sr_serial_dev_inst_new(conn, NULL);
	sdi->conn = ser;
	if (!ser)
		goto probe_fail;
	ret = serial_open(ser, 0);
	if (ret != SR_OK)
		goto probe_fail;

	ret = hp_1650b_get_model(ser, &code, &hwver, &fwver);
	if (ret != SR_OK)
		goto probe_fail;

	sdi->vendor = g_strdup("HP");
	// sdi->model = g_strdup(model->name);
	// sdi->version = g_strdup_printf("HW%u FW%u", hwver, fwver);
	sdi->connection_id = g_strdup(conn);
	sdi->driver = &hp_1650b_driver_info;
	sdi->inst_type = SR_INST_SERIAL;

	return sdi;

probe_fail:
	if (ser) {
		serial_close(ser);
		sr_serial_dev_inst_free(ser);
	}
	if (devc) {
		g_free(devc);
	}
	if (sdi) {
		sdi->priv = NULL;
		sr_dev_inst_free(sdi);
		sdi = NULL;
	}
	return sdi;
}

static GSList *scan(struct sr_dev_driver *di, GSList *options)
{
	struct drv_context *drvc;
	const char *conn;
	GSList *devices;
	struct sr_dev_inst *sdi;

	drvc = di->context;
	drvc->instances = NULL;

	conn = NULL;
	(void)sr_serial_extract_options(options, &conn, NULL);
	if (!conn || !*conn)
		return NULL;

	devices = NULL;
	sdi = probe_device_conn(conn);
	if (sdi)
		devices = g_slist_append(devices, sdi);

	return std_scan_complete(di, devices);
}

// static GSList *scan(struct sr_dev_driver *di, GSList *options)
// {
// 	struct dev_context *devc;
// 	struct sr_dev_inst *sdi;
// 	struct sr_channel *ch;
// 	struct sr_channel_group *cg;
// 	// struct sr_config *src;
// 	// GSList *l;
// 	int num_logic_channels = DEFAULT_NUM_LOGIC_CHANNELS;
// 	// uint64_t limit_frames;
// 	char channel_name[16];

// 	sdi = g_malloc0(sizeof(struct sr_dev_inst));
// 	sdi->status = SR_ST_INACTIVE;
// 	sdi->model = g_strdup("Psuedo HP1650b");

// 	devc = g_malloc0(sizeof(struct dev_context));
// 	devc->cur_samplerate = SR_KHZ(200);
// 	devc->num_logic_channels = num_logic_channels;
// 	devc->logic_unitsize = (devc->num_logic_channels + 7) / 8;

// 	if (num_logic_channels > 0) {
// 		/* Logic channels, all in one channel group. */
// 		cg = sr_channel_group_new(sdi, "Logic", NULL);
// 		for (size_t i = 0; i < num_logic_channels; i++) {
// 			sprintf(channel_name, "D%d", i);
// 			ch = sr_channel_new(sdi, i, SR_CHANNEL_LOGIC, TRUE, channel_name);
// 			cg->channels = g_slist_append(cg->channels, ch);
// 		}
// 	}

// 	sdi->priv = devc;

// 	return std_scan_complete(di, g_slist_append(NULL, sdi));
// }

static int config_get(uint32_t key, GVariant **data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{
	struct dev_context *devc;

	if (!sdi)
		return SR_ERR_ARG;

	devc = sdi->priv;
	switch (key) {
	// case SR_CONF_ENABLED:
	// 	*data = g_variant_new_uint64(sdi->connection_id);
	// 	break;
	default:
		return SR_ERR_NA;
	}

	return SR_OK;
}

static int config_set(uint32_t key, GVariant *data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{
	struct dev_context *devc;
	// struct analog_gen *ag;
	// struct sr_channel *ch;
	// GVariant *mq_tuple_child;
	// GSList *l;
	// int logic_pattern, analog_pattern;

	devc = sdi->priv;

	switch (key) {
	// case SR_CONF_ENABLED:
	// 	/* Enable/disable all channels at the same time. */
	// 	on = g_variant_get_boolean(data);
	// 	break;
	// case SR_CONF_SAMPLERATE:
	// 	devc->cur_samplerate = g_variant_get_uint64(data);
	// 	break;
	default:
		return SR_ERR_NA;
	}

	return SR_OK;
}

static int config_list(uint32_t key, GVariant **data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{
	struct sr_channel *ch;

	if (!cg) {
		switch (key) {
		case SR_CONF_SCAN_OPTIONS:
		case SR_CONF_DEVICE_OPTIONS:
			return STD_CONFIG_LIST(key, data, sdi, cg, scanopts, drvopts, devopts);
		// case SR_CONF_SAMPLERATE:
		// 	*data = std_gvar_samplerates_steps(ARRAY_AND_SIZE(samplerates));
		// 	break;
		// case SR_CONF_TRIGGER_MATCH:
		// 	*data = std_gvar_array_i32(ARRAY_AND_SIZE(trigger_matches));
		// 	break;
		default:
			return SR_ERR_NA;
		}
	} else {
		ch = cg->channels->data;
		switch (key) {
		case SR_CONF_DEVICE_OPTIONS:
			*data = std_gvar_array_u32(ARRAY_AND_SIZE(devopts_cg));
			break;
		default:
			return SR_ERR_NA;
		}
	}

	return SR_OK;
}

// static int dev_acquisition_start(const struct sr_dev_inst *sdi)
// {

// }

// static int dev_acquisition_stop(struct sr_dev_inst *sdi)
// {

// }

static struct sr_dev_driver hp165xb_driver_info = {
	.name = "HP1650B",
	.longname = "HP 1650B logic analyzers",
	.api_version = 1,
	.init = std_init,
	.cleanup = std_cleanup,
	.scan = scan,
	.dev_list = std_dev_list,
	.dev_clear = std_dev_clear,
	.config_get = config_get,
	.config_set = config_set,
	.config_list = config_list,
	.dev_open = std_dummy_dev_open,
	.dev_close = std_dummy_dev_close,
	.dev_acquisition_start = std_dummy_dev_acquisition_start,
	.dev_acquisition_stop = std_dummy_dev_acquisition_stop,
	.context = NULL,
};

SR_REGISTER_DEV_DRIVER(hp165xb_driver_info);
