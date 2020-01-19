#include "ui/MainApplication.hpp"

#include <switch.h>
#include <curl/curl.h>

static int nxsock;

int main(int argc, char **argv) {
    /* Initialize socket for all our curl needs and nxlink with debug */
    socketInitializeDefault();
	curl_global_init(CURL_GLOBAL_ALL);
    #ifdef DEBUG
        nxsock = nxlinkStdio();
    #endif

    printf("INFO: starting application...\n");
    auto renderer = pu::ui::render::Renderer::New(SDL_INIT_EVERYTHING, pu::ui::render::RendererInitOptions::RendererNoSound, pu::ui::render::RendererHardwareFlags);
    auto amain = ui::MainApplication::New(renderer);
    amain->Prepare();
    amain->Show();
    printf("INFO: cleaning up...\n");

    /* Close debug socket and exit sockets */
    #ifdef DEBUG
        close(nxsock);
    #endif
    curl_global_cleanup();
    socketExit();
    return 0;
}