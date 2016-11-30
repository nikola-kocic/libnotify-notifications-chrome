/*
 * Nick Mossie
 * ld@initiated.com
 *
 * invoke_notify.c
 * requires libnotify, json-c, and curl
 * loops and displays libnotify notifications
 *
 */

#include <stdio.h>
#include <string.h>
#include <json-c/json_tokener.h>
#include <libnotify/notify.h>
#include <curl/curl.h>
#include <stdlib.h>

//#define DEBUG

union buffer_len {
    // easy way to do pack & unpack
    uint32_t num;
    char str[4];
};

int main(int argc, char** argv) {

    union buffer_len input_len;
    char *input_buffer = NULL;
    json_object *json_obj = NULL;
    json_object *json_title = NULL;
    json_object *json_body = NULL;
    json_object *json_iconUrl = NULL;
    NotifyNotification * notification = NULL;
    FILE *fp;
    FILE *curl_fp;
    CURL *curl;
    CURLcode curl_ret;
    int got_icon = 0;
    char *icon_filename = NULL;

    while(1) {

        // reset
        input_len.num = 0;
        notification = NULL;
        got_icon = 0;
        icon_filename = NULL;

#ifdef DEBUG
        // debug
        fp=fopen("/tmp/bob", "a");
#endif

        // Native Messaging spec is 4 chars specifying an integer
        // length followed by JSON
        read(STDIN_FILENO, input_len.str, 4);
        if(input_len.num > 0) {
            // got a length, get the JSON
            input_buffer = malloc((sizeof(char) * input_len.num)+1);
            if(input_buffer == NULL) {
#ifdef DEBUG
                fprintf(fp, "Failed to allocate content\n");
                fclose(fp);
#endif
                exit(1);
            }
            read(STDIN_FILENO, input_buffer, input_len.num);
            if(ferror(stdin)) {
                free(input_buffer);
#ifdef DEBUG
                fprintf(fp, "Error reading from stdin.\n");
                fclose(fp);
#endif
                exit(1);
            }
            // always terminate your strings yo
            input_buffer[input_len.num] = '\0';

            // parse that stuff.. we know it's good JSON.. just cuz we're
            // awesome
            json_obj = json_tokener_parse(input_buffer);

#ifdef DEBUG
            // debug
            fprintf(fp, "%d\n", input_len.num);
            fprintf(fp, "%s\n", input_buffer);
            fprintf(fp, "new_obj.to_string()=%s\n", json_object_to_json_string(json_obj));
#endif

            // done with this now
            free(input_buffer);

            // grab all the entities
            json_title = json_object_object_get(json_obj, "title");
            json_body = json_object_object_get(json_obj, "body");
            json_iconUrl = json_object_object_get(json_obj, "iconUrl");

            if(strlen(json_object_get_string(json_iconUrl)) > 0) {
                // we have an image url.. use CURL to get it
                curl = curl_easy_init();
                if(curl) {
                    icon_filename = tmpnam(NULL);
#ifdef DEBUG
                    fprintf(fp, "icon_filename: %s\n", icon_filename);
#endif
                    if(icon_filename != NULL) {
                        curl_fp=fopen(icon_filename, "w");
                        curl_easy_setopt(curl, CURLOPT_URL, json_object_get_string(json_iconUrl));
                        // set to null to use the internal function, which is
                        // write to the file handle specified in
                        // CURLOPT_WRITEDATA
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl_fp);
#ifdef DEBUG
                        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                        curl_easy_setopt(curl, CURLOPT_STDERR, fp);
#endif
                        curl_ret = curl_easy_perform(curl);
                        if(curl_ret != CURLE_OK) {
#ifdef DEBUG
                            fprintf(fp, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_ret));
#endif
                            got_icon = 0;
                        } else {
                            got_icon = 1;
                        }
                        curl_easy_cleanup(curl);
                        fclose(curl_fp);
                    }
                }
            }

            // show the notification
            notify_init ("invoke_notify");
            if(got_icon) {
                notification = notify_notification_new (json_object_get_string(json_title), json_object_get_string(json_body), icon_filename);
            } else {
                notification = notify_notification_new (json_object_get_string(json_title), json_object_get_string(json_body), NULL);
            }
            notify_notification_show (notification, NULL);

            // free everything
            g_object_unref(G_OBJECT(notification));
            notify_uninit();

#ifndef DEBUG
            if(icon_filename != NULL) unlink(icon_filename);
#endif

#ifdef DEBUG
            fclose(fp);
#endif
        }
    }

    return 0;
}
