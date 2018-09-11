//
// Created by xiaoc on 2018/8/9.
//

#ifndef RMCC_CONFIG_H
#define RMCC_CONFIG_H

#include "kcc.h"
#define warningAsError "WA"
#define quitIfError "QE"
struct ConfigState{
    std::unordered_map<const char*,bool> settings;
    ConfigState(){
        settings[warningAsError] = false;
        settings[quitIfError] = true;
    }
    bool operator [] (const char *opt){
        return settings[opt];
    }
};

#endif //RMCC_CONFIG_H
