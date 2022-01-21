#include "data.h"

Data::Data(int m_c, int m_r, QObject *parent) :
    QAbstractTableModel(parent), m_columnCount(m_c), m_rowCount(m_r)
{
    for (int i = 0; i < m_rowCount; i++) {
        QVector<qreal>* data = new QVector<qreal>(m_columnCount);
        m_data.append(data);
    }
}

//void Model::setView(View* v) {view = v;}

Data::~Data()
{
    qDeleteAll(m_data);
}

int Data::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int Data::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant Data::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal){
        return QString("Col %1").arg(section + 1);
    }else{
        return QString("Row %1").arg(section + 1);
    }
}

/*bool Data::setHeaderData(int section, Qt::Orientation orientation,const QVariant &value, int role)
{
    qDebug()<<role;
    if (role != Qt::DisplayRole)
        return false;

    if(orientation == Qt::Horizontal){
        h_headers.at(section)->setValue(value);
        qDebug() << h_headers.at(section)->toString();
    }else{
        v_headers.at(section)->setValue(value);
        qDebug() << v_headers.at(section)->toString();
    }
    bool result = QAbstractItemModel::setHeaderData(section, orientation, value);
    qDebug()<< result;
    if (result)
        emit headerDataChanged(orientation, 0, this->columnCount());

    return true;
}*/

QVariant Data::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::BackgroundRole) {
        for (const QRect &rect : m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }

        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool Data::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()]->replace(index.column(), value.toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags Data::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool Data::insertRows(int position, int rows, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = position; row < position+rows; ++row) {
        m_data.insert(position, new QVector<qreal>(m_columnCount));
    }
    m_rowCount += rows;

    endInsertRows();
    return true;

}

bool Data::removeRows(int position, int rows, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(position >= 0 && position<=rowCount()){
        beginRemoveRows(QModelIndex(), position, position+rows-1);

        for (int row = 0; row < rows; ++row) {
            m_data.removeAt(position);
        }

        m_rowCount -= rows;

        endRemoveRows();
        return true;
    }
    return false;
}

bool Data::insertColumns(int position, int columns, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertColumns(QModelIndex(), position, position+columns-1);

    for(int row = 0; row < m_rowCount; ++row) {
        m_data[row]->insert(position, columns, qreal(0));
    }

    m_columnCount += columns;

    endInsertColumns();
    return true;
}

bool Data::removeColumns(int position, int columns, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(position >= 0 && position<=columnCount()){
        beginRemoveColumns(QModelIndex(), position, position+columns-1);

        for(int row = 0; row < m_rowCount; ++row) {
            m_data[row]->remove(position, columns);
        }

        m_columnCount -= columns;

        endRemoveColumns();
        return true;
    }
    return false;
}

void Data::addMapping(QString color, QRect area)
{
    m_mapping.insert(color, area);
}
void Data::clearMapping() {
    m_mapping.clear();
}

