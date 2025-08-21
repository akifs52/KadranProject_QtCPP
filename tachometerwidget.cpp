#include "TachometerWidget.h"
#include <QPainter>
#include <QtMath>

TachometerWidget::TachometerWidget(QWidget *parent)
    : QWidget(parent), value(0.0f)
{
    setMinimumSize(280, 280);
}

void TachometerWidget::setRpmValue(float rpm)
{
    if (rpm < 0.0f) rpm = 0.0f;
    if (rpm > 9.0f) rpm = 9.0f;
    value = rpm;
    update();
}

void TachometerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int side = qMin(w, h);

    p.translate(w / 2, h / 2);
    p.scale(side / 300.0, side / 300.0);

    QRectF arcRect(-140, -140, 280, 280);

    // --- Ana Yay (0–9 arası) ---
    QPen arcPen(Qt::cyan, 4);
    p.setPen(arcPen);
    p.drawArc(arcRect, -135 * 16, -270 * 16);

    // --- Kırmızı Yay (7–9 arası) ---
    QPen redPen(Qt::red, 6);
    p.setPen(redPen);
    int redStartAngle = -135 - int((7.0 / 9.0) * 270);
    int redSpanAngle  = int((2.0 / 9.0) * 270);
    p.drawArc(arcRect, redStartAngle * 16, -redSpanAngle * 16);

    // --- Sayılar ve Çentikler ---
    p.setPen(Qt::cyan);
    QFont font = p.font();
    font.setPointSize(14);
    p.setFont(font);

    for (int i = 0; i <= 9; ++i) {
        double angleDeg = -225.0 + (i * 270.0 / 9.0);
        double angleRad = qDegreesToRadians(angleDeg);

        int x = int(110 * qCos(angleRad));
        int y = int(110 * qSin(angleRad));
        QRect textRect(x - 15, y - 15, 30, 30);
        p.drawText(textRect, Qt::AlignCenter, QString::number(i));

        // Çentik çizimi
        QPointF start(130 * qCos(angleRad), 130 * qSin(angleRad));
        QPointF end(140 * qCos(angleRad), 140 * qSin(angleRad));
        p.drawLine(start, end);
    }

    // --- "x1000 RPM" yazısı ---
    p.drawText(QRect(-50, 40, 100, 30), Qt::AlignCenter, "x1000 RPM");

    // --- İbre ---
    double angleDeg = -225.0 + (value * 270.0 / 9.0);
    double angleRad = qDegreesToRadians(angleDeg);

    QPointF needle[3] = {
        QPointF(0, 0),
        QPointF(-5, 10),
        QPointF(120 * qCos(angleRad), 120 * qSin(angleRad))
    };

    p.setBrush(Qt::red);
    p.setPen(Qt::red);
    p.drawPolygon(needle, 3);
}
