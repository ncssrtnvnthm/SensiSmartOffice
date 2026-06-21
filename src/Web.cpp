#if defined(WEB) || defined(Compound)
#include "config.h"
#include "Web.h"
#include <WebServer.h>
#include <sys/time.h>

WebServer *server;

void Web::begin()
{
  WiFi.mode(WIFI_AP_STA);
  Setup_Wifi_AP();
  Setup_Wifi_Station();       // try internet for NTP (non-blocking)

  // Captive portal: redirect all DNS queries to AP IP
  _dnsServer.start(53, "*", WiFi.softAPIP());

  server = new WebServer(WEB_SERVER_PORT);
  server->on("/", std::bind(&Web::handle_root, this));
  server->on("/time", std::bind(&Web::handle_time, this));
  server->onNotFound(std::bind(&Web::handle_root, this));
  server->begin();
  Serial.println("HTTP server + captive portal started");
}

// Receive time from client browser via /time?t=<unix_timestamp>
void Web::handle_time() {
  if (server->hasArg("t")) {
    time_t t = server->arg("t").toInt();
    struct timeval tv = { .tv_sec = t, .tv_usec = 0 };
    settimeofday(&tv, nullptr);
    struct tm ti;
    getLocalTime(&ti, 0);
    Serial.print("Time set from client: ");
    Serial.println(&ti, "%Y-%m-%d %H:%M:%S");
  }
  server->send(200, "text/plain", "OK");
}

void Web::commitMeasures()
{
}

void Web::handleNetwork()
{
  _dnsServer.processNextRequest();
  server->handleClient();
}

// HTML & CSS contents which display on web server
String HTMLHeader = "<!DOCTYPE html>\n\
<html>\n\
<head>\n\
    <title>SensiSmartOffice</title>\n\
    <meta http-equiv=\"refresh\" content=\"" + String(WEB_REFRESH_SECONDS) + "\">\n\
    <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n\
    <meta charset=\"UTF-8\">\n\
    <style>\n\
      *{box-sizing:border-box;margin:0;padding:0}\n\
      body{font:16px system-ui,sans-serif;background:#2d2d2d;color:#ccc;padding:16px}\n\
      h1{text-align:center;font-size:1.3em;margin-bottom:16px;color:#4caf50}\n\
      .cards{display:flex;gap:10px;flex-wrap:wrap;margin-bottom:16px}\n\
      .card{flex:1;min-width:90px;background:#3a3a3a;border:1px solid #4caf50;border-radius:6px;padding:12px 8px;text-align:center}\n\
      .card .val{font-size:1.5em;font-weight:bold;color:#4caf50}\n\
      .card .lbl{font-size:0.75em;color:#888;margin-top:4px;text-transform:uppercase;letter-spacing:1px}\n\
      .log{font-size:0.7em;color:#777;max-height:200px;overflow-y:auto;border-top:1px solid #444;padding-top:8px}\n\
      .log p{margin:2px 0;line-height:1.3}\n\
    </style>\n\
    <script>fetch('/time?t='+Math.floor(Date.now()/1000))</script>\n\
</head>\n\
<body>\n\
<h1>SensiSmartOffice</h1>\n";

String HtmlFooter = "</body>\n</html>";

// Handle root url (/)
void Web::handle_root()
{
  server->send(200, "text/html", buildPage());
}

String Web::buildPage() {
  String response = HTMLHeader;

  float t = _tempHistory.empty() ? 0 : _tempHistory.back();
  float h = _humHistory.empty() ? 0 : _humHistory.back();
  float p = _presHistory.empty() ? 0 : _presHistory.back();

  response += "<div class='cards'>\n";
  response += "<div class='card'><div class='val'>"
           + String(t, 1) + "</div><div class='lbl'>&deg;C</div></div>\n";
  response += "<div class='card'><div class='val'>"
           + String(h, 1) + "</div><div class='lbl'>% RH</div></div>\n";
  response += "<div class='card'><div class='val'>"
           + String(p, 1) + "</div><div class='lbl'>hPa</div></div>\n";
  response += "</div>\n";

  response += "<div class='log'>\n";
  std::for_each(_records.rbegin(), _records.rend(), [&](String r) { response += r; });
  response += "</div>\n";

  response += HtmlFooter;
  return response;
}

void Web::visit(InfoRecord *record) { push_back("<p>" + record->toString() + "</p>"); }
void Web::visit(ErrorRecord *record) { push_back("<p>" + record->toString() + "</p>"); }
void Web::visit(MeasureRecord *record)
{
  push_back("<p>" + record->toString() + "</p>");

  // Keep last value for cards
  switch (record->Type) {
    case SensorValueType::TEMPERATURE:
      _tempHistory.push_back(record->Value);
      if (_tempHistory.size() > 1) _tempHistory.erase(_tempHistory.begin());
      break;
    case SensorValueType::HUMIDITY:
      _humHistory.push_back(record->Value);
      if (_humHistory.size() > 1) _humHistory.erase(_humHistory.begin());
      break;
    case SensorValueType::PRESSURE:
      _presHistory.push_back(record->Value);
      if (_presHistory.size() > 1) _presHistory.erase(_presHistory.begin());
      break;
  }
}

void Web::push_back(String entry)
{
  _records.push_back(entry);
  while (_records.size() > WEB_MAX_RECORDS)
  {
    _records.erase(_records.begin());
  }
}

#endif /* WEB */
