/*
 * TabsService.hpp
 *
 *  Created on: May 27, 2017
 *      Author: misha
 */

#ifndef TABSSERVICE_HPP_
#define TABSSERVICE_HPP_

#include <QObject>
#include <QVariantList>
#include <QVariantMap>

class TabsService: public QObject {
    Q_OBJECT
public:
    TabsService(QObject* parent = 0);
    virtual ~TabsService();

    Q_INVOKABLE QVariantList findAll();
    Q_INVOKABLE QVariantMap findById(const int& id);
    Q_INVOKABLE void create(const QString& name, const QString& icon);
    Q_INVOKABLE QVariantMap update(const int& id, const QString& name, const QString& icon);
    Q_INVOKABLE void remove(const int& id);
    Q_INVOKABLE QVariantMap lastCreated();
    Q_INVOKABLE bool isExists(const int& id);
    Q_INVOKABLE int count();
    Q_INVOKABLE int unreadCount(const int& id) const;

    void updateIndex(const int& id, const int& index);

    Q_SIGNALS:
        void created(const QVariantMap& tab);
        void removed(const int& id);

private:

    int maxTheIndex();
};

#endif /* TABSSERVICE_HPP_ */
