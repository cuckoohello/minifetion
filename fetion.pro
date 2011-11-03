# Add more folders to ship with the application, here
folder_01.source = qml/fetion
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

INCLUDEPATH += /usr/include/openfetion/
INCLUDEPATH += /usr/include/libxml2/

symbian:TARGET.UID3 = 0xEE5CC362

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

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
    fetion_buddylist.c

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

HEADERS += \
    qfetionsms.h \
    qfetionnetwork.h \
    qfetioncontacts.h \
    qfetionstoresms.h































