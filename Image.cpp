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
    for(int i = 0; i <= w*h ;i++)
    {
        pixels[i].b = 0;
        pixels[i].r = 0;
    }
}
void Image::filterBlue()
{
    for(int i = 0; i <= w*h ;i++)
    {
        pixels[i].g = 0;
        pixels[i].r = 0;
    }
}
void Image::greyScale()
{
    for(int i = 0; i <= w*h ;i++)
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

}
void Image::AdditionalFunction3()
{

}
void Image::AdditionalFunction1()
{
    Image *zImage = new Image(w*2,h*2);//Make a temp image 2 times the size as the displayed one

    {
        //zooming the image**************************************

        int nextPixel = 0;
        int nextRow = 0; //pixel at the start of the next row
        Image *zzz = new Image(w,h);
        zzz->pixels = pixels;

        int g=0;
        for (int y = 0; y < h; y++)// loop through all the pixels of main
        {
            g++;
            for (int x = 0; x < w; x++)
            {
                if (y == 0)//set the first row manually
                {
                    cout<<to_string(pixels[x,y].r)<<endl;
                    zImage->pixels[nextPixel,y] = pixels[x,y];
                    zImage->pixels[nextPixel + 1,y] = pixels[x,y];
                    zImage->pixels[nextPixel,y+1] = pixels[x,y];
                    zImage->pixels[nextPixel + 1,y+1] = pixels[x,y];

                    //zBmp.SetPixel(nextPixel, h, mainBmp.GetPixel(w, h));//assign each pixel in the copy to the original image
                    //zBmp.SetPixel(nextPixel + 1, h, mainBmp.GetPixel(w, h));//assign the next pixel to the same
                    //zBmp.SetPixel(nextPixel, h + 1, mainBmp.GetPixel(w, h));//do the same for row 1 as row 0
                    //zBmp.SetPixel(nextPixel + 1, h + 1, mainBmp.GetPixel(w, h));
                }

                zImage->pixels[nextPixel,nextRow] = pixels[x,y];
                zImage->pixels[nextPixel+1,nextRow] = pixels[x,y];
                zImage->pixels[nextPixel,nextRow+1] = pixels[x,y];
                zImage->pixels[nextPixel+1,nextRow+1] = pixels[x,y];

//                zBmp.SetPixel(nextPixel, nextRow, mainBmp.GetPixel(w, h));//assign each pixel in the copy to the original image
//                zBmp.SetPixel(nextPixel + 1, nextRow, mainBmp.GetPixel(w, h));//assign the next pixel to the same
//                zBmp.SetPixel(nextPixel, nextRow + 1, mainBmp.GetPixel(w, h));//do the same as above for current row + 1
//                zBmp.SetPixel(nextPixel + 1, nextRow + 1, mainBmp.GetPixel(w, h));

                if (nextPixel < (w - 2))
                {
                    nextPixel += 2;// jump 2 pixels each time
                }

            }

            nextPixel = 0; //reset to start of next row

            if (nextRow < h)// we dont want the last row as we'd get an "out of range" error
            {
                nextRow += 2;// jump 2 rows each time
            }

        }
        //now place the portion of the zoomed image into the picture
        cout<<to_string(h)+" "+to_string(w)<<endl;
        for (int cnt = 0; cnt < h*w-1; cnt++)
        {
            pixels[cnt].r = zImage->pixels[cnt].r;
            pixels[cnt].g = zImage->pixels[cnt].g;
            pixels[cnt].b = zImage->pixels[cnt].b;

        }
        //pixels=zzz->pixels;

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