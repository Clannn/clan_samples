#include "common/logging.h"
#include "platform/platform.h"
#include "plugins/plugins.h"

#include "platform/windows_platform.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    clan::WindowsPlatform platform{hInstance, hPrevInstance, lpCmdLine, nCmdShow};

    auto code = platform.initialize(plugins::get_all());

    LOGI("initialize  down!!!!..............")
    int i = 0;

    if (code == clan::ExitCode::Success) {
        code = platform.main_loop();
        if(i==0){
            LOGI("done first loop..................");
            i++;
        }
    }

    platform.terminate(code);

    return EXIT_SUCCESS;
}