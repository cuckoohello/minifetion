import Qt 4.7
import com.nokia.extras 1.0
import Qt.labs.components.native 1.0
import com.cuckoo.fetion 1.0

Page{
    id : setting
    anchors.fill : parent

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
            source: "images/account.png"
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
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            text: "Setting"
            font.pixelSize: 26

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


    Item{
        id: logincontent
        width: 386;
        height: 226;
        anchors.centerIn: parent

        Column{
            anchors.centerIn: parent
            spacing: 15
            Column{
                id: inputline
                spacing: 10
                Row{
                    spacing: 10
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Nickname:"
                        font.pixelSize: 26; font.bold: true; color: "black";
                    }
                    TextField {
                        id: nickNameText

                        placeholderText: qsTr("NickName")
                        inputMethodHints: Qt.ImhNoPredictiveText

                    }
                }


                Row{
                    spacing: 10
                    Text {
                        id: login
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Account :"
                        font.pixelSize: 26; font.bold: true; color: "black";
                    }
                    TextField {
                        id: accountText

                        placeholderText: qsTr("Account")
                        inputMethodHints: Qt.ImhDialableCharactersOnly | Qt.ImhNoPredictiveText
                    }
                }



               Row{
                   spacing: 10
                   Text {
                       id: pwd
                       anchors.verticalCenter: parent.verticalCenter
                       //font.family: myFont.name
                       text: "Password:"
                       font.pixelSize: 26; font.bold: true; color: "black";
                   }
                   TextField {
                       id: passwordText
                       placeholderText: qsTr("Password")
                       echoMode: TextInput.Password
                   }

               }
            }

        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 40
            Button { // log in
               id: loginbutton
               width: logincontent.width - 80
               height: 60
               text: "Save"
               enabled: (accountText.text!="")&&(nickNameText.text!="")&&(passwordText.text!="")
               onClicked: {
                   mainPage.updateAccountSetting(nickNameText.text,accountText.text,passwordText.text);
                   pageStack.pop();
               }
            }

          } // end row

        }
    }



    tools : ToolBarLayout {
        id: tabTools

        ToolIcon { iconId: "toolbar-back"; onClicked: {  pageStack.pop(); }}
    }


}
