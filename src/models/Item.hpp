/*
 * Item.hpp
 *
 *  Created on: May 31, 2017
 *      Author: misha
 */

#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <QObject>
#include <QVariantMap>

class Item: public QObject {
    Q_OBJECT
    Q_ENUMS(Status)
public:
    enum Status {
        ACTIVE = 0,
        DELETED
    };

    Item(QObject* parent = 0);
    Item(const Item& item);
    virtual ~Item();

    Item& operator=(const Item& item);
    bool operator==(const Item& item);

    Q_INVOKABLE int getId() const;
    Q_INVOKABLE Item& setId(const int& id);

    Q_INVOKABLE int getChannelId() const;
    Q_INVOKABLE Item& setChannelId(const int& channelId);

    Q_INVOKABLE const QString& getTitle() const;
    Q_INVOKABLE Item& setTitle(const QString& title);

    Q_INVOKABLE const QString& getLink() const;
    Q_INVOKABLE Item& setLink(const QString& link);

    Q_INVOKABLE const QString& getDescription() const;
    Q_INVOKABLE Item& setDescription(const QString& description);

    Q_INVOKABLE const QString& getAuthor() const;
    Q_INVOKABLE Item& setAuthor(const QString& author);

    Q_INVOKABLE const QString& getCategory() const;
    Q_INVOKABLE Item& setCategory(const QString& category);

    Q_INVOKABLE const QString& getComments() const;
    Q_INVOKABLE Item& setComments(const QString& comments);

    Q_INVOKABLE const QString& getGuid() const;
    Q_INVOKABLE Item& setGuid(const QString& guid);

    Q_INVOKABLE const QString& getPubDate() const;
    Q_INVOKABLE Item& setPubDate(const QString& pubDate);

    Q_INVOKABLE const QString& getSource() const;
    Q_INVOKABLE Item& setSource(const QString& source);

    Q_INVOKABLE bool isRead() const;
    Q_INVOKABLE Item& setRead(const int& read);

    Q_INVOKABLE int getStatus() const;
    Q_INVOKABLE Item& setStatus(const int& status);

    QVariantMap toMap();
    Item& fromMap(const QVariantMap& map);

private:
    int m_id;
    int m_channelId;
    QString m_title;
    QString m_link;
    QString m_description;
    QString m_author;
    QString m_category;
    QString m_comments;
    QString m_guid;
    QString m_pubDate;
    QString m_source;
    int m_read;
    int m_status;

    void swap(const Item& item);
};

Q_DECLARE_METATYPE(Item::Status)

#endif /* ITEM_HPP_ */
