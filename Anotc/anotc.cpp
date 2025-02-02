#include "anotc.h"
#include <QDateTime>

enum anotc_decode_status
{
    HEAD,
    SRC_ADDR,
    DST_ADDR,
    FUNC,
    LEN1,
    LEN2,
    READ_DATA,
    SUM_CHECK,
    ADD_CHECK
};

struct anotc_decode_data
{
    enum anotc_decode_status status;
    unsigned short frame_index;
    unsigned short frame_read_count;
    unsigned char sum_check;
    unsigned char add_check;
    union _un_anotc_v8_frame frame;

    unsigned long receive_count;
    unsigned int receive_error_count;
    unsigned int receive_exceed_count;
};

static struct anotc_decode_data _decode_data = {
    .status = HEAD,
    .frame_index = 0,
    .frame_read_count = 0,
    .receive_count = 0,
    .receive_error_count = 0,
    .receive_exceed_count = 0
};

BlockingQueue<struct anotc_blocking_queue_item> anotc_queue;

static inline int _sum_check(union _un_anotc_v8_frame *frame, unsigned char sum_check, unsigned char add_check);

void anotc_parse_data(QByteArray *data)
{
    struct anotc_blocking_queue_item item;
    for (int i = 0; i < data->size(); i++)
    {
        if (_decode_data.status==HEAD) {
            if ((unsigned char)data->at(i)!=ANOTC_V8_HEAD) continue; //找帧头
            _decode_data.frame.rawBytes[_decode_data.frame_index++] = ANOTC_V8_HEAD;
            _decode_data.status = SRC_ADDR;
        } else if (_decode_data.status==SRC_ADDR) { //源地址
            _decode_data.frame.rawBytes[_decode_data.frame_index++] = (unsigned char)data->at(i);
            _decode_data.status = DST_ADDR;
        } else if (_decode_data.status==DST_ADDR) { //目标地址
            _decode_data.frame.rawBytes[_decode_data.frame_index++] = (unsigned char)data->at(i);
            _decode_data.status = FUNC;
        } else if (_decode_data.status == FUNC) { //func
            _decode_data.frame.rawBytes[_decode_data.frame_index++] = (unsigned char)data->at(i);
            _decode_data.status = LEN1;
        } else if (_decode_data.status == LEN1) {
            _decode_data.frame.rawBytes[_decode_data.frame_index++] = (unsigned char)data->at(i);
            _decode_data.status = LEN2;
        } else if (_decode_data.status == LEN2) {
            _decode_data.frame.rawBytes[_decode_data.frame_index++] = (unsigned char)data->at(i);
            _decode_data.status = READ_DATA;
        } else if (_decode_data.status == READ_DATA) { //read data
            if (_decode_data.frame_read_count<ANOTC_DATA_MAX_SIZE) {
                _decode_data.frame.rawBytes[_decode_data.frame_index++] = (unsigned char)data->at(i);
            }
            _decode_data.frame_read_count++;
            if (_decode_data.frame_read_count==_decode_data.frame.frame.len) {
                _decode_data.status = SUM_CHECK;
            }
        } else if (_decode_data.status==SUM_CHECK) { //sc
            _decode_data.sum_check = (unsigned char)data->at(i);
            _decode_data.status = ADD_CHECK;
        } else if (_decode_data.status==ADD_CHECK) { //ac
            _decode_data.add_check = (unsigned char)data->at(i);
            _decode_data.receive_count++;

            if (_decode_data.frame_read_count>ANOTC_DATA_MAX_SIZE) {
                _decode_data.receive_exceed_count++;
            } else {
                if (_sum_check(&_decode_data.frame, _decode_data.sum_check, _decode_data.add_check))
                {
                    item.frame = _decode_data.frame;
                    item.timestamp = QDateTime::currentMSecsSinceEpoch();
                    anotc_queue.put(item);
                } else {
                    _decode_data.receive_error_count++;
                }
            }

            anotc_reset();
        }
    }
}

void anotc_reset()
{
    _decode_data.status = HEAD;
    _decode_data.frame_index = 0;
    _decode_data.frame_read_count = 0;
}

static inline int _sum_check(union _un_anotc_v8_frame *frame, unsigned char sum_check, unsigned char add_check)
{
    unsigned char s = 0;
    unsigned char a = 0;
    for (int i=0; i<ANOTC_V8_HEAD_SIZE+frame->frame.len; i++) {
        s+=frame->rawBytes[i];
        a+=s;
    }
    if (s == sum_check && a == add_check)
    {
        return 1;
    }
    else
    {
        qDebug("bad checksum, expected:0x%x 0x%x, actual:0x%x 0x%x\n", s, a, sum_check, add_check);
        return 0;
    }
}

unsigned long anotc_receive_count()
{
    return _decode_data.receive_count;
}

unsigned int anotc_receive_error_count()
{
    return _decode_data.receive_error_count;
}

unsigned int anotc_receive_exceed_count()
{
    return _decode_data.receive_exceed_count;
}

void (*anotc_send_func)(unsigned char *data, int len) = 0;
