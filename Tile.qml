import QtQuick 2.15


Rectangle {
    id:root
    property string displayText:""
    color:"black"
    border.width:1

    border.color:"black"
    radius: 10
    Text{
        text:root.displayText
        color:"white"

        anchors.centerIn:root

        font{
            pointSize: Math.min(root.width,root.height)/3
            bold: true
        }

    }
}
