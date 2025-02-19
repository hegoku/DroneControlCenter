#include "Anotc/anotc_receive_check.h"
#include "Anotc/anotc_config_frame.h"

int check_recv_device_info(struct anotc_frame *send_frame, struct anotc_frame *recv_frame)
{
    if (send_frame->fun==ANOTC_FRAME_CONFIG_CMD && send_frame->data[0]==ANOTC_CONFIG_FRAME_CMD_DEVICE_INFO) {
        if (recv_frame->fun==ANOTC_FRAME_DEVICE_INFO) {
            return 0;
        }
    }
    return -1;
}

int check_recv_param_count(struct anotc_frame *send_frame, struct anotc_frame *recv_frame)
{
    if (send_frame->fun==ANOTC_FRAME_CONFIG_CMD && send_frame->data[0]==ANOTC_CONFIG_FRAME_CMD_READ_COUNT) {
        if (recv_frame->fun==ANOTC_FRAME_CONFIG_CMD && recv_frame->data[0]==ANOTC_CONFIG_FRAME_CMD_READ_COUNT) {
            return 0;
        }
    }
    return -1;
}

int check_recv_param_value(struct anotc_frame *send_frame, struct anotc_frame *recv_frame)
{
    if (send_frame->fun==ANOTC_FRAME_CONFIG_CMD && send_frame->data[0]==ANOTC_CONFIG_FRAME_CMD_READ_VALUE) {
        if (recv_frame->fun==ANOTC_FRAME_CONFIG_READ_WRITE) {
            unsigned short send_par_id = send_frame->data[1] | (((unsigned short)send_frame->data[2])<<8);
            unsigned short recv_par_id = recv_frame->data[0] | (((unsigned short)recv_frame->data[1])<<8);
            if (send_par_id==recv_par_id) {
                return 0;
            }
        }
    }
    return -1;
}

int check_recv_param_info(struct anotc_frame *send_frame, struct anotc_frame *recv_frame)
{
    if (send_frame->fun==ANOTC_FRAME_CONFIG_CMD && send_frame->data[0]==ANOTC_CONFIG_FRAME_CMD_READ_INFO) {
        if (recv_frame->fun==ANOTC_FRAME_CONFIG_INFO) {
            unsigned short send_par_id = send_frame->data[1] | (((unsigned short)send_frame->data[2])<<8);
            unsigned short recv_par_id = recv_frame->data[0] | (((unsigned short)recv_frame->data[1])<<8);
            if (send_par_id==recv_par_id) {
                return 0;
            }
        }
    }
    return -1;
}

int check_recv_param_set_value(struct anotc_frame *send_frame, struct anotc_frame *recv_frame)
{
    if (send_frame->fun==ANOTC_FRAME_CONFIG_READ_WRITE) {
        if (recv_frame->fun==ANOTC_FRAME_FRAME_CHECK && send_frame->fun==recv_frame->data[0]) {
            return 0;
        }
    }
    return -1;
}

int anotc_parse_check_frame(union _un_anotc_v8_frame *frame, struct anotc_parsed_check_frame *frame_value)
{
    frame_value->func = frame->frame.data[0];
    frame_value->code = frame->frame.data[3];
    if (frame_value->code) {
        frame_value->msg = QString::fromLocal8Bit((char*)(frame->frame.data+4), frame->frame.len-4);
    }
}
