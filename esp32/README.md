# ESP-32

This folder contains the main code to be uploaded onto the ESP-32 as well as 
tests that can be run natively or on-board.

## Tests

### Wi-Fi

This test will attempt to 
1. Establish a connection to the server. 
2. Send a `"ping"` to the server.
3. Expect a `"pong"` from the server.