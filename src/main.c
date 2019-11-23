#include <switch.h>

#include <stdio.h>
#include <sys/stat.h>   // MKDIR()

#include "sugarFs.h"

void routine(){
    int retVal;

    if (mkdir("/test", 0777) != 0 ){
        printf("Unable to create dir\n");
        return;
    }
    printf("Created folder /test/\n");

    retVal = touch("/test/testFile1");
    printf("File created? %i \n", retVal);

    retVal = touch("/test/testFile2");
    printf("File created? %i \n", retVal);

    if (mkdir("/test/testDirOne", 0777) != 0 ){
        printf("Unable to create dir\n");
        return;
    }
    printf("Created folder /test/testDirOne/\n");

    lsDir("/");

    printf("====\n");

    removeAll("/test");

    lsDir("/");
}

int main(int argc, char **argv)
{
    consoleInit(NULL);

    routine();
    // Main loop
    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
