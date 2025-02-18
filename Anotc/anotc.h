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

#define ANOTC_FRAME_FRAME_CHECK 0x0
#define ANOTC_FRAME_IMU 0x1
#define ANOTC_FRAME_MAG 0x2
#define ANOTC_FRAME_EULER 0x3
#define ANOTC_FRAME_QUAT 0x4
#define ANOTC_FRAME_ALT 0x5
#define ANOTC_FRAME_TARGET_ATTITUDE 0xA
#define ANOTC_FRAME_BATTERY 0xD

#define ANOTC_FRAME_PWM 0x20

#define ANOTC_FRAME_GPS 0x30

#define ANOTC_FRAME_RC 0x40

#define ANOTC_FRAME_CMD_SEND 0xC0

#define ANOTC_FRAME_CONFIG_CMD 0xE0
#define ANOTC_FRAME_CONFIG_READ_WRITE 0xE1
#define ANOTC_FRAME_CONFIG_INFO 0xE2
#define ANOTC_FRAME_DEVICE_INFO 0xE3

#define ANOTC_FRAME_LOG_STRING 0xA0
#define ANOTC_FRAME_LOG_STRING_NUM 0xA1

#define ANOTC_FRAME_CUSTOM_SYSTEM_INFO 0xF1

struct anotc_frame {
    unsigned char head;
    unsigned char saddr;
    unsigned char daddr;
    unsigned char fun;
    unsigned short len;
    unsigned char data[ANOTC_DATA_MAX_SIZE];
}__attribute__ ((__packed__));

union _un_anotc_v8_frame{
    struct anotc_frame frame;
    unsigned char rawBytes[sizeof(struct anotc_frame)];
};

Q_DECLARE_METATYPE(union _un_anotc_v8_frame);

struct anotc_blocking_queue_item {
    qint64 timestamp;
    union _un_anotc_v8_frame frame;
};
Q_DECLARE_METATYPE(struct anotc_blocking_queue_item);

#define PREPARE_ANOTC_FRAME(frame) { \
frame.head = ANOTC_V8_HEAD;\
    frame.saddr = 0xFE;\
    frame.daddr = 0x01;\
    frame.len = 0;\
}

static inline void anotc_add_float(struct anotc_frame *frame, float data)
{
    *(float*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(float);
}

static inline void anotc_add_double(struct anotc_frame *frame, double data)
{
    *(double*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(double);
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

static inline void anotc_add_ulong(struct anotc_frame *frame, uint64_t data)
{
    *(uint64_t*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(uint64_t);
}

static inline void anotc_add_long(struct anotc_frame *frame, int64_t data)
{
    *(int64_t*)(&(frame->data[frame->len])) = data;
    frame->len += sizeof(int64_t);
}

static inline void anotc_add_string(struct anotc_frame *frame, char *data)
{
    strncpy((char *)&(frame->data[frame->len]), data, strlen(data));
    frame->len += strlen(data);
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

extern BlockingQueue<struct anotc_blocking_queue_item> anotc_queue;
extern void (*anotc_send_func)(unsigned char *data, int len);
#endif // ANOTC_H
