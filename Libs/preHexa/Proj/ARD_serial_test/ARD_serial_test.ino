// void setup() {
//     Serial.begin(115200);
//     while (!Serial) ;
//     Serial.println("Serial ok\n\r");
    
//     Serial1.begin(9600);
//     // while (!Serial1) ;
//     Serial1.println("Serial1 ok\n\r");
// }

// void loop() {
//     delay(1000);
// }

void setup() {
  Serial.begin(115200);     // Инициализация основного порта Serial
  Serial1.begin(9600);    // Инициализация порта Serial1
}

void loop() {
  if (Serial1.available()) {   // Проверяем, есть ли доступные данные на Serial1
    char data = Serial1.read(); // Читаем данные с Serial1
    Serial.print(data);        // Выводим данные на основной порт Serial
    Serial1.print(data);        // Выводим данные на основной порт Serial
  }
  
  if (Serial.available()) {    // Проверяем, есть ли доступные данные на основном порту Serial
    char data = Serial.read();  // Читаем данные с основного порта Serial
    Serial1.print(data);        // Отправляем данные на порт Serial1
    Serial.print(data);        // Выводим данные на основной порт Serial
  }
}