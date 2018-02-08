#include <string>

#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>

#include "mainwindow.h"
#include "area_progress_bar.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    AreaProgressBar * progressBar = new AreaProgressBar();
    setCentralWidget(progressBar);

    QWidget * mainWidget = new QWidget(this);

    QSlider * slider = new QSlider();
    slider->setRange(0, 100);
    slider->setSingleStep(1);
    slider->setValue(0);
    slider->setOrientation(Qt::Horizontal);

    _valueLabel = new QLabel("0");

    QHBoxLayout * hbox = new QHBoxLayout;
    hbox->addWidget(slider);
    hbox->addWidget(_valueLabel);

    QVBoxLayout * vbox = new QVBoxLayout;
    vbox->addWidget(progressBar);
    vbox->addLayout(hbox);

    mainWidget->setLayout(vbox);

    setCentralWidget(mainWidget);

    connect(slider, SIGNAL(valueChanged(int)), progressBar, SLOT(setValue(int)));
    // On the slider value changes I want the text in the label to show the value.
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::valueChanged(int value)
{
    std::string s = std::to_string(value);
    _valueLabel->setText(QString::fromStdString(s));
}
