#ifndef ANOTC_RECEIVE_CHECK_H
#define ANOTC_RECEIVE_CHECK_H

#include "Anotc/anotc.h"

struct anotc_parsed_check_frame {
    qint64 timestamp;
    unsigned char func;
    unsigned char code;
    QString msg;
};
Q_DECLARE_METATYPE(struct anotc_parsed_check_frame);

int anotc_parse_check_frame(union _un_anotc_v8_frame *frame, struct anotc_parsed_check_frame *frame_value);

int check_recv_device_info(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
int check_recv_param_count(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
int check_recv_param_value(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
int check_recv_param_info(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
int check_recv_param_set_value(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
#endif // ANOTC_RECEIVE_CHECK_H
