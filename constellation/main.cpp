#include "plot.h"
#include "reader.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamReader>

static QVector<QPointF> readMarkers(const QString &fileName)
{
    QVector<QPointF> markers;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return markers;

    QXmlStreamReader xml(&file);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == QLatin1String("Point")) {
            double x = xml.attributes().value("X").toDouble();
            double y = xml.attributes().value("Y").toDouble();
            markers.append(QPointF(x, y));
        }
    }
    return markers;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<QVector<QPointF>>();

    QString xmlFile = QFileDialog::getOpenFileName(nullptr, QString(), DATA_DIR "/xml", "*.xml");
    if (xmlFile.isEmpty())
        return 0;

    Plot plot(readMarkers(xmlFile));
    plot.resize(600, 600);
    plot.show();

    Reader reader(DATA_DIR "/bin/iq_qam32.bin");
    QObject::connect(&reader, &Reader::pointsRead, &plot, &Plot::addPoints);
    reader.start();

    return app.exec();
}
