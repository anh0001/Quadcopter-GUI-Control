# Instalasi Quadcopter

## Komponen yang dibutuhkan
- Frame quadcopter F450
- Arduino Mega 2560
- ESC 30A
- Motor brushless DC 850kv
- Propeller 10x4
- 915 MHz radio frequency telemetry module
- MPU6050
- Baterai 3S 35C (2200 mAh)
- RC Receiver dengan support PPM (FlySky FS-iA6B)
- RC Transmitter (FlySky FS-i6)
- Kabel jumper

## Langkah-langkah Instalasi Quadcopter
### Petunjuk upload firmware quadcopter:
- Sambungkan Arduino Mega dengan PC menggunakan kabel USB
- Upload firmware manual pada folder `firmware\Manual\FirmwareManual.cpp` menggunakan PlatformIO atau Arduino IDE
- Jika terdapat error yang disebabkan library not found, pasanglah library `PID_v1`, `Simple_MPU6050` dan `PPMReader`

### Petunjuk koneksi telemetri dan receiver:
- Sambungkan RF telemetry module pada COM0 Arduino Mega
- Hubungkan RC receiver dengan kabel jumper pada channel 1 receiver dengan ketentuan sebagai berikut:

|RC Receiver    |Arduino Mega   |
|:-------------:|:-------------:|
|Port tengah    |VCC 5V         |
|Port atas      |Pin 19         |
|Port bawah     |Ground         |

### Petunjuk koneksi MPU6050:
- Sambungkan semua pin MPU6050 (kecuali pin AD0 dan INT) ke port yang telah disediakan di quadcopter

### Petunjuk binding RC transmitter dengan RC receiver:
- Sambungkan pin atas dengan bawah pada channel VCC RC receiver menggunakan kabel jumper
- Tekan dan tahan tombol binding di bagian kiri bawah RC transmitter
- Sambil menahan tombol binding, nyalakan RC transmitter
- Jika RC transmitter sudah memasuki mode binding, nyalakan RC receiver dengan cara menghubungkan baterai pada quadcopter
