#include "TemperatureGauge.h"
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QtMath>

TemperatureGauge::TemperatureGauge(QWidget *parent)
    : QWidget(parent), temperatureLevel(90.0) // Start at 90°C
{
    setMinimumSize(150, 150);
}

void TemperatureGauge::setTemperatureLevel(float temperature) {
    // Clamp temperature between 0 and 180°C
    temperatureLevel = qBound(0.0f, temperature, 180.0f);
    update();
}

void TemperatureGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int w = width();
    int h = height();
    int size = qMin(w, h) - 20;
    QPoint center(w / 2, h / 2);

    // Draw the arc background
    double totalAngle = 180.0;
    QRectF arcRect(center.x() - size/2, center.y() - size/2, size, size);

    // Cyan region (0–60°C)
    painter.setPen(QPen(QColor(0, 200, 200), 6));
    painter.drawArc(arcRect, (180 - 0) * 16, -60 * 16);

    // Green region (60–120°C)
    painter.setPen(QPen(QColor(0, 180, 0), 6));
    painter.drawArc(arcRect, (180 - 60) * 16, -60 * 16);

    // Red region (120–180°C)
    painter.setPen(QPen(Qt::red, 6));
    painter.drawArc(arcRect, (180 - 120) * 16, -60 * 16);

    // Draw ticks
    painter.setPen(QPen(Qt::cyan, 2));
    QStringList tickLabels = {"0°C", "45°C", "90°C", "135°C", "180°C"};
    int tickCount = tickLabels.size() - 1;

    for (int i = 0; i <= tickCount; i++) {
        double angle = 180.0 - (i * 45.0);
        double radianAngle = qDegreesToRadians(angle);

        QPointF innerPoint(
            center.x() + (size/2 - 15) * qCos(radianAngle),
            center.y() - (size/2 - 15) * qSin(radianAngle)
            );

        QPointF outerPoint(
            center.x() + (size/2 + 5) * qCos(radianAngle),
            center.y() - (size/2 + 5) * qSin(radianAngle)
            );

        painter.drawLine(innerPoint, outerPoint);

        // Draw labels
        QFont font = painter.font();
        font.setPointSize(12);
        painter.setFont(font);
        painter.setPen(Qt::cyan);

        QPointF textPoint(
            center.x() + (size/2 - 20) * qCos(radianAngle),
            center.y() - (size/2 - 20) * qSin(radianAngle)
            );

        // Adjust text alignment
        QRect textRect(textPoint.x() - 20, textPoint.y() - 10, 40, 20);
        painter.drawText(textRect, Qt::AlignCenter, tickLabels[i]);
    }

    // Draw needle
    double needleAngle = 180.0 - temperatureLevel; // 0°C at 180°, 180°C at 0°
    double needleRadians = qDegreesToRadians(needleAngle);

    painter.setPen(QPen(Qt::red, 3));
    QPointF needleTip(
        center.x() + (size/2 - 25) * qCos(needleRadians),
        center.y() - (size/2 - 25) * qSin(needleRadians)
        );
    painter.drawLine(center, needleTip);

    // Draw center circle
    painter.setBrush(Qt::cyan);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 8, 8);

    // Draw current temperature value
    painter.setPen(Qt::cyan);
    // Yazı ayarları
    QFont valueFont = painter.font();
    valueFont.setPointSize(12);
    painter.setFont(valueFont);

    // Kutu
    QRectF box(center.x() - 50, center.y() + size/2 - 50, 100, 40);
    painter.setPen(QPen(QColor(0,200,200), 2));
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(box,10,10);

    // Yazı
    painter.setPen(Qt::cyan);
    QString valueText = QString::number(temperatureLevel, 'f', 1) + "°C";
    painter.drawText(box, Qt::AlignCenter, valueText);
}
