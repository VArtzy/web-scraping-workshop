# Under the hood

## Web Scraping library

Kita sudah berlatih menggunakan berbagai macam web scraping library dari berbagai macam bahasa pemrograman. Mulai dari Puppeteer, Selenium, BeautifulSoup, Cheerio, XPath, dsb. Memang data selalu dapat diambil dan disetor dalam berbagai bentuk dengan style dan betuk serta fitur sendiri, untuk itulah kita membutuhkan berbagai macam library.

Walaupun banyak library, selalu refer ke manual page yaitu dokumentasi dari library yang kita pakai karena memang kita tidak perlu mengigat semua detil dan metode yang ada. Namun kita dapat membuka dokumentasi kapanpun!

Dokumentasi kadang tidak ada websitenya untuk library yang masih kecil/berkembang, namun pasti ia memiliki github repository yang mengandung dokumentasi jika memang library itu dibuat untuk tidak digunakan sendiri.

berikut beberapa tautan ke dokumentasi library (Anda dapat mencari sendiri untuk yang lain di web):

- [Puppeteer](https://pptr.dev/category/introduction)
- [Selenium](https://www.selenium.dev/documentation/)
- [BeautifulSoup](https://www.crummy.com/software/BeautifulSoup/bs4/doc/)
- https://github.com/cheeriojs/cheerio
- https://www.w3schools.com/xml/xpath_intro.asp

## Fundamental

seperti yang sudah kita lakukan di bagian pengantar, web scraping dalam bentuk paling sederhana adalah membuka website atau melakukan `curl` pada website tertentu, lalu menngambil datanya. Bedanya dengan web scraping sunguhan, langkah ini belum terotomatisasi.

Membuat web scraping library akan memperjelas hal ini, membangun dasar dan fundamental yang kuat. Mari membuat web scraping library dengan langkah dan konsep dasar web scraping yang sudah kita pahami!

## Membuat Library Web Scraper

Meskipun Python dan JavaScript sering digunakan untuk web scraping, membuat web scraper dasar dengan bahasa C dapat memberikan pemahaman lebih dalam tentang cara kerja HTTP request dan parsing HTML di level yang lebih rendah. Pada bagian ini, kita akan membuat library web scraping sederhana menggunakan C untuk Windows dengan memanfaatkan WinHTTP untuk request jaringan dan parser HTML minimal.

## Prasyarat

- Sistem operasi Windows
- Visual Studio atau MinGW untuk kompilasi
- Pemahaman dasar pemrograman C
- Pengetahuan tentang protokol HTTP dan struktur HTML

## Persiapan Proyek

Pertama, buat proyek C baru di IDE favorit Anda. Kita perlu menyertakan header Windows untuk komunikasi HTTP:

```c
// webscraper.h
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

#endif // WEBSCRAPER_H
```

## Implementasi HTTP Request

Selanjutnya, kita akan mengimplementasi fungsi inti untuk melakukan HTTP request menggunakan WinHTTP:

```c
// webscraper.c
#include "webscraper.h"

HttpResponse makeRequest(const wchar_t* host, const wchar_t* path) {
    HttpResponse response = {NULL, 0};
    
    // Inisialisasi WinHTTP
    HINTERNET hSession = WinHttpOpen(
        L"C Web Scraper/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    
    if (!hSession) {
        printf("Error opening WinHTTP session: %lu\n", GetLastError());
        return response;
    }
    
    // Koneksi ke server
    HINTERNET hConnect = WinHttpConnect(
        hSession,
        host,
        INTERNET_DEFAULT_HTTP_PORT,
        0);
    
    if (!hConnect) {
        printf("Error connecting to server: %lu\n", GetLastError());
        WinHttpCloseHandle(hSession);
        return response;
    }
    
    // Buat HTTP request
    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"GET",
        path,
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        0);
    
    if (!hRequest) {
        printf("Error creating HTTP request: %lu\n", GetLastError());
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return response;
    }
    
    // Kirim request
    BOOL bResult = WinHttpSendRequest(
        hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS,
        0,
        WINHTTP_NO_REQUEST_DATA,
        0, 
        0,
        0);
    
    if (!bResult) {
        printf("Error sending HTTP request: %lu\n", GetLastError());
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return response;
    }
    
    // Terima respons
    bResult = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResult) {
        printf("Error receiving HTTP response: %lu\n", GetLastError());
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return response;
    }
    
    // Baca data respons
    DWORD bytesAvailable = 0;
    DWORD bytesRead = 0;
    char* buffer = NULL;
    size_t bufferSize = 0;
    
    do {
        // Cek jumlah byte yang tersedia
        bytesAvailable = 0;
        bResult = WinHttpQueryDataAvailable(hRequest, &bytesAvailable);
        if (!bResult) {
            printf("Error querying data available: %lu\n", GetLastError());
            break;
        }
        
        // Tidak ada data lagi
        if (bytesAvailable == 0) {
            break;
        }
        
        // Alokasi atau resize buffer
        char* newBuffer = (char*)realloc(buffer, bufferSize + bytesAvailable + 1);
        if (!newBuffer) {
            printf("Memory allocation failed\n");
            free(buffer);
            buffer = NULL;
            break;
        }
        buffer = newBuffer;
        
        // Baca data
        bytesRead = 0;
        bResult = WinHttpReadData(
            hRequest,
            buffer + bufferSize,
            bytesAvailable,
            &bytesRead);
        
        if (!bResult) {
            printf("Error reading data: %lu\n", GetLastError());
            break;
        }
        
        bufferSize += bytesRead;
        buffer[bufferSize] = '\0';  // Null-terminate string
        
    } while (bytesAvailable > 0);
    
    // Tutup handle
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    
    response.data = buffer;
    response.size = bufferSize;
    
    return response;
}

void freeResponse(HttpResponse* response) {
    if (response && response->data) {
        free(response->data);
        response->data = NULL;
        response->size = 0;
    }
}
```

## Implementasi Parsing HTML Dasar

Selanjutnya, kita tambahkan fungsi parsing HTML sederhana:

```c
// html_parser.c
#include "webscraper.h"

// Ekstrak konten antara tag pembuka dan penutup
char* extractElement(const char* html, const char* tag) {
    if (!html || !tag) return NULL;
    
    char openTag[256];
    char closeTag[256];
    
    sprintf(openTag, "<%s", tag);
    sprintf(closeTag, "</%s>", tag);
    
    char* start = strstr(html, openTag);
    if (!start) return NULL;
    
    // Cari akhir tag pembuka
    start = strchr(start, '>');
    if (!start) return NULL;
    start++;  // Lewati '>'
    
    char* end = strstr(start, closeTag);
    if (!end) return NULL;
    
    size_t length = end - start;
    char* result = (char*)malloc(length + 1);
    if (!result) return NULL;
    
    strncpy(result, start, length);
    result[length] = '\0';
    
    return result;
}

// Ekstrak nilai atribut dari elemen
char* extractAttribute(const char* element, const char* attribute) {
    if (!element || !attribute) return NULL;
    
    char searchStr[256];
    sprintf(searchStr, "%s=\"", attribute);
    
    char* start = strstr(element, searchStr);
    if (!start) return NULL;
    
    start += strlen(searchStr);
    char* end = strchr(start, '"');
    if (!end) return NULL;
    
    size_t length = end - start;
    char* result = (char*)malloc(length + 1);
    if (!result) return NULL;
    
    strncpy(result, start, length);
    result[length] = '\0';
    
    return result;
}
```

## Contoh Penggunaan

Berikut contoh penggunaan library web scraping yang telah dibuat:

```c
// example.c
#include "webscraper.h"

int main() {
    // Lakukan request ke example.com
    const wchar_t* host = L"www.example.com";
    const wchar_t* path = L"/";
    
    printf("Membuat request ke %ls%ls\n", host, path);
    HttpResponse response = makeRequest(host, path);
    
    if (response.data) {
        printf("Menerima %zu byte data\n", response.size);
        
        // Ekstrak judul
        char* title = extractElement(response.data, "title");
        if (title) {
            printf("Judul: %s\n", title);
            free(title);
        }
        
        // Ekstrak paragraf pertama
        char* paragraph = extractElement(response.data, "p");
        if (paragraph) {
            printf("Paragraf pertama: %s\n", paragraph);
            free(paragraph);
        }
        
        // Ekstrak link
        char* html = response.data;
        char* linkStart;
        while ((linkStart = strstr(html, "<a ")) != NULL) {
            html = linkStart + 2;
            char* href = extractAttribute(linkStart, "href");
            if (href) {
                printf("Link ditemukan: %s\n", href);
                free(href);
            }
        }
        
        // Bebaskan respons
        freeResponse(&response);
    } else {
        printf("Gagal menerima respons\n");
    }
    
    return 0;
}
```

## Kompilasi dan Menjalankan

Untuk mengompilasi proyek, Anda perlu menghubungkan dengan library `winhttp.lib`. Berikut caranya dengan Visual Studio:

```
cl webscraper.c html_parser.c example.c /link winhttp.lib
```

Jika menggunakan MinGW, perintahnya adalah:

```
gcc webscraper.c html_parser.c example.c -o webscraper.exe -lwinhttp
```

Jalankan file executable yang telah dikompilasi:

```
webscraper.exe
```

## Kesimpulan

Membuat web scraper dengan C memberikan pemahaman mendalam tentang protokol HTTP dan parsing HTML. Meskipun implementasi ini masih dasar, ini bisa menjadi fondasi untuk membuat tools web scraping yang lebih canggih. Selalu ingat untuk jangan membebani server, menghormati aturan penggunaan website, dan file `robots.txt` saat melakukan scraping.

## Penutup

Dalam kursus ini, kita telah menjelajahi dunia web scraping, mulai dari pengenalan dasar tentang apa itu web scraping, metode-metodenya, fungsi, serta kegunaannya dalam berbagai bidang. Melalui latihan, kita mempraktikkan contoh nyata penggunaan web scraping dengan Python dan JavaScript, memungkinkan kita untuk memahami bagaimana teknik ini diterapkan dalam skenario dunia nyata.

Selain itu, kita juga membongkar cara kerja di balik layar dengan mempelajari library web scraping yang populer dan bahkan mencoba membuat library web scraping sederhana sendiri. Dengan pengetahuan ini, Anda kini memiliki fondasi yang kuat untuk mengembangkan tools scraping yang lebih canggih atau mengeksplorasi aplikasi web scraping dalam proyek-proyek masa depan. Selamat mencoba dan teruslah bereksperimen!
