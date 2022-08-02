String webPage(){
  String web;
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>HOME PC</title><style>button{color:red;padding: 10px 27px;}</style></head>";
  web += "<h1 style=\"text-align: center;font-family: Open sans;font-weight: 100;font-size: 20px;\">HOME PC</h1><div>";
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">MYPC SWITCHER</p>";
  if (mypc_state){
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">PC IS ON</div>";
  }else{
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">PC IS OFF</div>";
  }
  if (srv1_state){
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">SRV1 IS ON</div>";
  }else{
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">SRV1 IS OFF</div>";
  }

  // ========status=============
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"status\"><button>STATUS</button></a>&nbsp;</div>";
  // ========status=============

  web += "</div>";
  return(web);
}

String infoPage(){
  String web;
  web += "ChipID: ";
  web += ESP.getChipId();
  web += "<br>";
  web += "Core: ";
  web += ESP.getCoreVersion();
  web += "<br>";
  web += "SDK: ";
  web += ESP.getSdkVersion();
  web += "<br>";
  web += "CPU Freq: ";
  web += ESP.getCpuFreqMHz();
  web += "<br>";
  web += "Sketch size: ";
  web += ESP.getSketchSize();
  web += "<br>";
  web += "Free Space: ";
  web += ESP.getFreeSketchSpace();
  web += "<br>";
  web += "Flash ChipID: ";
  web += ESP.getFlashChipId();
  web += "<br>";
  web += "Flash Chip size: ";
  web += ESP.getFlashChipSize();
  web += "<br>";
  web += "Free Heap: ";
  web += ESP.getFreeHeap();
  web += "<br>";
  web += "Vcc: ";
  web += (float)ESP.getVcc()/1024;
  web += "<br>";

  return(web);
}
