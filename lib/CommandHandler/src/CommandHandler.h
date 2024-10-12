#ifndef HANDLER_H
#define HANDLER_H

#include <Core.h>

class CommandHandler {
    public:
        CommandHandler();
        void processCommand(String command);
        void assignCore(Core coreObject_);
        
    private:
        String lastCommand;
        Core coreObject;
};

#endif