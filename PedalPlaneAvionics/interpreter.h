// Copyright 2018 by Kevin Dahlhausen
//
#ifndef _interpreter_h
#define _interpreter_h

#define SERIALCOMMAND_DEBUG 1

#include <SerialCommand.h>

class SerialInterpreter
{
    public:
    SerialInterpreter();

    void update()
    {
        serial_command.readSerial();
    }

    friend void navlights_curve();

    protected:
        SerialCommand serial_command;
};


class BluetoothInterpreter
{
};

#endif
