#ifndef WEB_H
#define WEB_H

#include "uiInterface.h"
#include "record.h"
#include <vector>
#include <DNSServer.h>

void Setup_Wifi();
void Setup_Wifi_AP();
void Setup_Wifi_Station();
bool IsWifiStationConnected();
bool IsTimeSynced();

class Web : public uiInterface
{
private:
    static const int HISTORY_SIZE = 20;
    std::vector<String> _records;
    DNSServer _dnsServer;

    // Latest sensor values for display
    std::vector<float> _tempHistory;
    std::vector<float> _humHistory;
    std::vector<float> _presHistory;

    void push_back(String entry);
    String buildPage();

public:
    void handle_root();
    void handle_time();

    void begin();
    void commitMeasures();
    void handleNetwork();

    void visit(InfoRecord* record);
    void visit(ErrorRecord* record);
    void visit(MeasureRecord* record);
};

#endif /* WEB_H */