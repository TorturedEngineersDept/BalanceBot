for i in {1..10}; do
  sudo mosquitto_passwd -b /etc/mosquitto/passwd bot${i} esp32
  sudo mosquitto_passwd -b /etc/mosquitto/passwd website${i} ted
done