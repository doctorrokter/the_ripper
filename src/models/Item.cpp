/*
 * Item.cpp
 *
 *  Created on: May 31, 2017
 *      Author: misha
 */

#include <src/models/Item.hpp>

Item::Item(QObject* parent) : QObject(parent), m_id(0), m_channelId(0), m_title(""), m_link(""),
    m_description(""), m_author(""), m_category(""), m_comments(""), m_guid(""),
    m_pubDate(""), m_source(""), m_read(0), m_status(0) {}

Item::Item(const Item& item) : QObject(item.parent()) {
    swap(item);
}

Item::~Item() {}

Item& Item::operator=(const Item& item) {
    swap(item);
    return *this;
}

bool Item::operator==(const Item& item) {
    return m_id == item.getId();
}

int Item::getId() const { return m_id; }
Item& Item::setId(const int& id) {
    if (m_id != id) {
        m_id = id;
    }
    return *this;
}

int Item::getChannelId() const { return m_channelId; }
Item& Item::setChannelId(const int& channelId) {
    if (m_channelId != channelId) {
        m_channelId = channelId;
    }
    return *this;
}

const QString& Item::getTitle() const { return m_title; }
Item& Item::setTitle(const QString& title) {
    if (m_title.compare(title) != 0) {
        m_title = title;
    }
    return *this;
}

const QString& Item::getLink() const { return m_link; }
Item& Item::setLink(const QString& link) {
    if (m_link.compare(link) != 0) {
        m_link = link;
    }
    return *this;
}

const QString& Item::getDescription() const { return m_description; }
Item& Item::setDescription(const QString& description) {
    if (m_description.compare(description) != 0) {
        m_description = description;
    }
    return *this;
}

const QString& Item::getAuthor() const { return m_author; }
Item& Item::setAuthor(const QString& author) {
    if (m_author.compare(author) != 0) {
        m_author = author;
    }
    return *this;
}

const QString& Item::getCategory() const { return m_category; }
Item& Item::setCategory(const QString& category) {
    if (m_category.compare(category) != 0) {
        m_category = category;
    }
    return *this;
}

const QString& Item::getComments() const { return m_comments; }
Item& Item::setComments(const QString& comments) {
    if (m_comments.compare(comments) != 0) {
        m_comments = comments;
    }
    return *this;
}

const QString& Item::getGuid() const { return m_guid; }
Item& Item::setGuid(const QString& guid) {
    if (m_guid.compare(guid) != 0) {
        m_guid = guid;
    }
    return *this;
}

const QString& Item::getPubDate() const { return m_pubDate; }
Item& Item::setPubDate(const QString& pubDate) {
    if (m_pubDate.compare(pubDate) != 0) {
        m_pubDate = pubDate;
    }
    return *this;
}

const QString& Item::getSource() const { return m_source; }
Item& Item::setSource(const QString& source) {
    if (m_source.compare(source) != 0) {
        m_source = source;
    }
    return *this;
}

bool Item::isRead() const { return m_read == 1; }
Item& Item::setRead(const int& read) {
    if (m_read != read) {
        m_read = read;
    }
    return *this;
}

int Item::getStatus() const { return m_status; }
Item& Item::setStatus(const int& status) {
    if (m_status != status) {
        m_status = status;
    }
    return *this;
}

void Item::swap(const Item& item) {
    m_id = item.getId();
    m_channelId = item.getChannelId();
    m_title = item.getTitle();
    m_link = item.getLink();
    m_description = item.getDescription();
    m_author = item.getAuthor();
    m_category = item.getCategory();
    m_comments = item.getComments();
    m_guid = item.getGuid();
    m_pubDate = item.getPubDate();
    m_source = item.getSource();
    m_read = item.isRead() ? 1 : 0;
    m_status = item.getStatus();
}

QVariantMap Item::toMap() {
    QVariantMap map;
    map["id"] = m_id;
    map["channel_id"] = m_channelId;
    map["title"] = m_title;
    map["link"] = m_link;
    map["description"] = m_description;
    map["author"] = m_author;
    map["category"] = m_category;
    map["comments"] = m_comments;
    map["guid"] = m_guid;
    map["pub_date"] = m_pubDate;
    map["source"] = m_source;
    map["read"] = m_read;
    map["status"] = m_status;
    return map;
}

Item& Item::fromMap(const QVariantMap& map) {
    m_id = map.value("id", 0).toInt();
    m_channelId = map.value("channel_id", 0).toInt();
    m_title = map.value("title", "").toString();
    m_link = map.value("link", "").toString();
    m_description = map.value("description", "").toString();
    m_author = map.value("author", "").toString();
    m_category = map.value("category", "").toString();
    m_comments = map.value("comments", "").toString();
    m_guid = map.value("guid", "").toString();
    m_pubDate = map.value("pub_date", "").toString();
    m_source = map.value("source", "").toString();
    return *this;
}
