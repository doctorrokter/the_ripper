import bb.cascades 1.4

Sheet {
    id: root
    
    property string title: "title"
    property string link: ""
    
    Page {
        
        actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
        actionBarVisibility: ChromeVisibility.Overlay
        
        titleBar: TitleBar {
            title: root.title
            
            dismissAction: ActionItem {
                imageSource: "asset:///images/ic_previous.png"
                onTriggered: {
                    root.close();
                }
            }
        }
        
        Container {
            layout: DockLayout {}
            ScrollView {
                scrollRole: ScrollRole.Main
                Container {
                    WebView {
                        id: webView
                        url: root.link
                        html: defaultHTML()
                        onLoadingChanged: {
                            if (loadRequest.status === WebLoadStatus.Succeeded) {
                                activityIndicator.stop();
                            }
                            if (loadRequest.status === WebLoadStatus.Started) {
                                activityIndicator.start();
                            }
                        }    
                    }
                }
            }
            
            ActivityIndicator {
                id: activityIndicator
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                running: true
                
                minWidth: ui.du(20)
            }
        }
    }
    
    function defaultHTML() {
        return '<html><head></head><body>Loading...</body></html>';
    }
    
    onClosed: {
        root.title = "";
        root.link = "";
        webView.html = defaultHTML();
    }
}