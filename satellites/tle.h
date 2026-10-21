#pragma once

#include <QDateTime>
#include <QString>
#include <QVector>

struct Satellite
{
    QString name;
    int launchYear = 0;
    QDateTime epoch;
    double inclination = 0;
};

QVector<Satellite> parseTle(const QString &text, QString &error);
QString statistics(const QVector<Satellite> &satellites);
