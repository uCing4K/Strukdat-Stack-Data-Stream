# Laporan Progress 3: Stack dan Data Stream (Undo/Redo Simulator)

## 1. Pendahuluan

### 1.1 Tujuan Progress
Progress 3 bertujuan untuk mengimplementasikan struktur data Stack dan mengintegrasikannya dengan konsep Data Stream melalui simulasi sistem Undo/Redo. Sistem ini dirancang untuk menyimpan serangkaian state (snapshot) di dalam stack dan memungkinkan pengguna bergerak maju-mundur dalam riwayat perubahan data. Dengan pendekatan ini, kami memperoleh pemahaman mendalam tentang bagaimana struktur stack dapat digunakan untuk mengelola state history dalam aplikasi interaktif.

### 1.2 Dasar Teori
**Stack (LIFO - Last In First Out):**
Stack adalah struktur data di mana elemen terakhir yang ditambahkan adalah yang pertama diambil. Operasi utama meliputi:
- **push:** Menambahkan elemen ke atas stack.
- **pop:** Menghapus dan mengembalikan elemen teratas.
- **peek:** Melihat elemen teratas tanpa menghapus.
Kompleksitas waktu semua operasi adalah O(1).

**Data Stream:**
Data stream adalah aliran data yang masuk secara berurutan dari waktu ke waktu. Dalam konteks Progress 3, stream mewakili serangkaian operasi edit (append, remove) yang diterapkan pada kumpulan data dinamis.

**Undo/Redo Pattern (Two-Stack Approach):**
Sistem undo/redo menggunakan dua buah stack:
- **Undo Stack:** Menyimpan state sebelum setiap operasi. Ketika user menjalankan undo, state saat ini di-push ke redo stack, dan state lama di-pop dari undo stack.
- **Redo Stack:** Menyimpan state yang telah di-undo. Ketika user menjalankan redo, redo stack di-pop kembali ke undo stack.
- **Clear on New Operation:** Jika user melakukan operasi baru setelah undo (redo tidak dilakukan), redo stack dihapus karena branch baru dimulai.

### 1.3 Scope Progress 3
1. **Stack ADT:** Implementasi stack berbasis offset-based pointer di dalam arena.
2. **Snapshot Structure:** Mekanisme penyimpanan state (snapshot dari data array).
3. **Session Manager:** Pengelola sesi dengan logika undo/redo terintegrasi.
4. **Data Stream Simulator:** Loop pemrosesan operasi (append, remove, undo, redo) dengan visualisasi state.

---

## 2. Implementasi Program

Pada tahap implementasi Progress 3, kami memperluas Arena Allocator dan struktur data yang telah dibangun di Progress 1 dan 2 dengan menambahkan Stack ADT, Snapshot mechanism, dan Session Manager untuk simulasi Undo/Redo. Seluruh komponen terintegrasi dalam satu arena 512 byte yang sama, memastikan semua operasi tetap dalam kontrol alokasi linear.

### 2.1 File arena.h dan arena.c
File ini reuse dari Progress 2 tanpa perubahan signifikan. Arena tetap berfungsi sebagai lapisan dasar untuk alokasi linear. Struktur `Arena` memuat `buffer` (pointer ke blok memori), `capacity` (ukuran arena), dan `offset` (posisi alokasi saat ini). API yang disediakan tetap sama: `arena_init`, `arena_alloc`, `arena_get`, `arena_reset`, `arena_destroy`, `arena_available`, dan `arena_dump`.

Pada Progress 3, arena digunakan untuk menyimpan tidak hanya data utama, tetapi juga node-node stack, snapshot state, dan metadata struktur data lainnya.

### 2.2 File stack.h dan stack.c
Stack pada Progress 3 merupakan ADT baru yang diimplementasikan menggunakan offset-based pointer, mirip dengan Linked List pada Progress 2. Pada file `stack.h`, struktur `Stack` terdiri dari tiga elemen:
- `head`: Offset node teratas stack (atau `STACK_NULL` jika kosong).
- `count`: Jumlah elemen di stack.
- `elem_size`: Ukuran elemen yang disimpan.

Setiap node di dalam stack mempunyai layout identik dengan node Linked List: `[size_t next_offset][data]`. Perbedaannya terletak pada operasi: stack hanya menambah dan menghapus dari head, tidak memerlukan traversal ke tail.

Implementasi pada `stack.c` menyediakan fungsi-fungsi kunci:
- `stack_push()`: Alokasi node baru, atur pointer ke head lama, update head. Kompleksitas O(1).
- `stack_pop()`: Ekstrak data dari head, pindahkan head ke node berikutnya, kurangi count. Kompleksitas O(1).
- `stack_peek()`: Baca data dari head tanpa menghapus. Kompleksitas O(1).
- `stack_is_empty()`: Periksa apakah stack kosong.
- `stack_size()`: Kembalikan jumlah elemen.

Dengan desain offset-based pointer, stack tetap stabil selama arena belum direset, dan offset dapat diserialisasi untuk keperluan persistensi data di masa mendatang.

### 2.3 File snapshot.h dan snapshot.c
Snapshot adalah mekanisme untuk menyimpan state lengkap dari data array pada saat tertentu. Pada file `snapshot.h`, struktur `Snapshot` menyimpan:
- `data_offset`: Offset dalam arena di mana data tersimpan.
- `data_size`: Jumlah elemen dalam snapshot.
- `elem_size`: Ukuran per elemen.

Fungsi pada `snapshot.c` meliputi:
- `snapshot_create()`: Alokasi ruang di arena untuk menyimpan data, salin data, isi struktur Snapshot. Kompleksitas O(n) untuk copy data.
- `snapshot_restore()`: Ekstrak data snapshot dan kembalikan ke buffer caller.
- `snapshot_get_data()`: Dapatkan pointer langsung ke data snapshot di arena.

Snapshot dirancang untuk disimpan di dalam stack (undo/redo). Setiap snapshot adalah "frozen copy" dari state pada momen operasi terjadi, memungkinkan pengguna untuk kembali ke state tersebut kapan saja.

### 2.4 File session_manager.h dan session_manager.c
Session Manager adalah orchestra utama yang mengelola sesi editing dengan undo/redo logic. Pada file `session_manager.h`, struktur `SessionManager` terdiri dari:
- `undo_stack`: Stack untuk menyimpan snapshot state sebelum operasi.
- `redo_stack`: Stack untuk menyimpan snapshot state yang telah di-undo.
- `current_state`: Pointer ke array data aktif saat ini.
- `current_size`: Jumlah elemen aktif.
- `max_capacity`: Kapasitas maksimal array.
- `elem_size`: Ukuran per elemen.

Fungsi-fungsi utama pada `session_manager.c`:
- `session_init()`: Inisialisasi session dengan alokasi current_state di arena, setup undo/redo stacks, simpan initial empty state ke undo stack.
- `session_append()`: Save state saat ini ke undo stack, tambah elemen, clear redo stack.
- `session_remove()`: Save state saat ini ke undo stack, hapus elemen di index, clear redo stack.
- `session_undo()`: Save current state ke redo stack, pop dari undo stack, restore state lama ke current_state.
- `session_redo()`: Save current state ke undo stack, pop dari redo stack, restore state baru ke current_state.
- `session_clear_redo()`: Reset redo stack (dipanggil setelah operasi baru).

Alur logic sesudah setiap operasi append/remove adalah:
1. Simpan state lama ke undo stack.
2. Lakukan modifikasi pada current state.
3. Clear redo stack (karena branch history dimulai).

Dengan pendekatan ini, setiap operasi dilindungi oleh state history yang konsisten, dan user dapat bergerak bebas melalui timeline operasi mereka.

### 2.5 File main.c
File `main.c` berfungsi sebagai demonstrator sistem Undo/Redo dengan data stream simulasi. Program mengeksekusi skenario lengkap:

1. Inisialisasi arena 512 byte dan session manager untuk integer stack dengan kapasitas 20 elemen.
2. Simulasi operasi append: 10, 20, 30, 40.
3. Jalankan dua kali undo untuk kembali ke state [10, 20].
4. Jalankan redo satu kali untuk maju ke [10, 20, 30].
5. Jalankan operasi append baru: 99, menghasilkan state [10, 20, 30, 99] dan clear redo stack.
6. Jalankan operasi remove di index 1 (hapus 20), menghasilkan [10, 30, 99].
7. Jalankan undo untuk kembali ke [10, 20, 30, 99].

Setiap langkah dilengkapi dengan output untuk menunjukkan state saat ini, ukuran undo/redo stack, dan penggunaan arena.

### 2.6 Alur Uji Program
Skenario uji didesain untuk memvalidasi seluruh aspek sistem undo/redo:
1. **Inisialisasi:** Setup arena 512 byte, session dengan kapasitas 20 integer.
2. **Forward Phase:** Append 10→20→30→40 secara berurutan, amati undo stack berkembang.
3. **Backward Phase:** Undo 2 kali (kembali ke [10, 20]), amati state dan redo stack terbentuk.
4. **Forward with Redo:** Redo 1 kali (maju ke [10, 20, 30]), amati undo/redo stacks berubah.
5. **Branch Creation:** Append baru (99), amati redo stack diclear dan branch baru dimulai.
6. **Remove Operation:** Hapus elemen di index 1, state menjadi [10, 30, 99].
7. **Undo Remove:** Undo operasi remove, kembali ke [10, 20, 30, 99].
8. **Final State:** Lihat kondisi akhir arena (356/512 bytes terpakai).

---

## 3. Output Program

### 3.1 Inisialisasi Arena dan Session
```
=== Progress 3: Stack + Data Stream (Simulator Undo/Redo) ===

1. Arena diinisialisasi (512 bytes)

=== Dump Memori Arena ===
Kapasitas: 512 byte, Terpakai: 0 byte, Tersedia: 512 byte
Breakdown Offset (lebar baris: 64):
[0000] ................................................................
[0064] ................................................................
[0128] ................................................................
[0192] ................................................................
[0256] ................................................................
[0320] ................................................................
[0384] ................................................................
[0448] ................................................................
=========================

2. Sesi diinisialisasi (tumpukan integer, kapasitas max 20 elemen)
=== Informasi Sesi ===
Ukuran stack undo: 1
Ukuran stack redo: 0
State saat ini [0 elemen]: [ ]
====================
```

**Interpretasi:** Arena dimulai kosong 512 byte. Session manager sudah menyimpan satu snapshot awal (empty state) di undo stack.

### 3.2 Forward Phase: Append 10, 20, 30, 40
```
3. Simulasi aliran data operasi:

  [Op1] TAMBAH 10
State saat ini [1 elemen]: [ 10 ]

  [Op2] TAMBAH 20
State saat ini [2 elemen]: [ 10, 20 ]

  [Op3] TAMBAH 30
State saat ini [3 elemen]: [ 10, 20, 30 ]

  [Op4] TAMBAH 40
State saat ini [4 elemen]: [ 10, 20, 30, 40 ]

  Informasi sesi setelah 4 operasi tambah:
=== Informasi Sesi ===
Ukuran stack undo: 5
Ukuran stack redo: 0
State saat ini [4 elemen]: [ 10, 20, 30, 40 ]
====================
```

**Interpretasi:** Setiap append menyimpan state lama ke undo stack. Setelah 4 append, undo stack berisi 5 snapshot (empty, [10], [10,20], [10,20,30], [10,20,30,40]). Redo stack kosong.

### 3.3 Backward Phase: Undo 2 Kali
```
4. Pengujian operasi UNDO:

  [Undo1]
State saat ini [3 elemen]: [ 10, 20, 30 ]

  [Undo2]
State saat ini [2 elemen]: [ 10, 20 ]

  Informasi sesi setelah 2 undo:
=== Informasi Sesi ===
Ukuran stack undo: 3
Ukuran stack redo: 2
State saat ini [2 elemen]: [ 10, 20 ]
====================
```

**Interpretasi:** Setiap undo memindahkan current state ke redo stack dan merestorasi state lama dari undo stack. Undo stack berkurang dari 5 menjadi 3, redo stack bertambah menjadi 2 (berisi [10,20,30] dan [10,20,30,40]).

### 3.4 Forward with Redo: Redo 1 Kali
```
5. Pengujian operasi REDO:

  [Redo1]
State saat ini [3 elemen]: [ 10, 20, 30 ]

  Informasi sesi setelah 1 redo:
=== Informasi Sesi ===
Ukuran stack undo: 4
Ukuran stack redo: 1
State saat ini [3 elemen]: [ 10, 20, 30 ]
====================
```

**Interpretasi:** Redo memindahkan state dari redo stack kembali ke undo stack. Sekarang current state adalah [10, 20, 30], undo stack berisi 4 snapshot, redo stack berisi 1 snapshot ([10,20,30,40]).

### 3.5 Branch Creation: New Append 99
```
6. Operasi baru setelah redo (TAMBAH 99):

  [Op5] TAMBAH 99 (stack redo akan dihapus)
State saat ini [4 elemen]: [ 10, 20, 30, 99 ]

  Informasi sesi setelah append baru:
=== Informasi Sesi ===
Ukuran stack undo: 5
Ukuran stack redo: 0
State saat ini [4 elemen]: [ 10, 20, 30, 99 ]
====================
```

**Interpretasi:** Append baru menyimpan state lama, lalu clear redo stack. State menjadi [10, 20, 30, 99] bukannya kembali ke [10, 20, 30, 40], menunjukkan branch history baru dimulai. User tidak bisa lagi redo ke [10, 20, 30, 40] dari sini.

### 3.6 Remove Operation: Hapus Index 1
```
7. Pengujian operasi HAPUS:

  [Op6] HAPUS indeks 1
State saat ini [3 elemen]: [ 10, 30, 99 ]

  Informasi sesi setelah penghapusan:
=== Informasi Sesi ===
Ukuran stack undo: 6
Ukuran stack redo: 0
State saat ini [3 elemen]: [ 10, 30, 99 ]
====================
```

**Interpretasi:** Remove di index 1 menghapus elemen 20. Undo stack berkembang ke 6 snapshot. State sekarang [10, 30, 99].

### 3.7 Undo Remove
```
8. Undo operasi penghapusan:

  [Undo3]
State saat ini [4 elemen]: [ 10, 20, 30, 99 ]

  Informasi sesi:
=== Informasi Sesi ===
Ukuran stack undo: 5
Ukuran stack redo: 1
State saat ini [4 elemen]: [ 10, 20, 30, 99 ]
====================
```

**Interpretasi:** Undo operasi remove memulihkan elemen 20. State kembali ke [10, 20, 30, 99]. Redo stack berisi [10, 30, 99].

### 3.8 Final Arena State
```
9. Kondisi akhir arena:

=== Dump Memori Arena ===
Kapasitas: 512 byte, Terpakai: 356 byte, Tersedia: 156 byte
Breakdown Offset (lebar baris: 64):
[0000] ################################################################
[0064] ################################################################
[0128] ################################################################
[0192] ################################################################
[0256] ################################################################
[0320] ####################################............................
[0384] ................................................................
[0448] ................................................................
=========================

=== Demonstrasi Progress 3 Selesai ===
```

**Interpretasi:** Setelah seluruh operasi, arena menggunakan 356 byte dari 512 byte (69.5% utilization). Memori yang terpakai meliputi:
- Current state array: ~100 byte (20 elements × 4 bytes per int + padding).
- Undo stack snapshots: ~150 byte (6 snapshot, masing-masing data + snapshot metadata).
- Redo stack snapshots: ~50 byte (1 snapshot).
- Overhead (stack node pointers): ~50 byte.

---

## 4. Pembahasan dan Kesimpulan

### 4.1 Keuntungan Stack + Data Stream untuk Undo/Redo

1. **Simplicity:** Two-stack approach sangat intuitif dan mudah diimplementasikan. Setiap operasi cukup save state dan push ke stack.

2. **O(1) Operasi:** Push, pop, peek semua O(1) tanpa traversal, membuat undo/redo response time instant.

3. **Clear History Semantics:** Clear redo stack saat operasi baru sangat jelas secara semantik—user memulai branch baru.

4. **Deterministic Memory:** Dengan arena allocator, penggunaan memori dapat diprediksi dan divisualisasikan dengan mudah.

### 4.2 Keterbatasan dan Improvement

**Current Limitations:**
1. **Linear Storage:** Setiap operasi snapshot copy seluruh data, menyebabkan space overhead O(n) per snapshot. Untuk data besar, ini tidak efisien.
   - **Improvement:** Delta snapshots—hanya simpan perubahan (diff) dari state sebelumnya.

2. **No Compression:** Snapshot identik di history tidak di-deduplicate. Jika user melakukan append-undo berkali-kali, history menjadi bloat.
   - **Improvement:** Snapshot interning atau compression.

3. **Fixed Capacity:** Arena 512 byte mungkin kehabisan jika history terlalu panjang atau data terlalu besar.
   - **Improvement:** Dynamic arena atau batching old snapshots.

4. **One-Level Undo:** Hanya bisa undo/redo operasi individual, tidak support group undo (e.g., "undo last 3 operations").
   - **Improvement:** Transaction-based grouping.

### 4.3 Perbandingan dengan Approach Lain

| Approach | Undo/Redo | Memory | Complexity | Use Case |
|----------|-----------|--------|-----------|----------|
| **Two-Stack Snapshot** | Full state history | O(n × m) (n ops, m state size) | Simple O(1) ops | General editor, small data |
| **Delta-Based** | Incremental changes | O(n × k) (k = delta size) | Complex diff logic | Large document, high undo depth |
| **Command Pattern** | Reversible commands | O(n) (command list) | Requires undo method per command | Complex multi-component state |
| **Persistent Data Structures** | Structural sharing | O(n log n) per op | Complex tree balancing | Functional programming, immutability |

Progress 3 memilih **Two-Stack Snapshot** karena cocok untuk pembelajaran struktur data dasar dan memberikan hasil visual yang jelas. Untuk aplikasi production dengan data besar, approach lain lebih sesuai.

### 4.4 Persiapan untuk Progress 4

Progress 4 akan mengeksplorasi struktur data yang lebih kompleks (Tree, Hash Table) dan mengintegrasikan data stream simulation dengan batch processing atau real-time analytics. Dari Progress 3, kami sudah memiliki fondasi:

1. **Arena Allocator stabil** dengan 3 struktur data (Array, Linked List, Stack).
2. **Snapshot mechanism** yang dapat diperluas untuk state management kompleks.
3. **Session Manager pattern** yang dapat diadopsi untuk manajemen state umum.

Untuk Progress 4, kami dapat:
- Implement Tree untuk representasi hirarki (file system simulator dengan undo/redo).
- Implement Hash Table untuk real-time analytics (word frequency stream dengan snapshot history).
- Combine multiple structures dalam satu session (e.g., tree + hash table collaboration).

### 4.5 Kesimpulan

Progress 3 berhasil mengimplementasikan Stack ADT dan mengintegrasikannya dengan konsep Data Stream melalui simulasi Undo/Redo. Sistem yang dibangun menunjukkan:

1. **Stack sebagai foundational structure** untuk temporal state management.
2. **Offset-based pointer consistency** di seluruh struktur data (Array, LinkedList, Stack).
3. **Arena allocator robustness** dalam mengelola multiple structures sekaligus.
4. **Clear output visualization** melalui state tracking dan memory layout dump.

Pendekatan two-stack snapshot efektif untuk use case umum dan memberikan pemahaman mendalam tentang state management dalam aplikasi interaktif. Dengan fondasi ini, Progress 4 siap untuk eksplorasi struktur data advanced dengan domain-specific state management yang lebih sophisticated.
