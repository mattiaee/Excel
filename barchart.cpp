#include "barchart.h"

BarChart::BarChart(Data*& d, QModelIndexList i, QWidget *parent) : QWidget(parent), data(d), indexes(i) {
    QVBoxLayout* barLayout = new QVBoxLayout;
    QBarSeries *series = new QBarSeries;
    QVBarModelMapper *barMapper = new QVBarModelMapper(this);

    //forse va inizializzato nel costruttore e poi cambiato qui ogni volta
    QChart* chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);

    barMapper->setFirstBarSetColumn(indexes.first().column());
    barMapper->setLastBarSetColumn(indexes.last().column());
    barMapper->setFirstRow(indexes.first().row());
    barMapper->setRowCount((indexes.last().row()+1)-(indexes.first().row()));
    barMapper->setSeries(series);
    barMapper->setModel(data);
    chart->addSeries(series);

    data->clearMapping();

    QString seriesColorHex = "#000000";

    QList<QBarSet *> barsets = series->barSets();

    for (int i = 0; i < barsets.count(); i++) {
        seriesColorHex = "#" + QString::number(barsets.at(i)->brush().color().rgb(), 16).right(6).toUpper();
        data->addMapping(seriesColorHex, QRect(indexes.first().column() + i, indexes.first().row(), 1,barMapper->rowCount()));
    }

    QStringList categories;

    for (int i = indexes.first().row(); i<=indexes.last().row(); i++){
        categories.push_back(data->headerData(i, Qt::Vertical).toString());
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    //forse va fatto delete vecchio chart
    QChartView* chartView = new QChartView;

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);
    barLayout->addWidget(chartView);
    setLayout(barLayout);

}
