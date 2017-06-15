import bb.cascades 1.4

CustomListItem {
    
    property string header: 'Header'
    
    maxHeight: ui.du(4.5)
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        background: ui.palette.plain
        layout: DockLayout {}
        Container {
            verticalAlignment: VerticalAlignment.Center
            margin.leftOffset: ui.du(2)
            margin.rightOffset: ui.du(2)
            Label {
                text: header
            }
        }
    }
    
    opacity: 0.8
}
