import bb.cascades 1.4

Container {
    id: root
    
    property string imageSource: ""
    
    leftPadding: ui.du(1)
    topPadding: ui.du(1)
    
    ImageView {
        imageSource: root.imageSource
        filterColor: ui.palette.textOnPlain
        maxWidth: ui.du(10)
        maxHeight: ui.du(10)
    }
}
