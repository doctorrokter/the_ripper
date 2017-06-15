/*
 * ItemsService.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: misha
 */

#include "ItemsService.hpp"
#include "../config/DBConfig.hpp"

ItemsService::ItemsService(QObject* parent) : QObject(parent), m_readStatus(0) {}

ItemsService::~ItemsService() {}

QVariantList ItemsService::findAll() {
    return DBConfig::execute("SELECT * FROM items").toList();
}

QVariantMap ItemsService::findById(const int& id) {
    return DBConfig::execute(QString("SELECT * FROM items WHERE id = %1").arg(QString::number(id))).toList().at(0).toMap();
}

QVariantList ItemsService::findForChannel(const int& channelId) {
    return DBConfig::execute(QString("SELECT * FROM items WHERE channel_id = %1 AND status = %2").arg(QString::number(channelId)).arg(QString::number(Item::ACTIVE))).toList();
}

QVariantList ItemsService::findForChannels(const QVariantList& channelsIds) {
    QString ids;
    for (int i = 0; i < channelsIds.size(); i++) {
        ids.append(channelsIds.at(i).toString());
        if (i < channelsIds.size() - 1) {
            ids.append(",");
        }
    }
    return DBConfig::execute(QString("SELECT * FROM items WHERE channel_id IN (%1) AND status = %2").arg(ids).arg(QString::number(Item::ACTIVE))).toList();
}

Item ItemsService::create(Item& item) {
    QString query = "INSERT INTO items (channel_id, title, link, description, author, category, comments, guid, pub_date, source, status) "
            "VALUES (:channel_id, :title, :link, :description, :author, :category, :comments, :guid, :pub_date, :source, :status)";
    DBConfig::execute(query, item.toMap());

    QVariantMap last = lastCreated();
    emit created(last);
    return item.fromMap(last);
}

void ItemsService::remove(const int& id, const QVariantList& indexPath) {
    DBConfig::execute(QString("UPDATE items SET status = %1 WHERE id = %2").arg(QString::number(Item::DELETED)).arg(QString::number(id)));
    emit removed(id, indexPath);
}

void ItemsService::deleteItem(const int& id) {
    DBConfig::execute(QString("DELETE FROM items WHERE id = %1").arg(QString::number(id)));
    emit deleted(id);
}

void ItemsService::read(const int& id) {
    DBConfig::execute(QString("UPDATE items SET read = 1 WHERE id = %1").arg(QString::number(id)));
}

QVariantMap ItemsService::lastCreated() {
    return DBConfig::execute("SELECT * FROM items ORDER BY id DESC LIMIT 1").toList().at(0).toMap();
}

QVariantMap ItemsService::lastCreatedEnclosure() {
    return DBConfig::execute("SELECT * FROM enclosures ORDER BY id DESC LIMIT 1").toList().at(0).toMap();
}

Enclosure ItemsService::addEnclosure(const int& itemId, Enclosure& enclosure) {
    QString query = "INSERT INTO enclosures (item_id, url, path, length, type) VALUES(:item_id, :url, :path, :length, :type)";
    enclosure.setItemId(itemId);

    DBConfig::execute(query, enclosure.toMap());
    return enclosure.fromMap(lastCreatedEnclosure());
}

QVariantList ItemsService::findEnclosures(const int& itemId) {
    return DBConfig::execute(QString("SELECT * FROM enclosures WHERE item_id = %1").arg(QString::number(itemId))).toList();
}

int ItemsService::getReadStatus() const { return m_readStatus; }
void ItemsService::setReadStatus(const int& readStatus) {
    if (m_readStatus != readStatus) {
        m_readStatus = readStatus;
        emit readStatusChanged(m_readStatus);
    }
}
