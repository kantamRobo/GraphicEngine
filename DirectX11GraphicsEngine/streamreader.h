#pragma once


/*
* 
éRìc âpêL 2019-09-14
https://github.com/techlabxe/vulkan_book_1/tree/master/04_DrawModel
vulkan_book_1/04_DrawModel/streamreader.hÇÊÇËà¯óp

Permission is hereby granted, free of charge, 
to any person obtaining a copy of this software and associated documentation
files (the ÅgSoftwareÅh), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ÅgAS ISÅh, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>
#if _MSC_VER > 1922 && !defined(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif

#include <experimental/filesystem>

class StreamReader : public Microsoft::glTF::IStreamReader
{
public:
    StreamReader(std::filesystem::path pathBase) : m_pathBase(std::move(pathBase)) { }

    std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override
    {
        auto streamPath = m_pathBase / std::experimental::filesystem::u8path(filename).c_str();
        auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);
        if (!stream || !(*stream))
        {
            throw std::runtime_error("Unable to create valid input stream.");
        }
        return stream;
    }

private:
    std::filesystem::path m_pathBase;
};

