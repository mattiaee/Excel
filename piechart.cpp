#include "piechart.h"

PieChart::PieChart(Data*& d, QModelIndexList i, QWidget *parent) : QWidget(parent), data(d), indexes(i) {
    QVBoxLayout* barLayout = new QVBoxLayout;
    QPieSeries *series = new QPieSeries;

    QVPieModelMapper *pieMapper = new QVPieModelMapper(this);
    QChart* chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);

    data->clearMapping();

    pieMapper->setLabelsColumn(indexes.first().column());
    pieMapper->setValuesColumn(indexes.first().column()+1);
    pieMapper->setFirstRow(indexes.first().row());
    pieMapper->setRowCount((indexes.last().row()+1)-(indexes.first().row()));
    pieMapper->setSeries(series);
    pieMapper->setModel(data);
    chart->addSeries(series);

    QString sliceColorHex = "#000000";

    QList<QPieSlice*> slices = series->slices();
    int slicesCount = pieMapper->firstRow();
    for(auto it = slices.begin(); it != slices.end(); ++it) {
        sliceColorHex = "#" + QString::number((*it)->brush().color().rgb(), 16).right(6).toUpper();
        data->addMapping(sliceColorHex, QRect(indexes.first().column(), slicesCount,2,1));
        slicesCount++;
    }

    //forse va fatto delete vecchio chart
    QChartView* chartView = new QChartView;

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);
    barLayout->addWidget(chartView);
    setLayout(barLayout);
}
