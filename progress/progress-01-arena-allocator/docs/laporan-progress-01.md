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

### 2.2 File arena.h
File header ini berisi deklarasi struktur data dan fungsi-fungsi utama yang digunakan oleh sistem arena. Struktur `Arena` didefinisikan dengan tiga komponen inti, yaitu `buffer` bertipe `unsigned char *` sebagai lokasi penyimpanan data mentah, `capacity` bertipe `size_t` sebagai ukuran total arena, dan `offset` bertipe `size_t` sebagai penanda posisi alokasi saat ini.

Pada header juga dideklarasikan fungsi-fungsi utama API arena, yaitu `arena_init`, `arena_alloc`, `arena_get`, `arena_reset`, `arena_destroy`, `arena_available`, dan `arena_dump`. Deklarasi ini bertujuan memisahkan antarmuka (interface) dari implementasi sehingga kode lebih terstruktur dan mudah dikembangkan.

### 2.3 File arena.c
File ini berisi implementasi seluruh fungsi yang telah dideklarasikan pada header. Setiap fungsi memiliki peran spesifik dalam mekanisme pengelolaan memori arena.

Fungsi `arena_init` bertugas melakukan inisialisasi arena dengan mengalokasikan satu blok memori dari sistem operasi, kemudian mengatur nilai `capacity` dan `offset` awal. Apabila input tidak valid atau alokasi memori gagal, fungsi mengembalikan status gagal agar program utama dapat menangani kondisi tersebut.

Fungsi `arena_alloc` merupakan inti mekanisme bump allocation. Fungsi ini memeriksa apakah `offset + size` masih berada dalam batas `capacity`. Jika tidak mencukupi, fungsi mengembalikan status gagal. Jika mencukupi, nilai offset saat ini disimpan sebagai hasil alokasi, lalu offset digeser maju sebesar ukuran yang diminta.

Fungsi `arena_get` digunakan untuk menerjemahkan nilai offset menjadi alamat memori aktual. Validasi dilakukan agar offset tidak melebihi batas arena. Dengan demikian, proses pemesanan ruang (`arena_alloc`) dipisahkan dari proses pengambilan alamat data (`arena_get`) sehingga akses memori lebih aman dan terkontrol.

Fungsi `arena_reset` mengatur ulang `offset` ke nilai nol sehingga seluruh area arena dianggap kosong kembali secara logis. Proses ini tidak menghapus isi byte satu per satu, tetapi menandai bahwa alokasi baru akan dimulai dari awal.

Fungsi `arena_destroy` bertugas membebaskan blok memori arena pada akhir siklus program. Setelah pembebasan, pointer diatur ke `NULL` dan metadata arena dikembalikan ke nilai aman untuk mencegah dangling pointer.

Fungsi `arena_available` menghitung sisa kapasitas yang belum terpakai dengan rumus `capacity - offset`. Informasi ini berguna untuk pemantauan kondisi memori saat debugging maupun pengujian.

Fungsi `arena_dump` menampilkan kondisi arena secara visual dalam bentuk grid ASCII. Byte yang telah terpakai ditandai dengan `[#]` dan byte kosong ditandai dengan `[.]`. Parameter lebar tampilan digunakan agar visualisasi lebih mudah dibaca.

### 2.4 File main.c
File `main.c` berfungsi sebagai program pengujian implementasi arena allocator. Program dimulai dengan deklarasi variabel arena, dilanjutkan inisialisasi kapasitas 32 byte, dan penampilan kondisi awal arena melalui `arena_dump`.

Setelah itu program melakukan alokasi bertahap untuk menguji skenario normal dan skenario gagal. Alokasi blok A dan blok B digunakan untuk memverifikasi bahwa offset bergerak sesuai ukuran alokasi. Data kemudian ditulis ke area memori hasil alokasi melalui pointer yang diperoleh dari `arena_get`.

Program juga menguji kasus batas dengan mencoba mengalokasikan blok C yang melebihi sisa kapasitas. Kondisi ini harus menghasilkan status gagal, sehingga dapat dibuktikan bahwa validasi batas memori berjalan benar. Pada tahap akhir, `arena_reset` dipanggil untuk memastikan arena kembali ke kondisi awal secara logis.

### 2.5 Alur Uji Program
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
Berdasarkan hasil eksekusi program, implementasi Arena Allocator pada Progress 1 telah berjalan sesuai rancangan awal. Kondisi arena pada tahap awal berada pada keadaan kosong, kemudian berubah menjadi terisi setelah proses alokasi blok A dan blok B. Hal ini menunjukkan bahwa mekanisme alokasi linear berbasis offset berhasil diterapkan secara konsisten.

Ketika dilakukan percobaan alokasi blok C dengan ukuran yang melebihi sisa kapasitas, sistem menolak alokasi tersebut. Perilaku ini menegaskan bahwa validasi kapasitas pada fungsi `arena_alloc` sudah bekerja dengan benar dan mampu mencegah akses memori di luar batas arena. Dari sisi keandalan, mekanisme ini penting untuk menjaga stabilitas program.

Selain itu, fungsi `arena_reset` berhasil mengembalikan offset ke posisi awal tanpa perlu membebaskan setiap blok secara individual. Pendekatan ini menjadi keunggulan utama Arena Allocator karena proses pembersihan memori logis dapat dilakukan cepat dan sederhana.

Namun, hasil pembahasan juga menunjukkan keterbatasan pada tahap ini. Arena belum mendukung deallocation per-blok dan belum memiliki strategi reuse sebagian area memori. Artinya, model ini efektif untuk pola alokasi sementara, tetapi belum cukup untuk kasus yang membutuhkan pelepasan memori granular.

### 4.2 Kesimpulan
Progress 1 berhasil membangun fondasi Arena Allocator sebagai ADT manajemen memori linear dalam bahasa C. Fungsi inti seperti inisialisasi arena, alokasi berbasis offset, pengambilan data, reset arena, dan pelepasan memori telah terimplementasi dan tervalidasi melalui skenario uji.

Secara umum, target tahap awal telah tercapai: arena dapat digunakan sebagai blok memori terpusat dengan proses alokasi yang ringan dan terkontrol. Dengan fondasi ini, pengembangan dapat dilanjutkan ke Progress 2, yaitu integrasi struktur data Stack berbasis arena sebagai langkah lanjutan dari sistem yang sedang dibangun.
