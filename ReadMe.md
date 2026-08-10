Current CMakeUserPresets.json outline:
{
    "version": 8,
    "configurePresets": [
        {
            "name": "Cats",
            "displayName": "GCC 16.1.0 x86_64-w64-mingw32",
            "description": "Local MinGW compiler paths",
            "inherits": "Cats-base",
            "cacheVariables": {
                "CMAKE_C_COMPILER": "C:/path/to/gcc.exe",
                "CMAKE_CXX_COMPILER": "C:/path/to/g++.exe"
            }
        }
    ]
}

Make sure to match the version and compiler type (MinGW) to prevent most issues on separate devices