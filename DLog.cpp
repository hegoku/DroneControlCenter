#include "DLog.h"
#include <QDateTime>
#include <QString>

void (*DLog_print)(QString content);

void DLogI(QString content)
{
    DLog_print(QString("<span style='color:gray;'>%1:<br/></span>").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zz")));
    DLog_print(QString("<span style='color:blue;'>%1<br/></span>").arg(content));
}

void DLogN(QString content)
{
    DLog_print(QString("<span style='color:gray;'>%1:<br/></span>").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zz")));
    DLog_print(QString("<span style='color:black;'>%1<br/></span>").arg(content));
}

void DLogE(QString content)
{
    DLog_print(QString("<span style='color:gray;'>%1:<br/></span>").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zz")));
    DLog_print(QString("<span style='color:red;'>%1<br/></span>").arg(content));
}

void DLogW(QString content)
{
    DLog_print(QString("<span style='color:gray;'>%1:<br/></span>").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zz")));
    DLog_print(QString("<span style='color:orange;'>%1<br/></span>").arg(content));
}
