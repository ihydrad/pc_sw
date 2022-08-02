void HTTP_init(void) {
  HTTP.on("/restart", handle_Restart); // Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
  // Запускаем HTTP сервер
  HTTP.begin();
}

// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");
  if (restart == "ok") ESP.reset();
  HTTP.send(200, "text/plain", "OK");
}

byte httpget(String cmd)
{

  if (!client.connect(ip_MYPC, TCPport))
  {
    Serial.println("Connection to GETadmin filed!");
    return 0;
  }
  client.print("GET " + cmd + " HTTP/1.1\r\n" +
               "Host:" + "myPC" + "\r\n" +
               "Connection: close\r\n\r\n");

#ifdef DEBUG_GETADMIN
  while (client.available())
  {

    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
#endif

  client.stop();
  return 1;
}

void HTTPcmd()
{
  HTTP.on("/state", []() {
    if (!HTTP.authenticate(www_username, www_password))
      return HTTP.requestAuthentication();
    HTTP.send(200, "text/html", webPage());
  });

  HTTP.on("/wakeup", []() {
    wakeup_cmd = 1;
    HTTP.send(200, "text/html", webPage());
    delay(100);
  });

  HTTP.on("/status", []() {
    wakeup_cmd = 2;
    HTTP.send(200, "text/html", webPage());
    delay(100);
  });

  HTTP.on("/sleep", []() {
    wakeup_cmd = 3;
    HTTP.send(200, "text/html", webPage());
    delay(100);
  });

  HTTP.on("/lock", []() {
    wakeup_cmd = 4;
    HTTP.send(200, "text/html", webPage());
    delay(100);
  });

  HTTP.on("/info", []() {
    HTTP.send(200, "text/html", infoPage());
    delay(100);
  });
}
