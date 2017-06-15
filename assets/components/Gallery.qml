import bb.cascades 1.4
import chachkouski.util 1.0

Container {
    id: root
    
    property variant items: []
    property variant currentItem: [0]
    
    horizontalAlignment: HorizontalAlignment.Fill
    
    preferredWidth: ui.du(35)
    preferredHeight: ui.du(25)
    
    ListView {
        id: galleryList
        
        dataModel: ArrayDataModel {
            id: galleryModel
        }
        
        layout: StackListLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        
        flickMode: FlickMode.SingleItem
        
        listItemComponents: [
            ListItemComponent {
                CustomListItem {
                    preferredWidth: ui.du(35)
                    preferredHeight: ui.du(25)
                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        imageSource: ListItemData.path
                        scalingMethod: ScalingMethod.AspectFill
                    }
                }
            }
        ]
    }
    
    attachedObjects: [
        Timer {
            id: timer
            interval: 5000
            
            onTimeout: {
                if (currentItem[0] < (galleryModel.size() - 1)) {
                    currentItem = [currentItem[0] + 1];
                } else {
                    currentItem = [0];
                }
                galleryList.scrollToItem(currentItem);
            }
        }
    ]
    
    function startTimer() {
        if (galleryModel.size() > 1) {
            currentItem = [0];
            timer.start();
        }
    }
    
    onCreationCompleted: {
        galleryModel.clear();
        galleryModel.append(items);
        startTimer();
    }
        
    onItemsChanged: {
        timer.stop();
        galleryModel.clear();
        if (items) {
            galleryModel.append(items);
        }
        startTimer();
    }
}
