#include <string>
#include <switch.h>
#include <pu/Plutonium>
#include <stdio.h>
#include <Swurl.hpp>
#include "MainApplication.hpp"
#include "web.hpp"
#include <thread>

#define DEBUG 1
using namespace swurl;
model::theme theme;

void init(){
    printf("INFO: setting theme\n");
    theme = *new model::theme;
    theme.background = pu::ui::Color::FromHex("#0d0d0dff");
    theme.hoverColor = pu::ui::Color::FromHex("#1f1f1fff");
    theme.tagBg = pu::ui::Color::FromHex("#666666ff");
    theme.textColor = pu::ui::Color::FromHex("#d9d9d9ff");
}
int main(int argc, char **argv) {
    socketInitializeDefault();
    SessionManager::initialize();
    SessionManager::userAgent = "gui-test/0.9.0";
    SessionManager::requestHeaders.insert(
        std::pair<std::string, std::string>(
            "Cache-Control",
            "no-cache"
        )
    );
    SessionManager::onProgressChanged = web::onProgressChanged;
    SessionManager::onCompleted = web::onCompleted;
    SessionManager::onError = web::onError;
    #ifdef DEBUG
        nxlinkStdio();
    #endif

    init();

    printf("INFO: starting application...\n");
    ui::MainApplication *amain = new ui::MainApplication();
    amain->Show();
    printf("INFO: cleaning up...\n");
    delete amain;

    SessionManager::dealloc();
    printf("INFO: exiting...\n");
    return 0;
}