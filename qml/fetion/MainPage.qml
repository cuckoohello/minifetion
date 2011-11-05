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


    Component.onCompleted:
    {
        phonenumber = contacts.mobileno;
        password    = contacts.password;
        accountName = contacts.nickname;
        personSMSPage.initial();
    }

    FetionContacts{
        id : contacts
        onContacts_Count_Changed:progressBar.maximumValue = total_contacts_count;
        onPresence_Count_Changed:progressBar.value = presence_contacts_count;
        onSync_contacts_finished:contactStatusDialog.close();
    }

    Dialog {
           id: contactStatusDialog
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

           content: ProgressBar {
               id: progressBar
               width: parent.width
               minimumValue: 0
           }
   }


    function syncContacts(phonenumber,password)
    {
        progressBar.maximumValue = 100;
        progressBar.value = 0;
        contactStatusDialog.open();

        contacts.sync_contacts(phonenumber,password);
    }

    function sendSMS(sendto,messageid,cellphone,message)
    {
        Network.sendSMS(server,phonenumber,password,sendto,cellphone,message,messageid,callback);
    }

    function  callback(status,sendto,id,log)
    {
        if (status !== 200)
            smsStatusBanner.text = "Send to "+sendto+" network error!";
        else
            smsStatusBanner.text = "Send to "+sendto+" "+log;
        personSMSPage.smsInfo = smsStatusBanner.text;
        if(log ==="Success")
            personSMSPage.updateMessage(id,1);
        else
            personSMSPage.updateMessage(id,3);

        console.log(smsStatusBanner.text);
        smsStatusBanner.show();
        personSMSPage.showInfoBanner();
    }

    InfoBanner{
        id: smsStatusBanner
        timerEnabled:true
        timerShowTime:3000
        topMargin: parent.height/3
       // leftMargin:

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
