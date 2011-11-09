
# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

INCLUDEPATH += /usr/include/openfetion/
INCLUDEPATH += /usr/include/libxml2/

symbian:TARGET.UID3 = 0xEE5CC362

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility
CONFIG += network
MOBILITY += messaging

QT  += sql

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
CONFIG += qt-components

QT += declarative

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    qfetionsms.cpp \
    qfetionnetwork.cpp \
    qfetioncontacts.cpp \
    qfetionstoresms.cpp \
    fetion_user.c \
    fetion_sip.c \
    fetion_share.c \
    fetion_message.c \
    fetion_login.c \
    fetion_list.c \
    fetion_history.c \
    fetion_group.c \
    fetion_directsms.c \
    fetion_debug.c \
    fetion_conversation.c \
    fetion_contact.c \
    fetion_connection.c \
    fetion_config.c \
    fetion_buddylist.c \
    fpsdeclarativeview.cpp

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/fetion/Storage.js \
    qml/fetion/Network.js \
    qml/fetion/constants.js \
    qml/fetion/smsui.qml \
    qml/fetion/Setting.qml \
    qml/fetion/PersonSMS.qml \
    qml/fetion/MainPage.qml \
    qml/fetion/ContactListDelegate.qml \
    qml/fetion/AddContact.qml \
    qml/fetion/AccTitleBar.qml \
    qml/fetion/AccountSetting.qml \
    qml/fetion/AccListSina.qml \
    qml/fetion/MyDialog.qml \
    qml/fetion/Popup.qml \
    qml/fetion/Fader.qml \
    qml/fetion/tmp.qml

HEADERS += \
    qfetionsms.h \
    qfetionnetwork.h \
    qfetioncontacts.h \
    qfetionstoresms.h \
    fpsdeclarativeview.h

RESOURCES += qml/fetion/qmlfiles.qrc

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/fetion/bin
    desktop.files = fetion_harmattan.desktop
    desktop.path = /usr/share/applications

    icon.files = fetion80.png
    icon.path = /usr/share/icons/hicolor/80x80/apps/

    INSTALLS += target icon desktop
}






