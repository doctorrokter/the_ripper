/*
 * Enclosure.hpp
 *
 *  Created on: May 31, 2017
 *      Author: misha
 */

#ifndef ENCLOSURE_HPP_
#define ENCLOSURE_HPP_

#include <QObject>
#include <QVariantMap>

class Enclosure: public QObject {
    Q_OBJECT
public:
    Enclosure(QObject* parent = 0);
    Enclosure(const Enclosure& enclosure);
    virtual ~Enclosure();

    Enclosure& operator=(const Enclosure& enclosure);
    bool operator==(const Enclosure& enclosure);

    Q_INVOKABLE int getId() const;
    Q_INVOKABLE Enclosure& setId(const int& id);

    Q_INVOKABLE int getItemId() const;
    Q_INVOKABLE Enclosure& setItemId(const int& itemId);

    Q_INVOKABLE const QString& getUrl() const;
    Q_INVOKABLE Enclosure& setUrl(const QString& url);

    Q_INVOKABLE const QString& getPath() const;
    Q_INVOKABLE Enclosure& setPath(const QString& path);

    Q_INVOKABLE int getLength() const;
    Q_INVOKABLE Enclosure& setLength(const int& length);

    Q_INVOKABLE const QString& getType() const;
    Q_INVOKABLE Enclosure& setType(const QString& type);

    QVariantMap toMap();
    Enclosure& fromMap(const QVariantMap& map);

private:
    int m_id;
    int m_itemId;
    QString m_url;
    QString m_path;
    int m_length;
    QString m_type;

    void swap(const Enclosure& enclosure);
};

#endif /* ENCLOSURE_HPP_ */
