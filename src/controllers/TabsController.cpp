/*
 * TabsController.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: misha
 */

#include <src/controllers/TabsController.hpp>

TabsController::TabsController(TabsService* tabsService, QObject* parent) : QObject(parent), m_tabs(tabsService) {}

TabsController::~TabsController() {}

void TabsController::requestTabChanging(const QVariantMap& tab) {
    emit tabChangingRequested(tab);
}

void TabsController::updateTab(const int& id, const QString& name, const QString& icon) {
    QVariantMap tab = m_tabs->update(id, name, icon);
    emit tabUpdated(tab);
}

void TabsController::rearrangeTabs(const QVariantList& idsIndexes) {
    foreach(QVariant var, idsIndexes) {
        QVariantMap map = var.toMap();
        m_tabs->updateIndex(map.value("id").toInt(), map.value("index").toInt());
    }
    emit tabsRearranged();
}
