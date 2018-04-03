#ifndef DECODEIMAGE_H
#define DECODEIMAGE_H

/**
 * @brief class DecodeImage - decodes image to raw image data. Input file format has to be decode-able by member decoder component.
 */
class DecodeImage
{
public:
    DecodeImage();
    virtual ~DecodeImage();

    void Init();
    void Run();
    void Destroy();

private:
    class DataClass;
    DataClass* d;
};

#endif // DECODEIMAGE_H
