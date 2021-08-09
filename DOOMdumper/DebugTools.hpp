/* 
This file (DebugTools.hpp) is a part of DOOMdumper.

Copyright (C) 2021 SEWsam

DOOMdumper is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DOOMdumper is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DOOMdumper If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <iostream>


class NullBuffer : public std::streambuf
{
public:
    int overflow(int c)
    {
        return c;
    }
};

class NullBufferW : public std::wstreambuf
{
public:
    int overflow(int c)
    {
        return c;
    }
};

class NullStream : public std::ostream
{
private:
    NullBuffer n_sb;

public:
    NullStream() : std::ostream(&n_sb) {}
};

class NullStreamW : public std::wostream
{
private:
    NullBufferW n_sb;

public:
    NullStreamW() : std::wostream(&n_sb) {}
};

class DebugStream
{
private:
    NullStream nullstream;
    NullStreamW nullstreamw;
    bool verbose = false;
public:
    DebugStream() {}
    DebugStream(bool verbosity) : verbose(verbosity) {}
    void enabled(bool verbosity) { verbose = verbosity; }

    std::ostream& dbgCout(bool noprefix = false)
    {
        if (verbose) {
            if (!noprefix) {
                std::cout << "\033[33mVerbose: \033[0m";
            }

            return std::cout;
        }
        else {
            return nullstream;
        }
    }

    std::wostream& dbgCoutW(bool noprefix = false, bool nocolor = false)
    {
        if (verbose) {
            if (!noprefix) {
                if (nocolor) {
                    std::wcout << "Verbose: ";
                }
                else
                {
                    std::wcout << "\033[33mVerbose: \033[0m";
                }
            }

            return std::wcout;
        }
        else {
            return nullstreamw;
        }
    }
};