#ifndef DATA_HPP
#define DATA_HPP

#include "../gl.hpp"
#include "../texture.hpp"
#include <string>

namespace vc {

  struct DataValue {
    virtual GLuint gl();
  };

  struct TextureValue : public DataValue {
    int channel;
    GLuint glObject;
    TextureFormat format;
    std::string sourceFile;

    inline virtual GLuint gl() { return glObject; }
  };
}

#endif // DATA_HPP
