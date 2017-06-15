/*
 * Channel.hpp
 *
 *  Created on: May 31, 2017
 *      Author: misha
 */

#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <QObject>
#include <QVariantMap>

class Channel: public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int tabId READ getTabId WRITE setTabId NOTIFY tabIdChanged)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString link READ getLink WRITE setLink NOTIFY linkChanged)
    Q_PROPERTY(QString image READ getImage WRITE setImage NOTIFY imageChanged)
public:
    Channel(QObject* parent = 0);
    Channel(const Channel& channel);
    virtual ~Channel();

    Channel& operator=(const Channel& channel);

    Q_INVOKABLE int getId() const;
    Q_INVOKABLE Channel& setId(const int& id);

    Q_INVOKABLE int getTabId() const;
    Q_INVOKABLE Channel& setTabId(const int& id);

    Q_INVOKABLE const QString& getTitle() const;
    Q_INVOKABLE Channel& setTitle(const QString& title);

    Q_INVOKABLE const QString& getLink() const;
    Q_INVOKABLE Channel& setLink(const QString& link);

    Q_INVOKABLE const QString& getImage() const;
    Q_INVOKABLE Channel& setImage(const QString& image);

    QVariantMap toMap();
    Channel& fromMap(const QVariantMap& map);

Q_SIGNALS:
    void idChanged(const int& id);
    void tabIdChanged(const int& tabId);
    void titleChanged(const QString& title);
    void linkChanged(const QString& link);
    void imageChanged(const QString& image);

private:
    int m_id;
    int m_tabId;
    QString m_title;
    QString m_link;
    QString m_image;

    void swap(const Channel& channel);
};

#endif /* CHANNEL_HPP_ */
