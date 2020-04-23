/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef GUAC_RDP_PLUGINS_GUACGFX_H
#define GUAC_RDP_PLUGINS_GUACGFX_H

#include <freerdp/constants.h>
#include <freerdp/dvc.h>
#include <freerdp/freerdp.h>
#include <guacamole/client.h>

#include "channels/common-svc.h"

/**
 * Extended version of the IWTSListenerCallback structure, providing additional
 * access to Guacamole-specific data. The IWTSListenerCallback provides access
 * to callbacks related to the receipt of new connections to the AUDIO_INPUT
 * channel.
 */
typedef struct guac_rdp_gfx_listener_callback {

    /**
     * The parent IWTSListenerCallback structure that this structure extends.
     * THIS MEMBER MUST BE FIRST!
     */
    IWTSListenerCallback parent;

    

    /**
     * The guac_client instance associated with the RDP connection using the
     * AUDIO_INPUT plugin.
     */
    guac_client* client;

} guac_rdp_gfx_listener_callback;

/**
 * Extended version of the IWTSVirtualChannelCallback structure, providing
 * additional access to Guacamole-specific data. The IWTSVirtualChannelCallback
 * provides access to callbacks related to an active connection to the
 * AUDIO_INPUT channel, including receipt of data. 
 */
typedef struct guac_rdp_gfx_channel_callback {

    /**
     * The parent IWTSVirtualChannelCallback structure that this structure
     * extends. THIS MEMBER MUST BE FIRST!
     */
    IWTSVirtualChannelCallback parent;

    guac_rdp_common_svc svc;

    /**
     * The actual virtual channel instance along which the AUDIO_INPUT plugin
     * should send any responses.
     */
    IWTSVirtualChannel* channel;

    /**
     * The guac_client instance associated with the RDP connection using the
     * AUDIO_INPUT plugin.
     */
    guac_client* client;

    guac_stream* graphics_stream;

} guac_rdp_gfx_channel_callback;

/**
 * All data associated with Guacamole's AUDIO_INPUT plugin for FreeRDP.
 */
typedef struct guac_rdp_gfx_plugin {

    /**
     * The parent IWTSPlugin structure that this structure extends. THIS
     * MEMBER MUST BE FIRST!
     */
    IWTSPlugin parent;

    /**
     * The listener callback structure allocated when the AUDIO_INPUT plugin
     * was loaded, if any. If the plugin did not fully load, this will be NULL.
     * If non-NULL, this callback structure must be freed when the plugin is
     * terminated.
     */
    guac_rdp_gfx_listener_callback* listener_callback;

    /**
     * The guac_client instance associated with the RDP connection using the
     * AUDIO_INPUT plugin.
     */
    guac_client* client;

} guac_rdp_gfx_plugin;

struct _RDPGFX_CAPSET_INT
{
	UINT32 version;
	UINT32 length;
	UINT32 flags;
};
typedef struct _RDPGFX_CAPSET_INT RDPGFX_CAPSET_INT;

struct _RDPGFX_CAPS_INT_ADVERTISE_PDU
{
	UINT16 capsSetCount;
	RDPGFX_CAPSET_INT* capsSets;
};
typedef struct _RDPGFX_CAPS_INT_ADVERTISE_PDU RDPGFX_CAPS_INT_ADVERTISE_PDU;

#define RDPGFX_CAPSET_INT_BASE_SIZE 8
#define RDPGFX_CAPVERSION_8 0x00080004   /** [MS-RDPEGFX] 2.2.3.1 */
#define RDPGFX_CAPVERSION_81 0x00080105  /** [MS-RDPEGFX] 2.2.3.2 */
#define RDPGFX_CAPVERSION_10 0x000A0002  /** [MS-RDPEGFX] 2.2.3.3 */
#define RDPGFX_CAPVERSION_101 0x000A0100 /** [MS-RDPEGFX] 2.2.3.4 */
#define RDPGFX_CAPVERSION_102 0x000A0200 /** [MS-RDPEGFX] 2.2.3.5 */
#define RDPGFX_CAPVERSION_103 0x000A0301 /** [MS-RDPEGFX] 2.2.3.6 */
#define RDPGFX_CAPVERSION_104 0x000A0400 /** [MS-RDPEGFX] 2.2.3.7 */
#define RDPGFX_CAPVERSION_105 0x000A0502 /** [MS-RDPEGFX] 2.2.3.8 */
#define RDPGFX_CAPVERSION_106 0x000A0600 /** [MS-RDPEGFX] 2.2.3.9, [MS-RDPEGFX-errata] 2018-12-10 \
	            */

#endif

