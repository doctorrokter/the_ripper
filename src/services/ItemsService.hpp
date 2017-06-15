/*
 * ItemsService.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: misha
 */

#ifndef ITEMSSERVICE_HPP_
#define ITEMSSERVICE_HPP_

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include "../models/Item.hpp"
#include "../models/Enclosure.hpp"

class ItemsService: public QObject {
    Q_OBJECT
    Q_PROPERTY(int readStatus READ getReadStatus WRITE setReadStatus NOTIFY readStatusChanged)
public:
    ItemsService(QObject* parent = 0);
    virtual ~ItemsService();

    Q_INVOKABLE QVariantList findAll();
    Q_INVOKABLE QVariantMap findById(const int& id);
    Q_INVOKABLE QVariantList findForChannel(const int& channelId);
    Q_INVOKABLE QVariantList findForChannels(const QVariantList& channelsIds);
    Q_INVOKABLE Item create(Item& item);
    Q_INVOKABLE void remove(const int& id, const QVariantList& indexPath);
    Q_INVOKABLE void deleteItem(const int& id);
    Q_INVOKABLE void read(const int& id);
    Q_INVOKABLE QVariantMap lastCreated();
    Q_INVOKABLE QVariantMap lastCreatedEnclosure();
    Q_INVOKABLE Enclosure addEnclosure(const int& itemId, Enclosure& enclosure);
    Q_INVOKABLE QVariantList findEnclosures(const int& itemId);

    Q_INVOKABLE int getReadStatus() const;
    Q_INVOKABLE void setReadStatus(const int& readStatus);

    Q_SIGNALS:
        void created(const QVariantMap& item);
        void removed(const int& id, const QVariantList& indexPath);
        void deleted(const int& id);
        void readStatusChanged(const int& readStatus);

private:
    int m_readStatus;
};

#endif /* ITEMSSERVICE_HPP_ */
