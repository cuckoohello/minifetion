import Qt 4.7
import com.nokia.extras 1.0
import Qt.labs.components.native 1.0


PageStackWindow{
    id : mainWindow
    initialPage:MainPage{}

    function showBanner(text)
    {
        smsStatusBanner.text = text;
        smsStatusBanner.show();
    }

    InfoBanner{
        id: smsStatusBanner
        timerEnabled:true
        timerShowTime:3000
        topMargin: parent.height/3
        z : 100
    }
}
