import bb.cascades 1.4

Container {
    id: root    
    
    property string text: "sdfsdfsdfsdf"
    
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    ImageView {
        imageSource: "asset:///images/ic_category.png"
        filterColor: ui.palette.secondaryTextOnPlain
        maxWidth: ui.du(3.5)
        maxHeight: ui.du(3.5)
        verticalAlignment: VerticalAlignment.Center
    }
    
    Label {
        text: root.text
        textStyle.base: SystemDefaults.TextStyles.SubtitleText
        textStyle.color: ui.palette.secondaryTextOnPlain
        textStyle.fontWeight: FontWeight.W400
        multiline: true
        textFormat: TextFormat.Html
        verticalAlignment: VerticalAlignment.Center
        
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
    }
}