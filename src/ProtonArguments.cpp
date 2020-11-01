//
// Created by avery on 27/10/2020.
//


#include "ProtonCaller.h"
#include "ProtonArguments.h"
#include "ProtonSetup.h"
#include <cstdio>
#include <cstdlib>

#define ex exit(EXIT_FAILURE)
#define HELP std::cout<<help_msg

#ifdef NORM
#define HELPLOCAL = "/usr/share/proton-caller/HELP"
#else
#define HELPLOCAL = "../HELP"
#endif


struct argsStruct {
    std::string _argv1;
    std::string _argv2;
    std::string _argv3;
};
struct argsStruct proArgs;

// This needs serious cleaning.
void Args(ProtonCaller &proObj, int argc, char *argv[]) {
    std::string help_msg = "Usage: [-c, -h][5.13, 5, 5.0, 4.11, 4.3, 3.16, 3.7][./foo.exe]\n"
                           "'proton-call 5 foo.exe'\n"
                           "'proton-call -c \"/Proton 5.13\" foo.exe'\n";


    if (argc == 1) {HELP;ex;}
    if (argv[1] != nullptr) {
        proArgs._argv1 = argv[1];
    } else {HELP;ex;}

    if (proArgs._argv1 == "-h") {
        HELPMSG();
        exit(EXIT_SUCCESS);
    } else if (proArgs._argv1 == "-c") {
        std::cout << "Custom mode: " << proObj.custom << std::endl << "Will not check for Proton.\n";
        if (argv[3] != NULL) {
            proObj.custom = true;
            proArgs._argv3 = argv[3];
        } else {std::cout<<"What program?\n";ex;}
    } else if (proArgs._argv1 == "-v") {
            PRVersion();
    } else if (proArgs._argv1 == "--setup") {
        setup(argv[1]);
        exit(EXIT_SUCCESS);
    } else {
        proObj.custom = false;
    }

    if (argv[2] != nullptr) {
        proArgs._argv2 = argv[2];
    } else {
        HELP;
        std::cout<<"What program?\n";
        ex;
    }
}

void setEnvironment(ProtonCaller &proObj) {
    if (proObj.custom) {
        proObj.program = proArgs._argv3;
        proObj.proton_path = proArgs._argv2;
    } else {
        proObj.common = findCommon();
        if (proArgs._argv1 == "5") {proObj.proton = "5.0";}
        else {proObj.proton = proArgs._argv1;}
        proObj.program = proArgs._argv2;
        std::string _proton = "Proton ";
        proObj.proton_path = proObj.common + _proton;
    }

    if (getenv(STEAM) != nullptr) {
        std::cout << STEAM << " located at: " << getenv(STEAM) <<"\n";
    } else {
        std::cout << STEAM << " must be added to your environment. Proton Will not run without it.\n";
        ex;
    }
}

const char* findCommon() {
    if (getenv(COMMON) != nullptr) {
        const char *cCommon = getenv(COMMON);
        std::cout << COMMON << " located at: " << cCommon << "\n";
        return cCommon;
    } else {
        std::cout << "\nPlease add a 'PC_COMMON' variable to your environment variables which point at the 'steamapps/common/' where your proton versions are installed.\n\n"
                     "export PC_COMMON='/steam/location/common/'\n";
        ex;
    }
}

void HELPMSG() {
    FILE *fFile;
    fFile = fopen("/usr/share/proton-caller/HELP", "r");
    if (fFile == NULL) {
        std::cout << "Error opening help message.\n";
        exit(EXIT_FAILURE);
    }
    char fName[4], c;
    c = fgetc(fFile);
    while (c != EOF) {
        std::cout << c;
        c = fgetc(fFile);
    }
    fclose(fFile);
}
