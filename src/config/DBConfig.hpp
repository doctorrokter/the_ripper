/*
 * DBConfig.hpp
 *
 *  Created on: Feb 20, 2017
 *      Author: misha
 */

#ifndef DBCONFIG_HPP_
#define DBCONFIG_HPP_

#include <QtCore/QObject>
#include <bb/data/SqlDataAccess>
#include <QtSql/QtSql>
#include <QVariant>
#include <QVariantMap>

using namespace bb::data;

class DBConfig: public QObject {
    Q_OBJECT
public:
    DBConfig(QObject* parent = 0);
    virtual ~DBConfig();

    SqlDataAccess* connection();
    bool isNewDb() const;

    static QVariant execute(const QString& query);
    static QVariant execute(const QString& query, const QVariantMap& values);

private:
    static SqlDataAccess* m_pSda;

    QSqlDatabase m_database;
    bool m_newDb;

    void runMigration(const QString& path);
    bool hasVersion(const int version);
    void setVersion(const int version);
    int getVersion();
    bool hasSchemaVersionTable();
    int getMigrationVersion(const QString& path);
    void migrate();
    int maxMigrationVersion();
};

#endif /* DBCONFIG_HPP_ */
