/*
 * ItemsController.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: misha
 */

#include "ItemsController.hpp"
#include <QVariantMap>

ItemsController::ItemsController(ItemsService* items, QObject* parent) : QObject(parent), m_items(items) {}
ItemsController::~ItemsController() {}

void ItemsController::readItem(const int& id, const int& tabId, const QVariantList& indexPath) {
    m_items->read(id);
    QVariantMap itemMap = m_items->findById(id);
    emit readItemRequested(itemMap, tabId, indexPath);
}
