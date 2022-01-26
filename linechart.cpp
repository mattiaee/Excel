#include "linechart.h"

LineChart::LineChart(Data*& d, QModelIndexList i, QWidget *parent) : QWidget(parent), data(d), indexes(i) {
    QVBoxLayout* barLayout = new QVBoxLayout;
    QLineSeries *series;

    QVXYModelMapper *lineMapper;

    QChart* chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);

    int firstCol = indexes.first().column();
    int lastCol = indexes.last().column();

    QString seriesColorHex = "#000000";

    data->clearMapping();

    for(int i=firstCol+1; i<=lastCol; i++)
    {
        series = new QLineSeries;
        series->setName(data->headerData(i,Qt::Horizontal).toString());
        lineMapper = new QVXYModelMapper(this);
        lineMapper->setXColumn(firstCol);
        lineMapper->setYColumn(i);
        lineMapper->setFirstRow(indexes.first().row());
        lineMapper->setRowCount((indexes.last().row()+1)-(indexes.first().row()));
        lineMapper->setSeries(series);
        lineMapper->setModel(data);
        chart->addSeries(series);
        seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
        data->addMapping(seriesColorHex, QRect(i, indexes.first().row(), 1,lineMapper->rowCount()));
    }

    chart->createDefaultAxes();

    //forse va fatto delete vecchio chart
    QChartView* chartView = new QChartView;

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);
    barLayout->addWidget(chartView);
    setLayout(barLayout);
}
