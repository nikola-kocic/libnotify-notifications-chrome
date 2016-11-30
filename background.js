"use strict";

var port = null;

function showNativeNotification(request, sender) {
  if (!port) {
    port = chrome.runtime.connectNative('io.github.nikola_kocic.libnotify_notifications_chrome');
    port.onDisconnect.addListener(function() {
      let reason = chrome.runtime.lastError ? chrome.runtime.lastError.message : "unknown";
      console.log("Native port disconnected. Reason: " + reason);
      port = null;
    });
  }

  // Check if port is still connected
  if (port) {
    port.postMessage({"title": request.title, "body": request.body, "iconUrl": request.iconUrl});
  }
}

function handleNotification(request, sender, callback) {
  showNativeNotification(request, sender);
}

chrome.runtime.onMessage.addListener(handleNotification);
chrome.runtime.onMessageExternal.addListener(handleNotification);
