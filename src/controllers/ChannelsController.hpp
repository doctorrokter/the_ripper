/*
 * ChannelsController.hpp
 *
 *  Created on: May 28, 2017
 *      Author: misha
 */

#ifndef CHANNELSCONTROLLER_HPP_
#define CHANNELSCONTROLLER_HPP_

#include <QObject>
#include "../services/ChannelsService.hpp"
#include "../services/ItemsService.hpp"
#include "../models/Channel.hpp"
#include "../models/Item.hpp"
#include "../models/Enclosure.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ChannelsController: public QObject {
    Q_OBJECT
public:
    ChannelsController(ChannelsService* channels, ItemsService* items, QObject* parent = 0);
    virtual ~ChannelsController();

    Q_INVOKABLE void load(const int& tabId, const QString& url);

Q_SIGNALS:
    void loaded(const int& id);
    void error(const int& id);
    void invalidRssUrl(const QString& url);
    void logoChanged(const int& channelId, const QString& imagePath);
    void thumbnailChanged(const int& itemId, const QVariantMap& enclosure);

private Q_SLOTS:
    void onLoad();
    void onError(QNetworkReply::NetworkError e);
    void onLogoDownloaded();
    void onThumbnailDownloaded();

private:
    QNetworkAccessManager* m_network;
    ChannelsService* m_channels;
    ItemsService* m_items;

    void downloadLogo(const Channel& channel, const QString& url);
    void downloadThumbnail(const Item& item, const QString& url);
};

#endif /* CHANNELSCONTROLLER_HPP_ */
