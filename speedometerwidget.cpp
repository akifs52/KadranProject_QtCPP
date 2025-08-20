#include "SpeedometerWidget.h"
#include <QPainter>
#include <QtMath>

SpeedometerWidget::SpeedometerWidget(QWidget *parent)
    : QWidget(parent), speed(0)
{
    setMinimumSize(280, 280);
}

void SpeedometerWidget::setSpeed(int value)
{
    if (value < 0) value = 0;
    if (value > 260) value = 260;
    speed = value;
    update();
}

void SpeedometerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int side = qMin(w, h);

    p.translate(w/2, h/2);
    p.scale(side / 300.0, side / 300.0);

    // --- Yay (3/4 daire: 270°) ---
    QPen pen(Qt::cyan, 4);
    p.setPen(pen);
    QRectF arcRect(-140, -140, 280, 280);


    // 270° yay (başlangıç: 225°, bitiş: -45°)
    p.drawArc(arcRect, -135*16, -270*16);

    // --- Sayılar ---
    p.setPen(Qt::cyan);
    QFont font = p.font();
    font.setPointSize(14);
    p.setFont(font);

    for (int i = 0; i <= 260; i += 20) {
        // 0 = 225° (sol alt), 260 = -45° (sağ alt)
        double startAngle = -225.0;
        double endAngle   = 45.0;
        double angleDeg = startAngle - (i * (startAngle - endAngle) / 260.0);
        double angle = angleDeg * M_PI / 180.0;

        int x = int(120 * qCos(angle));
        int y = int(120 * qSin(angle));
        QString text = QString::number(i);
        QRect textRect(x-15, y-15, 30, 30);
        p.drawText(textRect, Qt::AlignCenter, text);
    }

    // --- "Km/h" yazısı ---
    p.drawText(QRect(-40, 40, 80, 30), Qt::AlignCenter, "Km/h");

    // --- İbre ---
    double startAngle = -225.0;
    double endAngle   = +45.0;
    double angleDeg = startAngle - (speed * (startAngle - endAngle) / 260.0);
    double angle = angleDeg * M_PI / 180.0;

    QPointF needle[3] = {
        QPointF(0,0),
        QPointF(-5,10),
        QPointF(120*qCos(angle), 120*qSin(angle))
    };

    p.setBrush(Qt::red);
    p.setPen(Qt::red);
    p.drawPolygon(needle, 3);

}
