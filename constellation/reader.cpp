#include "reader.h"

#include <QFile>
#include <QtEndian>

Reader::Reader(const QString &fileName, QObject *parent)
    : QThread(parent), fileName(fileName)
{
}

Reader::~Reader()
{
    requestInterruption();
    wait();
}

void Reader::run()
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly) || file.size() < 4)
        return;

    while (!isInterruptionRequested()) {
        QByteArray chunk = file.read(4000);
        while (chunk.size() < 4000) {
            file.seek(0);
            chunk += file.read(4000 - chunk.size());
        }

        QVector<QPointF> points;
        const uchar *data = reinterpret_cast<const uchar *>(chunk.constData());
        for (int i = 0; i < 4000; i += 4)
            points.append(QPointF(qFromLittleEndian<qint16>(data + i), qFromLittleEndian<qint16>(data + i + 2)));

        emit pointsRead(points);
        msleep(20);
    }
}
