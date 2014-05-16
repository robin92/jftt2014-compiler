cmake_minimum_required (VERSION 2.6)

project (maszyna-wirtualna)

set (SOURCEDIR "src")

add_executable (vm
        ${SOURCEDIR}/interpreter.cc
    )

