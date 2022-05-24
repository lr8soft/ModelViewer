#include "AppFrame.h"

int main()
{

    AppFrame *inst = AppFrame::getInstance();
    inst->FrameInit();
    inst->FrameLoop();
    inst->FrameFinalize();

    return 0;
}