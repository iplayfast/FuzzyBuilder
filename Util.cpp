#include <qstring.h>
QString FormatLabel(const char *text, double min, double v, double max)
{
    QString ss;
    ss.sprintf("%s %05.5f>=%05.5f<=%05.5f",text,min,v,max);
    return ss;
}
