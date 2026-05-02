# Strukdat 2026 - Stack & Data Stream (Arena Allocator)

Repository ini digunakan untuk penugasan akhir **Struktur Data 2026** dengan topik **Stack & Data Stream**.
Fokus implementasi inti adalah membangun **Arena Allocator** berbasis C sebagai fondasi manajemen memori untuk struktur data berbasis sistem.

## Tujuan Proyek
- Mengimplementasikan Arena Allocator secara bertahap dan terukur.
- Mendokumentasikan setiap tahap dalam 4 progress.
- Menjaga sinkronisasi antara source code, hasil output, dan laporan praktikum.

## Referensi Lokal
- Format laporan: `format laporan.txt`
- Referensi laporan PDF:
  - `LAPORAN PROGRES1_KELOMPOK2.pdf`
  - `LAPORAN PROGRES1_KELOMPOK2/LAPORAN PROGRES1_KELOMPOK 2.pdf`
- Referensi kode awal Progress 1 (lama):
  - `LAPORAN PROGRES1_KELOMPOK2/arena.h`
  - `LAPORAN PROGRES1_KELOMPOK2/arena.c`
  - `LAPORAN PROGRES1_KELOMPOK2/main.c`
- Implementasi ulang Progress 1 (aktif):
  - `progress/progress-01-arena-allocator/src/arena.h`
  - `progress/progress-01-arena-allocator/src/arena.c`
  - `progress/progress-01-arena-allocator/src/main.c`

## Struktur Repository Saat Ini
- `progress/progress-01-arena-allocator/src/`: source code Progress 1
- `progress/progress-01-arena-allocator/docs/`: draft laporan dan output eksekusi Progress 1
- `progress/progress-02-array-linked-list/src/`: source code Progress 2 (aktif)
- `progress/progress-02-array-linked-list/docs/`: draft laporan dan output eksekusi Progress 2
- `LAPORAN PROGRES1_KELOMPOK2/`: referensi implementasi awal
- `format laporan.txt`: format wajib laporan setiap progress
- `README.md`: dokumen utama proyek yang akan di-*patch* setiap progress

## Build dan Run (Progress 2 - Aktif)
Masuk ke folder Progress 2, lalu kompilasi dengan GCC:

```bash
cd "progress/progress-02-array-linked-list/src"
gcc main.c arena.c array.c linked_list.c -o main
./main
```

Jika menggunakan PowerShell di Windows:

```powershell
Set-Location "progress/progress-02-array-linked-list/src"
gcc main.c arena.c array.c linked_list.c -o main
.\main.exe
```

## Ringkasan Implementasi Progress 1
Komponen utama Arena Allocator:
- `arena_init`: inisialisasi buffer arena, kapasitas, dan offset awal dengan validasi input.
- `arena_alloc`: alokasi linear berdasarkan offset dengan output offset aman.
- `arena_get`: mengakses pointer data berdasarkan offset valid.
- `arena_reset`: reset offset tanpa mengalokasi ulang memori.
- `arena_destroy`: membebaskan memori arena dengan aman.
- `arena_available`: menghitung sisa kapasitas arena.
- `arena_dump`: visualisasi penggunaan arena (`[#]` terpakai, `[.]` kosong).

Artefak Progress 1:
- Source: `progress/progress-01-arena-allocator/src/`
- Draft laporan: `progress/progress-01-arena-allocator/docs/laporan-progress-01.md`
- Output eksekusi: `progress/progress-01-arena-allocator/docs/output-progress-01.txt`

## Ringkasan Implementasi Progress 2
Komponen utama Struktur Data Dasar (Array dan Linked List berbasis Arena):

**Array ADT** (`array.h` / `array.c`):
- `array_create`: membuat array kontigu di dalam arena dengan kapasitas tetap.
- `array_append`: menambah elemen ke akhir array (linear push).
- `array_get`: mengakses data elemen berdasarkan index.
- `array_set`: mengubah nilai elemen pada index tertentu.
- `array_print_info`: menampilkan metadata array (offset, count, kapasitas, ukuran elemen).

**Linked List ADT** (`linked_list.h` / `linked_list.c`):
- Menggunakan **offset-based pointer** (bukan pointer C biasa).
- Setiap node memiliki struktur: `[size_t next_offset][data]`.
- `linked_list_create`: inisialisasi linked list kosong.
- `linked_list_append`: menambah elemen ke akhir list.
- `linked_list_prepend`: menambah elemen ke awal list.
- `linked_list_get`: mengakses data elemen pada index tertentu.
- `linked_list_traverse`: melakukan traversal dan menampilkan semua elemen.
- `linked_list_print_info`: menampilkan metadata list (head offset, count, ukuran data).

**Karakteristik Implementasi:**
- Seluruh data array dan linked list disimpan **dalam arena**, tidak menggunakan malloc/free.
- Offset-based pointer memungkinkan serialisasi dan cache locality yang lebih baik.
- Arena dump memvisualisasikan fragmentasi dan penggunaan memori (`[#]` terpakai, `[.]` kosong).

Artefak Progress 2:
- Source: `progress/progress-02-array-linked-list/src/`
- Draft laporan: `progress/progress-02-array-linked-list/docs/laporan-progress-02.md`
- Output eksekusi: `progress/progress-02-array-linked-list/docs/output-progress-02.txt`

## Roadmap Progress (1-4)
### Progress 1 - Dasar Arena Allocator
- Status: Selesai (implementasi ulang dari awal tersedia dan tervalidasi).
- Fokus:
  - Inisialisasi arena.
  - Alokasi linear sederhana.
  - Simulasi isi data dan reset.
  - Visualisasi kondisi memori.
  - Validasi skenario gagal saat kapasitas tidak cukup.

### Progress 2 - Struktur Data Dasar (Array & Linked List)
- Status: Selesai (Array dan Linked List berbasis offset diimplementasikan dan tervalidasi).
- Fokus:
  - Implementasi Array ADT dengan alokasi kontigu dalam arena.
  - Implementasi Linked List ADT dengan offset-based pointer.
  - Operasi append, prepend, get, set, traverse.
  - Visualisasi memory layout setelah alokasi struktur data.
  - Demonstrasi efisiensi offset-based representation.

### Progress 3 - Stack dengan Data Stream
- Status: Belum dimulai.
- Target:
  - Implementasi Stack ADT berbasis arena.
  - Integrasi dengan data stream (simulasi push/pop dengan aliran data).
  - Operasi push/pop/peek dengan validasi kapasitas.
  - Pengujian skenario overflow/underflow.

### Progress 4 - Finalisasi Sistem dan Evaluasi
- Status: Belum dimulai.
- Target:
  - Finalisasi stack + data stream berbasis arena.
  - Uji skenario gabungan dan pembandingan hasil.
  - Penyusunan kesimpulan akhir dan evaluasi desain.
  - Presentasi dan dokumentasi lengkap.

## Template Isi Laporan Setiap Progress
Setiap progress wajib mengikuti format berikut (sesuai `format laporan.txt`):

1. **Pendahuluan**
   - Tujuan progress.
   - Dasar teori yang relevan.

2. **Implementasi Program**
   - Studi kasus/soal yang dikerjakan.
   - Penjelasan singkat desain solusi.
   - Source code yang diimplementasikan.

3. **Output Program**
   - Hasil eksekusi aktual.
   - Bukti output sesuai implementasi.

4. **Pembahasan dan Kesimpulan**
   - Analisis hasil program.
   - Analisis singkat source code.
   - Kesimpulan progress.

## Aturan Patch README per Progress
Saat masuk progress baru, lakukan pembaruan README dengan pola ini:
- Ubah status progress aktif.
- Tambahkan ringkasan fitur/operasi baru.
- Tambahkan cara build/run terbaru.
- Tambahkan ringkasan hasil output penting.
- Tambahkan masalah yang ditemukan dan solusi singkat.

## Rencana GitHub
Remote repository tujuan:
- https://github.com/uCing4K/Strukdat-Stack-Data-Stream.git

Saran alur commit per progress:
1. Commit implementasi kode.
2. Commit pembaruan README.
3. Commit artefak laporan (jika diperlukan).

Dengan alur ini, histori perubahan per progress akan tetap jelas dan mudah ditelusuri.
