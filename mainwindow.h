#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FuelGauge.h"
#include "SpeedometerWidget.h"
#include "TachometerWidget.h"
#include "TemperatureGauge.h"
#include "qlabel.h"
#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void toggleSignal(QLabel *label, bool &state);

    void on_leftSignal_clicked();

    void on_rightSignal_clicked();

    void on_RpmSlider_valueChanged(int value);

    void on_TemperatureSlider_valueChanged(int value);

    void on_speedSlider_valueChanged(int value);

    void on_fuelSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    SpeedometerWidget *speedometer;
    FuelGauge *fuelGauge;
    TachometerWidget *rpm;
    TemperatureGauge *tempGauge;
    QTimer *blinkTimer1;
    QTimer *blinkTimer2;
    bool isOn1 = false;
    bool isOn2 = false;
};

#endif // MAINWINDOW_H
