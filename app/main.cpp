#include "common/logging.h"
#include "platform/platform.h"
#include "plugins/plugins.h"

#include "platform/windows_platform.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    clan::WindowsPlatform platform{hInstance, hPrevInstance, lpCmdLine, nCmdShow};

    auto code = platform.initialize(plugins::get_all());

    if (code == clan::ExitCode::Success) {
        code = platform.main_loop();
    }

    platform.terminate(code);

    return EXIT_SUCCESS;
}