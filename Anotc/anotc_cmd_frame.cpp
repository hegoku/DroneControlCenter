#include "Anotc/anotc.h"
#include "Anotc/anotc_cmd_frame.h"

static inline void anotc_add_cmd_id(struct anotc_frame *frame, unsigned int cid)
{
    frame->data[frame->len++] = cid>>16;
    frame->data[frame->len++] = cid>>8;
    frame->data[frame->len++] = cid;
}


void anotc_send_cmd_calibrate_gyro()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CMD_SEND;

    anotc_add_cmd_id(&frame, ANOTC_CMD_CALIBRATE_GYRO);
    // frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_GYRO>>16;
    // frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_GYRO>>8;
    // frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_GYRO;
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_cmd_calibrate_accel(char direction)
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CMD_SEND;

    anotc_add_cmd_id(&frame, ANOTC_CMD_CALIBRATE_ACCEL);
    // frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_ACCEL>>16;
    // frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_ACCEL>>8;
    // frame.data[frame.len++] = ANOTC_CMD_CALIBRATE_ACCEL;
    frame.data[frame.len++] = direction;
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_cmd_reboot()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CMD_SEND;

    anotc_add_cmd_id(&frame, ANOTC_CMD_REBOOT);
    // frame.data[frame.len++] = ANOTC_CMD_REBOOT>>16;
    // frame.data[frame.len++] = ANOTC_CMD_REBOOT>>8;
    // frame.data[frame.len++] = ANOTC_CMD_REBOOT;
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_cmd_motor_test_status(unsigned char enable)
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CMD_SEND;

    anotc_add_cmd_id(&frame, ANOTC_CMD_TOGGLE_MOTOR_TEST_STATUS);
    frame.data[frame.len++] = enable;
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_cmd_motor_test_throttle(unsigned short *value, int count)
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CMD_SEND;

    anotc_add_cmd_id(&frame, ANOTC_CMD_MOTOR_TEST_THROLLE);
    for(int i=0;i<count;i++) {
        anotc_add_short(&frame, value[i]);
    }
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}
