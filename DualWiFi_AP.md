# Обновляем прошивку и пакеты
## bash shell:
    apt update && apt full-upgrade -y
    rpi-eeprom-update -a   # прошивка Wi-Fi

# Создаём виртуальный интерфейс AP
Добавляем однократный udev-хук, чтобы ap0 появлялся после каждого перезапуска Wi-Fi-чипа:
## bash shell:
    cat >/etc/udev/rules.d/70-wifi-dual.rules <<'EOF'
    ACTION=="add", SUBSYSTEM=="net", KERNEL=="wlan0", \
    RUN+="/sbin/iw dev wlan0 interface add ap0 type __ap"
    EOF
    udevadm control --reload

# Проверяем:
### bash shell:
    apt install -y iw      # если ещё нет
    iw list | grep -A3 -i "valid interface combinations"
    ip link show ap0

# Ставим hostapd и проверяем его «сухим запуском»
Установка пакета
## bash shell:
    apt install -y hostapd

# Создаём минимальный конфиг

## bash shell:
    cat >/etc/hostapd/hostapd_ap0.conf <<'EOF'
interface=ap0
driver=nl80211

ssid=X-MRA01
hw_mode=g
channel=5          # тот же канал, что у вашего STA (2432 МГц)

wpa=2
wpa_passphrase=TakeALook
EOF
## bash shell END

## bash shell:
    hostapd -t /etc/hostapd/hostapd_ap0.conf


# Запускаем hostapd как системный сервис
Команды выполняйте подряд и присылайте вывод systemctl status hostapd --no-pager.

## bash shell:
 # 4-A. Сообщаем hostapd, где лежит наш конфиг
    sed -i 's|^#*DAEMON_CONF=.*|DAEMON_CONF="/etc/hostapd/hostapd_ap0.conf"|' /etc/default/hostapd

 # 4-B. Включаем сервис в автозапуск и запускаем прямо сейчас
    systemctl unmask hostapd
    systemctl enable hostapd
    systemctl restart hostapd

 # 4-C. Устраняем условие и запускаем hostapd
    ln -s /etc/hostapd/hostapd_ap0.conf /etc/hostapd/hostapd.conf
    systemctl start hostapd
    systemctl status hostapd --no-pager


## bash shell:
    nmcli con add type wifi ifname ap0 con-name PiAP autoconnect yes \
      ssid PiZeroLink mode ap 802-11-wireless.band bg 802-11-wireless.channel 5 \
      ipv4.addresses 10.0.0.1/24 ipv4.method manual
