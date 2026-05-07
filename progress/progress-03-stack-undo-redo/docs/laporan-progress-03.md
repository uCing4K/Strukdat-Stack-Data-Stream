# Laporan Progress 3 — Stack + Data Stream (Undo/Redo Simulator)

**Kelompok:** 10

## 1. Pendahuluan

Pada Progress 3 kami mengembangkan eksperimen awal berupa Undo/Redo Simulator yang menggabungkan struktur data `Stack` dengan konteks data stream. Tujuan eksperimen ini adalah: (1) menerapkan ADT `Stack` berbasis Arena (offset pointer), (2) mendemonstrasikan mekanisme undo/redo menggunakan dua stack (undo/redo), dan (3) mengevaluasi trade-off snapshot-based state saving pada arena allocator.

Lingkup: implementasi hanya memanfaatkan Arena allocator (tanpa `malloc`/`free` pada level struktur data), menyimpan snapshot state pada stack, serta mensimulasikan serangkaian operasi: append, remove, undo, redo.

## 2. Implementasi Program

- `arena.h` / `arena.c`
  - Arena statik 4096 byte (buffer statik) dengan fungsi `arena_alloc`, `arena_get`, `arena_reset`, `arena_dump`.
  - Arena digunakan sebagai satu-satunya sumber memori untuk seluruh struktur data.

- `stack.h` / `stack.c`
  - ADT `Stack` menggunakan offset-based nodes: setiap node menyimpan `size_t next_offset` diikuti payload.
  - Operasi: `stack_create`, `stack_push`, `stack_pop`, `stack_peek`, `stack_traverse`.
  - Stack digunakan untuk menyimpan `Snapshot` (payload adalah struktur `Snapshot`).

- `snapshot.h` / `snapshot.c`
  - `Snapshot` menyimpan offset ke blob data pada arena, ukuran elemen, dan jumlah elemen.
  - `snapshot_create` mengalokasikan ruang di arena dan menyalin state saat ini.
  - `snapshot_restore` menyalin kembali data snapshot ke `current_state` manager.

- `session_manager.h` / `session_manager.c`
  - Manajer sesi menyimpan `current_state` (array integer) di arena, `undo_stack`, dan `redo_stack`.
  - Set operasi tingkat tinggi: `session_append`, `session_remove`, `session_undo`, `session_redo`, `session_save_state`, `session_clear_redo`.
  - Setiap perubahan yang valid menyimpan snapshot sebelum modifikasi (snapshot-based undo).

- `main.c`
  - Program demonstrator: inisialisasi arena 512 byte, session manager (max 20 elemen), serangkaian operasi (append x4, undo x2, redo x1, append baru, remove, undo).
  - Memanggil `arena_dump` untuk visualisasi memori.

Semua file menyimpan hubungan pointer menggunakan offset (tidak ada pointer heap tersendiri) dan tidak memanggil `malloc`/`free` pada level struktur data.

## 3. Output Program

Contoh output eksekusi (ringkasan):

- Inisialisasi arena 512 byte, dump awal menunjukkan semua kosong.
- Setelah 4 append: current state = [10, 20, 30, 40]
- Setelah 2 undo: current state = [10, 20]
- Setelah 1 redo: current state = [10, 20, 30]
- Setelah append 99 (paska redo): current state = [10, 20, 30, 99] (redo cleared)
- Setelah remove index 1: current state = [10, 30, 99]
- Setelah undo remove: current state = [10, 20, 30, 99]
- Final arena used: 356 bytes (dari 512)

File output lengkap disimpan di `docs/output-progress-03.txt`.

## 4. Pembahasan & Kesimpulan

- Kelebihan:
  - Implementasi offset-based memastikan serialisasi mudah dan mematuhi aturan penugasan (tanpa `malloc` di level struktur data).
  - Snapshot-based undo sederhana dan robust untuk ukuran state kecil hingga menengah.
  - Visualisasi `arena_dump` membantu memahami penggunaan memori.

- Keterbatasan:
  - Snapshot menyimpan salinan penuh state sehingga cepat menghabiskan memori jika state besar atau operasi sering.
  - Undo/Redo berbasis stack memerlukan strategi pembersihan snapshot lama jika memori terbatas.
  - Saat ini append/remove O(n) untuk operasi tertentu (remove shifting), dapat ditingkatkan dengan struktur lain.

- Rekomendasi pengembangan lanjut:
  - Implementasi copy-on-write atau delta snapshot untuk mengurangi penggunaan memori.
  - Variasi eksperimen: gunakan `Queue + Batch Processing` atau `Hash Table + Data Stream` pada Progress 4.

---

_Dokumentasi singkat ini disiapkan untuk Progress 3: Stack + Data Stream (Undo/Redo Simulator). Jika mau, saya akan perinci bagian implementasi kode per fungsi dan menambahkan diagram alur._
