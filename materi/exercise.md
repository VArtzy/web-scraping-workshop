# Latihan

## Implementasi Web Scraper dengan Python

Kita akan membuat program automation sederhana menggunakan Python untuk melakukan web scraping. Program ini akan mengumpulkan informasi tentang kelas-kelas yang ada di Dicoding, seperti nama kelas, jam belajar, rating, level, deskripsi, jumlah modul, dan total siswa terdaftar.

### Langkah 1: Import Module

Pertama, kita perlu mengimpor beberapa modul Python yang akan digunakan untuk web scraping, seperti Selenium dan BeautifulSoup (BS4).

```python
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.firefox.options import Options
from bs4 import BeautifulSoup
```

- **webdriver**: Antarmuka untuk mengotomatisasi browser web.
- **By**: Metode pencarian elemen dalam halaman web.
- **expected_conditions**: Kondisi yang digunakan bersama dengan WebDriverWait.
- **WebDriverWait**: Menunggu hingga kondisi tertentu terpenuhi.
- **Options**: Mengatur opsi browser Firefox.
- **BeautifulSoup**: Menguraikan dan mengekstrak data dari dokumen HTML atau XML.

### Langkah 2: Konfigurasi WebDriver

Selanjutnya, kita mengonfigurasi WebDriver untuk menggunakan browser Firefox dalam mode headless (tanpa tampilan UI).

```python
def scraper(url):
    try:
        options = Options()
        options.add_argument('-headless')
        driver = webdriver.Firefox(options=options)
```

### Langkah 3: Akses Website

Kemudian, kita mengakses website yang akan di-scrape dan menunggu hingga elemen tertentu muncul.

```python
        driver.get(url)
        try:
            wait = WebDriverWait(driver, timeout=5)
            wait.until(EC.presence_of_element_located((By.ID, 'course-list')))
        except:
            raise LookupError("There is no element specified")
```

### Langkah 4: Parse HTML

Setelah halaman web dimuat, kita menggunakan BeautifulSoup untuk menguraikan HTML dan mengekstrak informasi yang diperlukan.

```python
        content = driver.page_source
        soup = BeautifulSoup(content, 'html.parser')
        courses = []
```

### Langkah 5: Ekstrak Informasi

Kita mengekstrak informasi seperti nama kelas, jam belajar, rating, level, deskripsi, jumlah modul, dan total siswa terdaftar.

```python
        for course in soup.find_all('div', class_='col-md-6 mb-3'):
            course_name = course.find('h5', class_='course-card__name').text
            course_hour = course.find_all('span', {'class':'mr-2'})[0].text
            course_summary = course.select('div.course-card__summary p')[0].text
            course_total_module = course.find_all('div', class_= 'course-card__info-item')[0].find_all('span')[0].contents[0]
            course_level = course.find('span', attrs={'class': None}).text
            
            try:
                course_rating = course.find_all('span', {'class':'mr-2'})[1].text
            except IndexError:
                course_rating = ''
            
            try:
                course_total_students = course.find_all('span', {'class':'mr-3'})[1].get_text()
            except:
                course_total_students = ''
```

### Langkah 6: Penambahan Data

Data yang telah diekstrak dimasukkan ke dalam list `courses`.

```python
            courses.append(
                {
                    'Course Name': course_name,
                    'Learning Hour': course_hour,
                    'Rating': course_rating,
                    'Level': course_level,
                    'Summary': course_summary,
                    'Total Modules': course_total_module,
                    'Total Students': course_total_students
                }
            )
```

### Langkah 7: Penutupan Sesi

Setelah selesai, kita menutup sesi WebDriver dan mengembalikan hasil scraping.

```python
        driver.quit()
        return courses
```

### Langkah 8: Penanganan Error

Kita juga menambahkan penanganan error untuk menangkap kesalahan yang mungkin terjadi selama proses scraping.

```python
    except Exception as e:
        print('An error occurred: ', e)
        driver.quit()
```

### Langkah 9: Pemanggilan Fungsi Scraper

Terakhir, kita memanggil fungsi `scraper()` dan menyimpan hasilnya dalam file JSON.

```python
import json

if __name__ == '__main__':
    url = 'https://www.dicoding.com/academies/list'
    data = scraper(url)
    
    with open('dicoding_data.json', 'w') as json_file:
        json.dump(data, json_file, indent=4)
```

## Hasil

Setelah menjalankan script, kita akan mendapatkan file `dicoding_data.json` yang berisi data kelas dari Dicoding.

```json
[
    {
        "Course Name": "Belajar Analisis Data dengan Python",
        "Learning Hour": "30 Jam",
        "Rating": "4,83",
        "Level": "Pemula",
        "Summary": "Pelajari berbagai konsep dasar analisis data beserta tahapannya, dilengkapi pembahasan studi kasus menggunakan bahasa pemrograman Python.",
        "Total Modules": "61 Modul",
        "Total Students": "26.692 Siswa Terdaftar"
    },
    {
        "Course Name": "Belajar Tools Front-End Web Intermediate",
        "Learning Hour": "65 Jam",
        "Rating": "4,79",
        "Level": "Mahir",
        "Summary": "Pelajari tools berstandar industri dengan Sass, Bootstrap, Lit, Axios, dan Firebase dalam membangun aplikasi web yang efisien dan powerful.",
        "Total Modules": "131 Modul",
        "Total Students": "1.283 Siswa Terdaftar"
    },
    ...
]
```

## Latihan: JavaScript

Buat skrip web scraping yang sama, menggunakan bahasa JavaScript. Anda dapat mengikuti langkah-langkah yang sebelumnya sudah dijelaskan tetapi dalam bahasa JavaScript. ingat, Anda selalu bisa dan butuh untuk melihat dokumentasi sebagai manual dan latihan yang Anda lakukan akan sangat berharga untuk mempertajam intuisi Anda. Semangat!

## Penutup

Dengan Python, kita dapat membuat program automation sederhana seperti web scraper untuk mengumpulkan data dari internet secara otomatis. Program ini sangat berguna untuk meningkatkan efisiensi dan produktivitas pekerjaan sehari-hari. Selanjutnya, coba buat automation versi Anda sendiri dan eksplorasi lebih lanjut!
