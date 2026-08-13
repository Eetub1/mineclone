#include <string>

class Texture
{
private:
    unsigned int id;
    std::string filepath;
    unsigned char *localBuffer;
    int width, height, BPP;
public:
    Texture(const std::string &filepath);
    ~Texture();
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    int getWidth() { return width; }
    int getHeight() { return height; }
};