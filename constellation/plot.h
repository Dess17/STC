#pragma once

#include <QPointF>
#include <QVector>
#include <QWidget>

class Plot : public QWidget
{
    Q_OBJECT

public:
    explicit Plot(const QVector<QPointF> &markers, QWidget *parent = nullptr);
    void addPoints(const QVector<QPointF> &newPoints);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPointF> markers;
    QVector<QPointF> points;
};
