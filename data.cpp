#include "data.h"

Data::Data(int m_c, int m_r, QObject *parent) :
    QAbstractTableModel(parent), m_columnCount(m_c), m_rowCount(m_r)
{
    for (int i = 0; i < m_rowCount; i++) {
        QVector<qreal>* data = new QVector<qreal>(m_columnCount);
        m_data.append(data);
        ver_headers.insert(i, QString("%1").arg(i + 1));
    }
    for (int i = 0; i < m_columnCount; i++)
        hor_headers.insert(i, QString("%1").arg(i + 1));
}

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
        //QString("%1").arg(section + 1);
        return (section < hor_headers.length() ? hor_headers[section] : QVariant());
        //return QString("%1").arg(section + 1);
    }else{
        return (section < ver_headers.length() ? ver_headers[section] : QVariant());
        //return ver_headers[section];
        //return QString("%1").arg(section + 1);
    }
}
bool Data:: setHeaderData(int section, Qt::Orientation orientation,const QVariant &value, int role){
    if (role != Qt::EditRole && role != Qt::DisplayRole)
        return false;
    if(orientation == Qt::Horizontal)
        if(section<hor_headers.length()){
            hor_headers.replace(section,value.toString());
            emit headerDataChanged(Qt::Horizontal, 0, hor_headers.length());
            return true;
        }else return false;
    else
        if(section<ver_headers.length()){
            ver_headers.replace(section,value.toString());
            emit headerDataChanged(Qt::Vertical, 0, ver_headers.length());
            return true;
        }else return false;
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

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
    //aggiorno vertical header manualmente
    ver_headers.insert(position, QString("%1").arg(position+1));
    endInsertRows();
    return true;

}

bool Data::removeRows(int position, int rows, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(position >= 0 && position<rowCount()){
        beginRemoveRows(QModelIndex(), position, position+rows-1);

        for (int row = 0; row < rows; ++row) {
            m_data.removeAt(position);
        }

        m_rowCount -= rows;
        //aggiorno vertical header manualmente
        ver_headers.removeAt(position);
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
    //aggiorno horizontal header manualmente
    hor_headers.insert(position, QString("%1").arg(position+1));
    return true;
}

bool Data::removeColumns(int position, int columns, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(position >= 0 && position<columnCount()){
        beginRemoveColumns(QModelIndex(), position, position+columns-1);

        for(int row = 0; row < m_rowCount; ++row) {
            m_data[row]->remove(position, columns);
        }

        m_columnCount -= columns;
        //aggiorno horizontal header manualmente
        hor_headers.removeAt(position);
        endRemoveColumns();
        return true;
    }
    return false;
}

void Data::addMapping(QString color, QRect area) {
    m_mapping.insert(color, area);
}
void Data::clearMapping() {
    m_mapping.clear();
}

