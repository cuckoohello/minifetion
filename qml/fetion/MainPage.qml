import Qt 4.7
import com.nokia.extras 1.0
import Qt.labs.components.native 1.0
import com.cuckoo.fetion 1.0

import "Storage.js" as Storage
import "Network.js" as Network
import "constants.js" as UI

Page{
    id : mainPage
    anchors.fill : parent

    property string phonenumber
    property string password
    property string accountName
    property string server  : "http://202.112.3.1/sms/sendsms.php"
    orientationLock : PageOrientation.LockPortrait


    Component.onCompleted:
    {
        phonenumber = contacts.mobileno;
        password    = contacts.password;
        accountName = contacts.nickname;
    }

    FetionContacts{
        id : contacts
        property bool needAuth
        property bool isSyncContacts : false

        onContacts_Count_Changed:{
            progressBar.indeterminate = false;
        }
        onSync_contacts_finished:{
            contactStatusDialog.close();
            contacts.isSyncContacts = false;
        }
        onSync_Need_Auth:{
            needAuth = true;
        }
    }

    MyDialog {
           id: contactStatusDialog
           anchors.centerIn: parent
           title: Item {
               id: titleField
               height: contactStatusDialog.platformStyle.titleBarHeight
               width: parent.width
               Image {
                   id: supplement
                   source: "image://theme/icon-l-contacts"
                   height: parent.height - 10
                   width: 75
                   fillMode: Image.PreserveAspectFit
                   anchors.leftMargin: 5
                   anchors.rightMargin: 5
               }

               Label {
                   id: titleLabel
                   anchors.left: supplement.right
                   anchors.verticalCenter: titleField.verticalCenter
                   font.capitalization: Font.MixedCase
                   color: "white"
                   text: "Sync Contacts"
               }
           }

           content: Item {
               width: parent.width
               height: childrenRect.height
               ProgressBar {
                   id: progressBar
                   anchors.top: parent.top
                   anchors.topMargin: 10
                   width: parent.width
                   minimumValue: 0
                   maximumValue : contacts.total_contacts_count
                   value : contacts.presence_contacts_count
               }

               Text {
                   id: text
                   anchors.top: progressBar.bottom
                   anchors.topMargin : 10
                   font.pixelSize: 22
                   color: "white"
                   text: contacts.sync_status_message
                   verticalAlignment: Text.AlignVCenter
               }

               Image{
                   id : authImage
                   anchors.top: text.bottom
                   anchors.topMargin : 10
                   anchors.horizontalCenter: parent.horizontalCenter
                   source: "file:/home/user/.fetion/code.jpeg"
                   width: 180
                   height: contacts.needAuth ? 72 : 0
                   fillMode:Image.Stretch
                   visible: contacts.needAuth
               }
               TextField{
                   id : authCode
                   anchors.top: authImage.bottom
                   anchors.topMargin: 10
                   width: parent.width
                   visible: contacts.needAuth
                   height: contacts.needAuth ? 52 : 0
               }
               Item {
                   height: 15
                   anchors.top: authCode.bottom
               }
           }

           buttons: ButtonRow {
               platformStyle: ButtonStyle { }
               anchors.horizontalCenter: parent.horizontalCenter
               Button {
                   id: b1;
                   text: "OK";
                   visible:contacts.needAuth;enabled:authCode.text != "";
                   onClicked: {contacts.setAuthCode(authCode.text); contacts.needAuth = false;}
                   }
               Button {id: b2; text: "Hide"; onClicked: contactStatusDialog.close()}
           }

   }


    function syncContacts(phonenumber,password)
    {
        if (contacts.isSyncContacts)
            contactStatusDialog.open();
        else
        {
            progressBar.indeterminate = true;
            contacts.needAuth = false;
            contacts.isSyncContacts = true;
            contactStatusDialog.open();
            contacts.sync_contacts(phonenumber,password);
        }
    }

    function sendSMS(sendto,messageid,cellphone,message)
    {
        Network.sendSMS(server,phonenumber,password,sendto,cellphone,message,messageid,callback);
    }

    function  callback(status,sendto,id,log)
    {
        var text;
        if (status !== 200)
            text = "Send to "+sendto+" network error!";
        else
            text = "Send to "+sendto+" "+log;
        if(log ==="Success")
            personSMSPage.updateMessage(id,1);
        else
            personSMSPage.updateMessage(id,3);

        console.log(smsStatusBanner.text);
        mainWindow.showBanner(text);
    }




    PersonSMS{
        id : personSMSPage
    }

    AccTitleBar{
        id: mainPageTitle
        width: parent.width
        height: 65
        anchors.top: parent.top
        z: contactView.z + 1

        Image {
            id: profileImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: "images/default.jpg"
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

        Image {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right; anchors.rightMargin: 20
            source: refreshbut.pressed? "images/refresh_press_blue.png":"images/refresh_blue.png"
            MouseArea{
                id: refreshbut
                anchors.fill: parent
                onClicked: {
                    mainPage.syncContacts(mainPage.phonenumber,mainPage.password);
                }
            }
        }

        Label {
            id: mainText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: profileImg.right
            anchors.leftMargin: 10
            text: mainPage.accountName
            //font.weight: Font.Bold
            font.pixelSize: 26
        }


    }

    ListView{
        id : contactView
        anchors.top: mainPageTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        model: contacts


        delegate: ContactListDelegate{
            id : listdelegate
            onClicked: {
                personSMSPage.cellphone = phone;
                personSMSPage.name = name;
                personSMSPage.image_url = image;
                personSMSPage.uid = userid;
                pageStack.push(personSMSPage);

            }
        }



        section.criteria: ViewSection.FullString
        section.property: "group"
        section.delegate:Item {
            id: sectionItem


            property int titleSize: UI.LIST_TILE_SIZE
            property int titleWeight: Font.Bold
            property color titleColor: theme.inverted ? UI.LIST_TITLE_COLOR_INVERTED : UI.LIST_TITLE_COLOR

            property int subtitleSize: UI.LIST_SUBTILE_SIZE
            property int subtitleWeight: Font.Light
            property color subtitleColor: theme.inverted ? UI.LIST_SUBTITLE_COLOR_INVERTED : UI.LIST_SUBTITLE_COLOR


            width: contactView.width
            height: UI.LIST_ITEM_HEIGHT-30

            Rectangle {
                anchors.fill: parent
                // Fill page porders
                anchors.leftMargin: -UI.MARGIN_XLARGE
                anchors.rightMargin: -UI.MARGIN_XLARGE
                visible : mouseArea.pressed
                color   : "#ffffff"
            }

            Row {
                anchors.fill: parent
                spacing: UI.LIST_ITEM_SPACING

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    width: UI.LIST_ICON_SIZE-10
                    height: UI.LIST_ICON_SIZE-10
                    source: contacts.getGroupVisible(section) ? "images/down.png" : "images/right.png"
                }


                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    id: mainText
                    text: contacts.getGroupName(section)
                    font.weight: sectionItem.titleWeight
                    font.pixelSize: sectionItem.titleSize
                    color: sectionItem.titleColor
                }
            }
            MouseArea {
                id: mouseArea;
                anchors.fill: parent
                onClicked: {
                    contacts.setGroupShow(section);
                }
            }
        }
    }

    SectionScroller{
        listView: contactView
    }

    ScrollDecorator{
        flickableItem: contactView
    }

    tools : ToolBarLayout {
        id: tabTools

//        ToolIcon { iconId: "toolbar-back"; onClicked: { colorMenu.close(); pageStack.pop(); }}
        ToolIcon {
            iconId: "toolbar-view-menu"
            anchors.right: parent.right
            onClicked: {
                if (colorMenu.status == DialogStatus.Open)
                    colorMenu.close();
                else
                    colorMenu.open();
            }
        }
    }


    Menu {
        id: colorMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Set Theme inverted";
                onClicked: theme.inverted = !theme.inverted
            }
            MenuItem {text: "Lock Orientation";
                onClicked:screen.allowedOrientations = Screen.Portrait;
            }
            MenuItem {text: "Sync Contacts";
                onClicked: {
                    mainPage.syncContacts(mainPage.phonenumber,mainPage.password);
                }
            }
            MenuItem {text: "Setting";
                //anchors.centerIn: parent
                onClicked: { pageStack.push(Qt.createComponent("Setting.qml"))}
            }
        }
    }

}
