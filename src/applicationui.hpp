/*
 * Copyright (c) 2013-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include "config/AppConfig.hpp"
#include "config/DBConfig.hpp"
#include "services/TabsService.hpp"
#include "services/ChannelsService.hpp"
#include "services/ItemsService.hpp"
#include "controllers/ChannelsController.hpp"
#include "controllers/ItemsController.hpp"
#include "controllers/TabsController.hpp"

namespace bb {
    namespace cascades {
        class LocaleHandler;
    }
    namespace system {
        class InvokeManager;
    }
}

class QTranslator;

/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI: public QObject
{
    Q_OBJECT
public:
    ApplicationUI();
    virtual ~ApplicationUI();

    Q_INVOKABLE void resendNotification();

private slots:
    void onSystemLanguageChanged();
private:
    QTranslator* m_translator;
    bb::cascades::LocaleHandler* m_localeHandler;
    bb::system::InvokeManager* m_invokeManager;

    AppConfig* m_pAppConfig;
    DBConfig* m_pDbConfig;
    TabsService* m_tabs;
    ChannelsService* m_channels;
    ItemsService* m_items;
    ChannelsController* m_channelsController;
    ItemsController* m_itemsController;
    TabsController* m_tabsController;
};

#endif /* ApplicationUI_HPP_ */
