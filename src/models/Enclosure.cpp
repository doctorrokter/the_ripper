/*
 * Enclosure.cpp
 *
 *  Created on: May 31, 2017
 *      Author: misha
 */

#include <src/models/Enclosure.hpp>

Enclosure::Enclosure(QObject* parent) : QObject(parent), m_id(0), m_itemId(0), m_url(""), m_path(""), m_length(0), m_type("") {}

Enclosure::Enclosure(const Enclosure& enclosure) : QObject(enclosure.parent()) {
    swap(enclosure);
}

Enclosure::~Enclosure() {}

Enclosure& Enclosure::operator=(const Enclosure& enclosure) {
    swap(enclosure);
    return *this;
}

bool Enclosure::operator==(const Enclosure& enclosure) {
    return m_id == enclosure.getId();
}

int Enclosure::getId() const { return m_id; }
Enclosure& Enclosure::setId(const int& id) {
    if (m_id != id) {
        m_id = id;
    }
    return *this;
}

int Enclosure::getItemId() const { return m_itemId; }
Enclosure& Enclosure::setItemId(const int& itemId) {
    if (m_itemId != itemId) {
        m_itemId = itemId;
    }
    return *this;
}

const QString& Enclosure::getUrl() const { return m_url; }
Enclosure& Enclosure::setUrl(const QString& url) {
    if (m_url.compare(url) != 0) {
        m_url = url;
    }
    return *this;
}

const QString& Enclosure::getPath() const { return m_path; }
Enclosure& Enclosure::setPath(const QString& path) {
    if (m_path.compare(path) != 0) {
        m_path = path;
    }
    return *this;
}

int Enclosure::getLength() const { return m_length; }
Enclosure& Enclosure::setLength(const int& length) {
    if (m_length != length) {
        m_length = length;
    }
    return *this;
}

const QString& Enclosure::getType() const { return m_type; }
Enclosure& Enclosure::setType(const QString& type) {
    if (m_type.compare(type) != 0) {
        m_type = type;
    }
    return *this;
}

QVariantMap Enclosure::toMap() {
    QVariantMap map;
    map["id"] = m_id;
    map["item_id"] = m_itemId;
    map["url"] = m_url;
    map["path"] = m_path;
    map["length"] = m_length;
    map["type"] = m_type;
    return map;
}

Enclosure& Enclosure::fromMap(const QVariantMap& map) {
    m_id = map.value("id", 0).toInt();
    m_itemId = map.value("item_id", 0).toInt();
    m_url = map.value("url", "").toString();
    m_length = map.value("length", 0).toInt();
    m_type = map.value("type", "").toString();
    return *this;
}

void Enclosure::swap(const Enclosure& enclosure) {
    m_id = enclosure.getId();
    m_itemId = enclosure.getItemId();
    m_url = enclosure.getUrl();
    m_path = enclosure.getPath();
    m_length = enclosure.getLength();
    m_type = enclosure.getType();
}
