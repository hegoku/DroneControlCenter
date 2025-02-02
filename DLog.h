#ifndef DLOG_H
#define DLOG_H

#include <QString>

void DLogI(QString content);
void DLogN(QString content);
void DLogE(QString content);
void DLogW(QString content);

extern void (*DLog_print)(QString content);

#endif // DLOG_H
