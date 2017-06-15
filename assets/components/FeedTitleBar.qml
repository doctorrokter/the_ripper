import bb.cascades 1.4

TitleBar {
    id: root
    
    property string name: ""
    property string icon: ""
    
    kind: TitleBarKind.FreeForm
    
    kindProperties: FreeFormTitleBarKindProperties {
        content: Container {
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            leftPadding: ui.du(1)
            topPadding: ui.du(0.5)
            
            ImageView {
                verticalAlignment: VerticalAlignment.Center
                
                imageSource: root.icon
                maxWidth: ui.du(10)
                maxHeight: ui.du(10)
                filterColor: ui.palette.textOnPlain
            }
            
            Label {
                verticalAlignment: VerticalAlignment.Center
                
                text: root.name
                textStyle.base: SystemDefaults.TextStyles.TitleText
                textStyle.fontWeight: FontWeight.W500
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
        }
    }
}