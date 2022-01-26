#include "tableview.h"

TableView::TableView(Data*& d,QWidget *parent) : QWidget(parent), data(d) {
    QVBoxLayout* tableLayout = new QVBoxLayout;

    //forse va fatto delete vecchio chart
    tableView = new QTableView;
    tableView->setModel(data);
    tableView->setMinimumWidth(100);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(customHeaderMenuRequestedH(QPoint)));
    tableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(customHeaderMenuRequestedV(QPoint)));

    data->setParent(tableView);

    tableLayout->addWidget(tableView);
    setLayout(tableLayout);
}

TableView::~TableView() {
    delete tableView;
}

void TableView::reset() {
    tableView->reset();
}

QModelIndexList TableView::getIndexesSelection() const {
    return tableView->selectionModel()->selectedIndexes();
};

void TableView::customHeaderMenuRequestedH(QPoint pos){
    int column= tableView->horizontalHeader()->logicalIndexAt(pos);

    QMenu *menu= new QMenu(this);
    menu->addAction(new QAction("Rename Column", this));
    menu->addAction(new QAction("Delete Column", this));

    connect(menu->actions()[0], &QAction::triggered, this, [=] {
        QString txt = QInputDialog::getText(this, tr("Enter a value"), tr("Enter a value"), QLineEdit::Normal, "");
        data->setHeaderData(column, Qt::Horizontal, txt);
        tableView->verticalHeader()->resizeSection(column,5);//????
    });
    connect(menu->actions()[1], &QAction::triggered, this, [=] { data->removeColumns(column,1);});
    menu->popup(tableView->horizontalHeader()->viewport()->mapToGlobal(pos));
}

void TableView::customHeaderMenuRequestedV(QPoint pos){
    int row=tableView->verticalHeader()->logicalIndexAt(pos);

    QMenu *menu=new QMenu(this);
    menu->addAction(new QAction("Rename Row", this));
    menu->addAction(new QAction("Delete Row", this));

    connect(menu->actions()[0], &QAction::triggered, this, [=] {
        QString txt =  QInputDialog::getText(this, tr("Enter a value"), tr("Enter a value"), QLineEdit::Normal, "");
        data->setHeaderData(row,Qt::Vertical,txt);
        tableView->verticalHeader()->resizeSection(row,5);
    });
    connect(menu->actions()[1],  &QAction::triggered, this, [=] { data->removeRows(row, 1);});
    menu->popup(tableView->horizontalHeader()->viewport()->mapToGlobal(pos));
}
