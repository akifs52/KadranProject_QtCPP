#ifndef FUELGAUGE_H
#define FUELGAUGE_H

#include <QWidget>

class FuelGauge : public QWidget
{
    Q_OBJECT

public:
    explicit FuelGauge(QWidget *parent = nullptr);

    void setFuelLevel(double level); // 0.0 = boş (E), 1.0 = full (F)

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double fuelLevel;  // 0.0 ile 1.0 arası
};

#endif // FUELGAUGE_H
