import bb.cascades 1.4
import bb.system 1.2
import "../components"
import "../sheets"

Page {
    
    id: root
    
    property int tabId: 0
    property string tabName: ""
    property string tabIcon: ""
    
    titleBar: FeedTitleBar {
        name: root.tabName + ": " + (qsTr("Channels") + Retranslate.onLocaleOrLanguageChanged)
        icon: "asset:///images/tabs/" + root.tabIcon
    }
    
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    actionBarVisibility: ChromeVisibility.Overlay
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        layout: DockLayout {}
        
        ListView {
            id: channelsList
            
            dataModel: ArrayDataModel {
                id: channelsModel
            }
            
            verticalAlignment: VerticalAlignment.Center
            
            listItemComponents: [
                ListItemComponent {
                    CustomListItem {
                        
                        contextActions: [
                            ActionSet {
                                actions: [
                                    DeleteActionItem {
                                        id: deleteChannelAction
                                        
                                        onTriggered: {
                                            _channels.remove(ListItemData.id);
                                        }
                                        
                                        shortcuts: [
                                            Shortcut {
                                                key: "d"
                                                
                                                onTriggered: {
                                                    deleteChannelAction.triggered();
                                                }
                                            }
                                        ]
                                    }
                                ]
                            }    
                        ]
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            maxHeight: ui.du(12)
                            
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            
                            Container {
                                visible: ListItemData.image !== undefined && ListItemData.image !== ""
                                
                                ImageView {
                                    preferredWidth: ui.du(12)
                                    preferredHeight: ui.du(12)
                                    imageSource: "file://" + ListItemData.image
                                    scalingMethod: ScalingMethod.AspectFit
                                }
                            }
                            
                            Container {
                                preferredHeight: ui.du(12)
                                
                                leftPadding: ui.du(2)
                                topPadding: ui.du(0.5)
                                bottomPadding: ui.du(0.5)
                                
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1
                                }
                                
                                layout: DockLayout {}
                                
                                Label {
                                    verticalAlignment: VerticalAlignment.Top
                                    text: ListItemData.title
                                    textStyle.base: SystemDefaults.TextStyles.PrimaryText
                                }
                                
                                Label {
                                    verticalAlignment: VerticalAlignment.Bottom
                                    text: ListItemData.link
                                    textStyle.base: SystemDefaults.TextStyles.BodyText
                                    textStyle.fontWeight: FontWeight.W300
                                }
                            }
                        }
                    }
                }
            ]
        }
        
        ActivityIndicator {
            id: spinner
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            minWidth: ui.du(20)
        }
    }
    
    onTabIdChanged: {
        channelsModel.clear();
        channelsModel.append(_channels.findForTab(root.tabId));
    }
    
    actions: [
        ActionItem {
            id: addChannel
            imageSource: "asset:///images/ic_add.png"
            title: qsTr("Add channel") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.Signature
            
            onTriggered: {
                channelSheet.tabId = root.tabId;
                channelSheet.open();
            }
            
            shortcuts: [
                Shortcut {
                    key: "c"
                    
                    onTriggered: {
                        addChannel.triggered();
                    }
                }
            ]
        }
    ]
    
    attachedObjects: [
        ChannelSheet {
            id: channelSheet
            
            onCreateChannel: {
                spinner.start();
            }
        }
    ]
    
    onCreationCompleted: {
        _channels.created.connect(root.addChannel);
        _channels.removed.connect(root.removeChannel);
        _channelsController.logoChanged.connect(root.imageChanged);
    }
    
    function addChannel(channel) {
        if (root.tabId === channel.tab_id) {
            channelsModel.append(channel);
        }
        spinner.stop();
    }
    
    function removeChannel(id) {
        for (var i = 0; i < channelsModel.size(); i++) {
            if (channelsModel.value(i).id === id) {
                channelsModel.removeAt(i);
            }
        }
    }
    
    function imageChanged(channelId, imagePath) {
        for (var i = 0; i < channelsModel.size(); i++) {
            var channel = channelsModel.value(i);
            if (channel.id === channelId) {
                var newChannel = {
                    id: channel.id,
                    tab_id: channel.tab_id,
                    title: channel.title,
                    link: channel.link,
                    image: imagePath
                }
                channelsModel.replace(i, newChannel);        
            }
        }
    }
    
    function cleanUp() {
        _channels.created.disconnect(root.addChannel);
        _channels.removed.disconnect(root.removeChannel);
        _channelsController.logoChanged.disconnect(root.imageChanged);
    }
}
