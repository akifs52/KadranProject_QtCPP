#ifndef SPEEDOMETERWIDGET_H
#define SPEEDOMETERWIDGET_H

#include <QWidget>

class SpeedometerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpeedometerWidget(QWidget *parent = nullptr);

    void setSpeed(int value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int speed; // 0 - 200 arası hız
};

#endif // SPEEDOMETERWIDGET_H
