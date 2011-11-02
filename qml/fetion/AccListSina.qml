import QtQuick 1.1


Item
{
    id: smsDelegate
    width: parent.width
    height: forwardarea.height
    //            userphotoarea > nametimearea.height + contentarea.height ? userphotoarea : nametimearea.height + contentarea.height

    property int showPic: 0

    MouseArea
    {
        id:contentArea
        anchors.fill: parent
        onReleased:
        {
          //  console.log("Item select" + index);
            //     parent.itemClicked(index);
        }
    }

    //forward
    Item{
        id: forwardarea
        width: parent.width - 70
        height: forward_head.height + forward_middle.height + forward_end.height + 5
     //   anchors.left: status ? "" : parent.left
     //   anchors.right: status ? parent.right : ""

        Component.onCompleted: {
    //        forwardText.width = Math.min(forwardarea.width-40,forwardText.implicitWidth);
    //        forward_middle.width = Math.max(forwardText.width,timeText.implicitWidth)+30;
            if (status)
                anchors.right = parent.right;
            else
                anchors.left = parent.left;

        }

       // anchors.leftMargin: -10
        anchors.top: parent.top
        //        color: "gray"
        //        border.color:"lightgray"; border.width: 1

        //topbg
        Image{
            id: forward_head
            width: forward_middle.width
            anchors.top:parent.top; //anchors.topMargin: 5
            anchors.left: parent.left; anchors.leftMargin: 5
            smooth: true
            source: "images/topic_box_top.png"
            fillMode:Image.Stretch
        }

        Image{
            id: forward_middle
            anchors.top:forward_head.bottom
            anchors.left: parent.left; anchors.leftMargin: 5
            smooth: true
            source: "images/topic_box_central.png"
            width: Math.max(forwardText.width,timeText.implicitWidth)+30
            height: forwardText.height + timeText.height
            fillMode:Image.Stretch
            Text{
                id: forwardText
           //     width:Math.min(forwardarea.width-40,forwardText.implicitWidth)
                width:forwardarea.width-40
                anchors.left: parent.left; anchors.leftMargin: 15
                anchors.top: parent.top;
                text: body
                color:"#4e4e4f"
                smooth:true;
                horizontalAlignment:Text.AlignLeft
                font.pointSize: 15
                wrapMode:Text.WrapAnywhere


            //    onWrapModeChanged: console.log("Wrap mode changed!");

            }
            Text{
                id: timeText
                anchors.left: parent.left; anchors.leftMargin: 15
                anchors.top: forwardText.bottom
                text:date
                color: "#646464"
                smooth:true;
                font.pointSize: 13
            }
        }
        Image{
            id: forward_end
            anchors.top:forward_middle.bottom
            anchors.left: parent.left; anchors.leftMargin: 5
            smooth: true
            source: "images/topic_box_bottom.png"
            width: forward_middle.width
            fillMode:Image.Stretch
        }
    }

    Rectangle
    {
        id:marginfoucus
        anchors.fill:parent
        color: "lightsteelblue";
        opacity: 0.3
        z:parent.z + 1;
        visible: contentArea.pressed;
    }

 }


