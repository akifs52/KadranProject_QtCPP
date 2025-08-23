#include "FuelGauge.h"
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QtMath>

FuelGauge::FuelGauge(QWidget *parent)
    : QWidget(parent), fuelLevel(0)   // Başlangıçta %50
{
    setMinimumSize(150, 150);
}

void FuelGauge::setFuelLevel(int level) {
    fuelLevel = qBound(0, level, 800);
    update();
}

void FuelGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int w = width();
    int h = height();
    int size = qMin(w, h) - 20;
    QPoint center(w / 2, h / 2);

    // Yakıt göstergesinin açısı (180° yay)
    double startAngle = 180;   // sol (F)
    double endAngle   = 0;     // sağ (E)

    QRectF arcRect(center.x() - size/2, center.y() - size/2, size, size);

    // Yarım daire (açık mavi)
    QPen arcPen(QColor(0, 200, 200), 6);
    painter.setPen(arcPen);
    painter.drawArc(arcRect, startAngle * 16, (endAngle - startAngle) * 16);

    // Çentikler
    painter.setPen(QPen(QColor(0, 200, 200), 2));
    int tickCount = 4; // F, 1/4, 1/2, 3/4, E
    for (int i = 0; i <= tickCount; i++) {
        double t = (double)i / tickCount;
        double angle = startAngle + t * (endAngle - startAngle);

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

    // Kırmızı bölge (son çentik ile E arası)
    painter.setPen(QPen(Qt::red, 6));
    double lastTickAngle = startAngle + (double)(tickCount-1) / tickCount * (endAngle - startAngle);
    painter.drawArc(arcRect, lastTickAngle * 16, (endAngle - lastTickAngle) * 16);

    // Yazılar (F, 1/2, E)
    QFont font = painter.font();
    font.setPointSize(12);
    font.setWeight(QFont::Bold);
    painter.setFont(font);
    painter.setPen(QColor(0, 200, 200));

    // F
    painter.drawText(center.x() - size/2 + 10, center.y(), "F");
    // 1/2 (orta üst)
    painter.drawText(center.x() - 15, center.y() - size/2 + 25, "1/2");
    // E
    painter.drawText(center.x() + size/2 - 20, center.y(), "E");

    // İbre
    double ratio = (double)fuelLevel / 800.0;
    double angle = startAngle + (1.0 - ratio) * (endAngle - startAngle);


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


    // --- Kalan KM kutusu ---
    QRectF box(center.x() - 50, center.y() + size/2 -50, 100, 40);
    painter.setPen(QPen(QColor(0,200,200), 2));
    painter.setBrush(QColor(0, 0, 0));  // koyu arka plan
    painter.drawRoundedRect(box, 10, 10);  // <-- yuvarlatılmış köşeler

    painter.setPen(Qt::cyan);
    font.setPointSize(16);
    font.setWeight(QFont::Bold);
    painter.setFont(font);
    painter.drawText(box, Qt::AlignCenter, QString("%1 km").arg(fuelLevel));
}
