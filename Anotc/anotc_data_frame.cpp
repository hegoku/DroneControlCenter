#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHash>
#include "Anotc/anotc.h"
#include "Anotc/anotc_data_frame.h"

QMap<unsigned char, struct anotc_frame_defination*> anotc_frame_defination_list;

unsigned char mappingParamType(QString type)
{
    if (type.compare(QString::fromLocal8Bit("UInt8"))==0) {
        return 0;
    } else if (type.compare(QString::fromLocal8Bit("Int8"))==0) {
        return 1;
    } else if (type.compare(QString::fromLocal8Bit("UInt16"))==0) {
        return 2;
    } else if (type.compare(QString::fromLocal8Bit("Int16"))==0) {
        return 3;
    } else if (type.compare(QString::fromLocal8Bit("UInt32"))==0) {
        return 4;
    } else if (type.compare(QString::fromLocal8Bit("Int32"))==0) {
        return 5;
    } else if (type.compare(QString::fromLocal8Bit("UInt64"))==0) {
        return 6;
    } else if (type.compare(QString::fromLocal8Bit("Int64"))==0) {
        return 7;
    } else if (type.compare(QString::fromLocal8Bit("Float"))==0) {
        return 8;
    } else if (type.compare(QString::fromLocal8Bit("Double"))==0) {
        return 9;
    } else if (type.compare(QString::fromLocal8Bit("String"))==0) {
        return 10;
    }
}

void loadDataFrameDefination(QString path)
{
    QString val;
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8(), &errorPtr);
    if (doc.isNull()) {
        qDebug() << errorPtr.errorString();
    }

    QJsonArray root = doc.array();

    struct anotc_frame_defination *de;
    struct anotc_frame_param *p;
    for (int i=0;i<root.size();i++) {
        QJsonObject func = root.at(i).toObject();
        de = (struct anotc_frame_defination*)malloc(sizeof(struct anotc_frame_defination));
        de->func = (unsigned char)func.value("id").toInt();
        de->name = func.value("name").toString();
        de->desc = func.value("desc").toString();
        de->formater = NULL;
        QJsonArray params = func.value("params").toArray();
        for (int j=0;j<params.size();j++) {
            QJsonObject param = params.at(j).toObject();
            p = (struct anotc_frame_param*)malloc(sizeof(struct anotc_frame_param));
            p->name = param.value("name").toString();
            p->desc = param.value("desc").toString();
            p->type = param.value("type").toString();
            de->params.append(p);
        }
        anotc_frame_defination_list.insert(de->func, de);
    }

    anotc_frame_defination_list.value(ANOTC_FRAME_EULER)->formater = anotc_euler_formater;
    anotc_frame_defination_list.value(ANOTC_FRAME_ALT)->formater = anotc_alt_formater;
}

int anotc_parse_data_frame(union _un_anotc_v8_frame *frame, QList<struct anotc_value> *frame_value)
{
    if (!anotc_frame_defination_list.contains(frame->frame.fun)) {
        return -1;
    }
    struct anotc_frame_defination *d = anotc_frame_defination_list.value(frame->frame.fun);
    unsigned int index = 0;
    for (int i=0;i<d->params.size();i++) {
        struct anotc_frame_param *param = d->params.at(i);
        struct anotc_value value;
        value.name = param->name;
        if (index+1>frame->frame.len) break;
        if (param->type.compare(QString::fromLocal8Bit("UInt8"))==0) {
            value.type = 0;
            value.value.uint8 = frame->frame.data[index++];
        } else if (param->type.compare(QString::fromLocal8Bit("Int8"))==0) {
            value.type = 1;
            value.value.int8 = (char)frame->frame.data[index++];
        } else if (param->type.compare(QString::fromLocal8Bit("UInt16"))==0) {
            value.type = 2;
            value.value.uint16 = frame->frame.data[index++];
            if (index+1>frame->frame.len) break;
            value.value.uint16 |= ((unsigned short)frame->frame.data[index++])<<8;
        } else if (param->type.compare(QString::fromLocal8Bit("Int16"))==0) {
            value.type = 3;
            value.value.int16 = frame->frame.data[index++];
            if (index+1>frame->frame.len) break;
            value.value.int16 |= ((short)frame->frame.data[index++])<<8;
        } else if (param->type.compare(QString::fromLocal8Bit("UInt32"))==0) {
            value.type = 4;
            value.value.uint32 = frame->frame.data[index++];
            if (index+1>frame->frame.len) break;
            value.value.uint32 |= ((unsigned int)frame->frame.data[index++])<<8;
            if (index+1>frame->frame.len) break;
            value.value.uint32 |= ((unsigned int)frame->frame.data[index++])<<16;
            if (index+1>frame->frame.len) break;
            value.value.uint32 |= ((unsigned int)frame->frame.data[index++])<<24;
        } else if (param->type.compare(QString::fromLocal8Bit("Int32"))==0) {
            value.type = 5;
            value.value.int32 = frame->frame.data[index++];
            if (index+1>frame->frame.len) break;
            value.value.int32 |= ((int)frame->frame.data[index++])<<8;
            if (index+1>frame->frame.len) break;
            value.value.int32 |= ((int)frame->frame.data[index++])<<16;
            if (index+1>frame->frame.len) break;
            value.value.int32 |= ((int)frame->frame.data[index++])<<24;
        } else if (param->type.compare(QString::fromLocal8Bit("UInt64"))==0) {
            value.type = 6;
            value.value.uint64 = frame->frame.data[index++];
            if (index+1>frame->frame.len) break;
            value.value.uint64 |= ((uint64_t)frame->frame.data[index++])<<8;
            if (index+1>frame->frame.len) break;
            value.value.uint64 |= ((uint64_t)frame->frame.data[index++])<<16;
            if (index+1>frame->frame.len) break;
            value.value.uint64 |= ((uint64_t)frame->frame.data[index++])<<24;
            if (index+1>frame->frame.len) break;
            value.value.uint64 |= ((uint64_t)frame->frame.data[index++])<<32;
            if (index+1>frame->frame.len) break;
            value.value.uint64 |= ((uint64_t)frame->frame.data[index++])<<40;
            if (index+1>frame->frame.len) break;
            value.value.uint64 |= ((uint64_t)frame->frame.data[index++])<<48;
            if (index+1>frame->frame.len) break;
            value.value.uint64 |= ((uint64_t)frame->frame.data[index++])<<56;
        } else if (param->type.compare(QString::fromLocal8Bit("Int64"))==0) {
            value.type = 7;
            value.value.int64 = frame->frame.data[index++];
            if (index+1>frame->frame.len) break;
            value.value.int64 |= ((int64_t)frame->frame.data[index++])<<8;
            if (index+1>frame->frame.len) break;
            value.value.int64 |= ((int64_t)frame->frame.data[index++])<<16;
            if (index+1>frame->frame.len) break;
            value.value.int64 |= ((int64_t)frame->frame.data[index++])<<24;
            if (index+1>frame->frame.len) break;
            value.value.int64 |= ((int64_t)frame->frame.data[index++])<<32;
            if (index+1>frame->frame.len) break;
            value.value.int64 |= ((int64_t)frame->frame.data[index++])<<40;
            if (index+1>frame->frame.len) break;
            value.value.int64 |= ((int64_t)frame->frame.data[index++])<<48;
            if (index+1>frame->frame.len) break;
            value.value.int64 |= ((int64_t)frame->frame.data[index++])<<56;
        } else if (param->type.compare(QString::fromLocal8Bit("Float"))==0) {
            value.type = 8;
            uint32_t tmp = frame->frame.data[index++];
            tmp = frame->frame.data[index++];
            if (index+1>frame->frame.len) break;
            tmp |= ((uint32_t)frame->frame.data[index++])<<8;
            if (index+1>frame->frame.len) break;
            tmp |= ((uint32_t)frame->frame.data[index++])<<16;
            if (index+1>frame->frame.len) break;
            tmp |= ((uint32_t)frame->frame.data[index++])<<24;
            value.value.f = *((float*)&tmp);
        }
        frame_value->append(value);
    }
    if (d->formater) {
        d->formater(frame_value);
    }
    return 0;
}

void anotc_euler_formater(QList<struct anotc_value> *frame_value)
{
    for (int i=0;i<frame_value->size();i++) {
        if (frame_value->at(i).name.compare("ROLL")==0 || frame_value->at(i).name.compare("PITCH")==0 || frame_value->at(i).name.compare("YAW")==0) {
            (*frame_value)[i].type = 8;
            (*frame_value)[i].value.f = ((float)frame_value->at(i).value.int16) / 100.0;
        }
    }
}

void anotc_alt_formater(QList<struct anotc_value> *frame_value)
{
    for (int i=0;i<frame_value->size();i++) {
        if (frame_value->at(i).name.compare("ALT_BARO")==0 || frame_value->at(i).name.compare("ALT_ADD")==0 || frame_value->at(i).name.compare("ALT_FU")==0) {
            (*frame_value)[i].type = 8;
            (*frame_value)[i].value.f = ((float)frame_value->at(i).value.int32) / 100.0;
        }
    }
}
