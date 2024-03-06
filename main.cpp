#include "inc/widget.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int font_id = QFontDatabase::addApplicationFont(":/font/font/LanTing.ttf");
    QStringList font_list =  QFontDatabase::applicationFontFamilies(font_id);
    if (!font_list.isEmpty())
    {
        QFont LanTing;
        LanTing.setFamily(font_list[0]);
        a.setFont(LanTing);
    }

    Widget w;
    w.show();

    return a.exec();
}
