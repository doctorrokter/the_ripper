import bb.cascades 1.4
import "../components"

Sheet {
    
    id: root
    
    signal createChannel()
    
    property int tabId: 0
    
    Page {
        titleBar: TitleBar {
            title: qsTr("Add channel") + Retranslate.onLocaleOrLanguageChanged
            
            acceptAction: ActionItem {
                title: "Ok"
                
                onTriggered: {
                    urlContainer.validate();
                    if (urlContainer.isValid()) {
                        createChannel();
                        _channelsController.load(root.tabId, urlContainer.result);
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
        
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                TextContainer {
                    id: urlContainer
                    label: "URL"
                    required: true
                }
            }
        }
    }
    
    onOpened: {
        urlContainer.requestFocus();
    }
    
    onClosed: {
        root.tabId = 0;
        urlContainer.resetText();
    }
}