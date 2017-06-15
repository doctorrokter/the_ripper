/*
 * DBConfig.cpp
 *
 *  Created on: Feb 20, 2017
 *      Author: misha
 */

#include "DBConfig.hpp"
#include "AppConfig.hpp"
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include "AppConfig.hpp"
#include "../Common.hpp"

SqlDataAccess* DBConfig::m_pSda = 0;

DBConfig::DBConfig(QObject* parent) : QObject(parent) {
     m_newDb = AppConfig::getStatic("db_created").toString().isEmpty();

     QString dbpath = QDir::currentPath() + "/data" + QString::fromLatin1("/").append(DB_NAME);
     m_database = QSqlDatabase::addDatabase("QSQLITE");
     m_database.setDatabaseName(dbpath);
     m_database.open();

     m_pSda = new SqlDataAccess(dbpath, this);
     m_pSda->execute("PRAGMA encoding = \"UTF-8\"");
     m_pSda->execute("PRAGMA foreign_keys = ON");

     if (m_newDb) {
         qDebug() << "Create DB from scratch" << endl;
         migrate();
         AppConfig::setStatic("db_created", "true");
     } else {
         qDebug() << "DB already exists. Use one." << endl;
         if (AppConfig::getStatic("db_migrated").toString().isEmpty()) {
             migrate();
             AppConfig::setStatic("db_migrated", "true");
         }

         if (maxMigrationVersion() > getVersion()) {
             migrate();
         } else {
             qDebug() << "DB versions matches!" << endl;
         }
         qDebug() << "Current DB version is: " << getVersion() << endl;
     }
}

DBConfig::~DBConfig() {
    m_database.close();
    delete m_pSda;
    m_pSda = NULL;
}

SqlDataAccess* DBConfig::connection() {
    return m_pSda;
}

bool DBConfig::isNewDb() const { return m_newDb; }

QVariant DBConfig::execute(const QString& query) {
    qDebug() << "===>>> SQL: " << query << endl;
    return m_pSda->execute(query);
}

QVariant DBConfig::execute(const QString& query, const QVariantMap& values) {
    qDebug() << "===>>> SQL: " << query << " VALUES: " << values << endl;
    return m_pSda->execute(query, values);
}

void DBConfig::runMigration(const QString& path) {
    qDebug() << "Process migration: " << path << endl;
    int version = getMigrationVersion(path);

    QFile migration(QString::fromLatin1(MIGRATIONS_PATH) + "/" + path);
    migration.open(QIODevice::ReadOnly);
    QString data = migration.readAll();

    qDebug() << data << endl;

    QStringList statements = data.split(";");
    foreach(QString stmt, statements) {
        if (!stmt.isEmpty()) {
            m_pSda->execute(stmt);
            if (m_pSda->hasError()) {
                qDebug() << "Sql Exception: " << m_pSda->error() << endl;
            }
        }
    }

    setVersion(version);
}

bool DBConfig::hasVersion(const int version) {
    return m_pSda->execute(QString::fromLatin1("SELECT EXISTS (SELECT 1 FROM schema_version WHERE version = %1 LIMIT 1) AS exists").arg(version))
            .toList().first().toMap().value("exists").toInt() != 0;
}

void DBConfig::setVersion(const int version) {
    m_pSda->execute(QString::fromLatin1("INSERT INTO schema_version (version) VALUES (%1)").arg(version));
}

int DBConfig::getVersion() {
    QVariantList res = m_pSda->execute("SELECT version FROM schema_version ORDER BY version DESC LIMIT 1").toList();
    return res.empty() ? 0 : res.first().toMap().value("version").toInt();
}

bool DBConfig::hasSchemaVersionTable() {
    return m_pSda->execute("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'schema_version'").toList().size() != 0;
}

int DBConfig::getMigrationVersion(const QString& path) {
    return path.split("_")[0].split("/").last().toInt();
}

void DBConfig::migrate() {
    QDir dir(MIGRATIONS_PATH);
    int currVersion = 0;

    if (hasSchemaVersionTable()) {
        currVersion = getVersion();
    }

    dir.setSorting(QDir::Name);
    QStringList paths = dir.entryList();
    foreach(QString path, paths) {
        qDebug() << path << endl;
        if (path.endsWith(".sql")) {
            if (getMigrationVersion(path) > currVersion) {
                qDebug() << "Found new migration" << endl;
                runMigration(path);
            }
        }
    }
}

int DBConfig::maxMigrationVersion() {
    QDirIterator iter("app/native/assets/migrations", QDirIterator::NoIteratorFlags);
    int startVersion = 0;
    while (iter.hasNext()) {
        QString path = iter.next();
        if (path.endsWith(".sql")) {
            int v = getMigrationVersion(path);
            if (v > startVersion) {
                startVersion = v;
            }
        }
    }
    return startVersion;
}

