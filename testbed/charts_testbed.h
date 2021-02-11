#pragma once

#include <QtCharts>

void chartsTestbed()
{
    using namespace QtCharts;
    QSplineSeries* series = new QSplineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);

    QChart* chart = new QChart();
    chart->legend()->show();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    QList<QPieSeries*> donuts;

    qreal minSize = 0.1;
    qreal maxSize = 0.9;
    int donutCount = 5;
    for (int i = 0; i < donutCount; i++) {
        QPieSeries *donut = new QPieSeries;
        int sliceCount =  3 + QRandomGenerator::global()->bounded(3);
        for (int j = 0; j < sliceCount; j++) {
            qreal value = 100 + QRandomGenerator::global()->bounded(100);
            QPieSlice *slice = new QPieSlice(QString("%1").arg(value), value);
            slice->setLabelVisible(true);
            slice->setLabelColor(Qt::white);
            slice->setLabelPosition(QPieSlice::LabelInsideTangential);
            donut->append(slice);
            donut->setHoleSize(minSize + i * (maxSize - minSize) / donutCount);
            donut->setPieSize(minSize + (i + 1) * (maxSize - minSize) / donutCount);
        }
        donuts << donut;
        chart->addSeries(donut);
    }
    QTimer* timer = new QTimer();
    timer->connect(timer, &QTimer::timeout, [donuts]()
    {
        const int i = QRandomGenerator::global()->bounded(donuts.size());
        auto& donut = donuts[i];
        auto slices = donut->slices();
        const int si = QRandomGenerator::global()->bounded(slices.size());
        slices[si]->setValue(slices[si]->value() + 1);
    });
    timer->start(10);


    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}
