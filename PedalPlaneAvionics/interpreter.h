// Copyright 2018 by Kevin Dahlhausen
//
#ifndef _interpreter_h
#define _interpreter_h

#include <SerialCommand.h>

class SerialInterpreter
{
    public:
    SerialInterpreter();

    void update()
    {
        serial_command.readSerial();
    }

    protected:
        SerialCommand serial_command;
};


class BluetoothInterpreter
{
};

#endif
