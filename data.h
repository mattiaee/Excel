#ifndef DATA_H
#define DATA_H
#include <QWidget>
#include <QAbstractTableModel>
#include <QRandomGenerator>

class Data : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<QVector<qreal> * > m_data;
    QHash<QString, QRect> m_mapping;
    int m_columnCount;
    int m_rowCount;
public:
    explicit Data(int m_c=0, int m_r=0, QObject *parent = 0);
    virtual ~Data();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool insertColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());

    void addMapping(QString color, QRect area);
    void clearMapping();

    //void setView(View* v);
};

#endif // DATA_H
