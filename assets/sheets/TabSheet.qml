import bb.cascades 1.4
import "../components"

Sheet {
    id: root
    
    property int tabId: 0
    property string tabName: ""
    property string tabIcon: ""
    
    Page {
        
        titleBar: TitleBar {
            title: qsTr("Create tab") + Retranslate.onLocaleOrLanguageChanged
            
            acceptAction: ActionItem {
                title: "Ok"
                
                onTriggered: {
                    tabName.validate();
                    if (tabName.isValid() && iconsList.selected().length !== 0) {
                        var image = iconsModel.data(iconsList.selected()).image;
                        
                        if (root.tabId !== 0) {
                            _tabsController.updateTab(root.tabId, tabName.result, image);
                        } else {
                            _tabs.create(tabName.result, image);
                        }
                        tabName.resetText();
                        iconsList.clearSelection();
                        root.close();
                    }
                }
            }
            
            dismissAction: ActionItem {
                title: qsTr("Cancel") + Retranslate.onLocaleOrLanguageChanged
                
                onTriggered: {
                    root.close();
                }
            }
        }
        
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                
                TabNameContainer {
                    id: tabName
                    value: root.tabName
                }
                
                Container {
                    
                    Container {
                        leftPadding: ui.du(2)
                        topPadding: ui.du(2)
                        
                        Label {
                            text: qsTr("Icon") + Retranslate.onLocaleOrLanguageChanged
                        }    
                    }
                    
                    ListView {
                        id: iconsList
                        
                        dataModel: ArrayDataModel {
                            id: iconsModel
                        }
                        
                        layout: GridListLayout {
                            columnCount: 5
                        }
                        
                        listItemComponents: [
                            ListItemComponent {
                                CustomListItem {
                                    dividerVisible: false
                                    Icon {
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        imageSource: "asset:///images/tabs/" + ListItemData.image
                                    }
                                }
                            }
                        ]
                        
                        onTriggered: {
                            iconsList.clearSelection();
                            iconsList.select(indexPath, true);
                        }
                    }
                }
            }
        
        onCreationCompleted: {
            var data = [];
            data.push({image: "ic_blackberry.png"});
            data.push({image: "ic_music.png"});
            data.push({image: "ic_films.png"});
            data.push({image: "ic_news.png"});
            data.push({image: "ic_soccer.png"});
            data.push({image: "ic_pool.png"});
            data.push({image: "ic_blackberry.png"});
            data.push({image: "ic_music.png"});
            data.push({image: "ic_films.png"});
            data.push({image: "ic_news.png"});
            data.push({image: "ic_blackberry.png"});
            data.push({image: "ic_music.png"});
            data.push({image: "ic_films.png"});
            data.push({image: "ic_news.png"});
            iconsModel.append(data);
        }
    }
    
    onOpened: {
        tabName.requestFocus();
        if (tabId !== 0) {
            for (var i = 0; i < iconsModel.size(); i++) {
                var data = iconsModel.value(i);
                if (data.image === tabIcon) {
                    iconsList.clearSelection();
                    iconsList.select([i], true);
                    return;
                }
            }
        }
    }
    
    onClosed: {
        tabId = 0;
        tabName = "";
        tabIcon = "";
        tabName.resetText();
    }
}