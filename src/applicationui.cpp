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

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/system/InvokeManager>
#include <bb/cascades/ThemeSupport>

using namespace bb::cascades;
using namespace bb::system;

ApplicationUI::ApplicationUI() : QObject(),
        m_translator(new QTranslator(this)),
        m_localeHandler(new LocaleHandler(this)),
        m_invokeManager(new InvokeManager(this)) {

    m_pDbConfig = new DBConfig(this);
    m_pAppConfig = new AppConfig(this);
    m_tabs = new TabsService(this);
    m_channels = new ChannelsService(this);
    m_items = new ItemsService(this);
    m_channelsController = new ChannelsController(m_channels, m_items, this);
    m_itemsController = new ItemsController(m_items, this);
    m_tabsController = new TabsController(m_tabs, this);

    QString theme = m_pAppConfig->get("theme").toString();
    if (theme.compare("") != 0) {
        if (theme.compare("DARK") == 0) {
            Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Dark);
        } else {
            Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Bright);
        }
    }


    if (!QObject::connect(m_localeHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()))) {
        qWarning() << "Recovering from a failed connect()";
    }

    onSystemLanguageChanged();

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    QDeclarativeEngine* engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext* rootContext = engine->rootContext();
    rootContext->setContextProperty("_appConfig", m_pAppConfig);
    rootContext->setContextProperty("_tabs", m_tabs);
    rootContext->setContextProperty("_channels", m_channels);
    rootContext->setContextProperty("_items", m_items);
    rootContext->setContextProperty("_channelsController", m_channelsController);
    rootContext->setContextProperty("_itemsController", m_itemsController);
    rootContext->setContextProperty("_tabsController", m_tabsController);

    qml->setContextProperty("_app", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();

    Application::instance()->setScene(root);
}

ApplicationUI::~ApplicationUI() {
    m_translator->deleteLater();
    m_localeHandler->deleteLater();
    m_invokeManager->deleteLater();
    m_pAppConfig->deleteLater();
    m_pDbConfig->deleteLater();
    m_tabs->deleteLater();
    m_channels->deleteLater();
    m_items->deleteLater();
    m_channelsController->deleteLater();
}

void ApplicationUI::onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_translator);
    QString locale_string = QLocale().name();
    QString file_name = QString("TheRipper_%1").arg(locale_string);
    if (m_translator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_translator);
    }
}

void ApplicationUI::resendNotification() {
    InvokeRequest request;
    request.setTarget("chachkouski.TheRipperService");
    request.setAction("chachkouski.TheRipperService.RESET");
    m_invokeManager->invoke(request);
    Application::instance()->minimize();
}
