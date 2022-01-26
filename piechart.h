#ifndef PIECHART_H
#define PIECHART_H
#include <QWidget>
#include <QChart>
#include <QPieSeries>
#include <QChartView>
#include <QVPieModelMapper>
#include <QValueAxis>
#include <QVBoxLayout>

#include "data.h"

class PieChart : public QWidget {
    Q_OBJECT
private:
    Data* data;
    QModelIndexList indexes;
public:
    PieChart(Data*& d, QModelIndexList i, QWidget *parent = 0);
};

#endif // PIECHART_H
