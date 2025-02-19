#ifndef ANOTC_CONFIG_FRAME_H
#define ANOTC_CONFIG_FRAME_H

#include <QMap>
#include <QMetaType>

#define ANOTC_CONFIG_FRAME_CMD_DEVICE_INFO 0x00
#define ANOTC_CONFIG_FRAME_CMD_READ_COUNT 0x01
#define ANOTC_CONFIG_FRAME_CMD_READ_VALUE 0x02
#define ANOTC_CONFIG_FRAME_CMD_READ_INFO 0x03
#define ANOTC_CONFIG_FRAME_CMD_0X10 0x10

#define ANOTC_CONFIG_FRAME_VAL_RESET_PARAM 0xAA
#define ANOTC_CONFIG_FRAME_VAL_SAVE_PARAM 0xAB

enum ANOTC_CONFIG_INFO_PAR_ID {
    ANOTC_CONFIG_PAR_WIFI_NAME,
    ANOTC_CONFIG_PAR_WIFI_PASSWORD,
    ANOTC_CONFIG_PAR_WIFI_UDP_PORT,
    ANOTC_CONFIG_PAR_ACCEL_K_X,
    ANOTC_CONFIG_PAR_ACCEL_K_Y,
    ANOTC_CONFIG_PAR_ACCEL_K_Z,
    ANOTC_CONFIG_PAR_ACCEL_OFFSET_X,
    ANOTC_CONFIG_PAR_ACCEL_OFFSET_Y,
    ANOTC_CONFIG_PAR_ACCEL_OFFSET_Z,
    ANOTC_CONFIG_PAR_GYRO_OFFSET_X,
    ANOTC_CONFIG_PAR_GYRO_OFFSET_Y,
    ANOTC_CONFIG_PAR_GYRO_OFFSET_Z,
    ANOTC_CONFIG_PAR_ESC_PROTOCOL,
    ANOTC_CONFIG_PAR_MOTOR_MAPPING
};

struct anotc_p_value {
    QString name;
    unsigned char type;
    union {
        uint8_t uint8;
        int8_t int8;
        uint16_t uint16;
        int16_t int16;
    } value;
    QString string;
};

struct anotc_parsed_parameter_frame {
    qint64 timestamp;
    unsigned char func;
    QList<struct anotc_p_value> frame_value;
};
Q_DECLARE_METATYPE(struct anotc_parsed_parameter_frame);

void anotc_send_config_get_device_info();
void anotc_send_config_get_count();
void anotc_send_config_get_param_value(unsigned short par_id);
void anotc_send_config_get_param_info(unsigned short par_id);
void anotc_send_config_reset_param();
void anotc_send_config_save_param();
void anotc_send_config_set_param(unsigned char par_id, unsigned char type, unsigned char *value);

int anotc_parse_config_frame(union _un_anotc_v8_frame *frame, QList<struct anotc_p_value> *frame_value);

#endif // ANOTC_CONFIG_FRAME_H
