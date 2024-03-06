#include "inc/sleep.h"

namespace Zy
{
    void mSleep(unsigned long ms)
    {
        QTime _timer = QTime::currentTime().addMSecs(ms);
        while (QTime::currentTime() < _timer) {
            QCoreApplication::processEvents(QEventLoop::AllEvents);
        }
    }
}
