#include <WiFi.h>
#include <esp_now.h>

// Ganti dengan MAC Address Slave milikmu
uint8_t slaveAddress[] = {
  0xf4, 0x2d, 0xc9, 0x89, 0xe8, 0x8c
};

int command;

// Callback status pengiriman
void OnDataSent(const wifi_tx_info_t *info,
                esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Pengiriman berhasil pada lapisan MAC");
  } else {
    Serial.println("Pengiriman gagal");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  Serial.print("MAC Address Master: ");
  Serial.println(WiFi.macAddress());

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal menginisialisasi ESP-NOW");
    return;
  }

  // Daftarkan callback pengiriman
  esp_now_register_send_cb(OnDataSent);

  // Daftarkan Slave
  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, slaveAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Gagal menambahkan Slave");
    return;
  }

  Serial.println();
  Serial.println("=== KONTROL LED SLAVE ===");
  Serial.println("Ketik 1 = LED ON");
  Serial.println("Ketik 0 = LED OFF");
  Serial.println("=========================");
}

void loop() {
  // Periksa apakah ada input dari Serial Monitor
  if (Serial.available() > 0) {
    char input = Serial.read();

    // Perintah menyalakan LED
    if (input == '1') {
      command = 1;

      esp_now_send(
        slaveAddress,
        (uint8_t *)&command,
        sizeof(command)
      );

      Serial.println("Perintah ON dikirim");
    }

    // Perintah mematikan LED
    else if (input == '0') {
      command = 0;

      esp_now_send(
        slaveAddress,
        (uint8_t *)&command,
        sizeof(command)
      );

      Serial.println("Perintah OFF dikirim");
    }
  }
}