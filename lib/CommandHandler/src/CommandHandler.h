#ifndef HANDLER_H
#define HANDLER_H

#include <Core.h>

class CommandHandler {
    public:
        CommandHandler();
        void processCommand(String command);
        void assignCore(Core coreObject_);
        void enqueueCommand(String command);
        
    private:
        String lastCommand;
        QueueHandle_t commandQueue;
        Core coreObject;
};

#endif