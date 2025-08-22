#ifndef TEMPERATUREGAUGE_H
#define TEMPERATUREGAUGE_H

#include <QWidget>

class TemperatureGauge : public QWidget
{
    Q_OBJECT

public:
    explicit TemperatureGauge(QWidget *parent = nullptr);
    void setTemperatureLevel(float level); // 0.0 - 1.0 arası

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double temperatureLevel;
};

#endif // TEMPERATUREGAUGE_H
