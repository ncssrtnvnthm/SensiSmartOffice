#if defined(WEB) || defined(Compound)
#include "config.h"
#include "Web.h"
#include <WebServer.h>

WebServer *server;

void Web::begin()
{
  Setup_Wifi_AP();
  server = new WebServer(WEB_SERVER_PORT);
  server->on("/", std::bind(&Web::handle_root, this));
  server->begin();
  Serial.println("HTTP server started");
}

void Web::commitMeasures()
{
}

void Web::handleNetwork()
{
  server->handleClient();
}

// HTML & CSS contents which display on web server
String HTMLHeader = "<!DOCTYPE html>\
<html>\
<head>\
    <title>Sensirion BLE</title>\
    <meta http-equiv=\"refresh\" content=\"" + String(WEB_REFRESH_SECONDS) + "\">\
</head>\
<body>\
<h1>Web Server with ESP32 &#128522;</h1>";

String HtmlFooter = "\
</body>\
</html>";

// Handle root url (/)
void Web::handle_root()
{
  String response = HTMLHeader;

  std::for_each(_records.rbegin(), _records.rend(), [&](String record)
                { response += record; });
  //_records.clear();

  response += HtmlFooter;

  server->send(200, "text/html", response.c_str());
}

void Web::visit(InfoRecord *record) { push_back("<p>" + record->toString() + "</p>"); }
void Web::visit(ErrorRecord *record) { push_back("<p>" + record->toString() + "</p>"); }
void Web::visit(MeasureRecord *record) { push_back("<p>" + record->toString() + "</p>"); }

void Web::push_back(String entry)
{
  _records.push_back(entry);
  while (_records.size() > WEB_MAX_RECORDS)
  {
    _records.erase(_records.begin());
  }
}

#endif /* WEB */
