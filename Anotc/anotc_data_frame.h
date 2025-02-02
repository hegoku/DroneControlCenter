#ifndef ANOTC_DATA_FRAME_H
#define ANOTC_DATA_FRAME_H

#include <QString>
#include <QList>
#include <QMap>
#include <QMetaType>

struct anotc_frame_param {
    QString name;
    QString desc;
    QString type;
};

struct anotc_frame_defination {
    unsigned char func;
    QString name;
    QString desc;
    QList<anotc_frame_param*> params;
    void (*formater)(QList<struct anotc_value> *frame_value);
};

struct anotc_value {
    QString name;
    unsigned char type;
    union {
        uint8_t uint8;
        int8_t int8;
        uint16_t uint16;
        int16_t int16;
        uint32_t uint32;
        int32_t int32;
        uint64_t uint64;
        int64_t int64;
        float f;
        double d;
    } value;
};

struct anotc_parsed_data_frame {
    qint64 timestamp;
    unsigned char func;
    QList<struct anotc_value> frame_value;
};
Q_DECLARE_METATYPE(struct anotc_parsed_data_frame);

void loadDataFrameDefination(QString path);
int anotc_parse_data_frame(union _un_anotc_v8_frame *frame, QList<struct anotc_value> *frame_value);

void anotc_euler_formater(QList<struct anotc_value> *frame_value);
void anotc_alt_formater(QList<struct anotc_value> *frame_value);

extern QMap<unsigned char, struct anotc_frame_defination*> anotc_frame_defination_list;

#endif // ANOTC_DATA_FRAME_H
