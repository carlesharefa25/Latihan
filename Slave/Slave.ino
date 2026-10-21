#include <WiFi.h>
#include <esp_now.h>

// Pin LED pada slave
const int LED_PIN = 2;

// Callback ketika data diterima
void OnDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *data,
                int len) {

  if (len != sizeof(int)) {
    Serial.println("Ukuran data tidak sesuai!");
    return;
  }

  int command;
  memcpy(&command, data, sizeof(command));

  Serial.print("Perintah diterima: ");
  Serial.println(command);

  if (command == 1) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED MENYALA");
  }
  else if (command == 0) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED MATI");
  }
  else {
    Serial.println("Perintah tidak dikenal");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Aktifkan Wi-Fi
  WiFi.mode(WIFI_STA);

  Serial.print("MAC Address Slave: ");
  Serial.println(WiFi.macAddress());

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal menginisialisasi ESP-NOW");
    return;
  }

  // Daftarkan callback penerimaan data
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Slave siap menerima perintah");
}

void loop() {
  // Slave menunggu data dari Master
}
