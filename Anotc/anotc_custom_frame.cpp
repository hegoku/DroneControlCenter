#include "Anotc/anotc_custom_frame.h"

void anotc_send_custom_connect()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_CMD_DEVICE_INFO;
    anotc_add_ushort(&frame, 1);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_custom_disconnect()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_CMD_DEVICE_INFO;
    anotc_add_ushort(&frame, 2);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}
