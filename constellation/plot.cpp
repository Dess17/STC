#include "plot.h"

#include <QPainter>

#include <cmath>

static double rms(const QVector<QPointF> &values)
{
    if (values.isEmpty())
        return 0;
    double sum = 0;
    for (const QPointF &p : values)
        sum += p.x() * p.x() + p.y() * p.y();
    return std::sqrt(sum / values.size());
}

Plot::Plot(const QVector<QPointF> &markers, QWidget *parent)
    : QWidget(parent), markers(markers)
{
}

void Plot::addPoints(const QVector<QPointF> &newPoints)
{
    points += newPoints;
    if (points.size() > 16000)
        points.remove(0, points.size() - 16000);
    update();
}

void Plot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    double maxValue = 0;
    for (const QPointF &m : markers)
        maxValue = qMax(maxValue, qMax(qAbs(m.x()), qAbs(m.y())));
    if (maxValue == 0)
        maxValue = 1;

    QPointF center(width() / 2.0, height() / 2.0);
    double scale = qMin(width(), height()) / 2.0 / (maxValue * 1.2);
    double pointsRms = rms(points);
    double pointsScale = pointsRms > 0 ? rms(markers) / pointsRms * scale : scale;

    QPolygonF dots;
    for (const QPointF &p : points)
        dots << center + QPointF(p.x(), -p.y()) * pointsScale;
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    painter.drawPoints(dots);

    painter.setPen(QPen(Qt::red, 2));
    for (const QPointF &m : markers) {
        QPointF c = center + QPointF(m.x(), -m.y()) * scale;
        painter.drawLine(c - QPointF(5, 0), c + QPointF(5, 0));
        painter.drawLine(c - QPointF(0, 5), c + QPointF(0, 5));
    }
}
