#ifndef TABLEVIEW_H
#define TABLEVIEW_H
#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QMenuBar>
#include <QInputDialog>

#include "data.h"

class TableView : public QWidget {
    Q_OBJECT
private:
    QTableView* tableView;
    Data* data;
public:
    TableView(Data*& d, QWidget *parent = 0);
    QModelIndexList getIndexesSelection() const;
    void reset();
    virtual ~TableView();
public slots:
    void customHeaderMenuRequestedH(QPoint pos);
    void customHeaderMenuRequestedV(QPoint pos);
};

#endif // TABLEVIEW_H
