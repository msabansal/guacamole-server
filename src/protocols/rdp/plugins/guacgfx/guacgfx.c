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

#include "channels/common-svc.h"
#include "channels/pipe-svc.h"
#include "plugins/guacgfx/guacgfx.h"
#include "plugins/ptr-string.h"
#include "rdp.h"

#include <freerdp/dvc.h>
#include <freerdp/settings.h>
#include <freerdp/channels/rdpgfx.h>
#include <guacamole/client.h>
#include <guacamole/string.h>
#include <winpr/stream.h>
#include <winpr/wtsapi.h>
#include <winpr/wtypes.h>

#include <stdlib.h>

/**
 * Callback which is invoked when data is received along a connection to the
 * GRAPHICS_INPUT plugin.
 *
 * @param channel_callback
 *     The IWTSVirtualChannelCallback structure to which this callback was
 *     originally assigned.
 *
 * @param stream
 *     The data received.
 *
 * @return
 *     Always zero.
 */
static UINT guac_rdp_gfx_data(IWTSVirtualChannelCallback* channel_callback,
        wStream* stream) {

    guac_rdp_gfx_channel_callback* ai_channel_callback =
        (guac_rdp_gfx_channel_callback*) channel_callback;
    /* Invoke generalized (API-independent) data handler */
    guac_client_log(ai_channel_callback->client, GUAC_LOG_DEBUG,
        "GFX Data recieve %d", Stream_GetRemainingLength(stream));
    guac_protocol_send_blob(ai_channel_callback->client->socket, ai_channel_callback->graphics_stream, Stream_Pointer(stream), Stream_GetRemainingLength(stream));
    return CHANNEL_RC_OK;

}

/**
 * Function description
 *
 * @return 0 on success, otherwise a Win32 error code
 */
// static UINT rdpgfx_send_caps_advertise_pdu(guac_rdp_gfx_channel_callback* callback,
//                                            const RDPGFX_CAPS_INT_ADVERTISE_PDU* pdu)
// {
// 	UINT error = CHANNEL_RC_OK;
// 	UINT16 index;
// 	RDPGFX_HEADER header;
// 	RDPGFX_CAPSET_INT* capsSet;
// 	wStream* s;
// 	header.flags = 0;
// 	header.cmdId = RDPGFX_CMDID_CAPSADVERTISE;
// 	header.pduLength = RDPGFX_HEADER_SIZE + 2;

//     guac_client* client = callback->client;

// 	for (index = 0; index < pdu->capsSetCount; index++)
// 	{
// 		capsSet = &(pdu->capsSets[index]);
// 		header.pduLength += RDPGFX_CAPSET_INT_BASE_SIZE + capsSet->length;
// 	}

//     guac_client_log(client, GUAC_LOG_DEBUG,
//         "SendCapsAdvertisePdu Number of caps %x", pdu->capsSetCount);
            
// 	s = Stream_New(NULL, header.pduLength);

// 	if (!s)
// 	{
//             guac_client_log(client, GUAC_LOG_ERROR,
//         "Stream_NeW Failed");
// 		return CHANNEL_RC_NO_MEMORY;
// 	}

// 	Stream_Write_UINT16(s, header.cmdId);     /* cmdId (2 bytes) */
// 	Stream_Write_UINT16(s, header.flags);     /* flags (2 bytes) */
// 	Stream_Write_UINT32(s, header.pduLength); /* pduLength (4 bytes) */
    
// 	/* RDPGFX_CAPS_ADVERTISE_PDU */
// 	Stream_Write_UINT16(s, pdu->capsSetCount); /* capsSetCount (2 bytes) */

// 	for (index = 0; index < pdu->capsSetCount; index++)
// 	{
// 		capsSet = &(pdu->capsSets[index]);
// 		Stream_Write_UINT32(s, capsSet->version); /* version (4 bytes) */
// 		Stream_Write_UINT32(s, capsSet->length);  /* capsDataLength (4 bytes) */
// 		Stream_Write_UINT32(s, capsSet->flags);   /* capsData (4 bytes) */
// 		Stream_Zero(s, capsSet->length - 4);
// 	}

// 	Stream_SealLength(s);
// 	error = callback->channel->Write(callback->channel, (UINT32)Stream_Length(s), Stream_Buffer(s),
// 	                                 NULL);
// 	Stream_Free(s, TRUE);
// 	return error;
// }


/**
 * Function description
 *
 * @return 0 on success, otherwise a Win32 error code
 */
// static UINT rdpgfx_send_supported_caps(guac_rdp_gfx_channel_callback* callback)
// {
// 	RDPGFX_CAPSET_INT* capsSet;
// 	RDPGFX_CAPSET_INT capsSets[10] = { 0 };
// 	RDPGFX_CAPS_INT_ADVERTISE_PDU pdu;

// 	if (!callback)
// 		return ERROR_BAD_ARGUMENTS;

// 	pdu.capsSetCount = 0;
// 	pdu.capsSets = (RDPGFX_CAPSET_INT*)capsSets;

//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_8;
//     capsSet->length = 4;
//     capsSet->flags = 0;

//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_81;
//     capsSet->length = 4;
//     capsSet->flags = 0;

//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_10;
//     capsSet->length = 4;
//     capsSet->flags = 0;

//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_101;
//     capsSet->length = 0x10;
//     capsSet->flags = 0;

//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_102;
//     capsSet->length = 0x4;
//     capsSet->flags = 0;

//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_103;
//     capsSet->length = 0x4;
//     capsSet->flags = 0;


//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_104;
//     capsSet->length = 0x4;
//     capsSet->flags = 0;

//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_105;
//     capsSet->length = 0x4;
//     capsSet->flags = 0;


//     capsSet = &capsSets[pdu.capsSetCount++];
//     capsSet->version = RDPGFX_CAPVERSION_106;
//     capsSet->length = 0x4;
//     capsSet->flags = 0;

//     return rdpgfx_send_caps_advertise_pdu(callback, &pdu);
// }


/**
 * Callback which is invoked when a connection to the GRAPHICS_INPUT plugin is
 * closed.
 *
 * @param channel_callback
 *     The IWTSVirtualChannelCallback structure to which this callback was
 *     originally assigned.
 *
 * @return
 *     Always zero.
 */
static UINT guac_rdp_gfx_close(IWTSVirtualChannelCallback* channel_callback) {

    guac_rdp_gfx_channel_callback* ai_channel_callback =
        (guac_rdp_gfx_channel_callback*) channel_callback;

    guac_client* client = ai_channel_callback->client;

    guac_rdp_client* rdp_client = (guac_rdp_client*) client->data;

    if (ai_channel_callback->graphics_stream) {
        guac_protocol_send_end(client->socket, ai_channel_callback->graphics_stream);

        /* Destroy stream */
        guac_client_free_stream(client, ai_channel_callback->graphics_stream);
        ai_channel_callback->graphics_stream = NULL;
    }


    /* Log closure of GRAPHICS_INPUT channel */
    guac_client_log(client, GUAC_LOG_INFO,
            "GRAPHICS_INPUT channel connection closed");

    guac_rdp_pipe_svc_process_terminate(&ai_channel_callback->svc);
    
    rdp_client->gfx_channel = NULL;

    free(ai_channel_callback);
    return CHANNEL_RC_OK;

}


/**
 * Function description
 *
 * @return 0 on success, otherwise a Win32 error code
 */
static UINT guac_rdp_gfx_open(IWTSVirtualChannelCallback* channel_callback)
{
    guac_rdp_gfx_channel_callback* ai_channel_callback =
        (guac_rdp_gfx_channel_callback*) channel_callback;

    guac_client* client = ai_channel_callback->client;
    guac_rdp_client* rdp_client = (guac_rdp_client*) client->data;

    rdp_client->gfx_channel = ai_channel_callback->channel;

    /* Log closure of GRAPHICS_INPUT channel */
    guac_client_log(client, GUAC_LOG_INFO,
            "GRAPHICS_INPUT channel on open");
            guac_stream* stream =   guac_client_alloc_stream(client);

    stream->data = ai_channel_callback;
    /* Open new pipe stream */
    guac_protocol_send_pipe(client->socket, stream, "application/binary", "guacgfx");

    ai_channel_callback->graphics_stream = stream;
    return CHANNEL_RC_OK;
}

/**
 * Callback which is invoked when a new connection is received by the
 * GRAPHICS_INPUT plugin. Additional callbacks required to handle received data
 * and closure of the connection must be installed at this point.
 *
 * @param listener_callback
 *     The IWTSListenerCallback structure associated with the GRAPHICS_INPUT
 *     plugin receiving the new connection.
 *
 * @param channel
 *     A reference to the IWTSVirtualChannel instance along which data related
 *     to the GRAPHICS_INPUT channel should be sent.
 *
 * @param data
 *     Absolutely no idea. According to Microsoft's documentation for the
 *     function prototype on which FreeRDP's API appears to be based: "This
 *     parameter is not implemented and is reserved for future use."
 *
 * @param accept
 *     Pointer to a flag which should be set to TRUE if the connection should
 *     be accepted or FALSE otherwise. In the case of FreeRDP, this value
 *     defaults to TRUE, and TRUE absolutely MUST be identically 1 or it will
 *     be interpreted as FALSE.
 *
 * @param channel_callback
 *     A pointer to the location that the new IWTSVirtualChannelCallback
 *     structure containing the required callbacks should be assigned.
 *
 * @return
 *     Always zero.
 */
static UINT guac_rdp_gfx_new_connection(
        IWTSListenerCallback* listener_callback, IWTSVirtualChannel* channel,
        BYTE* data, int* accept,
        IWTSVirtualChannelCallback** channel_callback) {

    guac_rdp_gfx_listener_callback* ai_listener_callback =
        (guac_rdp_gfx_listener_callback*) listener_callback;

    guac_client* client = ai_listener_callback->client;;
    /* Log new GRAPHICS_INPUT connection */
    guac_client_log(client, GUAC_LOG_INFO,
            "New GRAPHICS_INPUT channel connection");


    /* Allocate new channel callback */
    guac_rdp_gfx_channel_callback* ai_channel_callback =
        calloc(1, sizeof(guac_rdp_gfx_channel_callback));

    /* Init listener callback with data from plugin */
    ai_channel_callback->client = client;
    ai_channel_callback->channel = channel;
    ai_channel_callback->parent.OnDataReceived = guac_rdp_gfx_data;
    ai_channel_callback->parent.OnOpen = guac_rdp_gfx_open;
    ai_channel_callback->parent.OnClose = guac_rdp_gfx_close;

    /* Return callback through pointer */
    *channel_callback = (IWTSVirtualChannelCallback*) ai_channel_callback;

    return CHANNEL_RC_OK;

}

/**
 * Callback which is invoked when the GRAPHICS_INPUT plugin has been loaded and
 * needs to be initialized with other callbacks and data.
 *
 * @param plugin
 *     The GRAPHICS_INPUT plugin that needs to be initialied.
 *
 * @param manager
 *     The IWTSVirtualChannelManager instance with which the GRAPHICS_INPUT plugin
 *     must be registered.
 *
 * @return
 *     Always zero.
 */
static UINT guac_rdp_gfx_initialize(IWTSPlugin* plugin,
        IWTSVirtualChannelManager* manager) {

    
    /* Allocate new listener callback */
    guac_rdp_gfx_listener_callback* ai_listener_callback =
        calloc(1, sizeof(guac_rdp_gfx_listener_callback));

    /* Ensure listener callback is freed when plugin is terminated */
    guac_rdp_gfx_plugin* ai_plugin = (guac_rdp_gfx_plugin*) plugin;
    ai_plugin->listener_callback = ai_listener_callback;
    guac_client_log(ai_plugin->client, GUAC_LOG_INFO, "GFX Initialize.");

    /* Init listener callback with data from plugin */
    ai_listener_callback->client = ai_plugin->client;
    ai_listener_callback->parent.OnNewChannelConnection =
        guac_rdp_gfx_new_connection;

    /* Register listener for "GRAPHICS_INPUT" channel */
    manager->CreateListener(manager, RDPGFX_DVC_CHANNEL_NAME, 0,
            (IWTSListenerCallback*) ai_listener_callback, NULL);

    return CHANNEL_RC_OK;

}

/**
 * Callback which is invoked when all connections to the GRAPHICS_INPUT plugin
 * have closed and the plugin is being unloaded.
 *
 * @param plugin
 *     The GRAPHICS_INPUT plugin being unloaded.
 *
 * @return
 *     Always zero.
 */
static UINT guac_rdp_gfx_terminated(IWTSPlugin* plugin) {

    guac_rdp_gfx_plugin* ai_plugin = (guac_rdp_gfx_plugin*) plugin;
    guac_client* client = ai_plugin->client;

    /* Free all non-FreeRDP data */
    free(ai_plugin->listener_callback);
    free(ai_plugin);

    guac_client_log(client, GUAC_LOG_INFO, "GRAPHICS_INPUT plugin unloaded.");
    return CHANNEL_RC_OK;

}

/**
 * Entry point for GRAPHICS_INPUT dynamic virtual channel.
 */
int DVCPluginEntry(IDRDYNVC_ENTRY_POINTS* pEntryPoints) {

    /* Pull guac_client from arguments */
    ADDIN_ARGV* args = pEntryPoints->GetPluginData(pEntryPoints);
    guac_client* client = (guac_client*) guac_rdp_string_to_ptr(args->argv[1]);

    /* Pull previously-allocated plugin */
    guac_rdp_gfx_plugin* ai_plugin = (guac_rdp_gfx_plugin*)
        pEntryPoints->GetPlugin(pEntryPoints, "guacgfx");

    guac_client_log(client, GUAC_LOG_INFO, "Trying to initialite gfx");

    /* If no such plugin allocated, allocate and register it now */
    if (ai_plugin == NULL) {

        /* Init plugin callbacks and data */
        ai_plugin = calloc(1, sizeof(guac_rdp_gfx_plugin));
        ai_plugin->parent.Initialize = guac_rdp_gfx_initialize;
        ai_plugin->parent.Terminated = guac_rdp_gfx_terminated;
        ai_plugin->client = client;

        /* Register plugin as "guacai" for later retrieval */
        pEntryPoints->RegisterPlugin(pEntryPoints, "guacgfx",
                (IWTSPlugin*) ai_plugin);

        guac_client_log(client, GUAC_LOG_INFO, "GFX plugin loaded.");
    }

    return CHANNEL_RC_OK;

}

