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
    //    personSMSPage.initial();
    }

    FetionContacts{
        id : contacts
    }

    function addAccount(nickname,phone){
       contactList.append(
                    {
                        title : nickname,
                        subtitle : phone
                    });
        Storage.addAccount(nickname,phone);

    }

    function insertAccount(nickname,phone){
        contactList.append(
                     {
                         title : nickname,
                         subtitle : phone
                     });
    }


    function updateAccountSetting(nickname,phone,password) {
        Storage.setSetting("phonenumber",phone);
        Storage.setSetting("password",password);
        Storage.setSetting("accountName",nickname)
        phonenumber = phone;
        password = password;
        accountName = nickname;
    }


    function sendSMS(sendto,id,cellphone,message)
    {
        Network.sendSMS(server,phonenumber,password,sendto,cellphone,message,id,callback);
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
            source: "/home/user/.fetion/icons/default.jpg"
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
           //     personSMSPage.initial();
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
            MenuItem {text: "Add Contact";
                onClicked:pageStack.push(Qt.createComponent("AddContact.qml"))
            }
            MenuItem {text: "Sync Contacts";
            }
            MenuItem {text: "Setting";
                //anchors.centerIn: parent
                onClicked: { pageStack.push(Qt.createComponent("Setting.qml"))}
            }
        }
    }


}
