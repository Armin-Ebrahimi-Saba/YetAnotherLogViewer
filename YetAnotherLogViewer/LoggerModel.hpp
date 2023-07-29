#pragma once

#ifndef LOGGERMODEL_HPP
#define LOGGERMODEL_HPP

#include <QAbstractTableModel>
#include <QUrl>
#include <fstream>
#include <regex>
#include <QGuiApplication>
#include <QFont>
#include <QFontMetrics>

#include "Parser.hpp"
#include "Logger.hpp"

class LoggerModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(LoggerModel)

public:
    explicit LoggerModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE int columnWidth(const int index, const QFont* font = nullptr);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value,
        int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool setHeaderData(int section, Qt::Orientation orientation,
        const QVariant& value, int role = Qt::EditRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void reset(const QList<QStringList>& logs);
    Q_INVOKABLE void openFile(const QUrl& url);
    Q_INVOKABLE void fetchLogs();
    Q_INVOKABLE void setRegex(const QString& reg);
    Q_INVOKABLE void setEndpoint(const QString& ipv4,
                                const QString& portNum);
    Q_INVOKABLE void saveLogsToFile(const QString& logFile = "C:\\Users\\armin\\Desktop\\log.log") const;
    Q_INVOKABLE void setProto(const QString& selectedProto);
    Q_INVOKABLE void cleanLogs();

signals:
    void modelReloaded();

private:
    // Parser parser;
    YALV::Logger *logger;
    QList<QStringList> mLogs;
    QHash<int, QByteArray> mRoles;
    QList<int> mColumnWidth;
    QUrl logFileUrl;
    unsigned short mProto = 2;
};

#endif // LOGGERMODEL_H
