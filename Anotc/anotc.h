#ifndef ANOTC_H
#define ANOTC_H

#include <stdint.h>
#include <QByteArray>
#include <QList>
#include <QMetaType>
#include "Anotc/blockingqueue.h"

#define ANOTC_DATA_MAX_SIZE 512
#define ANOTC_V8_HEAD_SIZE 6
#define ANOTC_V8_HEAD 0xAB

#define ANOTC_V8_FRAME_SUMCHECK_INDEX 5
#define ANOTC_V8_FRAME_ADDCHECK_INDEX 6

#define ANOTC_FRAME_IMU 0x1;
#define ANOTC_FRAME_MAG_BAR_TEMP 0x2
#define ANOTC_FRAME_EULER 0x3
#define ANOTC_FRAME_ALT 0x5
#define ANOTC_FRAME_TARGET_ATTITUDE 0xA
#define ANOTC_FRAME_BATTERY 0xD

#define ANOTC_FRAME_GPS 0x30

#define ANOTC_FRAME_RC 0x40

#define ANOTC_FRAME_LOG_STRING 0xA0
#define ANOTC_FRAME_LOG_STRING_NUM 0xA1

struct anotc_frame {
    unsigned char head;
    unsigned char s_addr;
    unsigned char d_addr;
    unsigned char fun;
    unsigned short len;
    unsigned char data[ANOTC_DATA_MAX_SIZE];
}__attribute__ ((__packed__));

union _un_anotc_v8_frame{
    struct anotc_frame frame;
    unsigned char rawBytes[sizeof(struct anotc_frame)];
};

Q_DECLARE_METATYPE(union _un_anotc_v8_frame);

#define PREPARE_ANOTC_FRAME(frame) { \
frame.head = ANOTC_V8_HEAD;\
    frame.s_addr = 0xAF;\
    frame.d_addr = 0x01;\
    frame.len = 0;\
}

static inline void anotc_add_float(struct anotc_frame *frame, float data)
{
    *(float*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(float);
}

static inline void anotc_add_short(struct anotc_frame *frame, short data)
{
    *(short int*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(short int);
}

static inline void anotc_add_ushort(struct anotc_frame *frame, unsigned short data)
{
    *(unsigned short int*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(unsigned short int);
}

static inline void anotc_add_int(struct anotc_frame *frame, int data)
{
    *(int*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(int);
}

static inline void anotc_add_uint(struct anotc_frame *frame, unsigned int data)
{
    *(unsigned int*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(unsigned int);
}

static inline void anotc_add_checksum(struct anotc_frame *frame)
{
    frame->data[frame->len] = 0;
    frame->data[frame->len + 1] = 0;
    for (int i = 0; i < ANOTC_V8_HEAD_SIZE + frame->len; i++)
    {
        frame->data[frame->len]+=((unsigned char *)frame)[i];
        frame->data[frame->len+1] += frame->data[frame->len];
    }
}

void anotc_parse_data(QByteArray *data);
void anotc_reset();
unsigned long anotc_receive_count();
unsigned int anotc_receive_error_count();
unsigned int anotc_receive_exceed_count();

extern BlockingQueue anotc_queue;
#endif // ANOTC_H
