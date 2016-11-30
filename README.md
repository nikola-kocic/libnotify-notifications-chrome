# Libnotify Notifications - Chrome

Chrome extension that shows web notifications using libnotify in Linux OS.

Based on [Libnotify Notifications in Chrome extension](https://chrome.google.com/webstore/detail/libnotify-notifications-i/epckjefillidgmfmclhcbaembhpdeijg)

## Installation

1. Clone this repository to local folder
2. In Chrome, go to Menu->More tools...->Extensions
3. Check "Developer mode"
4. Click "Load unpacked extension..."
5. Select folder where you cloned this repository
6. Copy **ID** of extension, it will be needed in the following step
7. Open **host** folder inside the local repository folder
8. Open `io.github.nikola_kocic.libnotify_notifications_chrome.json` and change
   the word `___EXTENSION_ID___` to the extension ID you got in step 6
9. Open Terminal in **host** folder
10. Make sure libcurl and json-c are installed (and their -dev packages if your distro splits them up)
11. Run `make`
12. Run `./install.sh`
13. Don't move or delete repository folder because built executable is not copied anywhere
    and is invoked using absolute path
