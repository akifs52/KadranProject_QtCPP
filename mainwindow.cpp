#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , speedometer(new SpeedometerWidget)
    , fuelGauge(new FuelGauge)
    , rpm(new TachometerWidget)
{
    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/qml/CarController.qml"));
    ui->verticalLayout_6->addWidget(speedometer);
    speedometer->setSpeed(160);

    ui->verticalLayout_7->addWidget(fuelGauge);
    fuelGauge->setFuelLevel(1);

    ui->horizontalLayout_2->addWidget(rpm);
    rpm->setRpmValue(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
