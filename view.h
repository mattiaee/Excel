#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QLineSeries>
#include <QPieSeries>
#include <QGridLayout>
#include <QChartView>
#include <QVBarModelMapper>
#include <QVXYModelMapper>
#include <QVPieModelMapper>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPushButton>
#include <QMenuBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QToolBar>
#include "model.h"
#include "data.h"
#include "barchart.h"
#include "linechart.h"
#include "piechart.h"
#include "tableview.h"

class View : public QWidget
{
    Q_OBJECT

public:
    View(QWidget *parent = 0);

private:
    Data* data;

    QMenu* file;
    //QChart *chart;
    //QTableView *tableView;
    //QChartView *chartView;

    TableView* tableView;

    BarChart* barChart;
    LineChart* lineChart;
    PieChart* pieChart;

    void addMenu(QToolBar* toolBar);
    void addTableControls(QToolBar* toolBar);
    void addChartControls(QToolBar* toolBar, QGridLayout* mainLayout);

    void loadFile(const QString &fileName);

private slots:
    void openFile();
    void saveFile();
public slots:
    /*void addColumns(int col);
    void addRows(int row);
    void removeColumns(int col);
    void removeRows(int row);*/

    void addBarChart(QGridLayout *);
    void addLineChart(QGridLayout *);
    void addPieChart(QGridLayout *);
    void deleteChart(QGridLayout *);

    //void customHeaderMenuRequestedH(QPoint pos);
    //void customHeaderMenuRequestedV(QPoint pos);
};

#endif // VIEW_H
