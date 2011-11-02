import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: acctitlebar
    property string bgsource: "images/titlebg_blue.png"
    property string title: ""
    property string textcolor: "white"
    property int textOffset: 16
    property int titlesize: 30
    property bool isTexthorizontalCenter: false

    Image {
        id: bg
        anchors.fill: parent
        fillMode:Image.Stretch
        source: bgsource
    }

    Label {
        id:titlelabel
        anchors.verticalCenter: parent.verticalCenter
        text: title;
        color: textcolor
        font.pixelSize: titlesize
    }

    state: isTexthorizontalCenter ? "horizontalcenter" : "custom"
    states: [
        State {
             name: "horizontalcenter"
             AnchorChanges {
                 target: titlelabel
                 anchors.horizontalCenter: acctitlebar.horizontalCenter
             }
         },
        State {
             name: "custom"
             AnchorChanges {
                 target: titlelabel
                 anchors.left: acctitlebar.left
             }
             PropertyChanges {
                 target: titlelabel
                 anchors.leftMargin: acctitlebar.textOffset
             }
         }
    ]
}
