/*
 * ChannelsService.cpp
 *
 *  Created on: May 27, 2017
 *      Author: misha
 */

#include "ChannelsService.hpp"
#include "../config/DBConfig.hpp"

ChannelsService::ChannelsService(QObject* parent) : QObject(parent) {}

ChannelsService::~ChannelsService() {}

QVariantMap ChannelsService::findById(const int& id) {
    return DBConfig::execute(QString("SELECT * FROM channels WHERE id = %1").arg(id)).toList().at(0).toMap();
}

QVariantList ChannelsService::findForTab(const int& tabId) {
    return DBConfig::execute(QString("SELECT * FROM channels WHERE tab_id = %1").arg(tabId)).toList();
}

Channel ChannelsService::create(const int& tabId, const QString& link, const QString& title) {
    QVariantMap data;
    data["tab_id"] = tabId;
    data["link"] = link;
    data["title"] = title;
    DBConfig::execute("INSERT INTO channels (tab_id, link, title) VALUES (:tab_id, :link, :title)", data);

    QVariantMap createdChannel = lastCreated();

    Channel channel;
    channel.fromMap(createdChannel);
    emit created(createdChannel);
    return channel;
}

void ChannelsService::remove(const int& id) {
    DBConfig::execute(QString("DELETE FROM channels WHERE id = %1").arg(id));
    emit removed(id);
}

QVariantMap ChannelsService::lastCreated() {
    return DBConfig::execute("SELECT * FROM channels ORDER BY id DESC LIMIT 1").toList().at(0).toMap();
}

void ChannelsService::updateImage(const int& channelId, const QString& imagePath) {
    QVariantMap values;
    values["image"] = imagePath;
    values["id"] = channelId;
    DBConfig::execute(QString("UPDATE channels SET image = :image WHERE id = :id"), values);
}
