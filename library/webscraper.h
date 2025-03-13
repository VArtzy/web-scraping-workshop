#ifndef WEBSCRAPER_H
#define WEBSCRAPER_H

#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur data untuk menyimpan respons HTTP
typedef struct {
    char* data;
    size_t size;
} HttpResponse;

// Deklarasi fungsi
HttpResponse makeRequest(const wchar_t* host, const wchar_t* path);
void freeResponse(HttpResponse* response);
char* extractElement(const char* html, const char* tag);
char* extractAttribute(const char* element, const char* attribute);

#endif
