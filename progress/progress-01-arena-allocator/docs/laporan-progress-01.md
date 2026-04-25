# Laporan Progress 1 - Arena Allocator

## 1. Pendahuluan
### 1.1 Latar Belakang
Pada pengembangan struktur data berbasis sistem, pengelolaan memori menjadi faktor penting karena memengaruhi performa dan kestabilan program. Alokasi memori dinamis yang umum digunakan memang fleksibel, tetapi pada beberapa skenario dapat menimbulkan overhead dan kompleksitas pengelolaan.

Arena Allocator dipilih karena menggunakan pendekatan alokasi linear: memori dialokasikan sekali di awal dalam satu blok besar, kemudian permintaan alokasi kecil dilayani dengan pergeseran offset. Model ini sederhana, cepat, dan sesuai untuk kebutuhan alokasi sementara yang dapat dibersihkan sekaligus melalui operasi reset.

### 1.2 Tujuan Progress 1
Berdasarkan deskripsi penugasan yang diberikan, tujuan dari proyek ini adalah:
1. Mengimplementasikan Arena Allocator sebagai Abstract Data Type (ADT) yang mandiri tanpa bergantung pada alokasi memori dinamis di level struktur data.
2. Membangun struktur data (seperti array dan linked list) yang seluruh elemennya berada di dalam satu blok memori terpusat.
3. Melakukan analisis komparatif antara penggunaan Arena Allocator dengan mekanisme malloc/free dari sisi performa dan efisiensi memori.
4. Memvisualisasikan tata letak memori dalam bentuk grid ASCII untuk memahami perilaku alokasi secara riil.

### 1.3 Lingkup Tahapan Minggu ke-1
Pada tahap awal ini (Minggu 1), fokus utama tim adalah membangun fondasi sistem berupa Arena Allocator. Implementasi difokuskan pada pembuatan fungsi-fungsi dasar API arena yang meliputi:

1) `arena_init`: Inisialisasi blok memori awal dan kapasitas arena.
2) `arena_alloc`: Mekanisme pembagian memori di dalam arena secara linear.
3) `arena_get`: Pengambilan data berdasarkan `offset` yang sudah dialokasikan.
4) `arena_reset`: Pengosongan arena secara instan untuk memulai alokasi dari awal kembali.

### 1.4 Dasar Teori Singkat
Arena Allocator bekerja dengan prinsip bump allocation, yaitu menaikkan pointer atau offset setiap kali alokasi dilakukan tanpa mekanisme pembebasan per-blok. Keunggulan pendekatan ini adalah biaya alokasi yang rendah dan implementasi yang mudah dipahami. Konsekuensinya, pelepasan memori umumnya dilakukan secara kolektif melalui reset seluruh arena.

## 2. Implementasi Program
### 2.1 Konsep Implementasi
Pada tahap implementasi program ini, kami menerapkan konsep Arena Allocator sebagai blok memori utama untuk menampung alokasi data secara linear. Tahap awal difokuskan pada pembentukan arena sebagai wadah memori terpusat, kemudian program akan membagi memori tersebut melalui mekanisme offset sesuai kebutuhan alokasi data.

Sebelum masuk ke penerapan struktur data lanjutan pada progress berikutnya, kami menyiapkan fungsi-fungsi krusial untuk membangun arena, yaitu:

1) `arena_init`: Inisialisasi blok memori awal sebagai arena utama.
2) `arena_alloc`: Mekanisme pembagian memori di dalam arena secara berurutan.
3) `arena_get`: Pengambilan data berdasarkan `offset` hasil alokasi.
4) `arena_reset`: Pengosongan arena secara instan untuk memulai alokasi dari awal kembali.
5) `arena_destroy`: Pelepasan memori arena setelah program selesai digunakan.

### 2.2 Struktur Berkas dan Alur Uji Program
Implementasi dilakukan pada tiga berkas sumber utama, yaitu `src/arena.h`, `src/arena.c`, dan `src/main.c`. Berkas `arena.h` digunakan untuk mendeklarasikan struktur `Arena` serta prototipe fungsi, `arena.c` berisi implementasi seluruh fungsi utama arena, sedangkan `main.c` digunakan untuk menguji perilaku alokasi, penulisan data, pencetakan kondisi arena, dan proses reset.

Skenario uji pada program utama disusun sebagai berikut:
1. Inisialisasi arena dengan kapasitas 32 byte.
2. Alokasi blok A sebesar 6 byte dan isi dengan karakter `A`.
3. Alokasi blok B sebesar 10 byte dan isi dengan karakter `B`.
4. Coba alokasi blok C sebesar 20 byte untuk menguji kondisi gagal.
5. Tampilkan kondisi arena sebelum dan sesudah proses reset.

## 3. Output Program
### 3.1 Ringkasan Hasil Eksekusi
Output program menunjukkan hal-hal berikut:
- Kondisi awal arena (semua slot kosong).
- Keberhasilan alokasi blok A dan B beserta offset.
- Kegagalan alokasi blok C karena melebihi kapasitas.
- Kondisi arena setelah alokasi (slot terpakai bertambah).
- Kondisi arena setelah `arena_reset()` (offset kembali 0).

### 3.2 Catatan Output Aktual
Output aktual hasil eksekusi tersimpan pada berkas `docs/output-progress-01.txt`.

## 4. Pembahasan dan Kesimpulan
### 4.1 Pembahasan
Hasil implementasi menunjukkan Arena Allocator berhasil mengelola alokasi memori linear menggunakan offset. Mekanisme ini membuat proses alokasi efisien karena tidak memerlukan pencarian blok bebas seperti allocator umum. Operasi `arena_reset()` terbukti efektif untuk mengosongkan seluruh alokasi logis dalam satu langkah.

### 4.2 Kesimpulan
Keterbatasan pada Progress 1 adalah belum adanya dukungan deallocation per-blok dan belum terintegrasi dengan struktur data tingkat lanjut. Meski demikian, fondasi implementasi sudah memadai untuk melanjutkan ke Progress 2, yaitu pengembangan Stack berbasis arena.
