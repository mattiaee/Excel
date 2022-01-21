#include "view.h"

void View::customHeaderMenuRequestedH(QPoint pos){
    int column=tableView->horizontalHeader()->logicalIndexAt(pos);

    QMenu *menu=new QMenu(this);
    menu->addAction(new QAction("Rename Column", this));
    menu->addAction(new QAction("Delete Column", this));

    connect(menu->actions()[0], &QAction::triggered, this, [=] {
        QString txt = QInputDialog::getText(this, tr("Enter a value"), tr("Enter a value"), QLineEdit::Normal, "");
        data->setHeaderData(column, Qt::Horizontal, txt);
    });
    connect(menu->actions()[1], &QAction::triggered, this, [=] { data->removeColumns(column,1);});
    menu->popup(tableView->horizontalHeader()->viewport()->mapToGlobal(pos));
}

void View::customHeaderMenuRequestedV(QPoint pos){
    int row=tableView->verticalHeader()->logicalIndexAt(pos);
    qDebug() << row;

    QMenu *menu=new QMenu(this);
    menu->addAction(new QAction("Rename Row", this));
    menu->addAction(new QAction("Delete Row", this));

    connect(menu->actions()[0], &QAction::triggered, this, [=] {
        QString txt =  QInputDialog::getText(this, tr("Enter a value"), tr("Enter a value"), QLineEdit::Normal, "");
        data->setHeaderData(row,Qt::Vertical,txt);
    });
    connect(menu->actions()[1],  &QAction::triggered, this, [=] { data->removeRows(row, 1);});
    menu->popup(tableView->horizontalHeader()->viewport()->mapToGlobal(pos));
}


View::View(QWidget *parent) : QWidget(parent) {
    // create simple model for storing data
    // user's table data model
    data = new Data;
    //qDebug() << data->setHeaderData(2, Qt::Horizontal, QVariant("8"), Qt::EditRole);
    // create table view and add model to it
    tableView = new QTableView;
    tableView->setModel(data);
    tableView->setMinimumWidth(100);
    //tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    data->setParent(tableView);

    tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(customHeaderMenuRequestedH(QPoint)));

    tableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(customHeaderMenuRequestedV(QPoint)));

    chartView = new QChartView;

    // create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(tableView, 1, 0);
    gridLayout->addWidget(chartView, 1, 1);
    /*mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(0, 0);*/

    addMenu(mainLayout);

    QToolBar* toolBar = new QToolBar;
    addTableControls(toolBar);
    addChartControls(toolBar, gridLayout);

    mainLayout->addWidget(toolBar);

    mainLayout->addLayout(gridLayout);

    setLayout(mainLayout);
}

void View::addMenu(QVBoxLayout* mainLayout) {

    QMenuBar* menuBar = new QMenuBar(this);

    file = new QMenu("File", menuBar);

    menuBar->addMenu(file);

    // Menù "File"
    file->addAction(new QAction("Open", file));
    file->addAction(new QAction("Save as", file));
    file->addAction(new QAction("eXIT", file));


    connect(file->actions()[0], SIGNAL(triggered()), this, SLOT(openFile()));
    connect(file->actions()[1], SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(file->actions()[2], SIGNAL(triggered()), this, SLOT(close()));

    mainLayout->addWidget(menuBar);
}

void View::openFile()
{
    const QString fileName = QFileDialog::getOpenFileName(this, "Scegli file dati", "", "*.txt");
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void View::loadFile(const QString &fileName) {
    QMessageBox msgBox;
    QFile file(fileName);
    if (fileName.isEmpty()){
        msgBox.setText("File non presente");
        msgBox.exec();
    }
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        msgBox.setText("Formato file non supportato o tipo file di sola lettura");
        msgBox.exec();
    }
    QTextStream stream(&file);

    data->removeRows(0, data->rowCount());//rimuove tutte le righe???
    data->removeColumns(0, data->columnCount());//rimuove tutte le colonne???
    data->clearMapping();
    //clearChart();?????
    //opportuno definire un nuovo model?

    //leggo prima linea file
    QString line = stream.readLine();
    QStringList dataset;
    //per eventualmente saper num righe e colonne se voglio reistanziare un nuovo modello
    if (!line.isEmpty())
        dataset = line.split(QLatin1Char(';'), Qt::SkipEmptyParts);

    int row = 0;
    while (!stream.atEnd()) {
        line = stream.readLine();
        if (!line.isEmpty()) {
            data->insertRows(row, 1);//inserisco riga vuota
            dataset = line.split(QLatin1Char(';'), Qt::SkipEmptyParts);
            for(int col = 0; col<dataset.length();col++){
                if(row == 0)
                    data->insertColumns(col, 1);//inserisco colonna vuota
                data->setData(data->index(row, col), dataset.value(col));//setto dato per ogni cella
            }
            row++;
        }
    };
    file.close();
    //statusBar()->showMessage(tr("Loaded %1").arg(fileName), 2000);
}

void View::saveFile() {
    QMessageBox msgBox;
    QString fileName = QFileDialog::getSaveFileName(this, "Save file as", "", "*.txt");

    if (fileName.isEmpty()){
        msgBox.setText("File non presente");
        msgBox.exec();
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        msgBox.setText("Formato file non supportato o tipo file di sola lettura");
        msgBox.exec();
    }

    QTextStream stream(&file);
    stream << data->rowCount() << ";" << data->columnCount() << "\n";

    for (int row = 0; row < data->rowCount(); ++row) {
        QStringList dataset;
        for (int col = 0; col < data->columnCount(); ++col)
            dataset.append(data->data(data->index(row, col), Qt::DisplayRole).toString());

        stream << dataset.join(';') << "\n";
    }

    file.close();
    //statusBar()->showMessage(tr("Saved %1").arg(fileName), 2000);
}
/*
 tolti perchè ridondanti, tutte le chiamate al modello vengono fatte direttamente con data*
void View::addRows(int row) {
    data->insertRows(row, 1);
}

void View::addColumns(int col) {
    data->insertColumns(col, 1);
}

void View::removeRows(int row) {
    data->removeRows(row, 1);
}

void View::removeColumns(int col) {
    data->removeColumns(col, 1);
}
*/
void View::addTableControls(QToolBar* toolBar) {
    toolBar->addAction(QIcon(":/images/addRow.png"), "Add Row", [=] { if(!data->insertRows(data->rowCount(),1)) { QMessageBox msgBox;msgBox.setText("Errore nell'inserimento"); msgBox.exec(); };});
    toolBar->addAction(QIcon(":/images/addColumn.png"), "Add Column", [=] { if(!data->insertColumns(data->columnCount(),1)) { QMessageBox msgBox;msgBox.setText("Errore nell'inserimento"); msgBox.exec();};});
    toolBar->addAction(QIcon(":/images/removeRow.png"), "Remove Row", [=] { if(!data->removeRows(data->rowCount()-1,1)) { QMessageBox msgBox;msgBox.setText("Errore nella rimozione"); msgBox.exec(); };});
    toolBar->addAction(QIcon(":/images/removeColumn.png"), "Remove Column", [=] { if(!data->removeColumns(data->columnCount()-1,1)) { QMessageBox msgBox;msgBox.setText("Errore nella rimozione"); msgBox.exec(); }; });
    toolBar->addSeparator();
}

void View::addBarChart(QGridLayout* mainLayout) {

    QBarSeries *series = new QBarSeries;

    QVBarModelMapper *barMapper = new QVBarModelMapper(this);

    //forse va inizializzato nel costruttore e poi cambiato qui ogni volta
    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);

    QModelIndexList indexes = tableView->selectionModel()->selectedIndexes();
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
    data->clearMapping();
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
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);

    mainLayout->addWidget(chartView, 1, 1);
}

void View::addPieChart(QGridLayout* mainLayout) {

    QPieSeries *series = new QPieSeries;

    QVPieModelMapper *pieMapper = new QVPieModelMapper(this);

    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);

    QModelIndexList indexes = tableView->selectionModel()->selectedIndexes();
    pieMapper->setLabelsColumn(indexes.first().column());
    pieMapper->setValuesColumn(indexes.last().column());
    pieMapper->setFirstRow(indexes.first().row());
    pieMapper->setRowCount((indexes.last().row()+1)-(indexes.first().row()));
    pieMapper->setSeries(series);
    pieMapper->setModel(data);
    chart->addSeries(series);

    data->clearMapping();

    QString sliceColorHex = "#000000";

    QList<QPieSlice*> slices = series->slices();
    int slicesCount = pieMapper->firstRow();
    for(auto it = slices.begin(); it != slices.end(); ++it) {
        sliceColorHex = "#" + QString::number((*it)->brush().color().rgb(), 16).right(6).toUpper();
        data->addMapping(sliceColorHex, QRect(indexes.first().column(), slicesCount,2,1));
        slicesCount++;
    }

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);

    mainLayout->addWidget(chartView, 1, 1);
}

void View::addLineChart(QGridLayout* mainLayout) {

    QLineSeries *series;

    QVXYModelMapper *lineMapper;

    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);

    QModelIndexList indexes = tableView->selectionModel()->selectedIndexes();
    int firstCol = indexes.first().column();
    int lastCol = indexes.last().column();

    QString seriesColorHex = "#000000";

    data->clearMapping();

    for(int i=firstCol+1; i<=lastCol; i++)
    {
        series = new QLineSeries;
        series->setName(QString("Line %1").arg(i));
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

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);

    mainLayout->addWidget(chartView, 1, 1);
}

void View::addChartControls(QToolBar* toolBar, QGridLayout* mainLayout) {
    toolBar->addSeparator();
    toolBar->addAction(QIcon(":/images/addColumn.png"), "Add Bar Chart", [=] { this->addBarChart(mainLayout); });
    toolBar->addAction(QIcon(":/images/addColumn.png"), "Add Pie Chart", [=] { this->addPieChart(mainLayout); });
    toolBar->addAction(QIcon(":/images/addColumn.png"), "Add Line Chart", [=] { this->addLineChart(mainLayout); });
}
