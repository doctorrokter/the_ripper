import bb.cascades 1.4

CustomListItem {
    id: root
    
    signal itemRequested(variant indexPath);
    
    property int itemId: 0
    property string title: "BlackBerry KEY One is for selling!"
    property string category: "Science"
    property string author: "doctorrokter@gmail.com (Mikhail Chachkouski)"
    property string pubDate: "Fri, 02 Jun 2017 16:51:30 +0300"
    property string description: "Lorem ipsum dolor lorem ipsum dolor. Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor. Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor."
    property int read: 0
    property variant enclosures: []
    property string channel: "Onliner"
    
    property int startMove: 0
    
    animations: [
        ScaleTransition {
            id: scaling
            toX: 0.25
            toY: 0.25
            fromX: 1
            fromY: 1
            
            duration: 150
            
            onEnded: {
                deleteActionItem.triggered();
                root.scaleX = 1;
                root.scaleY = 1;
                root.translationX = 0;
            }
        }
    ]
    
    contextActions: [
        ActionSet {
            DeleteActionItem {
                id: deleteActionItem
                
                onTriggered: {
                    _items.remove(ListItemData.id, root.ListItem.indexPath);
                }
                
                shortcuts: [
                    Shortcut {
                        key: "d"
                        
                        onTriggered: {
                            deleteActionItem.triggered();
                        }
                    }
                ]
            }
            
            InvokeActionItem {
                id: shareActionItem
                imageSource: "asset:///images/ic_share.png"
                title: qsTr("Share") + Retranslate.onLocaleOrLanguageChanged
                
                query {
                    mimeType: "text/plain"
                    invokeActionId: "bb.action.SHARE"
                }
                
                onTriggered: {
                    data = ListItemData.link
                }
                
                shortcuts: [
                    Shortcut {
                        key: "s"
                        
                        onTriggered: {
                            shareActionItem.triggered();
                        }
                    }
                ]
            }
        }
    ]
    
    gestureHandlers: [
        TapHandler {
            onTapped: {
                root.itemRequested(root.ListItem.indexPath);
            }
        }
    ]
    
    onTouch: {
        if (event.isDown()) {
            root.startMove = event.windowX;
        }
        
        if (event.isMove()) {
            if ((root.startMove - event.windowX) > 75) {
                root.translationX = event.windowX - root.startMove;
            }
        }
        
        if (event.isUp() || event.isCancel()) {
            if (root.translationX <= -400) {
                root.translationX = 0;
                scaling.play();
            }
            root.translationX = 0;
        }
    }
    
    Container {
        
        leftPadding: ui.du(1.5)
        topPadding: ui.du(1.5)
        rightPadding: ui.du(1.5)
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            bottomPadding: ui.du(1.5)
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            Label {
                text: root.title
                multiline: true
                textStyle.base: SystemDefaults.TextStyles.BodyText
                textStyle.fontWeight: FontWeight.W500
                textFormat: TextFormat.Html
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            
            ImageView {
                visible: root.read === 0
                imageSource: "asset:///images/Splat_96x96.png"
                maxWidth: ui.du(5)
                maxHeight: ui.du(5)
            }
        }
                
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            Gallery {
                items: root.enclosures
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                
                leftPadding: ui.du(1)
                rightPadding: ui.du(1)
                bottomPadding: ui.du(1.5)
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                
                Container {
                    Tag {
                        text: root.channel + (root.category !== "" ? " / " + root.category : "") + (root.author !== "" ? " / " + root.author : "")
                    }
                    
                    PubTime {
                        topPadding: ui.du(1)
                        
                        pubDate: root.pubDate
                    }
                }
            }
        }
        
        Container {
            Label {
                text: root.description
                textFormat: TextFormat.Html
                multiline: true
                textStyle.base: SystemDefaults.TextStyles.BodyText
                textStyle.fontWeight: FontWeight.W300
            }
        }
    }
    
    function showByReadStatus(readStatus) {
        if (readStatus === 0) {
            visible = true;
        } else {
            if (root.read === readStatus) {
                visible = false;
            }
        }
    }
    
    onReadChanged: {
        visible = !(_items.readStatus === 1 && read === _items.readStatus);
    }
    
    onCreationCompleted: {
        _channelsController.thumbnailChanged.connect(function(itemId, enclosure) {
            if (root.itemId === itemId) {
                var enclosures = root.enclosures.slice();
                enclosures.push(enclosure);
                root.enclosures = enclosures;
            }
        });
        _items.readStatusChanged.connect(root.showByReadStatus);
    }
}