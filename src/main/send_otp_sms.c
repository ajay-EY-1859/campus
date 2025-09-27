


// MSG91 SMS API real implementation for OTP delivery
#define CURL_STATICLIB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MSG91_API_URL "https://api.msg91.com/api/v5/otp"
#define MSG91_AUTH_KEY "470892A1oibdRyfRjs68d6a4deP1" // Provided by user

int sendOTPSMS(const char *mobile, const char *otp) {
    CURL *curl;
    CURLcode res;
    int success = 0;
    struct curl_slist *headers = NULL;
    char postData[256];

    snprintf(postData, sizeof(postData),
        "{\"mobile\":\"%s\",\"otp\":\"%s\",\"authkey\":\"%s\"}",
        mobile, otp, MSG91_AUTH_KEY);

    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, MSG91_API_URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            printf("OTP sent to mobile %s via MSG91.\n", mobile);
            success = 1;
        } else {
            fprintf(stderr, "MSG91 API failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return success;
}
