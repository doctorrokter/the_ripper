/*
 * TabsController.hpp
 *
 *  Created on: Jun 14, 2017
 *      Author: misha
 */

#ifndef TABSCONTROLLER_HPP_
#define TABSCONTROLLER_HPP_

#include <QObject>
#include <QVariantMap>
#include "../services/TabsService.hpp"

class TabsController: public QObject {
    Q_OBJECT
public:
    TabsController(TabsService* tabsService, QObject* parent = 0);
    virtual ~TabsController();

    Q_INVOKABLE void requestTabChanging(const QVariantMap& tab);
    Q_INVOKABLE void updateTab(const int& id, const QString& name, const QString& icon);
    Q_INVOKABLE void rearrangeTabs(const QVariantList& idsIndexes);

    Q_SIGNALS:
        void tabChangingRequested(const QVariantMap& tab);
        void tabUpdated(const QVariantMap& tab);
        void tabsRearranged();

private:
    TabsService* m_tabs;
};

#endif /* TABSCONTROLLER_HPP_ */
