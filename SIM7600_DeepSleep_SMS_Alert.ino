#include <WiFi.h>
#include <Preferences.h>

// إعدادات LilyGo T-SIM7600G
#define MODEM_PWRKEY 4
#define MODEM_TX 27
#define MODEM_RX 26
#define BOARD_LED 12
#define TOUCH_THRESHOLD 450 // بناءً على قراءاتك السابقة

WiFiServer server(80);
Preferences prefs;

void enterSleep() {
    Serial.println("Going to Sleep...");
    delay(500);
    touchSleepWakeUpEnable(T4, TOUCH_THRESHOLD);
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);
    delay(3000); 
    pinMode(BOARD_LED, OUTPUT);

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TOUCHPAD) {
        // --- وضع الإرسال SMS ---
        digitalWrite(BOARD_LED, HIGH);
        prefs.begin("config", true);
        String p = prefs.getString("phone", "");
        String m = prefs.getString("msg", "Alert!");
        prefs.end();
        if (p != "") {
            Serial1.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
            pinMode(MODEM_PWRKEY, OUTPUT);
            digitalWrite(MODEM_PWRKEY, LOW); delay(100);
            digitalWrite(MODEM_PWRKEY, HIGH); delay(1000);
            digitalWrite(MODEM_PWRKEY, LOW); delay(10000);
            Serial1.println("AT+CMGF=1"); delay(500);
            Serial1.print("AT+CMGS=\""); Serial1.print(p); Serial1.println("\"");
            delay(500); Serial1.print(m); delay(100); Serial1.write(26);
        }
        enterSleep();
    } else {
        // --- وضع الشبكة (استخدام المكتبة الخفيفة) ---
        WiFi.softAP("SIM7600_SECURE", "CHANGE_ME_STRONG_AP_PASS");
        server.begin();
        digitalWrite(BOARD_LED, HIGH);
        Serial.println("Network UP: SIM7600_SECURE");
    }
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        if (request.indexOf("GET /get") >= 0) {
            // استخراج البيانات يدوياً لتقليل الذاكرة
            int pStart = request.indexOf("p=") + 2;
            int pEnd = request.indexOf("&", pStart);
            int mStart = request.indexOf("m=") + 2;
            int mEnd = request.indexOf(" ", mStart);
            
            prefs.begin("config", false);
            prefs.putString("phone", request.substring(pStart, pEnd));
            prefs.putString("msg", request.substring(mStart, mEnd));
            prefs.end();
            
            client.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nSaved. Sleeping...");
            delay(2000); enterSleep();
        } else {
            client.println("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
            client.println("<h2>Setup</h2><form action='/get'>Phone:<input name='p'><br>Msg:<input name='m'><br><button>Save</button></form>");
        }
        client.stop();
    }
}