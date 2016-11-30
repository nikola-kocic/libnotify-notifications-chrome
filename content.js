"use strict";

// Inject page.js into the actual page's DOM so we can interact with said page
var s = document.createElement("script");
s.src = chrome.extension.getURL("page.js");
s.type = "text/javascript";
(document.head || document.documentElement).appendChild(s);

window.addEventListener("message", function(event) {
  // We only accept messages from ourselves
  if (event.source == window && event.data.type && (event.data.type == "NOTIFICATION_SHOWN")) {
    //console.log("Content script received");
    chrome.runtime.sendMessage(event.data);
  }
}, false);
