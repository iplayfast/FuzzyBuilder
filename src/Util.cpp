#include <qstring.h>
#include <QPainter>
#include "Util.h"
QString FormatLabel(const char *text, double min, double v, double max)
{
    QString ss;
    ss.sprintf("%s %05.5f>=%05.5f<=%05.5f",text,min,v,max);
    return ss;
}
bool verbose=false;
void StartComment(QTextStream &s)
{
    if (verbose)
    s << "/**************************************************************\n";
}

void EndComment(QTextStream &s)
{
    if (verbose)
    s << "***************************************************************/\n";
}
