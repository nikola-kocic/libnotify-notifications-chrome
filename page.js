"use strict";

window.Notification = class extends window.Notification {
  constructor(title, options) {
    window.postMessage(
      { type: "NOTIFICATION_SHOWN", title:title, body:options.body, iconUrl:options.icon },
      "*"
    );
    //console.log("sent NOTIFICATION_SHOWN");
    return super(...arguments);
  }
};
