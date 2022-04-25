//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"



bool Image::load(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary | std::ios::in); // need to spec. binary & input mode for Windows users
    try
    {
        if (ifs.fail())
        {
            throw ("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw ("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h];  // this will throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines if necessary until we get to the binary data

        unsigned char pixel_buffer[3]; // buffer to store one pixel, with three channels red,green and blue

        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < (w * h); ++i)
        {
            ifs.read(reinterpret_cast<char *>(pixel_buffer), 3);
            this->pixels[i].r = pixel_buffer[0]; // / 255.f;
            this->pixels[i].g = pixel_buffer[1]; // / 255.f;
            this->pixels[i].b = pixel_buffer[2]; // / 255.f;
        }
        ifs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
    return true;
}
bool Image::loadRaw(string filename)
{
    return false;
}
bool Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0)
    {
        fprintf(stderr, "Can't save an empty image\n");
        return true;
    }
    std::ofstream ofs;
    try
    {
        ofs.open(filename, std::ios::binary|std::ios::out); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->w << " " <<  this->h << "\n255\n";

        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->w *  this->h; ++i)
        {
//            r = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].r) * 255);
//            g = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].g) * 255);
//            b = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].b) * 255);
//            ofs << r << g << b;

            ofs.write(reinterpret_cast<char *>(&this->pixels[i].r),1);    // write 1 byte;
            ofs.write(reinterpret_cast<char *>(&this->pixels[i].g),1);    // write 1 byte;
            ofs.write(reinterpret_cast<char *>(&this->pixels[i].b),1);    // write 1 byte;
        }
        ofs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }

    return true;
}

void Image::filterRed()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        pixels[i].b = 0;
        pixels[i].g = 0;
    }
}
void Image::filterGreen()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        pixels[i].b = 0;
        pixels[i].r = 0;
    }
}
void Image::filterBlue()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        pixels[i].g = 0;
        pixels[i].r = 0;
    }
}
void Image::greyScale()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        // Code given by andrew
        float grayScale = (pixels[i].r + pixels[i].g + pixels[i].b) /3;
        pixels[i].r = grayScale;
        pixels[i].g = grayScale;
        pixels[i].b = grayScale;
    }
}
void Image::flipHorizontal()
{
    Image *tmpImage = new Image();
    *tmpImage->pixels = *pixels;

    int y;
    int x;

    for (int total = 0; total<=(h*w) ; total+=w)
    {
        for (int x = 0; x < w ; x++)
        {
            tmpImage->pixels[y+x]  = pixels[(total+w) - (x+1)];
        }
        y = total;
    }

    pixels = tmpImage->pixels;

}
void Image::flipVertically()
{
    Image *tmpImage = new Image();
    *tmpImage->pixels = *pixels;

    int y = (h*w);
    int x;

    for (int total = 0; total<=(h*w) ; total+=w)
    {
        if (y > 0)
        {
            y = y - w;
        }

//        cout<<"y val = "<<y<<endl;
//        cout<<"total val = "<<total<<endl;
        for (int x = 0; x < w ; x++)
        {
            tmpImage->pixels[total+x] = pixels[(y+x)];
        }
    }
    pixels = tmpImage->pixels;
}
void Image::AdditionalFunction2()
{
    float depth = 20;
    float intensity = 2;
    for(int i = 0; i <= w*h-1 ;i++)
    {
        float average = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].r = average + (depth*2);
        pixels[i].g = average + depth;
        pixels[i].b = average - intensity;
        if(pixels[i].r > 255)
        {
            pixels[i].r = 255;
        }
        if(pixels[i].g > 255)
        {
            pixels[i].g = 255;
        }
        if(pixels[i].g > 255)
        {
            pixels[i].b = 255;
        }
        if(pixels[i].b > 255)
        {
            pixels[i].b = 255;
        }
        if(pixels[i].b < 0)
        {
            pixels[i].b = 0;
        }

    }
}
void Image::AdditionalFunction3()
{
    float depth = 20;
    float intensity = 2;
    for(int i = 0; i <= w*h-1 ;i++)
    {
        float average = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].r = average + (depth*2);
        pixels[i].g = average + depth;
        pixels[i].b = average - intensity;
        if(pixels[i].r > 255)
        {
            pixels[i].r = 255;
        }
        if(pixels[i].g > 255)
        {
            pixels[i].g = 255;
        }
        if(pixels[i].g > 255)
        {
            pixels[i].b = 255;
        }
        if(pixels[i].b > 255)
        {
            pixels[i].b = 255;
        }
        if(pixels[i].b < 0)
        {
            pixels[i].b = 0;
        }
}
void Image::AdditionalFunction1()
{
    float rTotal = 0, gTotal=0, bTotal=0;
    for(int y = 0 ; y < h; y++)
    {
        for(int x = 0 ; x < w; x++)
        {
            //128, 97, 60
            rTotal = 0; gTotal = 0; bTotal= 0;
            if(x==0||y==0||x==w-1||y==h-1)
                continue;
            else
            {
                Rgb neighbours[] =
                        {pixels[(y-1)*w+(x-1)],
                         pixels[(y-1)*w+x],
                         pixels[(y-1)*w+x+1],
                        pixels[(y)*w+x-1],
                        pixels[(y)*w+x],
                        pixels[(y)*w+x+1],
                        pixels[(y+1)*w+(x-1)],
                        pixels[(y+1)*w+x],
                        pixels[(y+1)*w+x+1]};

                for(Rgb p: neighbours)
                {
                    rTotal += p.r;
                    gTotal += p.g;
                    bTotal += p.b;
                }
                pixels[y*w+x].r=rTotal/9;
                pixels[y*w+x].g=gTotal/9;
                pixels[y*w+x].b=bTotal/9;

            }
        }
    }
}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

Rgb* Image::getImage()
{
    return pixels;
}