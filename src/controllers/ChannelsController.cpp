/*
 * ChannelsController.cpp
 *
 *  Created on: May 28, 2017
 *      Author: misha
 */

#include "ChannelsController.hpp"
#include <QUrl>
#include <QVariantMap>
#include <QDebug>
#include <bb/data/XmlDataAccess>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QDateTime>
#include "../models/Item.hpp"
#include "../models/Enclosure.hpp"
#include "../extenstions/Media.hpp"
#include "../Rss.hpp"

using namespace bb::data;

ChannelsController::ChannelsController(ChannelsService* channels, ItemsService* items, QObject* parent) : QObject(parent), m_channels(channels), m_items(items) {
    m_network = new QNetworkAccessManager(this);
}

ChannelsController::~ChannelsController() {
    m_network->deleteLater();
}

void ChannelsController::load(const int& tabId, const QString& url) {
    QNetworkRequest req;

    QUrl u(url);
    req.setUrl(u);

    QNetworkReply* reply = m_network->get(req);
    reply->setProperty("url", url);
    reply->setProperty("tab_id", tabId);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onLoad()));
    Q_ASSERT(res);
    res = QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void ChannelsController::onLoad() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    QString url = reply->property("url").toString();
    int tabId = reply->property("tab_id").toInt();

    QString dataStr = data.data();
    qDebug() << dataStr << endl;
    if (dataStr.contains("rss")) {
        XmlDataAccess xda;
        QVariantMap rss = xda.loadFromBuffer(data, "rss").toMap();

        if (rss.value("version").toString().compare("2.0") == 0) {
            QVariantMap channelMap = rss.value(CHANNEL).toMap();
            QString name = channelMap.value(TITLE).toString();
            Channel channel = m_channels->create(tabId, url, name);

            if (channelMap.contains(IMAGE)) {
                QVariantMap imageMap = channelMap.value(IMAGE).toMap();
                downloadLogo(channel, imageMap.value("url").toString());
            }

            QVariantList items = channelMap.value(ITEM).toList();
            foreach(QVariant itemVar, items) {
                QVariantMap itemMap = itemVar.toMap();
                itemMap["pub_date"] = itemMap.value(PUB_DATE, "").toString();
                Item item;
                item.fromMap(itemMap);
                item.setChannelId(channel.getId());
                item = m_items->create(item);

                if (itemMap.contains(ENCLOSURE)) {
                    downloadThumbnail(item, itemMap.value(ENCLOSURE).toMap().value("url").toString());
                }

                if (itemMap.contains(MEDIA_THUMBNAIL)) {
                    QVariant thumbVar = itemMap.value(MEDIA_THUMBNAIL);
                    if (thumbVar.type() == QVariant::Map) {
                        downloadThumbnail(item, thumbVar.toMap().value("url").toString());
                    } else if (thumbVar.type() == QVariant::List) {
                        QVariantList thumbs = thumbVar.toList();
                        foreach(QVariant tVar, thumbs) {
                            downloadThumbnail(item, tVar.toMap().value("url").toString());
                        }
                    }
                }

                if (itemMap.contains(MEDIA_CONTENT)) {
                    QVariant mediaVar = itemMap.value(MEDIA_CONTENT);
                    qDebug() << mediaVar << endl;
                    if (mediaVar.type() == QVariant::Map) {
                        downloadThumbnail(item, mediaVar.toMap().value("url").toString());
                    } else if (mediaVar.type() == QVariant::List) {
                        QVariantList thumbs = mediaVar.toList();
                        foreach(QVariant tVar, thumbs) {
                            downloadThumbnail(item, tVar.toMap().value("url").toString());
                        }
                    }
                }
            }
        }
    } else {
        emit invalidRssUrl(url);
    }

    reply->deleteLater();
}

void ChannelsController::onError(QNetworkReply::NetworkError e) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    qDebug() << "ChannelsController#onError: " << e << endl;
    qDebug() << "ChannelsController#onError: " << reply->errorString() << endl;

    emit invalidRssUrl(reply->property("url").toString());

    reply->deleteLater();
}

void ChannelsController::downloadLogo(const Channel& channel, const QString& url) {
    QNetworkRequest req;

    QUrl u(url);
    req.setUrl(u);

    QNetworkReply* reply = m_network->get(req);
    reply->setProperty("channel_id", channel.getId());
    reply->setProperty("url", url);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onLogoDownloaded()));
    Q_ASSERT(res);
    res = QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void ChannelsController::onLogoDownloaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    int channelId = reply->property("channel_id").toInt();
    QString url = reply->property("url").toString();

    QStringList urlParts = url.split("/");
    QString filename = urlParts[urlParts.length() - 1];

    QByteArray data = reply->readAll();

    QString dirPath = QDir::currentPath() + "/data/logos/" + QString::number(channelId);
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(dirPath);
    }

    QString filepath = dirPath + "/" + filename;
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        m_channels->updateImage(channelId, filepath);
        emit logoChanged(channelId, filepath);
        qDebug() << "Logo saved: " << filepath << endl;
    } else {
        qDebug() << "Logo error: " << file.errorString() << endl;
    }

    reply->deleteLater();
}

void ChannelsController::downloadThumbnail(const Item& item, const QString& url) {
    QNetworkRequest req;

    QUrl u(url);
    req.setUrl(u);

    QNetworkReply* reply = m_network->get(req);
    reply->setProperty("item_id", item.getId());
    reply->setProperty("url", url);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onThumbnailDownloaded()));
    Q_ASSERT(res);
    res = QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void ChannelsController::onThumbnailDownloaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    int itemId = reply->property("item_id").toInt();
    QString url = reply->property("url").toString();

    QString filename = url.split("/").last();

    QByteArray data = reply->readAll();

    QString dirPath = QDir::currentPath() + "/data/thumbnails/" + QString::number(itemId);
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(dirPath);
    }

    QString filepath = dirPath + "/" + filename;
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        Enclosure enclosure;
        enclosure.setUrl(url);
        enclosure.setPath(filepath);
        enclosure.setLength(file.size());
        enclosure.setType("image/" + filename.split(".").last());
        enclosure = m_items->addEnclosure(itemId, enclosure);
        emit thumbnailChanged(itemId, enclosure.toMap());
//        qDebug() << "Thumbnaul saved: " << filepath << endl;
    } else {
        qDebug() << "Thumbnaul error: " << file.errorString() << endl;
    }

    reply->deleteLater();
}
