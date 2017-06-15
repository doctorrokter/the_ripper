import bb.cascades 1.4
import "../sheets"

Page {
    id: root
    
    signal tabChannelsPageRequeted(int tabId, string tabName, string tabIcon);
    
    titleBar: TitleBar {
        title: qsTr("Tabs") + Retranslate.onLocaleOrLanguageChanged
    }
    
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    actionBarVisibility: ChromeVisibility.Overlay
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        ListView {
            id: tabsList
            
            dataModel: ArrayDataModel {
                id: tabsModel
            }
            
            onTriggered: {
                var data = tabsModel.data(indexPath);
                root.tabChannelsPageRequeted(data.id, data.name, data.icon);
            }
            
            rearrangeHandler {
                onMoveUpdated: {
                    event.denyMove();
                    tabsModel.move(event.fromIndexPath[0], event.toIndexPath[0]);
                }
                
                onMoveEnded: {
                    var idsIndexes = [];
                    for (var i = 0 ; i < tabsModel.size(); i++) {
                        var tab = tabsModel.value(i);
                        idsIndexes.push({id: tab.id, index: i});
                    }
                    _tabsController.rearrangeTabs(idsIndexes);
                }
            }
            
            listItemComponents: [
                ListItemComponent {
                    CustomListItem {
                        
                        contextActions: [
                            ActionSet {
                                actions: [
                                    DeleteActionItem {
                                        id: deleteTabAction
                                        
                                        onTriggered: {
                                            _tabs.remove(ListItemData.id);
                                        }
                                        
                                        shortcuts: [
                                            Shortcut {
                                                key: "d"
                                                
                                                onTriggered: {
                                                    deleteTabAction.triggered();
                                                }
                                            }
                                        ]
                                    },
                                    
                                    ActionItem {
                                        id: updateTabAction
                                        title: qsTr("Edit") + Retranslate.onLocaleOrLanguageChanged
                                        imageSource: "asset:///images/ic_compose.png"
                                        
                                        onTriggered: {
                                            _tabsController.requestTabChanging(ListItemData);
                                        }
                                        
                                        shortcuts: [
                                            Shortcut {
                                                key: "e"
                                                
                                                onTriggered: {
                                                    updateTabAction.triggered();
                                                }
                                            }
                                        ]
                                    }
                                ]
                            }    
                        ]
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Fill
                            
                            leftPadding: ui.du(1)
                            topPadding: ui.du(1)
                            rightPadding: ui.du(1)
                            bottomPadding: ui.du(1)
                            
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            
                            ImageView {
                                imageSource: "asset:///images/tabs/" + ListItemData.icon
                                filterColor: ui.palette.textOnPlain
                                
                                maxWidth: ui.du(10)
                                maxHeight: ui.du(10)
                            }
                            
                            Container {
                                verticalAlignment: VerticalAlignment.Center
                                Label {
                                    verticalAlignment: VerticalAlignment.Center
                                    text: ListItemData.name
                                    textStyle.base: SystemDefaults.TextStyles.TitleText
                                }
                                
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1
                                }
                            }
                        }
                    }
                }
            ]
        }
    }
    
    attachedObjects: [
        TabSheet {
            id: tabsSheet
        }
    ]
    
    actions: [
        ActionItem {
            id: addTabAction
            title: qsTr("Add tab") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.Signature
            imageSource: "asset:///images/ic_add.png"
            
            onTriggered: {
                tabsSheet.open();
            }
            
            shortcuts: [
                Shortcut {
                    key: "c"
                    
                    onTriggered: {
                        addTabAction.triggered();
                    }
                }
            ]
        },
        
        ActionItem {
            id: rearrangeAction
            title: qsTr("Rearrange") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: {
                if (tabsList.rearrangeHandler.active) {
                    return "asset:///images/ic_done.png";
                }
                return "asset:///images/ic_sort.png";
            }
            
            onTriggered: {
                tabsList.rearrangeHandler.setActive(!tabsList.rearrangeHandler.active);
            }
            
            shortcuts: [
                Shortcut {
                    key: "r"
                    
                    onTriggered: {
                        rearrangeAction.triggered();
                    }
                }
            ]
        }
    ]
    
    function addTab(tab) {
        tabsModel.append(tab);
    }
    
    function removeTab(id) {
        for (var i = 0; i < tabsModel.size(); i++) {
            var data = tabsModel.value(i);
            if (data.id === id) {
                tabsModel.removeAt(i);
            }
        }
    }
    
    function changeTab(tab) {
        tabsSheet.tabId = tab.id;
        tabsSheet.tabName = tab.name;
        tabsSheet.tabIcon = tab.icon;
        tabsSheet.open();
    }
    
    function updateTab(tab) {
        for (var i = 0; i < tabsModel.size(); i++) {
            var data = tabsModel.value(i);
            if (data.id === tab.id) {
                tabsModel.replace(i, tab);
            }
        }
    }
    
    function cleanUp() {
        _tabs.created.disconnect(root.addTab);
        _tabs.removed.disconnect(root.removeTab);
        _tabsController.tabChangingRequested.disconnect(root.changeTab);
        _tabsController.tabUpdated.disconnect(root.updateTab);
    }
    
    onCreationCompleted: {
        tabsModel.append(_tabs.findAll());
        _tabs.created.connect(root.addTab);
        _tabs.removed.connect(root.removeTab);
        _tabsController.tabChangingRequested.connect(root.changeTab);
        _tabsController.tabUpdated.connect(root.updateTab);
    }
}
