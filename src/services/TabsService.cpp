/*
 * TabsService.cpp
 *
 *  Created on: May 27, 2017
 *      Author: misha
 */

#include "TabsService.hpp"
#include "../config/DBConfig.hpp"

TabsService::TabsService(QObject* parent) : QObject(parent) {}

TabsService::~TabsService() {}

QVariantList TabsService::findAll() {
    return DBConfig::execute("SELECT * FROM tabs ORDER BY the_index").toList();
}

QVariantMap TabsService::findById(const int& id) {
    QString query = QString("SELECT * FROM tabs WHERE id = %1").arg(QString::number(id));
    return DBConfig::execute(query).toList().at(0).toMap();
}

void TabsService::create(const QString& name, const QString& icon) {
    QVariantMap data;
    data["name"] = name;
    data["icon"] = icon;
    data["the_index"] = (maxTheIndex() + 1);

    DBConfig::execute("INSERT INTO tabs (name, icon, the_index) VALUES (:name, :icon, :the_index)", data);
    emit created(lastCreated());
}

QVariantMap TabsService::update(const int& id, const QString& name, const QString& icon) {
    QVariantMap tab = findById(id);
    tab["name"] = name;
    tab["icon"] = icon;

    QVariantMap data;
    data["id"] = id;
    data["name"] = name;
    data["icon"] = icon;

    DBConfig::execute("UPDATE tabs SET name = :name, icon = :icon WHERE id = :id", data);
    return tab;
}

void TabsService::remove(const int& id) {
    DBConfig::execute(QString("DELETE FROM tabs WHERE id = %1").arg(QString::number(id)));
    emit removed(id);
}

QVariantMap TabsService::lastCreated() {
    return DBConfig::execute("SELECT * FROM tabs ORDER BY id DESC LIMIT 1").toList().at(0).toMap();
}

bool TabsService::isExists(const int& id) {
    return DBConfig::execute(QString("SELECT EXISTS (SELECT 1 FROM tabs WHERE id = %1) AS present").arg(id)).toList().at(0).toMap().value("present").toBool();
}

int TabsService::maxTheIndex() {
    return DBConfig::execute("SELECT MAX(the_index) AS the_index FROM tabs").toList().at(0).toMap().value("the_index").toInt();
}

int TabsService::count() {
    return DBConfig::execute("SELECT COUNT(*) AS count FROM tabs").toList().at(0).toMap().value("count").toInt();
}

int TabsService::unreadCount(const int& id) const {
    return DBConfig::execute(QString("SELECT COUNT(*) AS count FROM items i WHERE "
            "i.channel_id IN (SELECT id FROM channels WHERE tab_id = %1) AND i.read = 0").arg(QString::number(id)))
    .toList().at(0).toMap().value("count").toInt();
}

void TabsService::updateIndex(const int& id, const int& index) {
    DBConfig::execute(QString("UPDATE tabs SET the_index = %1 WHERE id = %2").arg(QString::number(index)).arg(QString::number(id)));
}
