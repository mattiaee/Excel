#ifndef LINECHART_H
#define LINECHART_H
#include <QWidget>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QVXYModelMapper>
#include <QValueAxis>
#include <QVBoxLayout>

#include "data.h"

class LineChart : public QWidget {
    Q_OBJECT
private:
    Data* data;
    QModelIndexList indexes;
public:
    LineChart(Data*& d, QModelIndexList i, QWidget *parent = 0);
};

#endif // LINECHART_H
