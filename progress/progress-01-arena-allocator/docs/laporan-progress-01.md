# Laporan Progress 1 - Arena Allocator

## 1. Pendahuluan
### 1.1 Latar Belakang
Pada pengembangan struktur data berbasis sistem, pengelolaan memori menjadi faktor penting karena memengaruhi performa dan kestabilan program. Alokasi memori dinamis yang umum digunakan memang fleksibel, tetapi pada beberapa skenario dapat menimbulkan overhead dan kompleksitas pengelolaan.

Arena Allocator dipilih karena menggunakan pendekatan alokasi linear: memori dialokasikan sekali di awal dalam satu blok besar, kemudian permintaan alokasi kecil dilayani dengan pergeseran offset. Model ini sederhana, cepat, dan sesuai untuk kebutuhan alokasi sementara yang dapat dibersihkan sekaligus melalui operasi reset.

### 1.2 Tujuan Progress 1
Progress 1 bertujuan untuk:
1. Mengimplementasikan Arena Allocator dasar dalam bahasa C.
2. Memvalidasi proses alokasi berhasil dan alokasi gagal saat kapasitas tidak mencukupi.
3. Menunjukkan mekanisme reset arena sebagai dasar integrasi ke Stack dan Data Stream pada progress berikutnya.

### 1.3 Dasar Teori Singkat
Arena Allocator bekerja dengan prinsip bump allocation, yaitu menaikkan pointer/offset setiap kali alokasi dilakukan tanpa mekanisme pembebasan per-blok. Keunggulan pendekatan ini adalah biaya alokasi yang rendah dan implementasi yang mudah dipahami. Konsekuensinya, pelepasan memori umumnya dilakukan secara kolektif melalui reset seluruh arena.

## 2. Implementasi Program
### 2.1 Struktur Berkas
Implementasi dilakukan pada tiga berkas sumber:
- `src/arena.h`: deklarasi struktur `Arena` dan API utama.
- `src/arena.c`: implementasi fungsi inisialisasi, alokasi, akses data, reset, destroy, cek sisa memori, dan dump visual.
- `src/main.c`: simulasi penggunaan arena dengan skenario sukses dan gagal alokasi.

### 2.2 Alur Uji Program
Skenario uji pada program utama:
1. Inisialisasi arena dengan kapasitas 32 byte.
2. Alokasi blok A (6 byte) dan isi dengan karakter `A`.
3. Alokasi blok B (10 byte) dan isi dengan karakter `B`.
4. Coba alokasi blok C (20 byte) untuk menguji kondisi gagal.
5. Tampilkan dump arena sebelum dan sesudah reset.

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
