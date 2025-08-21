#include "TemperatureGauge.h"
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QtMath>

TemperatureGauge::TemperatureGauge(QWidget *parent)
    : QWidget(parent), temperatureLevel(0.5) // Başlangıçta %50 (90°C)
{
    setMinimumSize(280, 280);
}

void TemperatureGauge::setTemperatureLevel(double level) {
    temperatureLevel = qBound(0.0, level, 1.0);
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

    double totalAngle = 180;
    QRectF arcRect(center.x() - size/2, center.y() - size/2, size, size);

    // Cyan bölge (0–60°C)
    painter.setPen(QPen(QColor(0, 200, 200), 6));
    painter.drawArc(arcRect, (180 - 0) * 16, -60 * 16);

    // Yeşil bölge (60–120°C)
    painter.setPen(QPen(QColor(0, 180, 0), 6));
    painter.drawArc(arcRect, (180 - 60) * 16, -60 * 16);

    // Kırmızı bölge (120–180°C)
    painter.setPen(QPen(Qt::red, 6));
    painter.drawArc(arcRect, (180 - 120) * 16, -60 * 16);

    // Çentikler (0, 45, 90, 135, 180)
    painter.setPen(QPen(Qt::gray, 2));
    int tickCount = 4;
    for (int i = 0; i <= tickCount; i++) {
        double t = (double)i / tickCount;
        double angle = 180 - t * totalAngle;

        QPointF p1(
            center.x() + (size/2 - 6) * qCos(qDegreesToRadians(angle)),
            center.y() - (size/2 - 6) * qSin(qDegreesToRadians(angle))
            );
        QPointF p2(
            center.x() + (size/2 + 6) * qCos(qDegreesToRadians(angle)),
            center.y() - (size/2 + 6) * qSin(qDegreesToRadians(angle))
            );
        painter.drawLine(p1, p2);
    }

    // Yazılar (0°C, 90°C, 180°C)
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);
    painter.setPen(QColor(0, 200, 200));

    painter.drawText(center.x() - size/2 + 10, center.y(), "0°C");
    painter.drawText(center.x() - 20, center.y() - size/2 + 25, "90°C");
    painter.drawText(center.x() + size/2 - 40, center.y(), "180°C");

    // İbre
    double angle = 180 - temperatureLevel * totalAngle;
    painter.setPen(QPen(Qt::red, 4));
    QPointF needle(
        center.x() + (size/2 - 20) * qCos(qDegreesToRadians(angle)),
        center.y() - (size/2 - 20) * qSin(qDegreesToRadians(angle))
        );
    painter.drawLine(center, needle);

    // Merkez yuvarlak
    painter.setBrush(QColor(0, 200, 200));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 10, 10);
}
