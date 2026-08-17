# X-MRA как "eXperimental Modular Robotic Arachnid"
    http://10.0.3.12:8080
    http://10.0.3.58

/opt/hexafpv/HexaFPV_app.elf
cd /opt/hexafpv/ && motion -c cam.conf
# SSH key auth
    eval "$(ssh-agent -s)"
    ssh-add ~/.ssh/id_ed25519_gitLab.txt

# debian 12
# libcamera-hello
    https://raspberrypi.stackexchange.com/questions/135522/camera-only-working-with-deprecated-legacy-options
    https://forums.raspberrypi.com/viewtopic.php?t=119566
    https://docs.arducam.com/Raspberry-Pi-Camera/Native-camera/Libcamera-User-Guide/

# Raspberry Pi Wifi (Station + AP modes)
    https://pifi.imti.co
    https://github.com/cjimti/iotwifi

# MPU6050
    https://github.com/jarzebski/Arduino-MPU6050
    Работа C Arduino И MPU6050
        https://alexgyver.ru/arduino-mpu6050/

# Servo Driver with ESP32
    https://www.waveshare.com/wiki/Servo_Driver_with_ESP32

# Servo Driver HAT (PWM)
    sudo apt install i2c-tools

    https://www.waveshare.com/wiki/Servo_Driver_HAT
    i2cdetect -y 1
    i2c addr 0x40
    i2cdump -y 1 0x40 b
    0x3c display SSD1306_128_32
    0x68 DS RTC clock

# BlueTooth joystick
```bash
sudo apt-get update
sudo apt-get install bluez bluez-tools
sudo apt-get install bluez libbluetooth-dev
rfkill list
0: hci0: Bluetooth
        Soft blocked: yes   <== bad
        Hard blocked: no
1: phy0: Wireless LAN
        Soft blocked: no
        Hard blocked: no
sudo rfkill unblock bluetooth
```

    [A0:FA:9C:EB:79:A9] hacky DualSense Wireless Controller
    [10:18:49:62:5C:3C] hacky DS
    [AC:36:1B:5C:14:6E] black DS

    # BT config
    sudo bluetoothctl
        power on
        agent on
        default-agent
        discoverable on
        pairable on
        scan on

    sudo bluetoothctl
        power on
        scan on
        pair [MAC]
        trust [MAC]
        connect [MAC]
    scan off
    exit

# Joystick test
```bash
sudo apt install joystick
jstest /dev/input/js0

#sudo apt install evtest
#sudo evtest
```

# SDL2 dev
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev
```


    Да, Raspberry Pi может обрабатывать прерывания от GPIO (General Purpose Input/Output) в операционной системе, основанной на Debian (например, Raspberry Pi OS), с использованием языка программирования C. Обработка прерываний GPIO позволяет Raspberry Pi реагировать на изменения состояния на GPIO пинах в реальном времени, что полезно для различных приложений, таких как обработка сигналов от датчиков, кнопок и других устройств.

    Вот базовый пример того, как можно обработать прерывание GPIO на Raspberry Pi с использованием библиотеки WiringPi в языке C:

# Шаг 1: Установка WiringPi
    Для начала убедитесь, что у вас установлена библиотека WiringPi. Эта библиотека предоставляет функции для работы с GPIO на Raspberry Pi. Установить её можно следующей командой:

```bash
    sudo apt-get install wiringpi
```

# Шаг 2: Пример кода на C для обработки прерываний GPIO
```c
#include <wiringPi.h>
#include <stdio.h>

// Функция обратного вызова, вызываемая при срабатывании прерывания
void interruptHandler(void) {
    printf("Прерывание обнаружено!\n");
}

int main(void) {
    // Инициализация WiringPi
    wiringPiSetup();

    // Номер GPIO пина (например, GPIO 17 как 0 в WiringPi)
    int pin = 0;

    // Установка режима пина на вход
    pinMode(pin, INPUT);

    // Включение подтягивающего резистора
    pullUpDnControl(pin, PUD_UP);

    // Установка прерывания на падающий фронт сигнала
    wiringPiISR(pin, INT_EDGE_FALLING, &interruptHandler);

    // Бесконечный цикл, чтобы программа продолжала работать
    while(1) {
        delay(1000); // Задержка в 1 секунду
    }

    return 0;
}
```
    В этом примере кода используется библиотека WiringPi для установки прерывания на GPIO пине. Когда на пине обнаруживается изменение состояния (например, от HIGH до LOW), вызывается функция interruptHandler.

    Шаг 3: Компиляция и запуск
    Скомпилируйте программу с помощью gcc, указав -lwiringPi для линковки с библиотекой WiringPi:

    bash
    Copy code
    gcc -o gpio_interrupt gpio_interrupt.c -lwiringPi
    Затем запустите скомпилированную программу:

    bash
    Copy code
    ./gpio_interrupt
    Важные замечания
    В примере используется WiringPi номер пина. Номера пинов WiringPi могут отличаться от физических номеров пинов на Raspberry Pi. Вам нужно будет убедиться, что вы используете правильный номер пина.
    WiringPi была объявлена устаревшей, но всё ещё широко используется. В качестве альтернативы можно рассмотреть другие библиотеки, такие как pigpio или bcm2835.
    Работая с GPIO, следует соблюдать осторожность, чтобы не повредить ваш Raspberry Pi. Убедитесь, что вы правильно подключили все компоненты и соблюдаете рекомендации по безопасности.

    edelon@HexaFPV01:~/HexaFPV_proj/HexaFPV $ git remote -v
origin  gituser@KnowBase.local:/repos/HexaFPV.git (fetch)
origin  gituser@KnowBase.local:/repos/HexaFPV.git (push)

remote set-url origin http://GitLab.local/child_edu/HexaFPV.git

# edits, commits
```bash
git pull                        # from gitlab
# edits, commits
git push                        # to gitlab
git push github master --force  # mirror
```
# push to both source
```bash
git push && git push github master --force
```

```bash
sudo apt install libgpiod-dev gpiod

gpioinfo -c gpiochip0
```
"
gpiochip0 - 58 lines:
        line   0:       "ID_SDA"                input
        line   1:       "ID_SCL"                input
        line   2:       "GPIO2"                 input
        line   3:       "GPIO3"                 input
        line   4:       "GPIO4"                 input
        line   5:       "GPIO5"                 input
        line   6:       "GPIO6"                 input
        line   7:       "GPIO7"                 input
        line   8:       "GPIO8"                 input
        line   9:       "GPIO9"                 input
        line  10:       "GPIO10"                input
        line  11:       "GPIO11"                input
        line  12:       "GPIO12"                input
        line  13:       "GPIO13"                input
        line  14:       "GPIO14"                input
        line  15:       "GPIO15"                input
        line  16:       "GPIO16"                input
        line  17:       "GPIO17"                input
        line  18:       "GPIO18"                input
        line  19:       "GPIO19"                input
        line  20:       "GPIO20"                input
        line  21:       "GPIO21"                input
        line  22:       "GPIO22"                input
        line  23:       "GPIO23"                input
        line  24:       "GPIO24"                input
        line  25:       "GPIO25"                input
        line  26:       "GPIO26"                input
        line  27:       "GPIO27"                input
        line  28:       "RGMII_MDIO"            input
        line  29:       "RGMIO_MDC"             input
        line  30:       "CTS0"                  input
        line  31:       "RTS0"                  input
        line  32:       "TXD0"                  input
        line  33:       "RXD0"                  input
        line  34:       "SD1_CLK"               input
        line  35:       "SD1_CMD"               input
        line  36:       "SD1_DATA0"             input
        line  37:       "SD1_DATA1"             input
        line  38:       "SD1_DATA2"             input
        line  39:       "SD1_DATA3"             input
        line  40:       "PWM0_MISO"             input
        line  41:       "PWM1_MOSI"             input
        line  42:       "STATUS_LED_G_CLK"      output consumer="ACT"
        line  43:       "SPIFLASH_CE_N"         input
        line  44:       "SDA0"                  input
        line  45:       "SCL0"                  input
        line  46:       "RGMII_RXCLK"           input
        line  47:       "RGMII_RXCTL"           input
        line  48:       "RGMII_RXD0"            input
        line  49:       "RGMII_RXD1"            input
        line  50:       "RGMII_RXD2"            input
        line  51:       "RGMII_RXD3"            input
        line  52:       "RGMII_TXCLK"           input
        line  53:       "RGMII_TXCTL"           input
        line  54:       "RGMII_TXD0"            input
        line  55:       "RGMII_TXD1"            input
        line  56:       "RGMII_TXD2"            input
        line  57:       "RGMII_TXD3"            input
"

# SparkFun_BNO080_Arduino_Library  [https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library/blob/main/src/SparkFun_BNO080_Arduino_Library.h]