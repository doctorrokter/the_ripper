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

import bb.cascades 1.4
import bb.system 1.2
import "pages"
import "sheets"

TabbedPane {
    id: root
    
    showTabsOnActionBar: true
    
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                var sp = settingsPage.createObject();
                activePane.push(sp);
                Application.menuEnabled = false;
            }
        }
    }
    
    activePane: NavigationPane {
        id: activePane
        
        FeedPage {
            id: activeFeedPage
            
            onTabsPageRequested: {
                activePane.push(tabsPage.createObject());
            }
            
            onItemsChanged: {
                for (var i = 0; i < root.tabs.length; i++) {
                    var tab = root.tabs[i];
                    if (tab.tabId === tabId) {
                        tab.updateCounter();
                    }
                }
            }
        }
        
        onPopTransitionEnded: {
            Application.menuEnabled = true;
            if (page.cleanUp !== undefined) {
                page.cleanUp();
            }
            page.destroy();
        }
    }
    
    function createTab(t) {
        var tabObj = tab.createObject();
        tabObj.tabId = t.id;
        tabObj.objectName = "tab_" + t.id;
        tabObj.title = t.name;
        tabObj.imageSource = "asset:///images/tabs/" + t.icon;
        tabObj.triggered.connect(function() {
            activeFeedPage.tabId = t.id;   
            activeFeedPage.name = t.name;   
            activeFeedPage.icon = tabObj.imageSource;   
        });
        root.add(tabObj);
    }
    
    function removeTab(id) {
        for (var i = 0; i < root.tabs.length; i++) {
            var tab = root.tabs[i];
            if (tab.objectName === ("tab_" + id)) {
                root.remove(tab);
            }
        }
        
        if (root.tabs.length === 0) {
            tabSheet.open();
        }
    }
    
    function updateTab(tab) {
        for (var i = 0; i < root.tabs.length; i++) {
            var tabObj = root.tabs[i];
            if (tabObj.tabId === tab.id) {
                tabObj.title = tab.name;
                tabObj.imageSource = "asset:///images/tabs/" + tab.icon;
            }
        }
    }
    
    function updateCounters() {
        for (var i = 0; i < root.tabs.length; i++) {
            var tab = root.tabs[i];
            tab.updateCounter();
        }
    }
    
    function rearrangeTabs() {
        toast.body = qsTr("Tabs rearranged. Reload an app for reflecting the changes.") + Retranslate.onLocaleOrLanguageChanged;
        toast.show();
    }
    
    function refresh() {
        var tabs = _tabs.findAll();
        if (tabs.length === 0) {
            tabSheet.open();
        } else {
            tabs.forEach(function(t) {
                createTab(t);
            });
            activeFeedPage.tabId = tabs[0].id;
            activeFeedPage.name = tabs[0].name;
            activeFeedPage.icon = "asset:///images/tabs/" + tabs[0].icon;
        }
    }
    
    onCreationCompleted: {
        refresh();
        _tabs.created.connect(root.createTab);
        _tabs.removed.connect(root.removeTab);
        _channels.removed.connect(root.updateCounters);
        _tabsController.tabUpdated.connect(root.updateTab);
        _tabsController.tabsRearranged.connect(root.rearrangeTabs);
    }
    
    attachedObjects: [
        TabSheet {
            id: tabSheet    
        },
        
        SystemToast {
            id: toast
        },
        
        ComponentDefinition {
            id: settingsPage
            SettingsPage {}
        },
        
        ComponentDefinition {
            id: tabsPage
            TabsPage {
                onTabChannelsPageRequeted: {
                    var page = tabChannelsPage.createObject();
                    page.tabName = tabName;
                    page.tabIcon = tabIcon;
                    page.tabId = tabId;
                    activePane.push(page);
                }
            }
        },
        
        ComponentDefinition {
            id: tab
            Tab {
                property int tabId: 0
                
                onTabIdChanged: {
                    updateCounter();
                }
                
                onTitleChanged: {
                    if (content !== undefined && content !== null && content.count() !== 0) {
                        content.at(0).name = title;
                    }
                }
                
                onImageSourceChanged: {
                    if (content !== undefined && content !== null && content.count() !== 0) {
                        content.at(0).icon = imageSource;
                    }
                }
                
                function updateCounter() {
                    var unread = _tabs.unreadCount(tabId);
                    newContentAvailable = unread > 0;
                    unreadContentCount = unread;
                }
            }
        },
        
        ComponentDefinition {
            id: tabChannelsPage
            TabChannelsPage {}
        }
    ]
}
