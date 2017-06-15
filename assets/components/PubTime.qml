import bb.cascades 1.4

Container {
    id: root
    
    property string pubDate: ""
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    ImageView {
        imageSource: "asset:///images/ic_history.png"
        maxWidth: ui.du(4)
        maxHeight: ui.du(4)
        verticalAlignment: VerticalAlignment.Center
        filterColor: ui.palette.primaryDark
    }
    
    Label {
        text: Qt.formatDateTime(new Date(root.pubDate), "HH:mm")
        verticalAlignment: VerticalAlignment.Center
        textStyle.color: ui.palette.primaryDark
        textStyle.base: SystemDefaults.TextStyles.SubtitleText
    }
}
