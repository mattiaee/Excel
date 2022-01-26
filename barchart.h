#ifndef BARCHART_H
#define BARCHART_H
#include <QWidget>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>
#include <QVBarModelMapper>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QVBoxLayout>

#include "data.h"

class BarChart : public QWidget {
    Q_OBJECT
private:
    Data* data;
    QModelIndexList indexes;
public:
    BarChart(Data*& d, QModelIndexList i, QWidget *parent = 0);

};

#endif // BARCHART_H
