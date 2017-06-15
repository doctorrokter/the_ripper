/*
 * Channel.cpp
 *
 *  Created on: May 31, 2017
 *      Author: misha
 */

#include <src/models/Channel.hpp>

Channel::Channel(QObject* parent) : QObject(parent), m_id(0), m_tabId(0), m_title(""), m_link(""), m_image("") {}

Channel::~Channel() {}

Channel::Channel(const Channel& channel) : QObject(channel.parent()) {
    swap(channel);
}

Channel& Channel::operator=(const Channel& channel) {
    swap(channel);
    return *this;
}

int Channel::getId() const { return m_id; }
Channel& Channel::setId(const int& id) {
    if (m_id != id) {
        m_id = id;
        emit idChanged(m_id);
    }
    return *this;
}

int Channel::getTabId() const { return m_tabId; }
Channel& Channel::setTabId(const int& tabId) {
    if (m_tabId != tabId) {
        m_tabId = tabId;
        emit tabIdChanged(m_tabId);
    }
    return *this;
}

const QString& Channel::getTitle() const { return m_title; }
Channel& Channel::setTitle(const QString& title) {
    if (m_title.compare(title) != 0) {
        m_title = title;
        emit titleChanged(m_title);
    }
    return *this;
}

const QString& Channel::getLink() const { return m_link; }
Channel& Channel::setLink(const QString& link) {
    if (m_link.compare(link) != 0) {
        m_link = link;
        emit linkChanged(m_link);
    }
    return *this;
}

const QString& Channel::getImage() const { return m_image; }
Channel& Channel::setImage(const QString& image) {
    if (m_image.compare(image) != 0) {
        m_image = image;
        emit imageChanged(m_image);
    }
    return *this;
}

QVariantMap Channel::toMap() {
    QVariantMap map;
    map["id"] = m_id;
    map["tab_id"] = m_tabId;
    map["title"] = m_title;
    map["link"] = m_link;
    map["image"] = m_image;
    return map;
}

Channel& Channel::fromMap(const QVariantMap& map) {
    m_id = map.value("id", 0).toInt();
    m_tabId = map.value("tab_id", 0).toInt();
    m_title = map.value("title", "").toString();
    m_link = map.value("link", "").toString();
    m_image = map.value("image", "").toString();
    return *this;
}

void Channel::swap(const Channel& channel) {
    m_id = channel.getId();
    m_tabId = channel.getTabId();
    m_title = channel.getTitle();
    m_link = channel.getLink();
    m_image = channel.getImage();
}
