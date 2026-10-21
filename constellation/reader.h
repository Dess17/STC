#pragma once

#include <QPointF>
#include <QString>
#include <QThread>
#include <QVector>

class Reader : public QThread
{
    Q_OBJECT

public:
    explicit Reader(const QString &fileName, QObject *parent = nullptr);
    ~Reader() override;

signals:
    void pointsRead(const QVector<QPointF> &points);

protected:
    void run() override;

private:
    QString fileName;
};
