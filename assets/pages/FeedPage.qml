import bb.cascades 1.4
import "../components"
import "../sheets"

Page {
    
    id: root
    
    property int tabId: 0
    property string name: ""
    property string icon: ""
    property variant channels: []
    
    signal tabsPageRequested();
    signal itemsChanged(int tabId);
    
    titleBar: FeedTitleBar {
        name: root.name
        icon: root.icon
    }
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        ListView {
            id: feedList
            
            property double pinchDistance: 0
            property int tabId: root.tabId
            
            dataModel: GroupDataModel {
                id: feedModel
                
                grouping: ItemGrouping.ByFullValue
                sortingKeys: ["sorting_date", "timestamp"]
                sortedAscending: false
            }
            
            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    ListItemHeader {
                        header: Qt.formatDateTime(new Date(ListItemData), "d MMM, yyyy")
                    }    
                },
                
                ListItemComponent {
                    type: "item"
                    ItemListItem {
                        itemId: ListItemData.id
                        title: ListItemData.title
                        description: ListItemData.description
                        category: ListItemData.category
                        author: ListItemData.author
                        pubDate: ListItemData.pub_date
                        read: ListItemData.read
                        enclosures: ListItemData.enclosures
                        channel: ListItemData.channel
                        
                        onItemRequested: {
                            var item = ListItem.view.dataModel.data(indexPath);
                            _itemsController.readItem(item.id, ListItem.view.tabId, indexPath);
                        }
                    }
                }
            ]
        }
    }
    
    actions: [
        ActionItem {
            id: tabsAction
            title: qsTr("Tabs management") + Retranslate.onLocaleOrLanguageChanged
            imageSource: "asset:///images/ic_tabs.png" 
            
            onTriggered: {
                root.tabsPageRequested();
            }
        },
        
        ActionItem {
            id: readStatusActions
            title: {
                if (_items.readStatus === 0) {
                    return qsTr("Show unread") + Retranslate.onLocaleOrLanguageChanged
                }
                return qsTr("Show all") + Retranslate.onLocaleOrLanguageChanged
            }
            imageSource: "asset:///images/ic_done_all.png"
            
            onTriggered: {
                _items.readStatus = _items.readStatus === 0 ? 1 : 0;
            }
        }
    ]
    
    attachedObjects: [
        WebSheet {
            id: webSheet
        }
    ]
    
    function addItem(item) {
        var channelExists = root.channels.some(function(id) {
            return item.channel_id === id;
        });
        if (channelExists) {
            item = prepareItem(item);
            feedModel.insert(item);
        }
        root.itemsChanged(root.tabId);
    }
    
    function removeItem(id, indexPath) {
        feedModel.removeAt(indexPath);
        root.itemsChanged(root.tabId);
    }
    
    function cleanUp() {
        _items.created.disconnect(root.addItem);
        _items.removed.disconnect(root.removeItem);
        _channels.removed.disconnect(root.refresh);
        _channels.created.disconnect(root.onChannelCreated);
        _itemsController.readItemRequested.disconnect(root.onRead);
    }
    
    function refresh() {
        feedModel.clear();
        _channels.findForTab(tabId).forEach(function(channel) {
            var items = _items.findForChannel(channel.id);
            items.forEach(function(item) {
                item = prepareItem(item);
                item.channel = channel.title;
            });
            root.channels.push(channel.id);
            feedModel.insertList(items);
        });
    }
    
    function onRead(newItem, tabId, indexPath) {
        if (root.tabId === tabId) {
            newItem = prepareItem(newItem);
            feedModel.updateItem(indexPath, newItem);
            root.itemsChanged(root.tabId);
            
            if (!webSheet.opened) {
                webSheet.title = newItem.title;
                webSheet.link = newItem.link;
                webSheet.open();
            }
        }
    }
    
    function getSortingDate(pubDate) {
        var d = new Date(pubDate);
        d.setHours(0);
        d.setMinutes(0);
        d.setSeconds(0);
        return d.getTime();
    }
    
    function prepareItem(item) {
        item.sorting_date = getSortingDate(item.pub_date);
        item.timestamp = new Date(item.pub_date).getTime();
        item.enclosures = _items.findEnclosures(item.id).map(function(enclosure) {
            enclosure.path = "file://" + enclosure.path;
            return enclosure;
        });
        return item;  
    }
    
    function onChannelCreated(channel) {
        if (root.tabId === channel.tab_id) {
            root.channels.push(channel.id);
        }
    }
    
    onCreationCompleted: {
        var data = [];
        data.push({
            id: 1,
            title: "BlackBerry KEY One is for selling!", 
            description: "Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor", 
            pub_date: "Fri, 02 Jun 2017 15:51:30 +0300",
            sorting_date: 1496350800000,
            read: 1,
            timestamp: 1,
            enclosures: [{path: "asset:///images/blackberry-z10.jpg"}],
            category: "Science", author: "doctorrokter@gmail.com (Mikhail Chachkouski)"});
        data.push({
            id: 2,
            title: "BlackBerry KEY One is for selling!", 
            description: "Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor Lorem ipsum dolor lorem ipsum dolor", 
            pub_date: "Fri, 31 May 2017 16:51:30 +0300",
            sorting_date: 1496178000000,
            read: 0,
            timestamp: 2,
            enclosures: [{path: "asset:///images/blackberry-z10.jpg"}],
            category: "Gadgets", author: ""});
        feedModel.insertList(data);
        
        _items.created.connect(root.addItem);
        _items.removed.connect(root.removeItem);
        _channels.removed.connect(root.refresh);
        _channels.created.connect(root.onChannelCreated);
        _itemsController.readItemRequested.connect(root.onRead);
    }
    
    onTabIdChanged: {
        refresh();
    }
}
