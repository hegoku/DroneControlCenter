#include "Anotc/anotc.h"
#include "Anotc/anotc_cmd_frame.h"


void anotc_send_cmd_calibrate_gyro()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CMD_SEND;

    frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_GYRO>>16;
    frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_GYRO>>8;
    frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_GYRO;
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}
