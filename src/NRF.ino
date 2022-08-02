void nrf_init(){

  SPI.setHwCs(true);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  radio.begin();                                        // Инициируем работу nRF24L01+
  radio.setChannel      (121);                          // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_250KBPS);                 // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_MAX);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.setPayloadSize  (5);
  radio.openReadingPipe (1, rx_addr[0]);
  radio.openWritingPipe (0xe7e7e7e7e7LL);
  radio.setCRCLength    (RF24_CRC_8);
  radio.startListening  ();
}

void nrf_sendtoled(){

  static unsigned long lastsend;

   if(millis()>lastsend){

    lastsend +=10000;
    radio.stopListening   ();
    radio.setChannel      (5);
    radio.setPayloadSize  (32);
    radio.setDataRate     (RF24_2MBPS);
    radio.openWritingPipe (0xe7e7e7e7e7LL);
    radio.write(&recive_data, 32);
    radio.setChannel      (121);
    radio.setPayloadSize  (5);
    radio.setDataRate     (RF24_250KBPS);
    radio.openReadingPipe (1, rx_addr[0]);
    radio.startListening  ();
   }
}

bool nrf_getdata(){

 if(radio.available()){                                          // Если в буфере имеются принятые данные
      radio.read(&recive_data, sizeof(recive_data));                 // Читаем данные в массив data и указываем сколько байт читать
      nrftemp=recive_data.temp;
      nrftemp+=(float)recive_data.ftemp/10;
      nrf_sendtoled();
      bat=recive_data.bat<<8;
      bat+=recive_data.fbat;
      bat/=190;

      if(DEBUG){
        Serial.print("NRF(");
        Serial.print(sizeof(recive_data));
        Serial.print("Converted data from nrf: ");
        Serial.println(nrftemp);
      }

      return 1;
    }
  return 0;
}
