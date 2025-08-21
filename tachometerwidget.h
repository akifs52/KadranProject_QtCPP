#ifndef TACHOMETERWIDGET_H
#define TACHOMETERWIDGET_H

#include <QWidget>

class TachometerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TachometerWidget(QWidget *parent = nullptr);

    void setRpmValue(float rpm);  // ibre kontrolü

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float value; // 0 - 9000 arası devir
};

#endif // TACHOMETERWIDGET_H
