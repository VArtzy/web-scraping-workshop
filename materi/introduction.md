# Pengantar Web Scraping

## Pendahuluan
Web scraping adalah area pengkodean yang sangat menarik, dengan kombinasi tantangan yang membuat frustasi dan hasil yang sangat memuaskan. Keindahan utama dari scraping adalah kemungkinan yang tak terbatas. Ada data yang hampir tak terhingga di internet, dan banyak di antaranya sangat mudah dikumpulkan dengan scraper sederhana.

Memulai web scraping sangatlah mudah. Bahkan dengan pengalaman koding yang minimal, Anda bisa membangun scraper web pertama Anda dalam waktu singkat. Tutorial ini akan membantu Anda melakukannya!

## Prasyarat
- Python atau Node.js terinstal di komputer Anda tergantung bahasa pilihan Anda
- IDE - PyCharm untuk Python atau VSCode untuk JavaScript/TypeScript disarankan
- Opsional: Ikuti kode dengan repo GitHub: https://github.com/thejoeosborne/simple-scraper

## Langkah 1: Persiapan Proyek
1. Buat folder baru di komputer Anda
2. Buat file `.js` atau `.py` baru di dalam folder tersebut (misalnya `scraper-python.py` atau `scraper-javascript.js`)
3. Buka folder dalam IDE pilihan Anda
4. Instal pustaka yang diperlukan:
   - Untuk Python: `pip install requests` dan `pip install beautifulsoup4`
   - Untuk JavaScript: `npm init` dan `npm install cheerio`
   - Untuk TypeScript: `npm install ts-node`

## Langkah 2: Membuat Permintaan Jaringan
Kita akan menggunakan example.com sebagai situs untuk discrape karena sederhana dan tidak memiliki pemblokiran atau autentikasi.

### Kode Python:
```python
# scraper-python.py
# Untuk menjalankan skrip ini, ketik `python scraper-python.py` di terminal

import requests
from bs4 import BeautifulSoup

def scrape():
    url = 'https://www.example.com'
    response = requests.get(url)
    soup = BeautifulSoup(response.text, 'html.parser')
    print(soup)

if __name__ == '__main__':
    scrape()
```

### Kode JavaScript:
```javascript
/**
 * scraper-javascript.js
 * Untuk menjalankan skrip ini, ketik `node scraper-javascript.js` di terminal
 */

const cheerio = require('cheerio');

(async () => {
  const url = 'https://www.example.com';
  const response = await fetch(url);

  const $ = cheerio.load(await response.text());
  console.log($.html());
})();
```

Jalankan skrip Anda dengan mengetik `python scraper-python.py` atau `node scraper-javascript.js` di terminal Anda.

## Langkah 3: Memilah Data dari HTML
BeautifulSoup dan cheerio adalah pustaka yang membantu kita menavigasi HTML dalam kode. Keduanya memungkinkan kita untuk menggunakan selector CSS tertentu untuk mengambil bagian tertentu dari HTML.

Mari kita ambil 3 hal dari halaman ini: judul, teks, dan tautan "More information...".

### Python:
```python
title = soup.select_one('h1').text
text = soup.select_one('p').text
link = soup.select_one('a').get('href')

print(title)
print(text)
print(link)
```

### JavaScript:
```javascript
const title = $('h1').text();
const text = $('p').text();
const link = $('a').attr('href');

console.log(title);
console.log(text);
console.log(link);
```

Setelah menambahkan baris-baris tersebut ke skrip Anda dan menjalankannya, konsol akan mencetak:
```
Example Domain
This domain is for use in illustrative examples in documents. You may use this domain in literature without prior coordination or asking for permission.More information...
https://www.iana.org/domains/example
```

## Kesimpulan
Selamat! Anda telah berhasil membangun web scraper. Menguasai web scraping membuka kemungkinan tak terbatas. Terutama dengan kemajuan AI baru-baru ini, pengumpulan data massal menjadi lebih berharga dari sebelumnya.

## Catatan Penting
Situs dengan lalu lintas tinggi seperti LinkedIn, Indeed, dll terkenal sulit untuk discrape karena memiliki deteksi bot yang canggih. Mulailah dengan situs yang lebih sederhana untuk mempelajari dasar-dasar.
