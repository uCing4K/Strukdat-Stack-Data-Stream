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
- `progress/progress-01-arena-allocator/src/`: source code Progress 1 (aktif)
- `progress/progress-01-arena-allocator/docs/`: draft laporan dan output eksekusi Progress 1
- `LAPORAN PROGRES1_KELOMPOK2/`: referensi implementasi awal
- `format laporan.txt`: format wajib laporan setiap progress
- `README.md`: dokumen utama proyek yang akan di-*patch* setiap progress

## Build dan Run (Progress 1 - Aktif)
Masuk ke folder Progress 1, lalu kompilasi dengan GCC:

```bash
cd "progress/progress-01-arena-allocator/src"
gcc main.c arena.c -o main
./main
```

Jika menggunakan PowerShell di Windows:

```powershell
Set-Location "progress/progress-01-arena-allocator/src"
gcc main.c arena.c -o main
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

## Roadmap Progress (1-4)
### Progress 1 - Dasar Arena Allocator
- Status: Selesai (implementasi ulang dari awal tersedia dan tervalidasi).
- Fokus:
  - Inisialisasi arena.
  - Alokasi linear sederhana.
  - Simulasi isi data dan reset.
  - Visualisasi kondisi memori.
  - Validasi skenario gagal saat kapasitas tidak cukup.

### Progress 2 - Integrasi ke Stack
- Status: Belum dimulai.
- Target:
  - Implementasi stack berbasis arena.
  - Operasi push/pop/peek dengan validasi kapasitas.
  - Pengujian skenario overflow/underflow.

### Progress 3 - Integrasi Data Stream
- Status: Belum dimulai.
- Target:
  - Simulasi aliran data (stream chunk/event).
  - Penempatan data stream ke arena secara terstruktur.
  - Monitoring offset, fragmentasi logis, dan efisiensi.

### Progress 4 - Finalisasi Sistem dan Evaluasi
- Status: Belum dimulai.
- Target:
  - Integrasi stack + data stream berbasis arena.
  - Uji skenario gabungan dan pembandingan hasil.
  - Penyusunan kesimpulan akhir dan evaluasi desain.

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
