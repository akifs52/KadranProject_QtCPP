#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , speedometer(new SpeedometerWidget)
    , fuelGauge(new FuelGauge)
    , rpm(new TachometerWidget)
    , tempGauge(new TemperatureGauge)
    , blinkTimer1(new QTimer)
    , blinkTimer2(new QTimer)
{
    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/qml/CarController.qml"));
    ui->verticalLayout_6->addWidget(speedometer);
    speedometer->setSpeed(160);

    ui->verticalLayout_7->addWidget(fuelGauge);
    fuelGauge->setFuelLevel(1);

    ui->horizontalLayout_2->addWidget(rpm);
    rpm->setRpmValue(0);

    ui->horizontalLayout_3->addWidget(tempGauge);
    tempGauge->setTemperatureLevel(0);

    blinkTimer1->setInterval(500);
    blinkTimer2->setInterval(500);

    // timer1
    connect(blinkTimer1, &QTimer::timeout, this, [this]() {
        toggleSignal(ui->leftLabelSignal, isOn1);
    });

    // timer2
    connect(blinkTimer2, &QTimer::timeout, this, [this]() {
        toggleSignal(ui->rightLabelSignal, isOn2);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleSignal(QLabel *label, bool &state)
{
    if (state) {
        label->setStyleSheet("color: rgb(255, 255, 255); background-color: darkgreen; border-radius: 10px;");
    } else {
        label->setStyleSheet("color: rgb(255, 255, 255); background-color: limegreen; border-radius: 10px;");
    }
    state = !state;
}


void MainWindow::on_leftSignal_clicked()
{
    if (blinkTimer1->isActive()) {
        blinkTimer1->stop();
        ui->leftLabelSignal->setStyleSheet("color: white; background-color: darkgreen; border-radius: 10px;");
        isOn1 = false;
    } else {
        blinkTimer1->start();
    }
}


void MainWindow::on_rightSignal_clicked()
{
    if (blinkTimer2->isActive()) {
        blinkTimer2->stop();
        ui->rightLabelSignal->setStyleSheet("color: white; background-color: darkgreen; border-radius: 10px;");
        isOn2 = false;
    } else {
        blinkTimer2->start();
    }
}

