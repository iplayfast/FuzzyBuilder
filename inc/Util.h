#ifndef UTIL_H
#define UTIL_H
#include <QTextStream>
QString FormatLabel(const char *text, double min, double v, double max);
void StartComment(QTextStream  &s);
void EndComment(QTextStream  &s);

#endif // UTIL_H
