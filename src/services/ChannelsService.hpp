/*
 * ChannelsService.hpp
 *
 *  Created on: May 27, 2017
 *      Author: misha
 */

#ifndef CHANNELSSERVICE_HPP_
#define CHANNELSSERVICE_HPP_

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include "../models/Channel.hpp"

class ChannelsService: public QObject {
    Q_OBJECT
public:
    ChannelsService(QObject* parent = 0);
    virtual ~ChannelsService();

    Q_INVOKABLE QVariantMap findById(const int& id);
    Q_INVOKABLE QVariantList findForTab(const int& tabId);
    Q_INVOKABLE Channel create(const int& tabId, const QString& link, const QString& title = "");
    Q_INVOKABLE void remove(const int& id);
    Q_INVOKABLE QVariantMap lastCreated();
    Q_INVOKABLE void updateImage(const int& channelId, const QString& imagePath);

    Q_SIGNALS:
        void created(const QVariantMap& channel);
        void removed(const int& id);
};

#endif /* CHANNELSSERVICE_HPP_ */
