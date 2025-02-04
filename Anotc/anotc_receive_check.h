#ifndef ANOTC_RECEIVE_CHECK_H
#define ANOTC_RECEIVE_CHECK_H

#include "Anotc/anotc.h"

int check_recv_device_info(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
int check_recv_param_count(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
int check_recv_param_value(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
int check_recv_param_info(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
#endif // ANOTC_RECEIVE_CHECK_H
