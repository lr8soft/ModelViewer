#include "AppFrame.h"

int main()
{

    AppFrame *inst = AppFrame::getInstance();
    inst->FrameInit();
    inst->FrameWork();
    inst->FrameFinalize();

    return 0;
}