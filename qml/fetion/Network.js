.pragma library

// Global variable containing jiepang server address
var server = "http://166.111.73.2/~cuckoo/sms/sendsms.php"

// Global variable containing the source parameter of the server calls

function handleRequestState(request,sendto,id, callback) {
    // There is a bug in qml XMLHttpRequest implementation: when the server
    // returns a http 500 response (server error), the request object
    // will have the status set to 0 when the readyState is DONE.
    // To bypass this bug we notice that the responseText is already set
    // when we are in the LOADING state.
    // TODO: try to remove this when qt version is higher than 4.7.3.
    if (request.readyState === XMLHttpRequest.LOADING) {
        if (request.status !== 200 && request.status !== 0 && request.responseText) {
            console.log("Warning: getting XMLHttpRequest result before request complete.");
            callback(request.status,sendto, request.responseText);
            // We use this attribute to flag the request as already done.
            request.done = true;
        }
    }

    if (request.readyState === XMLHttpRequest.DONE) {
        if (request.done)
            return;
        if (request.status !== 200) { // Got an error
            console.log("Error request", request.status);
        }
        callback(request.status,sendto, id,request.responseText);
    }
}


function sendSMS(url,user,password,sendto,phonenu,message,id,callback)
{
    var paramsStrings = [];
    paramsStrings.push("cellphone=" + user);
    paramsStrings.push("password="+password);
    paramsStrings.push("sendto="+phonenu);
    paramsStrings.push("message="+message);
    doPost(url,paramsStrings.join("&"),sendto,id,callback);
}


function doPost(url, params, sendto,id,callback) {
    // Add jiepang server address if no server is specified:
    url = resolveUrl(url);
    var request = new XMLHttpRequest;
    request.done = false; // Hack. See handleRequestState().
    request.open("Post", url);
    request.setRequestHeader("Content-Type","application/x-www-form-urlencoded;");
    request.onreadystatechange = function() {
        handleRequestState(request,sendto,id, callback)
    }
  //  console.debug("Send request", url, params)
    request.send(params);
}

function doGet(url, params, callback) {
    var paramsString = parseParam(params)
    var request = new XMLHttpRequest;
    url = resolveUrl(url);
    request.done = false; // Hack. See handleRequestState().
    request.open("Get", url + "?" + paramsString);
    request.setRequestHeader("Content-Type","application/x-www-form-urlencoded;");
    request.onreadystatechange = function() {
        handleRequestState(request, callback)
    }
    console.debug("Send request", url, paramsString)
    request.send()
}

function resolveUrl(url) {
    url = "" + url; // Turn url into a string.
    if (url.indexOf("http://") !== 0)
        url = server + url;
    return url;
}

