#include "Anotc/anotc.h"
#include "Anotc/anotc_config_frame.h"

void anotc_send_config_get_device_info()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_CMD_DEVICE_INFO;
    anotc_add_ushort(&frame, 0);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_config_get_count()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_CMD_READ_COUNT;
    anotc_add_ushort(&frame, 0);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_config_get_param_value(unsigned short par_id)
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_CMD_READ_VALUE;
    anotc_add_ushort(&frame, par_id);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_config_get_param_info(unsigned short par_id)
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_CMD_READ_INFO;
    anotc_add_ushort(&frame, par_id);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_config_reset_param()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_VAL_RESET_PARAM;
    anotc_add_ushort(&frame, ANOTC_CONFIG_FRAME_VAL_SAVE_PARAM);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_config_save_param()
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_CMD;

    frame.data[frame.len++] = ANOTC_CONFIG_FRAME_CMD_0X10;
    anotc_add_ushort(&frame, ANOTC_CONFIG_FRAME_VAL_SAVE_PARAM);
    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

void anotc_send_config_set_param(unsigned char par_id, unsigned char type, unsigned char *value)
{
    struct anotc_frame frame;
    PREPARE_ANOTC_FRAME(frame);
    frame.fun = ANOTC_FRAME_CONFIG_READ_WRITE;

    anotc_add_ushort(&frame, par_id);
    switch(type) {
    case 0:
    case 1:
        frame.data[frame.len++] = value[0];
        break;
    case 2:
        anotc_add_ushort(&frame, *((unsigned short*)value));
        break;
    case 3:
        anotc_add_short(&frame, *((short*)value));
        break;
    case 4:
        anotc_add_uint(&frame, *((unsigned int*)value));
        break;
    case 5:
        anotc_add_int(&frame, *((int*)value));
        break;
    case 6:
        anotc_add_ulong(&frame, *((uint64_t*)value));
        break;
    case 7:
        anotc_add_long(&frame, *((int64_t*)value));
        break;
    case 8:
        anotc_add_float(&frame, *((float*)value));
        break;
    case 9:
        anotc_add_double(&frame, *((double*)value));
        break;
    case 10:
        anotc_add_string(&frame, (char*)value);
        break;
    }

    anotc_add_checksum(&frame);
    anotc_send_func((unsigned char *)&frame, ANOTC_V8_HEAD_SIZE + frame.len + 2);
}

int anotc_parse_config_frame(union _un_anotc_v8_frame *frame, QList<struct anotc_p_value> *frame_value)
{
    struct anotc_p_value value;
    value.string.clear();
    switch (frame->frame.fun) {
    case ANOTC_FRAME_CONFIG_CMD:{
        value.value.uint8 = frame->frame.data[0];
        value.type = 0;
        value.name = QString("CMD");
        value.string.clear();
        frame_value->append(value);

        value.value.uint16 = frame->frame.data[1] | (((unsigned short)frame->frame.data[2])<<8);
        value.type = 2;
        value.name = QString("VAL");
        frame_value->append(value);
        break;
    }
    case ANOTC_FRAME_CONFIG_READ_WRITE:{
        value.value.uint16 = frame->frame.data[0] | (((unsigned short)frame->frame.data[1])<<8);
        value.type = 2;
        value.name = QString("PAR_ID");
        value.string.clear();
        frame_value->append(value);

        value.type = 10;
        value.string.clear();
        value.name = QString("PAR_VAL");
        value.string = QString(QLatin1String((char*)(frame->frame.data+2), frame->frame.len-2));
        frame_value->append(value);
        break;
    }
    case ANOTC_FRAME_CONFIG_INFO:{
        value.value.uint16 = frame->frame.data[0] | (((unsigned short)frame->frame.data[1])<<8);
        value.type = 2;
        value.name = QString("PAR_ID");
        value.string.clear();
        frame_value->append(value);

        value.value.uint8 = frame->frame.data[2];
        value.type = 0;
        value.name = QString("PAR_TYPE");
        frame_value->append(value);

        value.type = 10;
        value.name = QString("PAR_NAME");
        for (int i=0;i<20;i++) {
            if (frame->frame.data[3+i]!=0) {
                value.string.append(QChar(frame->frame.data[3+i]));
            }
        }
        frame_value->append(value);

        value.type = 10;
        value.string.clear();
        value.name = QString("PAR_INFO");
        value.string = QString::fromLocal8Bit((char*)(frame->frame.data+3+20), frame->frame.len-3-20);
        frame_value->append(value);
        break;
    }
    case ANOTC_FRAME_DEVICE_INFO:{
        value.value.uint8 = frame->frame.data[0];
        value.type = 0;
        value.name = QString("DEV_ID");
        value.string.clear();
        frame_value->append(value);

        value.value.int16 = frame->frame.data[1] | (((short)frame->frame.data[2])<<8);
        value.type = 3;
        value.name = QString("HW_VER");
        frame_value->append(value);

        value.value.int16 = frame->frame.data[3] | (((short)frame->frame.data[4])<<8);
        value.type = 3;
        value.name = QString("SW_VER");
        frame_value->append(value);

        value.value.int16 = frame->frame.data[5] | (((short)frame->frame.data[6])<<8);
        value.type = 3;
        value.name = QString("BL_VER");
        frame_value->append(value);

        value.value.int16 = frame->frame.data[7] | (((short)frame->frame.data[8])<<8);
        value.type = 3;
        value.name = QString("PT_VER");
        frame_value->append(value);

        value.type = 10;
        value.name = QString("DEV_NAME");
        value.string = QString::fromLocal8Bit((char*)(frame->frame.data+9), frame->frame.len-9);
        frame_value->append(value);
        break;
    }
    default:
        return -1;
        break;
    }
    return 0;
}
