# Balance Bot

Where all code should live. There may be some changes for CI to work properly but
I'll get those sorted out later. Docs can live in each individual folder as a 
README.

## Flashing the ESP32

Follow instructions on [the PlatformIO website](https://platformio.org/install/cli)
to download PlatformIO CLI.

Then run these following commands:

```bash
cd esp32
./run.sh
```

## Starting the website

To run the website on `localhost:3000`, run the following commands:

```bash
cd server/website
npm i
npm start
```

## Raspberry Pi setup

See the docs in the [`pi`](pi) folder.