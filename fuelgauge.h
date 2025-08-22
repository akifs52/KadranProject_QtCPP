#ifndef FUELGAUGE_H
#define FUELGAUGE_H

#include <QWidget>

class FuelGauge : public QWidget
{
    Q_OBJECT

public:
    explicit FuelGauge(QWidget *parent = nullptr);

    void setFuelLevel(int level); // 0 = boş (E), 800= full (F)

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int fuelLevel; // 0 ile 800 arası
};

#endif // FUELGAUGE_H
