# NFC Reader System
A system that reads the NFC tag's address then appends it to a specific google sheet file to monitor Co-Working Space.
It establishes a webserver in nodeMCU has RC522 Model connected to that used to read the NFC addresses, writing them in the internal queue waiting for python client to send a request to retrieve them, which will next append them to the google sheet file.


# Setting Up
- To set the libraries used in the python client run the following command:
```py
pip install -r requirements.txt
```
- Create a new project in [Google Console Developer](https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbkV4MnltcVJQdmtlSm5mNm1GR0Nic1FUaXl3UXxBQ3Jtc0trM0laLWcxNzd2ZUNfUnRMdi1TamRPSG55MkRzcTFib2FPQ29HSHJIY1VOVF92Q2EwZEV0QU1HNjNIZnNGTnZoQmw0cThSUm1iUWtDQzJ4VlhNSEc2ZC1xNHFzeDg1UVQzUk5Ma3h4QU9fUndmZHUyOA&q=https%3A%2F%2Fconsole.developers.google.com)
- Follow the steps provided in [google API doc](https://developers.google.com/identity/protocols/oauth2/service-account#python) to create a service credintial.
- Put the Service Key in JSON format in the same directory with this app in name 'service_key.json'


# RFID-RC522 Connection with NodeMCU ESP8266
- 3.3V  - 3V
- GND   - GND
- RST   - D4
- MISO  - D6
- MOSI  - D7
- SCK   - D5
- SDA   - D8