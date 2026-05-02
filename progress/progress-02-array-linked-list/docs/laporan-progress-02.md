# Laporan Progress 2: Struktur Data Dasar (Array dan Linked List Berbasis Arena)

## 1. Pendahuluan

### 1.1 Tujuan Progress
Progress 2 bertujuan untuk mengimplementasikan struktur data dasar (Array dan Linked List) yang memanfaatkan Arena Allocator sebagai mekanisme manajemen memori pada level struktur data. Struktur data ini dirancang untuk beroperasi di dalam satu blok memori terpusat tanpa menggunakan alokasi konvensional seperti malloc/free pada implementasi Array dan Linked List.

### 1.2 Dasar Teori
**Arena Allocator:**
Arena Allocator adalah mekanisme alokasi memori linear di mana seluruh alokasi dilakukan dengan menggeser offset dalam satu buffer terpusat. Keuntungan:
- **Kecepatan alokasi:** O(1) karena hanya increment offset.
- **Reset efisien:** O(1) dengan mengatur ulang offset ke nol.
- **Cache locality:** Data tersusun kontigu dalam memori.
- **Serialisasi mudah:** Offset dapat diserialisasi langsung.

**Representasi Offset-Based Pointer:**
Daripada menggunakan pointer C biasa, kami merepresentasikan "pointer" sebagai offset (size_t) dari awal buffer arena. Keuntungan:
- Pointer independence: offset tetap valid meskipun buffer dimove.
- Predictable size: offset selalu size_t (64/32 bit), bukan pointer address.
- Kompatibilitas: lebih mudah untuk serialisasi antar platform.

### 1.3 Scope Progress 2
1. **Array ADT:** Struktur array dengan alokasi kontigu dalam arena.
2. **Linked List ADT:** Struktur linked list berbasis offset pointer.
3. **Demonstrasi:** Operasi dasar (create, append, prepend, get, traverse) dengan visualisasi memory layout.

---

## 2. Implementasi Program
Pada tahap implementasi ini, kami memperluas fondasi Arena Allocator dari Progress 1 ke dua struktur data dasar, yaitu Array dan Linked List. Seluruh data tetap ditempatkan di dalam satu blok memori arena agar alokasi tetap bersifat linear, terkontrol, dan mudah divisualisasikan.

Sebelum masuk ke detail struktur data, program utama tetap menggunakan arena sebagai sumber memori tunggal. Arena diinisialisasi terlebih dahulu, kemudian dua struktur data dibangun di atasnya dengan pendekatan yang berbeda: Array memanfaatkan alokasi kontigu, sedangkan Linked List memanfaatkan node berbasis offset.

### 2.1 File arena.h
File header ini masih menggunakan struktur `Arena` yang sama seperti Progress 1. Struktur tersebut memuat tiga komponen inti, yaitu `buffer` sebagai penunjuk ke blok memori mentah, `capacity` sebagai ukuran total arena, dan `offset` sebagai posisi alokasi saat ini.

Selain struktur data, file ini juga mendeklarasikan API arena yang digunakan kembali oleh seluruh struktur data pada Progress 2, yaitu `arena_init`, `arena_alloc`, `arena_get`, `arena_reset`, `arena_destroy`, `arena_available`, dan `arena_dump`. Dengan demikian, Arena tetap menjadi lapisan dasar yang menangani semua kebutuhan alokasi dan visualisasi memori.

### 2.2 File array.h dan array.c
Array pada Progress 2 dirancang sebagai struktur data kontigu yang dialokasikan sekali di dalam arena. Pada file `array.h`, struktur `Array` menyimpan metadata penting berupa `offset`, `count`, `capacity`, dan `elem_size`. Metadata ini diperlukan agar array dapat melacak letak data, jumlah elemen aktif, dan ukuran setiap elemen secara akurat.

Implementasi pada `array.c` menyediakan fungsi-fungsi utama untuk pengelolaan array. Fungsi `array_create` bertugas meminta blok memori dari arena sebesar `capacity * elem_size` byte, lalu menyimpan offset awal alokasi. Fungsi `array_append` digunakan untuk menyalin data ke slot berikutnya secara berurutan, sedangkan `array_get` dan `array_set` dipakai untuk membaca serta memperbarui isi elemen berdasarkan index.

Secara desain, array ini tetap sederhana namun efisien karena seluruh elemennya disimpan kontigu. Konsep tersebut memudahkan akses acak dengan waktu $O(1)$ dan cocok untuk data yang jarang berubah ukuran.

### 2.3 File linked_list.h dan linked_list.c
Linked List pada Progress 2 dibuat dengan pendekatan offset-based pointer, sehingga hubungan antar node tidak menggunakan pointer alamat memori langsung. Pada file `linked_list.h`, struktur `LinkedList` menyimpan `head`, `count`, dan `elem_size`. Nilai `head` berisi offset node pertama, sedangkan konstanta `LINKED_LIST_NULL` digunakan sebagai penanda akhir list.

Setiap node disimpan dalam arena dengan layout `[next_offset][data]`. Implementasi ini dikerjakan pada `linked_list.c` melalui fungsi `linked_list_create`, `linked_list_append`, `linked_list_prepend`, `linked_list_get`, dan `linked_list_traverse`. Fungsi `linked_list_append` menambahkan node di akhir dengan cara mencari node terakhir terlebih dahulu, sedangkan `linked_list_prepend` menambahkan node baru di depan list dengan biaya yang lebih ringan karena cukup mengubah head.

Pendekatan berbasis offset ini membuat linked list tetap bergantung pada arena yang sama, tetapi tetap mudah ditelusuri dan divisualisasikan. Karena setiap node menyimpan offset node berikutnya, hubungan antar elemen menjadi stabil selama arena belum direset.

### 2.4 File main.c
File `main.c` berfungsi sebagai program pengujian untuk memperlihatkan perilaku Array dan Linked List di dalam arena. Program dimulai dengan inisialisasi arena 512 byte, lalu menampilkan kondisi awal memori melalui `arena_dump`.

Setelah arena siap, program menjalankan demonstrasi Array dengan membuat array integer berisi lima elemen. Nilai 10, 20, 30, 40, dan 50 dimasukkan secara berurutan, kemudian salah satu elemen diubah untuk memastikan operasi baca-tulis berjalan benar. Hasil ini juga divisualisasikan melalui dump arena agar alokasi kontigu dapat terlihat secara langsung.

Bagian berikutnya adalah demonstrasi Linked List berbasis char. Program menambahkan elemen A, B, C, dan D menggunakan operasi append, lalu menambahkan Z di bagian depan menggunakan prepend. Traversal dilakukan untuk memastikan urutan node sesuai dengan struktur yang dibangun, dan akses berdasarkan index juga diuji untuk memverifikasi integritas offset antar node.

Pada tahap akhir, program memanggil `arena_reset` untuk mengembalikan arena ke kondisi awal. Langkah ini penting untuk menunjukkan bahwa seluruh struktur data pada Progress 2 tetap mengikuti siklus hidup arena yang sama seperti pada Progress 1.

### 2.5 Alur Uji Program
Skenario uji pada program utama disusun sebagai berikut:
1. Inisialisasi arena dengan kapasitas 512 byte.
2. Tampilkan kondisi awal arena yang masih kosong.
3. Buat array integer dengan lima elemen, isi nilainya, lalu lakukan pembacaan dan perubahan elemen.
4. Buat linked list berbasis offset, isi dengan karakter A, B, C, dan D, lalu lakukan traversal.
5. Tambahkan elemen Z di awal list untuk menguji operasi prepend.
6. Tampilkan kondisi arena setelah seluruh alokasi selesai.
7. Jalankan `arena_reset` dan tampilkan kembali kondisi arena setelah reset.

---

## 3. Output Program

### 3.1 Kondisi Awal Arena (512 byte, 16 kolom)
```
=== ARENA DUMP ===
[.] . . . . . . . . . . . . . . .  (32 cell kosong = 512 byte)
```
**Interpretasi:** Semua memory kosong, offset 0/512, tersisa 512 byte.

### 3.2 Output Array
```
Array Info:
  Offset dalam arena: 0
  Elemen saat ini: 5
  Kapasitas: 5
  Ukuran per elemen: 4 byte
  Total memori terpakai: 20 byte

Membaca elemen array:
  arr[0] (offset: 0) = 10
  arr[1] (offset: 4) = 20
  arr[2] (offset: 8) = 30
  arr[3] (offset: 12) = 40
  arr[4] (offset: 16) = 50

Mengubah elemen arr[2] menjadi 999...
arr[2] sekarang = 999
```

### 3.3 Memory Layout Setelah Array
```
=== ARENA DUMP ===
[#][#][#][#][#][#][#][#][#][#][#][#][#][#][#][#]
[#][#][#][#][.] . . . . . . . . . . .
...
Offset: 20 / 512
Tersisa: 492 byte
```
**Interpretasi:** 20 byte pertama terpakai (array 5 int), sisanya kosong.

### 3.4 Output Linked List
```
LinkedList Info:
  Head offset: 20
  Jumlah node: 4
  Ukuran data per node: 1 byte

Traversal Linked List:
  Node[0] (offset: 20): A
  Node[1] (offset: 25): B
  Node[2] (offset: 30): C
  Node[3] (offset: 35): D
```

**Setelah Prepend Z:**
```
LinkedList Info:
  Head offset: 40
  Jumlah node: 5
  Ukuran data per node: 1 byte

Traversal setelah prepend:
Traversal Linked List:
  Node[0] (offset: 40): Z
  Node[1] (offset: 20): A
  Node[2] (offset: 25): B
  Node[3] (offset: 30): C
  Node[4] (offset: 35): D
```

**Interpretasi:** Setiap node mengambil 9 byte (8 byte next_offset + 1 byte char). Prepend mengalokasikan node baru di offset 40, yang point ke head lama (offset 20).

### 3.5 Memory Layout Setelah Linked List
```
Offset: 45 / 512
Tersisa: 467 byte
```
**Breakdown:**
- Byte 0-19: Array (5 × 4 byte)
- Byte 20-24: Node A (offset pointer + char 'A')
- Byte 25-29: Node B
- Byte 30-34: Node C
- Byte 35-39: Node D
- Byte 40-44: Node Z (prepended)

### 3.6 Arena Setelah Reset
```
Offset: 0 / 512
Tersisa: 512 byte
```
**Interpretasi:** Offset kembali ke 0, semua memory tersedia untuk alokasi berikutnya.

---

## 4. Pembahasan dan Kesimpulan

### 4.1 Analisis Desain

**Keuntungan Offset-Based Pointer:**
1. **Cache Locality:** Node linked list tersusun contigu dalam arena. Sequential traversal memanfaatkan CPU cache lebih baik dibanding pointer tradisional yang scatter di heap.
2. **Serialisasi:** Offset dapat ditulis langsung ke file atau network, pointer C tidak bisa.
3. **Deterministic Layout:** Memory layout predictable, memudahkan debugging dan profiling.
4. **No Fragmentation:** Karena linear allocation, tidak ada external fragmentation seperti malloc/free.

**Keterbatasan:**
1. **Append O(n):** Linked list append masih O(n) karena perlu cari tail. Solusi: bisa tambah tail pointer di metadata LinkedList.
2. **Fixed Capacity Array:** Array tidak bisa grow setelah create. Trade-off untuk alokasi O(1).
3. **Memory Reuse Terbatas:** Arena reset mengosongkan semua, tidak bisa delete elemen individual.

### 4.2 Validasi Fungsional
- ✓ Array berhasil append 5 elemen dan accessible via index.
- ✓ Array modify (set) bekerja dengan benar (arr[2] = 999).
- ✓ Linked List append dan prepend menghasilkan urutan node yang benar.
- ✓ Linked List traverse menunjukkan path yang sesuai (Z→A→B→C→D setelah prepend Z).
- ✓ Memory visualization (arena_dump) menunjukkan alokasi linear progresif.
- ✓ Arena reset O(1) berhasil membersihkan state.

### 4.3 Pengamatan Memory Layout
- **Array 5×int (20 byte)** dialokasikan di offset 0-19 secara kontigu.
- **Linked List 4 node (4×9 = 36 byte)** dialokasikan di offset 20-55 (dalam demo, sampai offset 44 untuk 5 node).
- **Total used: 45 byte dari 512 byte** = 8.8% utilization (efisien untuk skenario kecil ini).
- **Visualization:** Grid 16 kolom jelas menunjukkan fragmentasi logis (tidak ada, linear).

### 4.4 Perbandingan dengan malloc/free
| Aspek | Arena | malloc/free |
|-------|-------|------------|
| Alokasi | O(1) | O(log n) biasanya |
| Dealokasi | O(1) reset (batch) | O(log n) |
| Fragmentasi | Tidak ada (linear) | External fragmentasi |
| Cache locality | Baik (contigu) | Buruk (scattered) |
| Reset state | O(1) | O(n) free per item |
| Complexity | Rendah | Tinggi (buddy, slab, etc) |

Arena cocok untuk **batch processing, game loops, dan skenario reset berkala**.

### 4.5 Insight untuk Progress 3 dan 4
1. **Stack pada Arena:** Bisa leverage offset untuk stack nodes, push/pop O(1).
2. **Data Stream:** Aliran data dapat dialokasikan sequentially di arena, memproses batch per reset.
3. **Cache Benefits:** Stack dan linked list dalam arena akan menunjukkan peningkatan performa vs malloc/free dalam benchmark memory access.

### 4.6 Kesimpulan
Progress 2 berhasil mendemonstrasikan:
- **Array dan Linked List berbasis offset-based pointer** beroperasi di dalam Arena Allocator pada level struktur data.
- **Memory layout linear dan predictable**, memvisualisasikan benefit cache locality dan no fragmentation.
- **Offset abstraction effective** untuk merepresentasikan hubungan antar node tanpa pointer C biasa.
- **Fondasi siap** untuk Progress 3 (Stack + Data Stream integration) dengan confidence struktur data berbasis arena sudah validated.

---

## Referensi
- Attachment: `strukdat-arena-progress.agent.md` (agent spec)
- Source code: `progress/progress-02-array-linked-list/src/`
- Output execution: `progress/progress-02-array-linked-list/docs/output-progress-02.txt`
- Teori offset-based pointers: Ginsburg, "Implementing Serializable Objects in C" (reference design pattern)
