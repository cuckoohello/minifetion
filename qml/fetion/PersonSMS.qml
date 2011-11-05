import Qt 4.7
import com.nokia.extras 1.0
import Qt.labs.components.native 1.0
import QtMobility.messaging 1.1
import com.cuckoo.fetion 1.0

import "constants.js" as UI

Page{
    id : personSMS
    anchors.fill : parent
    property string cellphone
    property string name

    property string smsInfo

    property string image_url

    property string uid

    orientationLock : PageOrientation.LockPortrait

    function showInfoBanner(){
        smsStatusBanner.forceActiveFocus();
        smsStatusBanner.show();
    }

    function  initial(){
        fetionSMSModel.initial();
    }

    function  updateMessage(id,state)
    {
        fetionSMSModel.upDateMessage(id,state);
    }


    InfoBanner{
        id: smsStatusBanner
        text:personSMS.smsInfo
        timerEnabled:true
        timerShowTime:3000
        topMargin: parent.height/3
        z : smsList.z + 1
       // leftMargin:

    }


    AccTitleBar{
        id: smsTitle
        width: parent.width
        height: 65
        anchors.top: parent.top
        z: smsList.z + 1
        bgsource: "images/titlebg_blue.png"

        Button{
            id : backButton
            text:"Messages"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            width : 135
            height: 40
            onClicked: pageStack.pop()
        }

        Image {
            id: profileImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 16
            source: personSMS.image_url
            width: 55
            height: 55
            smooth:true
            fillMode: Image.PreserveAspectFit

            Image {
                id: mask
                anchors.fill: parent
                smooth: true
                fillMode: Image.PreserveAspectFit
                source: "images/mask_userimage_blue.png"
            }
        }


        Label {
            id: mainText
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: profileImg.left
            anchors.rightMargin: 10
            text: personSMS.name
            //font.weight: Font.Bold
            font.pixelSize: 26
        }


    }



    ListView {
        id: smsList
        anchors.top: smsTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: textinput.top
        anchors.margins: 10
        smooth: true

        onCountChanged: {
            if( contentHeight > smsList.height )
                contentY = (contentHeight - smsList.height)
            else
                contentY = 0
        }

        onHeightChanged: countChanged()




        model: FetionSMSModel {
            id : fetionSMSModel
            uid : personSMS.uid
         //   cellphone : personSMS.cellphone
        }

        delegate: AccListSina{

        }
    }

    TextArea {
        id : textinput
        z : smsList.z +1
        anchors {left: parent.left; right: parent.right; }
      //  anchors.bottom: activeFocus ? sendButton.top : parent.bottom
        anchors.bottom: sendButton.top
        anchors.bottomMargin: 5
        placeholderText: "Input Message Here!"
        // Set the minimum height to be 200
        height: Math.max (50, implicitHeight)
        platformSipAttributes:SipAttributes {
            actionKeyLabel: "Send"
            actionKeyEnabled: textinput.text!=""
        }

        Keys.onReturnPressed: {
            var messageId = fetionSMSModel.insertMessage(mainPage.accountName,personSMS.uid,textinput.text);
            mainPage.sendSMS(personSMS.name,messageId,personSMS.cellphone,textinput.text);
          //  fetionNetwork.sendSMS(mainPage.phonenumber,mainPage.password,personSMS.cellphone,textinput.text);
            textinput.text = "";
            textinput.closeSoftwareInputPanel();
        }

    }
    Button {
        id: sendButton
        text: "Send";
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 5
        anchors.bottomMargin: 5
 //       visible:textinput.activeFocus
        height: 50
        width: 100
        z : smsList.z + 1
        enabled: textinput.text!=""
        onClicked:
        {
            var messageId = fetionSMSModel.insertMessage(mainPage.accountName,personSMS.uid,textinput.text);
            mainPage.sendSMS(personSMS.name,messageId,personSMS.cellphone,textinput.text);
          //  fetionNetwork.sendSMS(mainPage.phonenumber,mainPage.password,personSMS.cellphone,textinput.text);
            textinput.text = "";
        }
    }

    ScrollDecorator{
        flickableItem: smsList
    }
}
