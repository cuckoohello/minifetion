#include "qfetioncontacts.h"
#include <QFile>

QFetionContacts::QFetionContacts(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames[NameRole] = "name";
    roleNames[MobileRole] = "phone";
    roleNames[GroupRole] = "group";
    roleNames[SectionRole] = "section";
    roleNames[SidRole] = "sid";
    roleNames[UserIdRole] = "userid";
    roleNames[ImageRole] = "image";
    roleNames[ShowRole] = "show";
    setRoleNames(roleNames);


    contactThread = new QFetionContactThread(this);
    //connect(this,SIGNAL(doThread(QFetionContacts::DoThread)),contactThread,SLOT(doThread(QFetionContacts::DoThread)),Qt::QueuedConnection);
    connect(this,SIGNAL(doThread(QFetionContacts::DoThread)),contactThread,SLOT(doThread(QFetionContacts::DoThread)));
    contactThread->start();
    initial_contacts();
    //usleep(500);
    //emit doThread(InitialContacts);

}

void QFetionContacts::initial_contacts()
{
    QSqlQuery query;
    QSqlQuery queryContacts;

    if(!query.exec("select mobileno,password,nickname from user"))
        qDebug() << "Get user information error!";
    else
    {
        query.next();
        nickname = query.value(2).toString();
        password = query.value(1).toString();
        mobileno = query.value(0).toString();
    }

    int groupId;
    if(!query.exec("select * from groups order by groupid"))
        qDebug() <<"Select from groups error!";

  //  beginResetModel();
    while(query.next())
    {
        groupId = query.value(0).toInt();

        groupsList[groupId] = QFetionGroups(query.value(1).toString());
        QString queryString("select sId,userId,localname,nickname,mobileno from contacts_2_2_0 where groupids='");
        if(!queryContacts.exec(queryString.append(query.value(0).toString()).append("' order by localname,nickname")))
            qDebug() <<"select contacts from " << groupsList[groupId].group<<" error ";

        while(queryContacts.next())
        {
            if (queryContacts.value(4).toString()!="")
              contactsList.append(QFetionContact(queryContacts.value(0).toString(),queryContacts.value(1).toString(),
                                               (queryContacts.value(2).toString()=="") ? queryContacts.value(3).toString() :
                                                                                         queryContacts.value(2).toString(),
                                               queryContacts.value(4).toString(),groupId));
        }
    }
  //  endResetModel();

    connect(this,SIGNAL(groupShowChanged(int)),this,SLOT(groupStateChanged(int)));
}

void QFetionContacts::classBegin(){
    qDebug() <<"QFetionContacts Class begin";
}

void QFetionContacts::componentComplete(){
    qDebug() <<"QFetionContacts completed!";
}

void QFetionContacts::groupStateChanged(int groupid){
    int from = -1,end = -1,count = contactsList.count();
    for (int i= 0;i <count;i++)
    {
        if(contactsList.at(i).groupId == groupid)
        {
            if (from == -1)
                from = i;
            end = i;
        }else if (from != -1)
            break;
    }
    QModelIndex idx_from = index(from, 0);
    QModelIndex idx_end = index(end,0);
    emit dataChanged(idx_from,idx_end);
}


QVariant QFetionContacts::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QString();
    if (index.row()>rowCount())
        return QString();
    QFetionContact  contact = contactsList.at(index.row());

    QString file("file:/home/user/.fetion/icons/");
    switch(role)
    {
    case NameRole:
        return contact.name;
        break;
    case MobileRole:
        return contact.mobileno;
        break;
    case GroupRole:
        return contact.groupId;
        break;
    case SectionRole:
        return groupsList[contact.groupId].group;
        break;
    case SidRole:
        return  contact.sid;
        break;
    case UserIdRole:
        return  contact.userId;
        break;
    case ImageRole:
        file.append(contact.sid).append(".jpg");
        if (QFile(file).exists())
            return file;
        else
            return QString("images/default.jpg");
        break;
    case ShowRole:
        return  groupsList[contact.groupId].show;
        break;
    default:
        break;
    }
    return QString();
}


int QFetionContacts::rowCount(const QModelIndex &parent ) const{
    return contactsList.count();
}

void QFetionContacts::fx_process_presence(User* user , const char* xml)
{
        Contact      *contactlist;
        Contact      *contact;

        contactlist = fetion_user_parse_presence_body(xml , user);
        contact = contactlist;

        foreach_contactlist(contactlist , contact){
                if (contact->imageChanged)
                        fetion_user_download_portrait(user , contact->sipuri);
                incPresenceCount();
        }
}

void QFetionContacts::fx_process_notifications(User* user , const char* sipmsg)
{
        int   event;
        int   notification_type;
        char  *xml;

        fetion_sip_parse_notification(sipmsg , &notification_type , &event , &xml);
        usleep(1);
        switch(notification_type)
        {
                case NOTIFICATION_TYPE_PRESENCE:
                        switch(event)
                        {
                                case NOTIFICATION_EVENT_PRESENCECHANGED :
                                        fx_process_presence(user , xml);
                                        break;
                                default:
                                        break;
                        }
                case NOTIFICATION_TYPE_CONVERSATION :
                        if(event == NOTIFICATION_EVENT_USERLEFT){
                                printf("\nuser left\n");
                                break;
                        }
                        break;
                case NOTIFICATION_TYPE_REGISTRATION :
                        if(event == NOTIFICATION_EVENT_DEREGISTRATION){
                                printf("\nuser deregistration\n");
                                break;
                        }
                        break;
                case NOTIFICATION_TYPE_SYNCUSERINFO :
                        if(event == NOTIFICATION_EVENT_SYNCUSERINFO){
                                printf("\nuser sync user info\n");
                                break;
                        }
                        break;
                case NOTIFICATION_TYPE_CONTACT :
                        if(event == NOTIFICATION_EVENT_ADDBUDDYAPPLICATION){
                                printf("\nuser add bubby \n");
                                break;
                        }
                        break;
                case NOTIFICATION_TYPE_PGGROUP :
                        if(event == NOTIFICATION_EVENT_PGGETGROUPINFO){
                                printf("\nget group info \n");
                                break;
                        }
                        if(event == NOTIFICATION_EVENT_PRESENCECHANGED){
                                printf("\npresence changed \n");
                                break;
                        }
                        break;
                default:
                        break;
        }
        free(xml);

}

void* QFetionContacts::fx_listen_func(User* user)
{
        FetionSip  *sip;
        SipMsg     *msg;
        SipMsg     *pos;
        int        type;
        int        ret;
        int        error;

        struct timeval tv;

        debug_info("A new thread entered");

        sip = user->sip;

        Contact      *contactlist;
        Contact      *contact;
        user->contactCount = 0;
        contactlist = contact = user->contactList;
        foreach_contactlist(contactlist , contact){
                user->contactCount++;
        }
        setTotalContacts(user->contactCount);

       // printf("contactList count %d\n",user->contactCount);

        for(;;){

                FD_ZERO(&fd_read);

                if(!sip || !sip->tcp){
                        debug_info("thread exited");
                        exit(-1);
                }
                FD_SET(sip->tcp->socketfd, &fd_read);

                tv.tv_sec = 13;
                tv.tv_usec = 0;

                ret = select(sip->tcp->socketfd+1, &fd_read, NULL, NULL, &tv);

                if(ret == 0)
                        continue;

                if (ret == -1) {
                        debug_info ("Error.. to read socket %d,exit thread",
                                        sip->tcp->socketfd);
                        if(sip != user->sip){
                                debug_info("Error.. thread sip freed\n");
                                free(sip);
                        }
                        exit(-1);
                }

                if (!FD_ISSET (sip->tcp->socketfd, &fd_read)) {
                        usleep (100);
                        continue;
                }

                msg = fetion_sip_listen(sip, &error);

                if(!msg && error){
                        /* if it is the main listening thread */
                        if(sip == user->sip){
                                printf("\n\nError ...... break out...\n\n");
                                exit(-1);
                        }else{
                                printf("\n\n Error ... user listen thread break out\n\n");
                                tcp_connection_free(sip->tcp);
                                exit(-1);
                        }
                }

                pos = msg;
                while(pos){

                        type = fetion_sip_get_type(pos->message);
                        switch(type){
                                case SIP_NOTIFICATION :
                                        fx_process_notifications(user , pos->message);
                                        break;
                                case SIP_MESSAGE:
                                        printf("\n\n sip message\n\n");
                                        printf("%s\n" , pos->message);
                                        break;
                                case SIP_INVITATION:
                                        printf("\n\n sip invitation\n\n");
                                        printf("%s\n" , pos->message);
                                        break;
                                case SIP_INCOMING :
                                        printf("\n\n sip incoming\n\n");
                                        printf("%s\n" , pos->message);
                                        break;
                                case SIP_SIPC_4_0:
                                        printf("\n\n sip 4.0\n\n");
                                        printf("%s\n" , pos->message);
                                        break;
                                default:
                                        printf("%s\n" , pos->message);
                                        break;
                        }
                        pos = pos->next;
                }


                if(msg)
                        fetion_sip_message_free(msg);

              //  printf("presence_count %d\n" , presence_count);
             //   setPresenceCount(presence_count);

                if (presence_count >= user->contactCount)
                        break;
        }

        return NULL;
}

int QFetionContacts::fx_login(const char *mobileno, const char *password)
{
        Config           *config;
        FetionConnection *tcp;
        FetionSip        *sip;
        char             *res;
        char             *nonce;
        char             *key;
        char             *aeskey;
        char             *response;
        int               local_group_count;
        int               local_buddy_count;
        int               group_count;
        int               buddy_count;
        int               ret;

        presence_count = 0;

        /* construct a user object */
        user = fetion_user_new(mobileno, password);
        /* construct a config object */
        config = fetion_config_new();
        /* attach config to user */
        fetion_user_set_config(user, config);

        /* start ssi authencation,result string needs to be freed after use */
        res = ssi_auth_action(user);
        /* parse the ssi authencation result,if success,user's sipuri and userid
         * are stored in user object,orelse user->loginStatus was marked failed */
        parse_ssi_auth_response(res, user);
        free(res);

        /* whether needs to input a confirm code,or login failed
         * for other reason like password error */
        if(USER_AUTH_NEED_CONFIRM(user) || USER_AUTH_ERROR(user)) {
                debug_error("authencation failed");
                return 1;
        }

        /* initialize configuration for current user */
        if(fetion_user_init_config(user) == -1) {
                debug_error("initialize configuration");
                return 1;
        }

        if(fetion_config_download_configuration(user) == -1) {
                debug_error("download configuration");
                return 1;
        }

        /* set user's login state to be hidden */
        fetion_user_set_st(user, P_HIDDEN);

        /* load user information and contact list information from local host */
        fetion_user_load(user);
        fetion_contact_load(user, &local_group_count, &local_buddy_count);

        /* construct a tcp object and connect to the sipc proxy server */
        tcp = tcp_connection_new();
        if((ret = tcp_connection_connect(tcp, config->sipcProxyIP, config->sipcProxyPort)) == -1) {
                debug_error("connect sipc server %s:%d\n", config->sipcProxyIP, config->sipcProxyPort);
                return 1;
        }

        /* construct a sip object with the tcp object and attach it to user object */
        sip = fetion_sip_new(tcp, user->sId);
        fetion_user_set_sip(user, sip);

        /* register to sipc server */
        if(!(res = sipc_reg_action(user))) {
                debug_error("register to sipc server");
                return 1;
        }

        parse_sipc_reg_response(res, &nonce, &key);
        free(res);
        aeskey = generate_aes_key();

        response = generate_response(nonce, user->userId, user->password, key, aeskey);
        free(nonce);
        free(key);
        free(aeskey);

        /* sipc authencation,you can printf res to see what you received */
        if(!(res = sipc_aut_action(user, response))) {
                debug_error("sipc authencation");
                return 1;
        }

        if(parse_sipc_auth_response(res, user, &group_count, &buddy_count) == -1) {
                debug_error("authencation failed");
                return 1;
        }

        free(res);
        free(response);

        if(USER_AUTH_ERROR(user) || USER_AUTH_NEED_CONFIRM(user)) {
                debug_error("login failed");
                return 1;
        }

        fetion_contact_subscribe_only(user);
        fx_listen_func(user);


        /* save the user information and contact list information back to the local database */
        fetion_user_save(user);
        fetion_contact_save(user);

        /* these... fuck the fetion protocol */
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        char buf[1024];
        if(setsockopt(user->sip->tcp->socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
                debug_error("settimeout");
                return 1;
        }
        tcp_connection_recv(user->sip->tcp, buf, sizeof(buf));
         fetion_user_free(user);

        return 0;
}
