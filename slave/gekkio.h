/**
 * Author : PHAM QUOC DU THIEN
 * Date : 28/10/2018 
 * 
 * */
#ifndef GEKKIO_H
#define GEKKIO_H
#include <cstdio>
#include <cstdlib>
#include <cstring>

char gkio_bye_char[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char gkio_char_bye[129] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63, 52, 53,
                           54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                           11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27,
                           28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
                           49, 50, 51, 0, 0, 0, 0, 0};

void gkio_decode_base64(const char *encoded_string, void *buff, int *size)
{
    int in_len = strlen(encoded_string);

    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    *size = 0;
    unsigned char *res = (unsigned char *)buff;

    while (in_len-- && (encoded_string[in_] != '='))
    {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = gkio_char_bye[char_array_4[i]];

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                res[(*size)++] = char_array_3[i];
            i = 0;
        }
    }

    if (i)
    {
        for (j = 0; j < i; j++)
            char_array_4[j] = gkio_char_bye[char_array_4[j]];

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; (j < i - 1); j++)
            res[(*size)++] = char_array_3[j];
    }
}

void gkio_encode_base64(void *s, int size, char *res)
{
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    unsigned char *bytes_to_encode = (unsigned char *)s;
    int in_len = size;
    int res_lenght = 0;
    while (in_len--)
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                res[res_lenght++] = gkio_bye_char[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            res[res_lenght++] = gkio_bye_char[char_array_4[j]];

        while ((i++ < 3))
            res[res_lenght++] = '=';
    }
    res[res_lenght++] = 0;
}

typedef void (*GKIO_Callback)(void *, int);

struct GKIOEventListenner
{
    const char *name;
    GKIO_Callback callback;
    GKIOEventListenner(const char *name, GKIO_Callback callback)
    {
        this->name = name;
        this->callback = callback;
    }

    GKIOEventListenner(){};
};

char *gkio_buffer;
char *gkio_data_raw;
int gkio_data_raw_size;
GKIOEventListenner *gkio_listenners;
int gkio_listenner_count;

char *event_name;
char *event_data;
int event_data_length, event_name_length;

void gkio_init(int buffer_size = 1024, int listenner_limit = 1024)
{
    setbuf(stdout, NULL);
    gkio_buffer = new char[buffer_size * 2];
    gkio_data_raw = new char[buffer_size];
    gkio_listenners = new GKIOEventListenner[listenner_limit];
    gkio_listenner_count = 0;
    event_data = new char[buffer_size * 4 / 3 + 1];
    event_name = new char[256];
    event_data_length = -1;
    event_name_length = -1;
}

void gkio_on(const char *event, GKIO_Callback callback)
{
    gkio_listenners[gkio_listenner_count++] = GKIOEventListenner(event, callback);
}

void gkio_send(const char *event, void *data, int size)
{
    gkio_encode_base64(data, size, event_data);
    printf("[%s](%s)", event, event_data);
}

void gkio_call_event(const char *name, const char *encoded_data)
{

    gkio_decode_base64(encoded_data, gkio_data_raw, &gkio_data_raw_size);

    for (int i = 0; i < gkio_listenner_count; ++i)
    {
        if (strcmp(name, gkio_listenners[i].name) == 0)
        {
            gkio_listenners[i].callback(gkio_data_raw, gkio_data_raw_size);
        }
    }
}

void gkio_loop()
{
    int flag = scanf("%s", gkio_buffer);

    if (flag)
    {
        int i = 0;
        char c;
        while (gkio_buffer[i])
        {
            c = gkio_buffer[i++];
            if (c == '[')
            {
                event_name_length = 0;
                event_data_length = -1; // reading name
            }
            else if (c == ']')
            {
                event_name[event_name_length] = 0;
            }
            else if (c == '(')
            {
                event_data_length = 0;
                event_name_length = -1;
            }
            else if (c == ')')
            {
                event_data[event_data_length] = 0;
                gkio_call_event(event_name, event_data);
                //TODO: e mit event
                event_data_length = -1;
                event_name_length = -1;
            }
            else
            {
                if (event_name_length > -1)
                {
                    event_name[event_name_length++] = c;
                }
                else if (event_data_length > -1)
                {
                    event_data[event_data_length++] = c;
                }
                else
                {
                    //notthing to do
                }
            }
        }
    }
}

#endif
