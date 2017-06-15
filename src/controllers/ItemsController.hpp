/*
 * ItemsController.hpp
 *
 *  Created on: Jun 12, 2017
 *      Author: misha
 */

#ifndef ITEMSCONTROLLER_HPP_
#define ITEMSCONTROLLER_HPP_

#include <QObject>
#include <QVariantMap>
#include "../services/ItemsService.hpp"

class ItemsController: public QObject {
    Q_OBJECT
public:
    ItemsController(ItemsService* items, QObject* parent = 0);
    virtual ~ItemsController();

    Q_INVOKABLE void readItem(const int& id, const int& tabId, const QVariantList& indexPath);

    Q_SIGNALS:
        void readItemRequested(const QVariantMap& itemMap, const int& tabId, const QVariantList& indexPath);

private:
    ItemsService* m_items;
};

#endif /* ITEMSCONTROLLER_HPP_ */
