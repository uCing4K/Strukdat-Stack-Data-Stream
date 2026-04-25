# Laporan Progress 1 - Arena Allocator

## 1. Pendahuluan
Progress 1 berfokus pada implementasi dasar Arena Allocator sebagai fondasi manajemen memori linear untuk kebutuhan struktur data berbasis sistem. Tujuan utama tahap ini adalah memahami mekanisme alokasi memori berurutan, melakukan reset alokasi secara cepat, dan menyiapkan landasan untuk integrasi Stack serta Data Stream pada progress berikutnya.

Secara teori, Arena Allocator menyimpan blok memori besar sekali di awal, lalu melayani alokasi kecil dengan menaikkan offset. Pendekatan ini sederhana, cepat, dan cocok untuk pola alokasi sementara yang dapat dihapus bersamaan melalui operasi reset.

## 2. Implementasi Program
Implementasi dilakukan pada tiga berkas sumber:
- `src/arena.h`: deklarasi struktur `Arena` dan API utama.
- `src/arena.c`: implementasi fungsi inisialisasi, alokasi, akses data, reset, destroy, cek sisa memori, dan dump visual.
- `src/main.c`: simulasi penggunaan arena dengan skenario sukses dan gagal alokasi.

Skenario uji pada program utama:
1. Inisialisasi arena dengan kapasitas 32 byte.
2. Alokasi blok A (6 byte) dan isi dengan karakter `A`.
3. Alokasi blok B (10 byte) dan isi dengan karakter `B`.
4. Coba alokasi blok C (20 byte) untuk menguji kondisi gagal.
5. Tampilkan dump arena sebelum dan sesudah reset.

## 3. Output Program
Output program menampilkan:
- Kondisi awal arena (semua slot kosong).
- Keberhasilan alokasi blok A dan B beserta offset.
- Kegagalan alokasi blok C karena melebihi kapasitas.
- Kondisi arena setelah alokasi (slot terpakai bertambah).
- Kondisi arena setelah `arena_reset()` (offset kembali 0).

Output aktual eksekusi dicatat setelah proses build dan run.

## 4. Pembahasan dan Kesimpulan
Hasil implementasi menunjukkan Arena Allocator berhasil mengelola alokasi memori linear menggunakan offset. Mekanisme ini membuat proses alokasi efisien karena tidak memerlukan pencarian blok bebas seperti allocator umum. Operasi `arena_reset()` terbukti efektif untuk mengosongkan seluruh alokasi logis dalam satu langkah.

Keterbatasan pada Progress 1 adalah belum adanya dukungan deallocation per-blok dan belum terintegrasi dengan struktur data tingkat lanjut. Meski demikian, fondasi sudah memadai untuk masuk ke Progress 2, yaitu implementasi Stack berbasis arena.
