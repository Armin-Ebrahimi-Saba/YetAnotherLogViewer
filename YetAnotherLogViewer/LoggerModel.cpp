#include "LoggerModel.hpp"

#define DEBUG 1

using namespace YALV;

LoggerModel::LoggerModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    Logger::init();
    logger = Logger::getInstance();
}

int LoggerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return mLogs.size();
}

int LoggerModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid() || mLogs.size() == 0)
        return 0;
    //qDebug() << "columnCount(): " << mLogs[0].size();
    return mLogs[0].size();
}

int LoggerModel::columnWidth(const int index, const QFont *font)
{
    if (index < 0 || index >= mRoles.size()) {
        // qDebug() << "columnWidht(): " << 0;
        return 0;
    }
    if (!mColumnWidth[index]) {
        QFontMetrics defaultFontMetrics = QFontMetrics(QGuiApplication::font());
        QFontMetrics fm = (font ?  QFontMetrics(*font) : defaultFontMetrics);
        int ret = fm.horizontalAdvance(headerData(index, Qt::Horizontal).toString() + QLatin1String(" ^")) + 8;
        // qDebug() << "columnWidth: " << "ret= " << ret;
        for (int i = 0; i < mLogs.count(); i++)
            ret = qMax(ret, fm.horizontalAdvance(mLogs[i][index]));
        mColumnWidth[index] = ret;
    }
    // qDebug() << "columnWidht(): " << mColumnWidth[index];
    return mColumnWidth[index];
}

QVariant LoggerModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 ||
        index.row() >= mLogs.count() ||
        index.column() >= mLogs[0].count() ||
        index.column() < 0)
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
        return mLogs[index.row()][index.column()];
        break;
    default:
        return QVariant();
    }
}

bool LoggerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::DisplayRole)
        return false;
    if (data(index, role) != value) {
        mLogs[index.row()][index.column()] = qvariant_cast<QString>(value);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}



Qt::ItemFlags LoggerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant LoggerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const int idx = Qt::UserRole + section + 1;
    if (role != Qt::DisplayRole ||
        orientation == Qt::Vertical ||
        section >= mRoles.size() || section < 0)
        return QVariant();
    // qDebug() << "debug";
    // qDebug() << "headerData(): " << "section: " << section << "    mRoles[i]: " << mRoles[idx];
    return mRoles[idx];
}

bool LoggerModel::setHeaderData(int section, Qt::Orientation orientation,
                                const QVariant &value, int role)
{
    const int idx = Qt::UserRole + section + 1;
    if (role != Qt::EditRole ||
        orientation == Qt::Vertical ||
        section >= mRoles.size() || section < 0)
        return false;
    // qDebug() << "debug";
    mRoles[idx] = qvariant_cast<QByteArray>(value);
    mColumnWidth[section] = 0;
    emit headerDataChanged(orientation, section, section);
    // qDebug() << "setHeaderData: value: " << qvariant_cast<QString>(value);
    return true;
}


QHash<int, QByteArray> LoggerModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"}, {Qt::EditRole, "edit"} };
}

void LoggerModel::reset(const QList<QStringList> &logs)
{
    if (logs.empty())
        return;
    mLogs.clear();
    mRoles.clear();
    mColumnWidth.clear();

    int rolesCount = logs.at(0).size(); // count of elements in on log line
    mColumnWidth.resize(rolesCount);
    for (int i = 1; i <= rolesCount; i++)
    {
        int roleId = Qt::UserRole + i;
        QString roleName = "Column #" + QString::number(i);
        mRoles[roleId] = roleName.toUtf8();
    }

    beginResetModel();
    mLogs = logs;
    endResetModel();
    //qDebug() << "reset(): " << "modelReloaded()";
    emit modelReloaded();
}

void LoggerModel::openFile(const QUrl &url)
{
    const QString filepath = url.toLocalFile();
    QList<QStringList> logs;
    logger->parseFile(filepath.toStdString(), logs);
    qDebug() << "openFile: reset()";
    logFileUrl = url;
    reset(logs);
}

void LoggerModel::fetchLogs()
{
    QList<QStringList> logs;
    if (mProto == 1)
        logger->fetchLogsTCP(logs);
    else
        logger->fetchLogsUDP(logs);
    reset(logs);
}

void LoggerModel::setRegex(const QString &reg)
{
    logger->setRegex(reg.toStdString());
    if (!logFileUrl.isEmpty())
        openFile(logFileUrl);
}

void LoggerModel::setEndpoint(const QString& ipv4, const QString& portNum)
{
#if DEBUG
    qDebug() << "setting Endpoint to: " << ipv4 << ":" << portNum << "\n" << __FILE__ << ": " << __LINE__;
#endif

    logger->setEndpoint(ipv4.toStdString(), portNum.toUShort());
}

void LoggerModel::saveLogsToFile(const QString& logFile) const
{
    logger->saveLogsToFile(mLogs, logFile.toStdString());
}

void LoggerModel::setProto(const QString& selectedProto)
{
    mProto = selectedProto.toUShort();
}

Q_INVOKABLE void LoggerModel::cleanLogs()
{
    //qDebug() << "cleaning the table";
    reset(QList<QStringList>{{}});
}

