## SETTING UP

To add a password:
``` bash
sudo mosquitto_passwd -c /etc/mosquitto/passwd username
```
Or:
``` bash
sudo mosquitto_passwd -b /etc/mosquitto/passwd username password
```

## SECURING WITH CLIENT CERTIFICATED

```bash
sudo apt-get install openssl -y
mkdir ~/certs
cd ~/certs
openssl genrsa -out ca.key 2048
openssl req -new -x509 -days 3650 -key ca.key -out ca.crt
```

Entered:
``` bash
Country Name (2 letter code) [AU]:UK
State or Province Name (full name) [Some-State]:London
Locality Name (eg, city) []:London
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Imperial
Organizational Unit Name (eg, section) []:EEE
Common Name (e.g. server FQDN or YOUR name) []:Sara
Email Address []:sara.chehab22@imperial.ac.uk
```

```bash
Country Name (2 letter code) [AU]:UK
State or Province Name (full name) [Some-State]:London
Locality Name (eg, city) []:London
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Imperial
Organizational Unit Name (eg, section) []:EEE
Common Name (e.g. server FQDN or YOUR name) []:18-171-187-186
Email Address []:sara.chehab22@imperial.ac.uk
Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:tedted
An optional company name []:ted
```

Also run
```bash
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 3650
openssl genrsa -out client.key 2048
openssl req -new -key client.key -out client.csr
```



## TESTING

To test the new secured MQTT server, use the command

``` bash
mosquitto_pub -h {IP} -t test -m "hello world" -u "bot{nb}" -P "esp32"
mosquitto_sub -h {IP} -t test -u "website{nb}" -P "ted"
```