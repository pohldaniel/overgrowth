//-----------------------------------------------------------------------------
//           Name: consolehandler.h
//      Developer: Wolfire Games LLC
//    Description:
//        License: Read below
//-----------------------------------------------------------------------------
//
//   Copyright 2022 Wolfire Games LLC
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//-----------------------------------------------------------------------------
#pragma once

#include <Logging/loghandler.h>

/*!
        Potential child to LogHandler and will print messages to std::cout
*/
class ConsoleHandler : public LogHandler {
   public:
    ConsoleHandler();

    ~ConsoleHandler() override;

    /*! \param message will be printed to std::cout */
    void Log(LogSystem::LogType type, int row, const char* filename, const char* cat, const char* message_prefix, const char* message) override;
    void Flush() override;
};
