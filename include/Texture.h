class Texture
{
private:
    unsigned int t_ID;
public:
    Texture(const char *filepath);
    ~Texture();
    void bind() const;
};